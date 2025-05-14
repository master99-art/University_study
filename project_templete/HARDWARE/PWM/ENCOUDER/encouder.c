#include "stm32f10x.h"
#include "encouder.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include "motor.h"
/***************
L
A6-TIM3CH1
A7-TIM3CH2

R
C6-TIM8CH1
C7-TIM8CH2
****************/

void Encouder_Init(void)
{

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 ,ENABLE);
	

	


	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);							
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure1;				
	TIM_TimeBaseInitStructure1.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseInitStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure1.TIM_Period = 65536 - 1;              
	TIM_TimeBaseInitStructure1.TIM_Prescaler = 1 - 1;               
	TIM_TimeBaseInitStructure1.TIM_RepetitionCounter = 0;           
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure1);            
	
	TIM_ICInitTypeDef TIM_ICInitStructure1;							
	TIM_ICStructInit(&TIM_ICInitStructure1);						
																	
																	
	TIM_ICInitStructure1.TIM_Channel = TIM_Channel_1;		
	TIM_ICInitStructure1.TIM_ICFilter = 0xF;						
	TIM_ICInit(TIM3, &TIM_ICInitStructure1);						
	TIM_ICInitStructure1.TIM_Channel = TIM_Channel_2;		
	TIM_ICInitStructure1.TIM_ICFilter = 0xF;						
	TIM_ICInit(TIM3, &TIM_ICInitStructure1);						
	

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);							
	TIM_Cmd(TIM3, ENABLE);		
	
	

	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure2);							


	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure2;				//ï¿½ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½ï¿½ï¿½ï¿½
	TIM_TimeBaseInitStructure2.TIM_ClockDivision = TIM_CKD_DIV1;     //Ê±ï¿½Ó·ï¿½Æµï¿½ï¿½Ñ¡ï¿½ñ²»·ï¿½Æµï¿½ï¿½ï¿½Ë²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ë²ï¿½ï¿½ï¿½Ê±ï¿½Ó£ï¿½ï¿½ï¿½Ó°ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½Ôªï¿½ï¿½ï¿½ï¿½
	TIM_TimeBaseInitStructure2.TIM_CounterMode = TIM_CounterMode_Up; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä£Ê½ï¿½ï¿½Ñ¡ï¿½ï¿½ï¿½ï¿½ï¿½Ï¼ï¿½ï¿½ï¿½
	TIM_TimeBaseInitStructure2.TIM_Period = 65536 - 1;               //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ú£ï¿½ï¿½ï¿½ARRï¿½ï¿½Öµ
	TIM_TimeBaseInitStructure2.TIM_Prescaler = 1 - 1;                //Ô¤ï¿½ï¿½Æµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½PSCï¿½ï¿½Öµ
	TIM_TimeBaseInitStructure2.TIM_RepetitionCounter = 0;            //ï¿½Ø¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß¼ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½Å»ï¿½ï¿½Ãµï¿½
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure2);             //ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½TIM_TimeBaseInitï¿½ï¿½ï¿½ï¿½ï¿½ï¿½TIM3ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½Ôª
	
	
	TIM_ICInitTypeDef TIM_ICInitStructure2;							
	TIM_ICStructInit(&TIM_ICInitStructure2);
	
	TIM_ICInitStructure2.TIM_Channel = TIM_Channel_1;				
	TIM_ICInitStructure2.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM8, &TIM_ICInitStructure2);							
	TIM_ICInitStructure2.TIM_Channel = TIM_Channel_2;			
	TIM_ICInitStructure2.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM8, &TIM_ICInitStructure2);							
	
	/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¿ï¿½ï¿½ï¿½ï¿½ï¿½*/
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																
	/*TIMÊ¹ï¿½ï¿½*/
	TIM_Cmd(TIM8, ENABLE);			//Ê¹ï¿½ï¿½TIM3ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½	
}




/***************************************************************
º¯Êý×÷ÓÃ»ñÈ¡ËÙ¶È
µ¥Î»									£ºmm/s

ÂÖ×Ó°ë¾¶								£º65/2	mm
ÖÜ³¤ 									£º65*PI	mm

//ÂÖ×Ó ×ó
ÂÖ×Ó×ªÒ»È¦Êä³öµÄÂö³åÊý	£º1560 1569 1568	average 1566											
Ò»¸öÂö³å×ßµÄ¾àÀë				£º0.000130398 = 65 * PI / 1566 * 0.001	m
·Ö±æÂÊ									£º0.0065199 m/s = 0.000130398 /£¨20*0.001£© 

//ÂÖ×Ó ÓÒ
ÂÖ×Ó×ªÒ»È¦Êä³öµÄÂö³åÊý	£º1543 1577 1560 	average 1560									
Ò»¸öÂö³å×ßµÄ¾àÀë				£º0.000130899 = 65 * PI / 1560 * 0.001	m
·Ö±æÂÊ									£º0.0065449 m/s = 0.0003925 /£¨20*0.001£© 

Ö®ºóÎÒ¾ÍÖ»ÐèÒªÕÒµ½Âö³åÖµÈ»ºó³ËÒÔ·Ö±æÂÊ¾ÍµÃµ½Êµ¼ÊËÙ¶ÈÁË
µ¥Î»Ê±¼ä								£º20ms

ËÙ¶È¹«Ê½								£ºÂö³åÊý*ËÙ¶È·Ö±æÂÊ


****************************************************************/

//·µ»Ø´øÓÐÕýÈ··ûºÅµÄspeed
int16_t Get_Speed_left(void)
{
	int16_t temp_speed=0;
	
	//		mm/s
	temp_speed = TIM_GetCounter(TIM3) * 0.0065199 * 1000;
	//TIM_SetCounter(TIM3, 0);
	return Get_Motor_Dir_left(temp_speed);	
}
int16_t Get_Speed_right(void)
{
	
	int16_t temp_speed=0;
	//		mm/s
	temp_speed = (int16_t)TIM_GetCounter(TIM8) * 0.0065449 * 1000;
	TIM_SetCounter(TIM8, 0);
	return Get_Motor_Dir_right(temp_speed);
}






















































