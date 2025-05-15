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
	

	

/////////////////////////////////////////
/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA6和PA7引脚初始化为上拉输入
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元
	
	/*输入捕获初始化*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//定义结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure);							//结构体初始化，若结构体没有完整赋值
																	//则最好执行此函数，给结构体所有成员都赋一个默认值
																	//避免结构体初值不确定的问题
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//选择配置定时器通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//选择配置定时器通道2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
	
	/*编码器接口配置*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//配置编码器模式以及两个输入通道是否反相
																	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
																	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行	
///////////////////////////////////////////////////////////////////////////
	


///////////////////////////////////////////////////////////////////////////
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure2);							


	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure2;			
	TIM_TimeBaseInitStructure2.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseInitStructure2.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure2.TIM_Period = 65536 - 1;               
	TIM_TimeBaseInitStructure2.TIM_Prescaler = 1 - 1;                
	TIM_TimeBaseInitStructure2.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure2);           
	
	
	TIM_ICInitTypeDef TIM_ICInitStructure2;							
	TIM_ICStructInit(&TIM_ICInitStructure2);
	
	TIM_ICInitStructure2.TIM_Channel = TIM_Channel_1;				
	TIM_ICInitStructure2.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM8, &TIM_ICInitStructure2);							
	TIM_ICInitStructure2.TIM_Channel = TIM_Channel_2;			
	TIM_ICInitStructure2.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM8, &TIM_ICInitStructure2);							
	
	
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																

	TIM_Cmd(TIM8, ENABLE);		
	///////////////////////////////////////////////////////////////////////////
}




/***************************************************************
函数作用获取速度
单位									：mm/s

轮子半径								：65/2	mm
周长 									：65*PI	mm

//轮子 左
轮子转一圈输出的脉冲数	：1560 1569 1568	average 1566											
一个脉冲走的距离				：0.000130398 = 65 * PI / 1566 * 0.001	m
分辨率									：0.0065199 m/s = 0.000130398 /（20*0.001） 

//轮子 右
轮子转一圈输出的脉冲数	：1543 1577 1560 	average 1560									
一个脉冲走的距离				：0.000130899 = 65 * PI / 1560 * 0.001	m
分辨率									：0.0065449 m/s = 0.0003925 /（20*0.001） 

之后我就只需要找到脉冲值然后乘以分辨率就得到实际速度了
单位时间								：20ms

速度公式								：脉冲数*速度分辨率


****************************************************************/

//返回带有正确符号的speed
int16_t Get_Speed_left(void)
{
	int16_t temp_speed=0;
	//		mm/s
	temp_speed = TIM_GetCounter(TIM3) * 0.0065199 * 1000;
	TIM_SetCounter(TIM3, 0);
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






















































