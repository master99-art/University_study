#include "stm32f10x.h"
#include "motor.h"
#include "pid.h"


void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF,ENABLE);
	
		/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure1;					//定义结构体变量
	
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6;				
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	
	GPIO_Init(GPIOE, &GPIO_InitStructure1);	


	//L
	GPIO_SetBits(GPIOE, GPIO_Pin_2);
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);
	
	//R
	GPIO_SetBits(GPIOE, GPIO_Pin_4);
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
}

int16_t Get_Motor_Dir_left(int16_t temp_speed)
{
	int16_t temp=(int16_t)temp_speed;
	return	temp;
}

int16_t Get_Motor_Dir_right(int16_t temp_speed)
{
	int temp=(int16_t)temp_speed;
	return temp;
}

/*******************
作用：控制电机正负极反转
********************/

void Motor_Set(int16_t Compare1,int16_t Compare2)
{
	//L
	if(Compare1>=0)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_2);
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
		
	}
	else
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_3);
		GPIO_ResetBits(GPIOE, GPIO_Pin_2);		
	}
	
	
	//R
	if(Compare2>=0)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_4);
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);		
	}
	else 
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		GPIO_ResetBits(GPIOE, GPIO_Pin_4);			
	}

}



// * ----------------------左右电机--------------------------
//左右轮电机PWM变量
int16_t motorLeft     = 0;
int16_t motorRight    = 0;         

//乘以1000之后的速度实时值
int16_t leftSpeedNow  = 0; 
int16_t rightSpeedNow = 0; 

//乘以1000之后的速度设定值
int16_t leftSpeedSet  = 0; 
int16_t rightSpeedSet = 0; 

// * ----------------------前轮舵机--------------------------
//舵机PWM变量
int16_t motorFrontSteer = 4500; // (60000 / 20ms) * 1.5ms = 4500 90度

//舵机控制角度的设定值
int16_t frontAngleSet = 0;

// 物理结构限幅
#define MAX_FRONT_ANGLE_SET (50)

/**************************************************************************
函数功能：舵机角度控制处理（SG90）角度线性变化
入口参数：ros端设定转向角度，需要给舵机的pwm，此处可根据不同的舵机自行更改
分辨率：1度 --> ((2.5ms - 0.5ms) / 180度) * (60000 / 20ms) = 33.3
0.5ms ------------------ 0度
1.5ms ------------------ 90度
2.5ms ------------------ 180度
返回  值：无
**************************************************************************/
void Steer_Ctrl(int16_t frontAngleSet,int16_t *motorFrontSteer)
{
	// 物理结构限幅
	if(frontAngleSet > MAX_FRONT_ANGLE_SET)
	{
		frontAngleSet = MAX_FRONT_ANGLE_SET;
	}
	if(frontAngleSet < -MAX_FRONT_ANGLE_SET)
	{
		frontAngleSet = -MAX_FRONT_ANGLE_SET;
	}
	// 正常计算角度
	if(frontAngleSet == 0) //默认初始角度 90度
	{
		*motorFrontSteer = 4500; // (60000 / 20ms) * 1.5ms = 4500 
	}
	else if(frontAngleSet > 0) // left
	{
		*motorFrontSteer = 4500 - (int16_t)(myabs(frontAngleSet) * 33.3 + 0.5);
	}
	else                       //right
	{
		*motorFrontSteer = 4500 + (int16_t)(myabs(frontAngleSet) * 33.3 + 0.5);
	}
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM、舵机PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int16_t motorLeft,int16_t motorRight,int16_t motorFrontSteer)
{
	// * ------------左右电机部分--------------
	
	Motor_Set(motorLeft,motorRight);
	TIM_SetCompare1(TIM5,myabs(motorLeft));
	TIM_SetCompare2(TIM5,myabs(motorRight));


	// * ------------舵机控制部分--------------
	TIM_SetCompare1(TIM4,motorFrontSteer);
}


/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}












