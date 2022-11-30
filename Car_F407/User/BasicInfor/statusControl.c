/*
 * statusControl.c
 *
 *  Created on: 2022年11月21日
 *      Author: Little Fish
 */

#include "statusControl.h"
#include "Car/infrared.h"
#include "Car/Control.h"
#include "math.h"

//typedef enum
//{
//	Command_Trigger_None,Command_Trigger_Left, Command_Trigger_Right, Command_Trigger_Cross
//} Command_Trigger;

#define Command_Turn_Duration 500
#define Command_Rotation_Duration Command_Turn_Duration*2
#define Command_CooldownTime 11

#define Turn_Speed_Step 10

uint8_t Command_Number = 4;

Command Commands[Command_Number_Max] = { { Command_Right, Command_Turn_Duration, 0 }, { Command_Right, Command_Rotation_Duration, 0 }, { Command_Rotation, Command_Turn_Duration, 0 }, { Command_Left, Command_Rotation_Duration, 0 } };

int Command_Index = -1;

uint32_t Last_RunCommand_Time;

//控制系统是否开始运行
uint8_t StatusControl_RuningFlag;
//指令持续标志
uint8_t Command_Duration_Flag;

//预设速度
float Default_Speed;

//红外传感器的真实状态
DriveState RealState;
//异常状态的修正值
DriveState AbnormaldState;

//指令模式
Command_Mode Mode;

//转弯步进
#define StepLength 0.25

void Status_Control_Clear()
{
	SetCarSpeed(0, 0, 0);
	StatusControl_RuningFlag = 0;
	Command_Duration_Flag = 0;
	Command_Index = -1;
	Last_RunCommand_Time = 0;
	AbnormaldState = DriveState_Normal;
}

void Status_Control_Start(Command_Mode mode, float default_Speed)
{
	StatusControl_RuningFlag = 1;
	Default_Speed = default_Speed == 0 ? Motor_Expected_Axis_Speeds[Axis_X] : default_Speed;
	Mode = mode;
}

//根据外设修正小车方向
void CorrectCarDirection(DriveState state)
{
	//判断是否停止
	if (Motor_Expected_Axis_Speeds[Axis_X] == 0 && Motor_Expected_Axis_Speeds[Axis_Z] == 0)
	{
		return;
	}

	//return;
	float k = StepLength + 0.005 * fabs(Motor_Expected_Axis_Speeds[Axis_X]);

	//向另一方向转弯时清空转弯角
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	switch (state)
	{
		case DriveState_Left:
			z = -k;
			break;
		case DriveState_Right:
			z = k;
			break;
		case DriveState_Sharp_Left:
			z = -k * 2;
			break;
		case DriveState_Sharp_Right:
			z = k * 2;
			break;
		case DriveState_Normal:
			Motor_Expected_Axis_Speeds[Axis_Z] = 0;
		default:
			break;
	}

	if (Motor_Expected_Axis_Speeds[Axis_X] < 0)
	{
		z = -z;
	}

	if (Motor_Expected_Axis_Speeds[Axis_Z] * z < 0)
	{
		Motor_Expected_Axis_Speeds[Axis_Z] = 0;
	}

	AddCarSpeed(x, y, z);
}

void UpdateCommand()
{
	//判断是否正在运行
	if (StatusControl_RuningFlag == 0)
	{
		return;
	}

	//判断是否在上个指令的持续时间
	if (Command_Index >= 0 && Commands[Command_Index].RunCommand_Time > 0 && uwTick - Last_RunCommand_Time < Commands[Command_Index].Duration)
	{
		Command_Duration_Flag = 1;
		return;
	}

	//是否第一次结束指令持续作用
	if (Command_Duration_Flag == 1)
	{

		switch (Commands[Command_Index].Type)
		{
			default:
				if (RealState == DriveState_Normal)
				{
					SetCarSpeed(Default_Speed, 0, 0);
				}
				else
				{
					Command_Duration_Flag = 1;
					//放弃这一回合
					return;

				}
				break;
		}

		Command_Duration_Flag = 0;

		AbnormaldState = DriveState_Normal;

	}

	//判断是否在冷却时间
	if (uwTick - Last_RunCommand_Time < Command_CooldownTime * (100.0 - Motor_Expected_Axis_Speeds[Axis_X]) + (Command_Index >= 0 ? Commands[Command_Index].Duration : 0))
	{
		return;
	}

	//判断是否到达下一指令触发条件
	if (DriveState_Intersection != RealState && DriveState_Turn_Left != RealState && DriveState_Turn_Right != RealState)
	{
		return;
	}

	int index = Command_Index + 1;
	if (index > Command_Number - 1)
	{
		//是否为循环模式
		if (Mode == Command_Mode_OneTime)
		{
			Status_Control_Clear();
		}
		else
		{
			index = 0;
		}
	}

	Command_Index = index;

	//执行触发后操作
	switch (Commands[Command_Index].Type)
	{
		case Command_Forward:
			break;
		case Command_Left:
			SetCarSpeed(Default_Speed * 0.5, 0, Default_Speed);
			AbnormaldState = DriveState_Sharp_Right;
			break;
		case Command_Right:
			SetCarSpeed(Default_Speed * 0.5, 0, -Default_Speed);
			AbnormaldState = DriveState_Sharp_Left;
			break;
		case Command_Rotation:
			SetCarSpeed(0, 0, Default_Speed);
			AbnormaldState = DriveState_Sharp_Right;
			break;
		case Command_Backward:

			break;
	}

	//更新执行时间
	Commands[Command_Index].RunCommand_Time = Last_RunCommand_Time = uwTick;
}

void ApplyCommand()
{
	DriveState state = RealState;
	//if (RealState != DriveState_Left && RealState != DriveState_Right && RealState != DriveState_Sharp_Left && RealState != DriveState_Sharp_Right && RealState != DriveState_Normal)
	if (AbnormaldState != DriveState_Normal)
	{
		state = DriveState_Derailment;
	}

	//修正实际偏差
	CorrectCarDirection(state);
	//将理论值应用到小车
	Periodic_UpdateAndSet_Car_ExpectedSpeed();
}

//周期性调用 控制小车运行
void Periodic_Status_Control()
{
	//获取当前红外状态
	RealState = GetDriveState();
	//根据辅助传感器更新异常状态指令
	UpdateCommand();
	//应用指令到实际运行
	ApplyCommand();
}
