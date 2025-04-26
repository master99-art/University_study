#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f10x.h"
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);

void MPU6050_Get_Data(float *AX, float *AY, float *AZ,
												float *GX, float *GY, float *GZ);
void MPU6050_Cla(float *AX, float *AY, float *AZ,
						 float *GX, float *GY, float *GZ,float* yaw,float* roll ,float* pitch);
int Get_Yaw(int Yaw);
#endif
