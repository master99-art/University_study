#ifndef __CALCULATE_H
#define __CALCULATE_H
#include "stm32f1xx_hal.h"
typedef struct Angle{
    float speed;
    float angle;
    float last_angle;
    float error_angle;
    float all_angle;
    int circle;
    int temp_circle;
    uint8_t circle_flag;
    
} ANGLEDATA;
extern ANGLEDATA AngleData;
void AngleDataInit(void);
void AngleDataUpdate(ANGLEDATA *AngleData);
#endif // !