#include "motor.h"
#include "package.h"


/*
the motor is 1/4 ,need 800 per 360 angle 
step 		is 0.45 angle    0.00785375 rad
speed 	is 15 	angle/s
change_speed 	to 33 	step/s

the radio of stepper motor
joint1 		7:1
joint2		6:1
joint3 		??
joint4		4.5:1
joint5,6	6:1

joint7 or hand 
*/

float radio[6]={7,6,0,4.5,6,6};
struct joint joint1,joint2,joint3,joint4,joint5,joint6;
struct hand hand;






	

void Motor_Init(void)
{
	
	joint1.tar_angle=0;
	joint1.now_angle=0;
	joint1.count_step=0;
	joint1.dir=Forward;
	joint1.isreset=Unreset;

	joint2.tar_angle=0;
	joint2.now_angle=0;
	joint2.count_step=0;
	joint2.dir=Forward;
	joint2.isreset=Unreset;

	joint3.tar_angle=0;
	joint3.now_angle=0;
	joint3.count_step=0;
	joint3.dir=Forward;
	joint3.isreset=Unreset;

	joint4.tar_angle=0;
	joint4.now_angle=0;
	joint4.count_step=0;
	joint4.dir=Forward;
	joint4.isreset=Unreset;

	joint5.tar_angle=0;
	joint5.now_angle=0;
	joint5.count_step=0;
	joint5.dir=Forward;
	joint5.isreset=Unreset;

	joint6.tar_angle=0;
	joint6.now_angle=0;
	joint6.count_step=0;
	joint6.dir=Forward;
	joint6.isreset=Unreset;
	
	hand.action=OFF;
	hand.limit =Unlimit;
	hand.angle=0;
	
	
}

//我现在只知道机器臂角度值，然后我想知道步进电机值的时候我只需要乘以减速比就可以了
/*
function:计算所需要的步长，设置方向，

//enter paramter : angle of robotic arm and action of hand
//rad
*/
int* motor_cal(float* tar_all_angle)
{
	float radio[6]={7,6,0,4.5,6,6};
	float tar_angle[6]={0};
	unsigned char end_action;
	
	//电机角度
	for(uint8_t i=0;i<6;i++)
	{
		//is angle before radio ,is the factly angle of stepper motor
		tar_angle[i]=tar_all_angle[i]*radio[i];
	}
	end_action=(uint8_t)tar_all_angle[6];
	float error_angle[6]={0};
	error_angle[0] = tar_angle[0] - joint1.now_angle;
	error_angle[1] = tar_angle[1] - joint2.now_angle;
	error_angle[2] = tar_angle[2] - joint3.now_angle;
	error_angle[3] = tar_angle[3] - joint4.now_angle;
	error_angle[4] = tar_angle[4] - joint5.now_angle;
	error_angle[5] = tar_angle[5] - joint6.now_angle;
	
	if(error_angle[0] <= 0) joint1.dir = Reverse;
		else joint1.dir = Forward;
	if(error_angle[1] <= 0) joint2.dir = Reverse;
		else joint2.dir = Forward;
	if(error_angle[2] <= 0) joint3.dir = Reverse;
		else joint3.dir = Forward;
	if(error_angle[3] <= 0) joint4.dir = Reverse;
		else joint4.dir = Forward;
	if(error_angle[4] <= 0) joint5.dir = Reverse;
		else joint5.dir = Forward;
	if(error_angle[5] <= 0) joint6.dir = Reverse;
		else joint6.dir = Forward;
	
	
	static int step[7]={0};
	for(uint8_t i=0;i<6;i++)
	{
		step[i]=(int)((int)error_angle[i]/Angle2Step);
	}
	step[6]=(int)end_action;
	joint1.now_angle = tar_angle[0];
	joint2.now_angle = tar_angle[1];
	joint3.now_angle = tar_angle[2];
	joint4.now_angle = tar_angle[3];
	joint5.now_angle = tar_angle[4];
	joint6.now_angle = tar_angle[5];
	
	return step;
}

/*
joint1 G13
joint2 G12
joint3 G11
joint4 G10
joint5 G9
joint6 D7
*/
/*
复位机械臂关节，让机械臂最开始可以进行复位
*/
u8 Reset_joint(void)
{
	//joint1->joint6
	//hand is completely open
	//first we move joint1 without stopping,then one by one
	//现在我需要知道是否有机械限位从1->6,如果没有我就可以直接使用一个一个遍历的模式，
	//矫正角度为0
	
	//方向全部为正
	Dir_Set(DIR1_GPIO_Port,DIR1_Pin,(GPIO_PinState)joint1.dir);
	Dir_Set(DIR2_GPIO_Port,DIR2_Pin,(GPIO_PinState)joint2.dir);
	Dir_Set(DIR3_GPIO_Port,DIR3_Pin,(GPIO_PinState)joint3.dir);
	Dir_Set(DIR4_GPIO_Port,DIR4_Pin,(GPIO_PinState)joint4.dir);
	Dir_Set(DIR5_GPIO_Port,DIR5_Pin,(GPIO_PinState)joint5.dir);
	Dir_Set(DIR6_GPIO_Port,DIR6_Pin,(GPIO_PinState)joint6.dir);
	
	int step=1;
	while(1)
	{
		Motor_Action(joint1_GPIO_Port,joint1_Pin,step);
		if(Read_KEY(K1_GPIO_Port,K1_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K1_GPIO_Port,K1_Pin) == GPIO_PIN_RESET)
			{
				joint1.isreset=Reset;
				break;
			}
		}
	}

	while(1)
	{
		Motor_Action(joint2_GPIO_Port,joint2_Pin,step);
		if(Read_KEY(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET)
			{
				joint2.isreset=Reset;
				break;
			}
		}
	}
	
	while(1)
	{
		Motor_Action(joint3_GPIO_Port,joint3_Pin,step);
		if(Read_KEY(K3_GPIO_Port,K3_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K3_GPIO_Port,K3_Pin) == GPIO_PIN_RESET)
			{
				joint3.isreset=Reset;
				break;
			}
		}
	}
	
	while(1)
	{
		Motor_Action(joint4_GPIO_Port,joint4_Pin,step);
		if(Read_KEY(K4_GPIO_Port,K4_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K4_GPIO_Port,K4_Pin) == GPIO_PIN_RESET)
			{
				joint4.isreset=Reset;
				break;
			}
		}
	}

	while(1)
	{
		Motor_Action(joint5_GPIO_Port,joint5_Pin,step);
		if(Read_KEY(K5_GPIO_Port,K5_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K5_GPIO_Port,K5_Pin) == GPIO_PIN_RESET)
			{
				joint5.isreset=Reset;
				break;
			}
		}
	}
	
	while(1)
	{
		Motor_Action(joint6_GPIO_Port,joint6_Pin,step);
		if(Read_KEY(K6_GPIO_Port,K6_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K6_GPIO_Port,K6_Pin) == GPIO_PIN_RESET)
			{
				joint6.isreset=Reset;
				break;
			}
		}
	}
	/* 
	@this is a servo debug ,so we should know how to control the servo
	
	while(1)
	{
		Motor_Action(joint2_GPIO_Port,joint2_Pin,step);
		if(Read_KEY(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET)
		{
			HAL_Delay(20);
			if(Read_KEY(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET)
			{
				joint2.isreset=Reset;
				break;
			}
		}
	}
	*/
	return OK;
}

//方向驱动
void Motor_Dir(void)
{
	Dir_Set(joint1_GPIO_Port,joint1_Pin,(GPIO_PinState)joint1.dir);
	Dir_Set(joint2_GPIO_Port,joint2_Pin,(GPIO_PinState)joint2.dir);
	Dir_Set(joint3_GPIO_Port,joint3_Pin,(GPIO_PinState)joint3.dir);
	Dir_Set(joint4_GPIO_Port,joint4_Pin,(GPIO_PinState)joint4.dir);
	Dir_Set(joint5_GPIO_Port,joint5_Pin,(GPIO_PinState)joint5.dir);
	Dir_Set(joint6_GPIO_Port,joint6_Pin,(GPIO_PinState)joint6.dir);
}

//电机驱动，没有考虑方向
u8 Motor_Move(int* step)
{
	Motor_Action(joint1_GPIO_Port,joint1_Pin,step[0]);
	Motor_Action(joint2_GPIO_Port,joint2_Pin,step[1]);
	Motor_Action(joint3_GPIO_Port,joint3_Pin,step[2]);
	Motor_Action(joint4_GPIO_Port,joint4_Pin,step[3]);
	Motor_Action(joint5_GPIO_Port,joint5_Pin,step[4]);
	Motor_Action(joint6_GPIO_Port,joint6_Pin,step[5]);
	if(hand.action == ON)
	{
		//夹取
	}
	else if(hand.action != OFF)
	{
		//放手
	}
	return OK;
}

//方向底层
u8 Dir_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState Dir)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin,Dir);
	return OK;
}
//电机驱动的底层，没有考虑方向
u8 Motor_Action(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,int Step)
{
	for(int i = 0 ; i < Step ; i++)
	{
		HIGH(GPIOx,GPIO_Pin);
		HAL_Delay(10);
		LOW(GPIOx,GPIO_Pin);
		HAL_Delay(10);		
	}
	return OK;
}


