/*
 * infrared.h
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#ifndef CAR_INFRARED_H_
#define CAR_INFRARED_H_

#include "common.h"

#define Infrared_Number 4

extern uint8_t Infrared_Datas[Infrared_Number];

//红外传感器编号
typedef enum
{
	Infrared_Center_Left, Infrared_Left, Infrared_Center_Right, Infrared_Right,
} InfraredName;

//0 到达或遇到三叉路口 1 左偏 2 右偏 3 正常 4 左转 5 右转 6 脱轨
typedef enum
{
	DriveState_Intersection, DriveState_Left, DriveState_Right, DriveState_Normal, DriveState_Turn_Left, DriveState_Turn_Right, DriveState_Derailment
} DriveState;

DriveState GetDriveState();
void UpdateInfraredData();

#endif /* CAR_INFRARED_H_ */
