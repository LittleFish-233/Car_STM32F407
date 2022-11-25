/*
 * GMR_Encoder.c
 *
 *  Created on: 2022年11月12日
 *      Author: Little Fish
 */
#include "GMR_Encoder.h"

//电机速度 -100 ~ 100
float Motor_Actual_Speeds[4];
//电机最大速度时编码器单位时间脉冲数
float Motor_Encoder_Max_Counts[4] = { 685, 685, 685, 685 };

TIM_HandleTypeDef *Motor_Encoders[4] = { &htim1, &htim8, &htim5, &htim4 };

//开启全部编码器
void EnableAllMotorEncoders()
{
	for (int var = 0; var < Motor_Number; ++var)
	{
		HAL_TIM_Encoder_Start(Motor_Encoders[var], TIM_CHANNEL_ALL);
	}
}

//

void UpdateAllMotorSpeed()
{
	short speed_count;
	float speed_duty;

	for (int var = 0; var < Motor_Number; ++var)
	{
		speed_count = (short) __HAL_TIM_GET_COUNTER(Motor_Encoders[var]);
		__HAL_TIM_SET_COUNTER(Motor_Encoders[var], 0);

		//计算速度
		speed_duty = speed_count * 100.0 / Motor_Encoder_Max_Counts[var];
		Motor_Actual_Speeds[var] = speed_duty;
	}

}
