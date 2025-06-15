#ifndef __DATA_CAL_H__
#define __DATA_CAL_H__
#include "main.h"

extern unsigned char sendCtrlFlag;
extern unsigned char receCtrlFlag;

#define START   0X11

//从linux接收并解析数据到参数地址中
extern int usartReceiveOneData(UART_HandleTypeDef huart,int16_t *p_leftSpeedSet,int16_t *p_rightSpeedSet,int16_t *p_frontSteerAngleSet,unsigned char *p_crtlFlag);   
//封装数据，调用USART1_Send_String将数据发送给linux
extern void usartSendData(UART_HandleTypeDef huart,short leftVel, short rightVel,short angle,unsigned char ctrlFlag); 
   
//计算八位循环冗余校验，得到校验值，一定程度上验证数据的正确性
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 

#endif /* __TIM_H__ */