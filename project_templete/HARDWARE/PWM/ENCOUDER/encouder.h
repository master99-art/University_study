#ifndef __ENCOUDER_H
#define __ENCOUDER_H
#include "stm32f10x.h"

void Encouder_Init(void);
int16_t Encoder_Get_L(void);
int16_t Encoder_Get_R(void);

int16_t Get_Speed_left(void);
int16_t Get_Speed_right(void);

#endif


