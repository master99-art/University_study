#ifndef __DATA_CAL_H__
#define __DATA_CAL_H__

#include "main.h"
extern float AngleSet[7];

#define START 0X11

int usartReceiveOneData(u8 *receiveBuff, UART_HandleTypeDef *huart);

void usartSendData(UART_HandleTypeDef *huart);

unsigned char getCrc8(unsigned char *ptr, unsigned short len);

#endif
