#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void MPU_Init(void);
void MPU_W_Reg(u8 RegAddress, u8 Data);
u8 MPU_R_Reg(u8 RegAddress);


void MPU_Get_Data(float *AX, float *AY, float *AZ,
												float *GX, float *GY, float *GZ);

void MPU_Cla(float *AX, float *AY, float *AZ,
												float *GX, float *GY, float *GZ,float* yaw,float* roll ,float* pitch);


#endif

