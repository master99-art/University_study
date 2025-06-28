#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

struct joint
{
	u8 name;
	float tar_angle;
	float now_angle;
	float count_step;
	enum
	{
		Forward = 1,
		Reverse = 0
	} dir;
	enum
	{
		Reset = 1,
		Unreset = 0
	} isreset;
};

struct hand
{
	u8 name;
	enum
	{
		ON = 1,
		OFF = 0
	} action;
	enum
	{
		Islimit = 1,
		Unlimit = 0
	} limit;
	float angle;
};

extern struct joint joint1, joint2, joint3,
	joint4, joint5, joint6;
extern struct hand hand;
extern float radio[6];

void Motor_Init(void);
void motor_cal(float *tar_all_angle, int *step);
u8 Reset_joint(void);
u8 Motor_Move(int *step);
u8 Motor_Action(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, int Step);

u8 Dir_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState Dir);
void Motor_Dir(void);
#endif
