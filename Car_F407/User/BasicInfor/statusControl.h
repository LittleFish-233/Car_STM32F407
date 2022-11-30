/*
 * statusControl.h
 *
 *  Created on: 2022年11月21日
 *      Author: Little Fish
 */

#ifndef BASICINFOR_STATUSCONTROL_H_
#define BASICINFOR_STATUSCONTROL_H_

#include "main.h"

#define Command_Number_Max 20

//指令类型
typedef enum
{
	Command_Forward, Command_Left, Command_Right, Command_Backward, Command_Rotation
} Command_Type;

//执行模式
typedef enum
{
	Command_Mode_Cycle, Command_Mode_OneTime
} Command_Mode;

//指令模型
typedef struct
{
	Command_Type Type;
	uint32_t Duration;
	uint32_t RunCommand_Time;
} Command;

extern uint8_t StatusControl_RuningFlag;
extern uint8_t Command_Number;
extern Command Commands[Command_Number_Max];

void Periodic_Status_Control();
void Status_Control_Clear();
void Status_Control_Start(Command_Mode mode, float default_Speed);

#endif /* BASICINFOR_STATUSCONTROL_H_ */
