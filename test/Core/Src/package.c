#include "package.h"
#include "motor.h"



GPIO_PinState Read_KEY(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}


void HIGH(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
}
void LOW(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
}