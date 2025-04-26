#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"  
#include <stdio.h>

void USART1_Init(void);
void USART1_SendByte(uint8_t Byte);
void USART1_SendString(char *String);
void USART1_Printf(char *format, ...);
uint8_t USART1_GetRxData(void);
void USART2_Init(void);
void USART3_Init(void);
void USART4_Init(void);


void yuying(u8 *data);


#endif
