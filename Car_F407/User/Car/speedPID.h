/*
 * Speed_PID.h
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#ifndef CAR_SPEEDPID_H_
#define CAR_SPEEDPID_H_

#include "common.h"

void InitMotorPID();
void CleanAllMotorPID();
float UpdateMotorSpeedPID(Motor_Name motor, float speed, float actualSpeed);
#endif /* CAR_SPEEDPID_H_ */
