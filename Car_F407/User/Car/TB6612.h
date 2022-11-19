/*
 * TB6612.h
 *
 *  Created on: Nov 11, 2022
 *      Author: Little Fish
 */

#ifndef CAR_TB6612_H_
#define CAR_TB6612_H_

#include "common.h"
#include "tim.h"

extern Motor_State Motor_States[4];

void SetMotorSpeed(Motor_Name motor, float speed);
void SetMotorState(Motor_Name motor, Motor_State direction);
void SetAllMotorSpeed(float speed);

#endif /* CAR_TB6612_H_ */
