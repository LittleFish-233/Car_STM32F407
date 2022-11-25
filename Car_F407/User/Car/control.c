/*
 * control.c
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#include "control.h"
#include "TB6612.h"
#include "GMR_Encoder.h"
#include "Speed_PID.h"
#include "infrared.h"
#include "math.h"
//电机理论速度
float Motor_Expected_Speeds[Motor_Number];
//三维速度向量 X Y Z
float Motor_Expected_Axis_Speeds[3];
//电机实际速度
extern float Motor_Actual_Speeds[Motor_Number];

#define Motor_Expected_Speed_Max 80
#define Motor_Expected_Speed_Min 5

float Abnormal_Axis_Speed[3];

Abnormal_Mode StateAbnormal_Mode;

void InitCar()
{
	//初始化PID
	InitMotorPID();
	//使能小车速度监控
	EnableAllMotorEncoders();
}

void SetAbnormalBehavior(Abnormal_Mode model, float x, float y, float z)
{
	StateAbnormal_Mode = model;
	Abnormal_Axis_Speed[0] = x;
	Abnormal_Axis_Speed[1] = y;
	Abnormal_Axis_Speed[2] = z;
}

//周期更新设置值并应用
void Periodic_UpdateAndSet_Car_ExpectedSpeed()
{
	//CorrectCarDirection();
	//更新实际速度
	Periodic_Update_Car_ActualSpeed();
	//级联更新计算PID
	for (int motor = 0; motor < Motor_Number; ++motor)
	{
		SetMotorSpeed(motor, UpdateMotorSpeedPID(motor, Motor_Expected_Speeds[motor], Motor_Actual_Speeds[motor]));
	}
}

//判断是否停止
uint8_t Is_Car_Runing()
{
	uint8_t flag = 0;
	for (int var = 0; var < Motor_Number; ++var)
	{
		if (Motor_Expected_Speeds[var] != 0)
		{
			flag = 1;
			break;
		}
	}
	return flag;
}

//周期更新实际值
void Periodic_Update_Car_ActualSpeed()
{
	UpdateAllMotorSpeed();

	//根据电机状态 修正已停止电机的速度
	for (int motor = 0; motor < Motor_Number; ++motor)
	{
		if (Motor_States[motor] == Motor_Stop && Motor_Actual_Speeds[motor] != 0)
		{
			Motor_Actual_Speeds[motor] = 0;
		}
	}
}

//跳过计算 直接设置某个轮子速度
//只有关闭巡线才能生效
void Set_Motor_ExpectedSpeed(Motor_Name motor, float speed)
{
	Motor_Expected_Speeds[motor] = speed;
}

//计算速度并保存
void Transform_AxisSpeed()
{
	Motor_Expected_Speeds[2] = (Motor_Expected_Axis_Speeds[Axis_X] - Motor_Expected_Axis_Speeds[Axis_Z]);
	Motor_Expected_Speeds[0] = (Motor_Expected_Axis_Speeds[Axis_X] - Motor_Expected_Axis_Speeds[Axis_Z]);
	Motor_Expected_Speeds[1] = (Motor_Expected_Axis_Speeds[Axis_X] + Motor_Expected_Axis_Speeds[Axis_Z]);
	Motor_Expected_Speeds[3] = (Motor_Expected_Axis_Speeds[Axis_X] + Motor_Expected_Axis_Speeds[Axis_Z]);

	//输出饱和
	for (int var = 0; var < Motor_Number; ++var)
	{
		if (Motor_Expected_Speeds[var] < 0)
		{
			if (Motor_Expected_Speeds[var] < -Motor_Expected_Speed_Max)
			{
				Motor_Expected_Speeds[var] = -Motor_Expected_Speed_Max;
			}
			else if (Motor_Expected_Speeds[var] > -Motor_Expected_Speed_Min)
			{
				Motor_Expected_Speeds[var] = -Motor_Expected_Speed_Min;
			}
		}
		else if (Motor_Expected_Speeds[var] > 0)
		{
			if (Motor_Expected_Speeds[var] > Motor_Expected_Speed_Max)
			{
				Motor_Expected_Speeds[var] = Motor_Expected_Speed_Max;
			}
			else if (Motor_Expected_Speeds[var] < Motor_Expected_Speed_Min)
			{
				Motor_Expected_Speeds[var] = Motor_Expected_Speed_Min;
			}
		}
	}
}

//在当前速度的基础上添加 速度
void AddCarSpeed(float vx, float vy, float vz)
{
	Motor_Expected_Axis_Speeds[Axis_X] += vx;
	Motor_Expected_Axis_Speeds[Axis_Y] += vy;
	Motor_Expected_Axis_Speeds[Axis_Z] += vz;

	Transform_AxisSpeed();
}

//设置速度
void SetCarSpeed(float vx, float vy, float vz)
{
	Motor_Expected_Axis_Speeds[0] = vx;
	Motor_Expected_Axis_Speeds[1] = vy;
	Motor_Expected_Axis_Speeds[2] = vz;

	Transform_AxisSpeed();
}
