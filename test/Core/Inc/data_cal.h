#ifndef __DATA_CAL_H__
#define __DATA_CAL_H__

#include "main.h"
extern float 				 AngleSet [7];

#define START   0X11

//从linux接收并解析数据到参数地址中
int usartReceiveOneData(u8 *receiveBuff);   
//封装数据，调用USART1_Send_String将数据发送给linux
void usartSendData(UART_HandleTypeDef* huart); 
   
//计算八位循环冗余校验，得到校验值，一定程度上验证数据的正确性
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 

#endif 

