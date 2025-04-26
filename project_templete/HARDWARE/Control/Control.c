#include "stm32f10x.h"
#include "pid.h"
#include "mpu6050.h"
#include "Control.h"
#include "motor.h"
#include "encouder.h"



#define ERROR_YAW   0.11
float Yaw=0;

//所有的计算之类的都会在这个里面
//20ms计算一次
void TIM2_IRQHandler(void)
{

	//获取Yaw
	Yaw=Get_Yaw(Yaw);
	//mpu累计误差
	Yaw+=ERROR_YAW;

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
