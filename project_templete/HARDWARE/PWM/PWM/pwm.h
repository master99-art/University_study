#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"
void PWM_Init(void);
void PWM_L(uint16_t Compare);
void PWM_R(uint16_t Compare);

void Servo_SetAngle(float Angle);
#endif
