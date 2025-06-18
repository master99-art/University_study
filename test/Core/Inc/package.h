#ifndef __PACKAGE_H
#define __PACKAGE_H
#include "main.h"

GPIO_PinState Read_KEY(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void HIGH(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LOW(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif

