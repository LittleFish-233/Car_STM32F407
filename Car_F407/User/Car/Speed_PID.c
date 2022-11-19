/*
 * Speed_PID.c
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#include "Speed_PID.h"

#define CAR_SPEED_MAX 80
#define CAR_SPEED_MIN 20

struct _pid
{
	float SetSpeed; //定义设定值
	float ActualSpeed; //定义实际值
	float err; //定义偏差值
	float err_next; //定义上一个偏差值
	float err_last; //定义最上前的偏差值
	float Kp, Ki, Kd; //定义比例、积分、微分系数
};

struct _pid Motor_PID[4];

//初始化PID
void InitMotorPID()
{
	for (uint8_t i = 0; i < Motor_Number; i++)
	{
		Motor_PID[i].SetSpeed = 0.0;
		Motor_PID[i].ActualSpeed = 0.0;
		Motor_PID[i].err = 0.0;
		Motor_PID[i].err_next = 0.0;
		Motor_PID[i].err_last = 0.0;
		Motor_PID[i].Kp = 2;
		Motor_PID[i].Ki = 4;
		Motor_PID[i].Kd = 2;
	}
}

float PID_realize(struct _pid *pid, float speed, float actualSpeed)
{
	float incrementSpeed;
	//设置当前值
	pid->SetSpeed = speed;
	pid->ActualSpeed = actualSpeed;
	//特殊处理 0 速度
	if (speed == 0)
	{
		return 0;
	}

	pid->err = pid->SetSpeed - pid->ActualSpeed;
	incrementSpeed = pid->Kp * (pid->err - pid->err_next) + pid->Ki * pid->err + pid->Kd * (pid->err - 2 * pid->err_next + pid->err_last); //计算出增量

	pid->ActualSpeed += incrementSpeed;
	pid->err_last = pid->err_next;
	pid->err_next = pid->err;
	return pid->ActualSpeed;
}

//限制电机速度在可控范围内
float LimitSpeed(float current, float settingValue)
{
	//使设置值与控制值极性相同
	if ((settingValue > 0 && current < 0) || (settingValue < 0 && current > 0))
	{
		current = -current;
		return 0;
	}

	//判断是否超过上下限
	if (current < - CAR_SPEED_MAX)
	{
		current = - CAR_SPEED_MAX;
	}
	else if (current > - CAR_SPEED_MIN && current < 0)
	{
		current = - CAR_SPEED_MIN;
	}
	else if (current > CAR_SPEED_MAX)
	{
		current = CAR_SPEED_MAX;
	}
	else if (current > 0 && current < CAR_SPEED_MIN)
	{
		current = CAR_SPEED_MIN;
	}

	return current;
}

//依照PID算法更新电机速度
float UpdateMotorSpeedPID(Motor_Name motor, float speed, float actualSpeed)
{
	//下一轮应用值
	float ac = 0;
	ac = PID_realize(&Motor_PID[motor], speed, actualSpeed);
	//判断是否在上下限内
	//ac = LimitSpeed(ac, speed);

	return ac;
}

//清空所有电机PID数据 停止
void CleanAllMotorPID()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		Motor_PID[i].ActualSpeed = 0;
		Motor_PID[i].SetSpeed = 0;
	}
}

