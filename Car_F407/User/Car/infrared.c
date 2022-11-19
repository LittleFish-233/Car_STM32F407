/*
 * infrared.c
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#include "infrared.h"

//右 -> 左
uint8_t Infrared_Datas[Infrared_Number];

GPIO_TypeDef *Infrared_GPIO_Ports[Infrared_Number] = { Infrared1_GPIO_Port, Infrared2_GPIO_Port, Infrared3_GPIO_Port, Infrared4_GPIO_Port };
uint16_t Infrared_Pins[Infrared_Number] = { Infrared1_Pin, Infrared2_Pin, Infrared3_Pin, Infrared4_Pin };

//获取红外数据
void UpdateInfraredData()
{
	for (int var = 0; var < Infrared_Number; ++var)
	{
		Infrared_Datas[var] = HAL_GPIO_ReadPin(Infrared_GPIO_Ports[var], Infrared_Pins[var]) == GPIO_PIN_RESET;
	}
}

//获取当前行驶状态
DriveState GetDriveState()
{
	UpdateInfraredData();

	//判断
	if (Infrared_Datas[Infrared_Left] && Infrared_Datas[Infrared_Right])
	{
		return DriveState_Intersection;
	}
	else if (Infrared_Datas[Infrared_Right])
	{
		return DriveState_Turn_Right;
	}
	else if (Infrared_Datas[Infrared_Left])
	{
		return DriveState_Turn_Left;
	}
	else if (Infrared_Datas[Infrared_Center_Left] == 0 && Infrared_Datas[Infrared_Center_Right] == 0)
	{
		return DriveState_Derailment;
	}
	else if (Infrared_Datas[Infrared_Center_Left] == 0)
	{
		return DriveState_Left;
	}
	else if (Infrared_Datas[Infrared_Center_Right] == 0)
	{
		return DriveState_Right;
	}
	else
	{
		return DriveState_Normal;
	}
}
