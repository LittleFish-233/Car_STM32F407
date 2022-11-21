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

//0 到达或遇到三叉路口  1 左偏 2 右偏 3 正常 4 大幅左偏 5 大幅右偏 6 脱轨 7 左转 8 右转
typedef enum
{
	DriveState_Intersection, DriveState_Left, DriveState_Right, DriveState_Normal, DriveState_Sharp_Left, DriveState_Sharp_Right, DriveState_Derailment, DriveState_Turn_Left, DriveState_Turn_Right
} DriveState;

DriveState GetDriveState();
void UpdateInfraredData();

#endif /* CAR_INFRARED_H_ */
