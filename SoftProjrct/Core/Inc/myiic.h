#ifndef _MYIIC_H
#define _MYIIC_H
#include "stm32f1xx_hal.h"
#include "i2c.h"

void IIC_Send(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void IIC_Read(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void IIC_Send_men()
#endif
