/*
 * control.h
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#ifndef CAR_CONTROL_H_
#define CAR_CONTROL_H_

#include "common.h"

extern float Motor_Actual_Speeds[4];
extern float Motor_Expected_Speeds[4];
extern float Motor_Expected_Axis_Speeds[3];
extern GPIO_PinState Infrared_Datas[4];

void InitCar();
void Periodic_Update_Car_ActualSpeed();
void Periodic_UpdateAndSet_Car_ExpectedSpeed();
void Set_Motor_ExpectedSpeed(Motor_Name motor, float speed);
void AddCarSpeed(float vx, float vy, float vz);
void SetCarSpeed(float vx, float vy, float vz);
uint8_t Is_Car_Runing();
#endif /* CAR_CONTROL_H_ */
