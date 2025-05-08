#include "mpu6050.h"
#include "stm32f10x.h"
#include "mpu6050_reg.h"
#include "math.h"
#include "Delay.h"
#define MPU6050_ADDRESS 0XD0

const float alpha = 0.95238;

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//给定超时计数时间
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//循环等待指定事件
	{
		Timeout --;										//等待时，计数值自减
		if (Timeout == 0)								//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;										//跳出等待，不等了
		}
	}
}


void MPU_W_Reg(u8 RegAddress, u8 Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//等待EV8
	
	I2C_SendData(I2C2, Data);												//硬件I2C发送数据
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);											//硬件I2C生成终止条件
	
}

u8 MPU_R_Reg(u8 RegAddress)
{
	u8 Data;
	I2C_GenerateSTART(I2C2, ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT ) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS,I2C_Direction_Transmitter );
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) != SUCCESS);
	
	I2C_SendData(I2C2, RegAddress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED ) != SUCCESS);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT ) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS,I2C_Direction_Receiver );
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) != SUCCESS);
		
	I2C_AcknowledgeConfig(  I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED ) != SUCCESS);
	
	Data = I2C_ReceiveData( I2C2);
	I2C_AcknowledgeConfig(  I2C2, ENABLE);
	
	return Data;
	
}

void MPU_Init(void)    //B10 B11
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);		//开启I2C2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PB10和PB11引脚初始化为复用开漏输出
	
	/*I2C初始化*/
	I2C_InitTypeDef I2C_InitStructure;						//定义结构体变量
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//模式，选择为I2C模式
	I2C_InitStructure.I2C_ClockSpeed = 50000;				//时钟速度，选择为50KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//时钟占空比，选择Tlow/Thigh = 2
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//应答，选择使能
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//应答地址，选择7位，从机模式下才有效
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;				//自身地址，从机模式下才有效
	I2C_Init(I2C2, &I2C_InitStructure);						//将结构体变量交给I2C_Init，配置I2C2
	
	/*I2C使能*/
	I2C_Cmd(I2C2, ENABLE);									//使能I2C2，开始运行
	
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU_W_Reg(MPU6050_PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU_W_Reg(MPU6050_PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU_W_Reg(MPU6050_SMPLRT_DIV, 0x04);				//采样率分频寄存器，配置采样率200HZ
	MPU_W_Reg(MPU6050_CONFIG, 0x06);					//配置寄存器，配置DLPF
	MPU_W_Reg(MPU6050_GYRO_CONFIG, 0x18);			//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU_W_Reg(MPU6050_ACCEL_CONFIG, 0x08);			//加速度计配置寄存器，选择满量程为±4g
	

}



void MPU_Get_Data(float *AX, float *AY, float *AZ,
												float *GX, float *GY, float *GZ)
{
	int16_t AccX; 
	int16_t AccY;
	int16_t AccZ;											
	int16_t GyroX; 
	int16_t GyroY; 
	int16_t GyroZ;
	
	u8 DataH,DataL;
	
	DataH = MPU_R_Reg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU_R_Reg(MPU6050_ACCEL_XOUT_L);
	
	AccX = (DataH<<8) | DataL;
	*AX = AccX * 1.0 * 8 / 65535;
	
	DataH = MPU_R_Reg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU_R_Reg(MPU6050_ACCEL_YOUT_L);
	
	AccY = (DataH<<8) | DataL;	
	*AY = AccY * 1.0 * 8 / 65535;
	
	DataH = MPU_R_Reg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU_R_Reg(MPU6050_ACCEL_ZOUT_L);
	
	AccZ = (DataH<<8) | DataL;
	*AZ = AccZ * 1.0 * 8 / 65535;	
	
	DataH = MPU_R_Reg(MPU6050_GYRO_XOUT_H);
	DataL = MPU_R_Reg(MPU6050_GYRO_XOUT_L);
	
	GyroX  = (DataH<<8) | DataL;	
	*GX = GyroX * 1.0 * 4000 / 65535;  ///////////////??±??á????
	
	DataH = MPU_R_Reg(MPU6050_GYRO_YOUT_H);
	DataL = MPU_R_Reg(MPU6050_GYRO_YOUT_L);
	
	GyroY  = (DataH<<8) | DataL;		
	*GY = GyroY * 1.0 * 4000 / 65535;
	
	DataH = MPU_R_Reg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU_R_Reg(MPU6050_GYRO_ZOUT_L);
	
	GyroZ  = (DataH<<8) | DataL;		
	*GZ = GyroZ * 1.0 * 4000 / 65535;
	
}




void MPU_Cla(float *AX, float *AY, float *AZ,
												float *GX, float *GY, float *GZ,float* yaw,float* roll ,float* pitch)
{
	float roll_a,pitch_a;
	float yaw_g,roll_g,pitch_g;
	
	
	roll_a = atan2(*AY,*AZ) * 180 / 3.141593 ;
	pitch_a = -atan2(*AX,*AZ) * 180 / 3.141593 ;
	
	
	roll_g = (*roll) + (*GX) * 0.005;
	yaw_g = (*yaw) + (*GZ) * 0.0146;
	pitch_g = (*pitch) + (*GY) * 0.005;
	
	*yaw =  yaw_g ;
	*roll = alpha * roll_g + ( 1 - alpha ) * roll_a;
	*pitch = alpha * pitch_g + ( 1 - alpha ) * pitch_a;
	
	
	
	
}















