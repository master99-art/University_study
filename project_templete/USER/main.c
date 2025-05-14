#include "stm32f10x.h"
#include "pwm.h"
#include "Delay.h"
#include "encouder.h"
#include "pid.h"
#include "motor.h"
#include "math.h"
#include "oled.h"
#include "mpu6050.h"
#include "usart.h"
#include <stm32f10x_tim.h>
#include "Cla.h"
#include "timer.h"




void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);
int main (void)
{
	MY_NVIC_PriorityGroupConfig(2);
	//LED,,
	
	

	
	//电机舵机初始化
	PWM_Init();
	Motor_Init();
	
	//编码器初始化
	Encouder_Init();
	
	//OLED初始化
	OLED_Init();

	//MPU初始化
	MPU_Init();
	

	
	//语音播报初始化
	USART1_Init();
	USART2_Init();
	USART3_Init();
	USART4_Init();
	
	
	//定时器初始化
	Timer_Init();
	
	//不停的向上位机发送数据
	u8 tik=0;
	while(1)
	{
		Servo_SetAngle(140);

		if(tik == 0)
		{
			//yaw扩大了50倍
			usartSendData(USART2,(short)pid_Task_Letf.speedNow,(short)pid_Task_Right.speedNow,(short)yaw*50,0);
			tik++;
		}
		else
		{
			tik++;
			if(tik==60)tik=0;
		}
			



	}
	


	
}
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}






void USART2_IRQHandler(void)
{
	
	static u8 i = 0 ;
	static u8 Data[16]= { 0 };
	static u8 RxState=0;
	u8 data=0;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	
	{
		usartReceiveOneData(USART2,&leftSpeedSet,&rightSpeedSet,&frontAngleSet,&data);   
		yuying(&data);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		
		
	}
	
}



 


