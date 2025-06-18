#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "package.h"

int* motor_cal(float* tar_all_angle);
u8 Reset_joint(void);
u8 Motor_Move(int* step);
u8 Motor_Action(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,int Step);
u8 Reset_joint(void);
u8 Dir_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState Dir);
void Motor_Dir(void);
#endif 