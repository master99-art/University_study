#include "calculate.h"
#include "freertos.h"
#include "ci.h"
#include "main.h"

ANGLEDATA AngleData;

void AngleDataInit(void)
{
    AngleData.speed = 0;
    AngleData.angle = 0;
    AngleData.last_angle = 0;
    AngleData.error_angle = 0;
    AngleData.all_angle = 0;
    AngleData.circle = 0;
    AngleData.temp_circle = 0;
    AngleData.circle_flag = 0;
}

void AngleDataUpdate(ANGLEDATA *AngleData)
{
    float last_all_angle = 0;
    AngleData->angle = Get_Angle();
    last_all_angle = AngleData->all_angle;
    AngleData->error_angle = AngleData->angle - AngleData->last_angle;

    if (AngleData->error_angle >= 300)
    {
        // 这种就是向反方向刷新圈数
        AngleData->circle_flag = 1;
        AngleData->temp_circle -= 1;
    }
    else if (AngleData->error_angle <= -300)
    {
        // 这种就是向正方向刷新圈数
        AngleData->circle_flag = 1;
        AngleData->temp_circle += 1;
    }

    if (AngleData->temp_circle < 0)
        AngleData->circle = AngleData->temp_circle + 1;
    else if (AngleData->temp_circle >= 0)
        AngleData->circle = AngleData->temp_circle;

    if (AngleData->circle_flag == 0)
    {
        AngleData->all_angle += AngleData->error_angle;
        AngleData->circle_flag = 0;
    }
    else if (AngleData->circle_flag == 1)
    {
        // 反方向
        if (AngleData->error_angle >= 0)
        {
            AngleData->error_angle = 360 - AngleData->error_angle;
            AngleData->all_angle -= AngleData->error_angle;
        }
        else if (AngleData->error_angle < 0)
        {
            AngleData->error_angle = 360 + AngleData->error_angle;
            AngleData->all_angle += AngleData->error_angle;
        }
        AngleData->circle_flag = 0;
    }
    AngleData->last_angle = AngleData->angle;
    AngleData->speed = (AngleData->all_angle - last_all_angle) / 0.01f; // 10ms采样一次
}