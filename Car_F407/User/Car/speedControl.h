/*
 * control.h
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#ifndef CAR_SPEEDCONTROL_H_
#define CAR_SPEEDCONTROL_H_

#include "common.h"
#include "infrared.h"

extern float Motor_Actual_Speeds[4];
extern float Motor_Expected_Speeds[4];
extern float Motor_Expected_Axis_Speeds[3];
extern GPIO_PinState Infrared_Datas[4];

/*三维坐标
 *    x
 ^
 / |
 /	  |
 Z<	   -------->y*/

typedef enum
{
	Axis_X, Axis_Y, Axis_Z
} Axis;

//非正常运行时 速度干预模式 累计 一次性
typedef enum
{
	Abnormal_Mode_None, Abnormal_Mode_Accumulate, Abnormal_Mode_Onetime
} Abnormal_Mode;

void InitCar();
void Periodic_Update_Car_ActualSpeed();
void Periodic_UpdateAndSet_Car_ExpectedSpeed();
void Set_Motor_ExpectedSpeed(Motor_Name motor, float speed);
void AddCarSpeed(float vx, float vy, float vz);
void SetCarSpeed(float vx, float vy, float vz);
uint8_t Is_Car_Runing();
void SetAbnormalBehavior(Abnormal_Mode model, float x, float y, float z);

#endif /* CAR_SPEEDCONTROL_H_ */
