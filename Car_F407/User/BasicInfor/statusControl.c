/*
 * statusControl.c
 *
 *  Created on: 2022年11月21日
 *      Author: Little Fish
 */

#include "statusControl.h"
#include "Car/infrared.h"
#include "Car/Control.h"
//指令类型
typedef enum
{
	Command_Forward, Command_Left, Command_Right, Command_Backward, Command_Rotation
} Command_Type;

//typedef enum
//{
//	Command_Trigger_None,Command_Trigger_Left, Command_Trigger_Right, Command_Trigger_Cross
//} Command_Trigger;

typedef struct
{
	Command_Type Type;
	uint32_t Duration;
	DriveState Trigger;
	uint32_t RunCommand_Time;
} Command;

#define Command_Number 4
#define Command_Turn_Duration 20
#define Command_CooldownTime 10000

#define Turn_Speed_Step 10

Command Commands[Command_Number] = { { Command_Right, Command_Turn_Duration, DriveState_Turn_Right, 0 }, { Command_Rotation, Command_Turn_Duration, DriveState_Derailment, 0 }, { Command_Left, Command_Turn_Duration, DriveState_Turn_Left, 0 }, { Command_Rotation, Command_Turn_Duration, DriveState_Derailment, 0 } };

int8_t Command_Index = -1;

uint32_t Last_RunCommand_Time;

//控制系统是否开始运行
uint8_t StatusControl_RuningFlag;
//指令持续标志
uint8_t Command_Duration_Flag;
//临时变量
float Temp_Speed;

void Status_Control_Clear()
{
	StatusControl_RuningFlag = 0;
	Command_Duration_Flag = 0;
	Command_Index = -1;
	Last_RunCommand_Time = 0;
}
//周期性调用 控制小车运行
void Periodic_Status_Control()
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
		Command_Duration_Flag = 0;

		switch (Commands[Command_Index].Type)
		{
			case Command_Forward:

				break;
			case Command_Left:
			case Command_Right:
				Motor_Expected_Axis_Speeds[Axis_Z] = 0;
				SetAbnormalBehavior(Abnormal_Mode_None, 0, 0, 0);
				break;
			case Command_Rotation:
				Motor_Expected_Axis_Speeds[Axis_X] = Temp_Speed;
				Motor_Expected_Axis_Speeds[Axis_Z] = 0;
				SetAbnormalBehavior(Abnormal_Mode_None, Temp_Speed, 0, 0);
				break;
			case Command_Backward:

				break;
		}
	}

	//判断是否在冷却时间
	if (uwTick - Last_RunCommand_Time < Command_CooldownTime)
	{
		return;
	}

	//获取当前红外状态
	DriveState state = GetDriveState();

	int8_t index = Command_Index + 1;
	if (index > Command_Number - 1)
	{
		index = 0;
	}

	//判断是否到达下一指令触发条件
	if (Commands[index].Trigger != state)
	{
		return;
	}

	Command_Index = index;
	//执行触发后操作
	switch (Commands[Command_Index].Type)
	{
		case Command_Forward:

			break;
		case Command_Left:
			SetAbnormalBehavior(Abnormal_Mode_Accumulate, 0, 0, -Turn_Speed_Step);
			break;
		case Command_Right:
			SetAbnormalBehavior(Abnormal_Mode_Accumulate, 0, 0, Turn_Speed_Step);
			break;
		case Command_Rotation:
			Temp_Speed = Motor_Expected_Axis_Speeds[Axis_X];
			SetAbnormalBehavior(Abnormal_Mode_Onetime, -Temp_Speed, 0, Temp_Speed);
			break;
		case Command_Backward:

			break;
	}
	Commands[Command_Index].RunCommand_Time = Last_RunCommand_Time = uwTick;
}
