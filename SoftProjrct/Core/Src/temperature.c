#include "main.h"
#include "temperature.h"
#include "iic_reg.h"
#include "myiic.h"

float TempertureReadLocal(void)
{
    float temp = 0;
    uint8_t data[2] = {0};
    uint8_t reg[2] = {TEMPER_Local_H, TEMPER_Local_L};
    float point = 0;
    // 读取高位
    IIC_Read(TEMPER_Devdata, reg[0], &data[0], 1);
    // 读取低位
    IIC_Read(TEMPER_Devdata, reg[1], &data[1], 1);
    temp = (float)data[0];
    point = (float)data[1] / 360.f * 0.9375;
    temp = temp + point;
    return temp;
}

float  TempertureReadRemote(void)
{
    float temp = 0;
    uint8_t data[2] = {0};
    uint8_t reg[2] = {TEMPER_Remote_H, TEMPER_Remote_L};
    float point = 0;
    // 读取高位
    IIC_Read(TEMPER_Devdata, reg[0], &data[0], 1);
    // 读取低位
    IIC_Read(TEMPER_Devdata, reg[1], &data[1], 1);
    temp = (float)data[0];
    point = (float)data[1] / 360.f * 0.9375;
    temp = temp + point;
    return temp;
}
void TempertureInit(void)
{
    uint8_t data[2];
    data[0] = TEMPER_PixAddress_WR;
    data[1] = TEMPER_Pix_00625;
    IIC_Send(TEMPER_Dev, data, 2);
}