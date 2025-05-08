#include "stm32f10x.h"
#include "pid.h"
#include "mpu6050.h"
#include "Control.h"
#include "motor.h"
#include "encouder.h"
#include "oled.h"

//这是yaw角度飘逸问题，计算其一分钟的误差然后转换为20ms的
#define ERROR_YAW   0.11			


float AX ;
float AY ;
float AZ ;

float GX ;
float GY ;
float GZ ;
float yaw;
float roll;
float pitch;



//所有的计算之类的都会在这个里面
//20ms计算一次
void TIM2_IRQHandler(void)
{
	u8 a=1;
	//获取Yaw
	MPU_Get_Data(&AX,&AY,&AZ,&GX,&GY,&GZ);
	MPU_Cla(&AX, &AY, &AZ, &GX, &GY, &GZ, &yaw, &roll ,&pitch);		
	OLED_ShowSignedNum(1,1,(int16_t)yaw,5);
	//获取速度   AND 		正负号
	leftSpeedNow  = Get_Speed_left();
	rightSpeedNow = Get_Speed_right();
	pid_Task_Letf.speedNow  = leftSpeedNow  ;
	pid_Task_Right.speedNow = rightSpeedNow ;
	pid_Task_Letf.speedSet  = leftSpeedSet  ;
	pid_Task_Right.speedSet = rightSpeedSet ;
	
	//pid运算	//设置舵机角度
	Pid_Ctrl( &motorLeft,&motorRight);
	Steer_Ctrl(frontAngleSet,&motorFrontSteer);
	Set_Pwm(motorLeft,motorRight,motorFrontSteer);
	
	//电量
	
	
	

	
	
	
	
	
	
}
