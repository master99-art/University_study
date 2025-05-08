#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"
#include "pid.h"
#include "control.h"
void Motor_Init(void);
void Motor_Set(int16_t Compare1,int16_t Compare2);
int16_t Get_Motor_Dir_left(int16_t temp_speed);
int16_t Get_Motor_Dir_right(int16_t temp_speed);

//电机PWM变量
extern int16_t    motorLeft,motorRight;                      
//速度
extern int16_t    leftSpeedNow; 
extern int16_t    rightSpeedNow; 
//乘以1000之后的速度设定值
extern int16_t    leftSpeedSet; 
extern int16_t    rightSpeedSet; 

//舵机PWM变量
extern int16_t motorFrontSteer;    
//舵机控制角度的设定值
extern int16_t frontAngleSet;

// * -------------左右轮电机---------------
#define PWMB1   TIM2->CCR4    //PA3
#define PWMB2   TIM2->CCR2    //PA1
#define PWMA1   TIM2->CCR3    //PA2
#define PWMA2   TIM2->CCR1    //PA0 

// * -------------舵机---------------------
#define PWMC    TIM1->CCR2    //PB0

void Steer_Ctrl(int16_t frontAngleSet,int16_t *motorFrontSteer);
void Set_Pwm(int16_t motorLeft,int16_t motorRight,int16_t motorFrontSteer);

u8 Turn_Off(int16_t voltage);
int16_t myabs(int16_t a);
#endif



