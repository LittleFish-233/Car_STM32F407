/*
 * dormancy.c
 *
 *  Created on: Nov 15, 2022
 *      Author: Little Fish
 */

#include "dormancy.h"
#include "Screen/image.h"

#define Dormancy_Count_Max 60*1
#define Dormancy_PreHibernate_Max 71*8

//休眠计数 -1 进入预休眠模式
int16_t Dormancy_Count = -1;

void Clean_Dormancy_Count()
{
	if (Dormancy_Count == -1)
	{
		//已经进入预休眠状态 重新启动定时器
		HAL_TIM_Base_Start_IT(&htim7);
	}
	Dormancy_Count = 0;
}

void PreHibernate()
{
	Screen_Clear();
	for (int var = 1; var < Dormancy_PreHibernate_Max; ++var)
	{
		Screen_ShowPicture_Num(1, (var % 70) + 1);
		HAL_Delay(40);
		//HAL_Delay(1000);
		//判断是否中途退出预休眠模式
		if (Dormancy_Count != -1)
		{
			Screen_Clear();
			return;
		}

	}
	//Screen_Clear();
}

void Hibernate()
{
	Screen_Close();
	HAL_PWR_EnterSTANDBYMode();
}

void Periodic_Dormancy_Counter()
{
	Dormancy_Count++;

	if (Dormancy_Count > Dormancy_Count_Max)
	{
		//停止定时器
		HAL_TIM_Base_Stop_IT(&htim7);
		Dormancy_Count = -1;
		//进入预休眠模式
		PreHibernate();
		if (Dormancy_Count != -1)
		{
			return;
		}
		//执行完成任务 进入休眠模式
		Hibernate();
	}
}

