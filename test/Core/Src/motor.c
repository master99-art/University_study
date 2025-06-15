#include "motor.h"

#define per_step 0.45

/*
the motor is 1/4 ,need 800 per 360 angle 
step 		is 0.45 angle
speed 	is 15 	angle/s
change 	to 33 	step/s
*/

enum dir{
	Forward=1,
	Reverse=0
}dir1,dir2,dir3,dir4;

struct joint{
	float tar_angle;
	float now_angle;
	float count_step;
	
}joint1,joint2,joint3,joint4,joint5,joint6;

struct joint
	

void Motor_Init(void)
{
	dir1=Forward,dir2=Forward,dir3=Forward,dir4=Forward;
	joint1.tar_angle=0;
	joint1.tar_angle=0;
	joint1.count_step=0;

	joint2.tar_angle=0;
	joint2.tar_angle=0;
	joint2.count_step=0;

	joint3.tar_angle=0;
	joint3.tar_angle=0;
	joint3.count_step=0;

	joint4.tar_angle=0;
	joint4.tar_angle=0;
	joint4.count_step=0;

	joint5.tar_angle=0;
	joint5.tar_angle=0;
	joint5.count_step=0;

	joint6.tar_angle=0;
	joint6.tar_angle=0;
	joint6.count_step=0;
	
	
}

void motor_cal(float* all_angel,)
	