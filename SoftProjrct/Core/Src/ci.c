#include "ci.h"
#include  "main.h"
#include "myiic.h"
#include "iic_reg.h"

float Get_Angle(void)
{
    uint8_t data[2]={0};
    uint64_t angle_h = 0;
    uint64_t angle_l = 0;
    uint64_t angle = 0;
    float res = 0;
    IIC_Read(CI_Dev, CI_Angle_H, &data[0], 1);
    IIC_Read(CI_Dev, CI_Angle_L, &data[1], 1);
    data[1] = data[1] >> 2;
    angle_h=data[0]<<6;
    angle_l = data[1];
    angle = angle_h + angle_l;
    res = angle / 16384.f * 360.f;
    return res;
}