#include "data_cal.h"
#include "math.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "string.h"
float AngleSet[7] = {0};

/*
有13个数据
还需要两个包头两个包尾一个数据长度位一个校验位
有六个轴，一个夹爪，12+1=13字节
//总共就是19个字节，我收发都需要19个字节
*/

/*--------------------------------发送协议-----------------------------------
//----------------55 aa size 00 00 00 00 00 crc8 0d 0a----------------------
//----------------55 aa size 00 00 00 00 00 0d 0a----------------------
//数据头55aa + 数据字节数size + 数据（利用共用体） + 校验crc8 + 数据尾0d0a
//注意：这里数据中预留了一个字节的控制位，其他的可以自行扩展，更改size和数据
--------------------------------------------------------------------------*/

/*--------------------------------接收协议-----------------------------------
//----------------55 aa size 00 00 00 00 00 00 00 crc8 0d 0a----------------
//数据头55aa + 数据字节数size + 数据（利用共用体） + 校验crc8 + 数据尾0d0a
//注意：这里数据中预留了一个字节的控制位，其他的可以自行扩展，更改size和数据
--------------------------------------------------------------------------*/

/**************************************************************************
通信的发送函数和接收函数必须的一些常量、变量、共用体对象
**************************************************************************/

// 数据接收暂存区
unsigned char receiveBuff[16] = {0};
// 通信协议常量
const unsigned char header[2] = {0x55, 0xaa};
const unsigned char ender[2] = {0x0d, 0x0a};

/*******************使用联合体可以快速的转换数据类型***************************/

union sendData
{
	// 这里传入d为数字
	short d;
	// 共用同一个指针，然后把short转换为了一个u8 的数组
	unsigned char data[2];
} joint11, joint21, joint31, joint41, joint51, joint61;

// 左右轮速控制速度、舵机共用体
union receiveData
{
	short d;
	unsigned char data[2];
} joint1Set, joint2Set, joint3Set, joint4Set, joint5Set, joint6Set;

/**************************************************************************
函数功能：通过串口中断服务函数,获取目标关节角度，夹爪标志位，传入参数中
入口参数：传入的数据
返回  值：无特殊意义
**************************************************************************/
int usartReceiveOneData(u8 *receiveBuff, UART_HandleTypeDef *huart)
{

	unsigned char USART_Receiver = 0;		   // 接收数据
	static unsigned char checkSum = 0;		   // ？
	static unsigned char USARTBufferIndex = 0; // 串口索引
	static short j = 0, k = 0;
	static unsigned char USARTReceiverFront = 0;
	static unsigned char Start_Flag = START; // 一帧数据传送开始标志位
	static short dataLength = 0;

	USART_Receiver = receiveBuff[1];
	USARTReceiverFront = receiveBuff[0];

	// 接收消息头
	if (Start_Flag == START)
	{
		if (USART_Receiver == 0xaa) // buf[1]
		{
			if (USARTReceiverFront == 0x55) // 数据头两位 //buf[0]
			{
				Start_Flag = !START; // 收到数据头，开始接收数据
				// printf("header ok\n");
				receiveBuff[0] = header[0]; // buf[0]
				receiveBuff[1] = header[1]; // buf[1]
				USARTBufferIndex = 0;		// 缓冲区初始化
				checkSum = 0x00;			// 校验和初始化
			}
		}
		else
		{
			USARTReceiverFront = USART_Receiver;
		}
	}
	if (Start_Flag != START)
	{
		dataLength = receiveBuff[2];
		checkSum = getCrc8(receiveBuff, 3 + dataLength);
		if (checkSum == receiveBuff[3 + dataLength])
		{
			// 进行角度赋值操作
			for (k = 0; k < 2; k++)
			{
				joint1Set.data[k] = receiveBuff[k + 3];
				joint2Set.data[k] = receiveBuff[k + 5];
				joint3Set.data[k] = receiveBuff[k + 7];
				joint4Set.data[k] = receiveBuff[k + 9];
				joint5Set.data[k] = receiveBuff[k + 11];
				joint6Set.data[k] = receiveBuff[k + 13];
			}

			AngleSet[0] = ((float)joint1Set.d / 1000.0);
			AngleSet[1] = -((float)joint2Set.d / 1000.0);
			AngleSet[2] = ((float)joint3Set.d / 1000.0);
			AngleSet[3] = ((float)joint4Set.d / 1000.0);
			AngleSet[4] = ((float)joint5Set.d / 1000.0);
			AngleSet[5] = ((float)joint6Set.d / 1000.0);
			AngleSet[6] = (float)receiveBuff[15];
			//-----------------------------------------------------------------
			// 完成一个数据包的接收，相关变量清零，等待下一字节数据
			USARTBufferIndex = 0;
			USARTReceiverFront = 0;
			Start_Flag = START;
			checkSum = 0;
			dataLength = 0;
			j = 0;
			k = 0;
			//-----------------------------------------------------------------
		}
		else
		{
			for (u8 i = 0; i < 19; i++)
				receiveBuff[i] = 0; // 清除接收缓存
			// 校验和错误，清除接收缓存
			USARTBufferIndex = 0;
			USARTReceiverFront = 0;
			Start_Flag = START;
			checkSum = 0;
			dataLength = 0;
			j = 0;
			k = 0;
			return 0; // 返回错误
		}
	}

	
	return 0;
}
/**************************************************************************

**************************************************************************/
void usartSendData(UART_HandleTypeDef *huart)
{
	// 协议数据缓存数组
	unsigned char buf[13] = {0};
	int i, length = 0;
	length = 13;
	// 计算joint角度
	joint11.d = (short)(joint1.now_angle / radio[0] * 1000.0);
	joint21.d = (short)(joint2.now_angle / radio[1] * 1000.0);
	joint31.d = (short)(joint3.now_angle / radio[2] * 1000.0);
	joint41.d = (short)(joint4.now_angle / radio[3] * 1000.0);
	joint51.d = (short)(joint5.now_angle / radio[4] * 1000.0);
	joint61.d = (short)(joint6.now_angle / radio[5] * 1000.0);

	// joint11.d = (short)(1);
	// joint21.d = (short)(122);
	// joint31.d = (short)(1223);
	// joint41.d = (short)(1234);
	// joint51.d = (short)(145);
	// joint61.d = (short)(145);
	// 设置
	for (i = 0; i < 2; i++)
		buf[i] = header[i]; // buf[0] buf[1]

	buf[2] = length; // buf[2]
	for (i = 0; i < 2; i++)
	{
		buf[i + 3] = joint11.data[i];  // buf[3]  buf[4]
		buf[i + 5] = joint21.data[i];  // buf[5]  buf[6]
		buf[i + 7] = joint31.data[i];  // buf[7]  buf[8]
		buf[i + 9] = joint41.data[i];  // buf[9]  buf[10]
		buf[i + 11] = joint51.data[i]; // buf[11] buf[12]
		buf[i + 13] = joint61.data[i]; // buf[13] buf[14]
	}
	// buf[i + 7] = 					   // buf[7] buf[8]// 预留控制指令
	buf[3 + length - 1] = (u8)hand.action; // 夹爪

	// 设置校验值、消息尾
	buf[3 + length] = getCrc8(buf, 3 + length); // buf[10]
	buf[3 + length + 1] = ender[0];				// buf[11]
	buf[3 + length + 2] = ender[1];				// buf[12]

	HAL_UART_Transmit(huart, (uint8_t *)buf, 19, 0xf);
}
/**************************************************************************
函数功能：发送指定大小的字符数组，被usartSendData函数调用
入口参数：数组地址、数组大小
返回  值：无
**************************************************************************/

/**************************************************************************
函数功能：计算八位循环冗余校验，被usartSendData和usartReceiveOneData函数调用
入口参数：数组地址、数组大小
返回  值：无
**************************************************************************/
unsigned char getCrc8(unsigned char *ptr, unsigned short len)
{
	unsigned char crc;
	unsigned char i;
	crc = 0;
	while (len--)
	{
		crc ^= *ptr++;
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x01)
				crc = (crc >> 1) ^ 0x8C;
			else
				crc >>= 1;
		}
	}
	return crc;
}
/**********************************END***************************************/
