/*
 * TB6612.c
 *
 *  Created on: Nov 11, 2022
 *      Author: Little Fish
 */

#include "TB6612.h"
//电机PWM控制定时器
#define Motor_PWM_TIM htim3
//PWM占空比最大值
#define PWM_Count_Max (16800-1)

//电机IN引脚
GPIO_TypeDef *Motor_IN_GPIO_Ports[][2] = { { Motor_1_IN1_GPIO_Port, Motor_1_IN2_GPIO_Port }, { Motor_2_IN1_GPIO_Port, Motor_2_IN2_GPIO_Port }, { Motor_3_IN1_GPIO_Port, Motor_3_IN2_GPIO_Port }, { Motor_4_IN1_GPIO_Port, Motor_4_IN2_GPIO_Port } };
uint16_t Motor_IN_Pins[][2] = { { Motor_1_IN1_Pin, Motor_1_IN2_Pin }, { Motor_2_IN1_Pin, Motor_2_IN2_Pin }, { Motor_3_IN1_Pin, Motor_3_IN2_Pin }, { Motor_4_IN1_Pin, Motor_4_IN2_Pin } };
//电机PWM控制信号通道
uint32_t Motor_PWM_Channels[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };
//电机状态缓存
Motor_State Motor_States[] = { Motor_Stop, Motor_Stop, Motor_Stop, Motor_Stop };

//设置驱动板状态
void Set_TB6612_State()
{
	if (Motor_States[0] != Motor_Stop || Motor_States[1] != Motor_Stop)
	{
		HAL_GPIO_WritePin(Motor_F_STBY_GPIO_Port, Motor_F_STBY_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(Motor_F_STBY_GPIO_Port, Motor_F_STBY_Pin, GPIO_PIN_RESET);
	}

	if (Motor_States[2] != Motor_Stop || Motor_States[3] != Motor_Stop)
	{
		HAL_GPIO_WritePin(Motor_B_STBY_GPIO_Port, Motor_B_STBY_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(Motor_B_STBY_GPIO_Port, Motor_B_STBY_Pin, GPIO_PIN_RESET);
	}
}

//设置单个电机状态
void SetMotorState(Motor_Name motor, Motor_State state)
{
	//缓存
	Motor_States[motor] = state;

	switch (state)
	{
		case Motor_Forward:
			HAL_TIM_PWM_Start(&Motor_PWM_TIM, Motor_PWM_Channels[motor]);
			HAL_GPIO_WritePin(Motor_IN_GPIO_Ports[motor][0], Motor_IN_Pins[motor][0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(Motor_IN_GPIO_Ports[motor][1], Motor_IN_Pins[motor][1], GPIO_PIN_RESET);
			break;
		case Motor_Backward:
			HAL_TIM_PWM_Start(&Motor_PWM_TIM, Motor_PWM_Channels[motor]);
			HAL_GPIO_WritePin(Motor_IN_GPIO_Ports[motor][0], Motor_IN_Pins[motor][0], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Motor_IN_GPIO_Ports[motor][1], Motor_IN_Pins[motor][1], GPIO_PIN_SET);
			break;
		case Motor_Stop:
			HAL_TIM_PWM_Stop(&Motor_PWM_TIM, Motor_PWM_Channels[motor]);
			HAL_GPIO_WritePin(Motor_IN_GPIO_Ports[motor][0], Motor_IN_Pins[motor][0], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Motor_IN_GPIO_Ports[motor][1], Motor_IN_Pins[motor][1], GPIO_PIN_RESET);
			break;
	}

	//设置驱动板状态
	Set_TB6612_State();
}

//设置单个电机速度
//速度取值 -100 ~ 100 对应占空比 0% - 100%
void SetMotorSpeed(Motor_Name motor, float speed)
{
	uint8_t state = 0;
	//设置状态
	if (speed == 0)
	{
		state = Motor_Stop;
	}
	else if (speed > 0)
	{
		state = Motor_Forward;
	}
	else
	{
		state = Motor_Backward;
	}

	//检查电机状态
	if (Motor_States[motor] != state)
	{
		SetMotorState(motor, state);
	}

	//修正速度
	if (speed < 0)
	{
		speed = -speed;
	}

	//设置PWM
	__HAL_TIM_SetCompare(&Motor_PWM_TIM, Motor_PWM_Channels[motor], (speed>100?100:speed)*0.01*PWM_Count_Max);
}

//设置全部电机速度
void SetAllMotorSpeed(float speed)
{
	SetMotorSpeed(Motor_1, speed);
	SetMotorSpeed(Motor_2, speed);
	SetMotorSpeed(Motor_3, speed);
	SetMotorSpeed(Motor_4, speed);
}
