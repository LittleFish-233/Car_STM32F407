/*
 * control.h
 *
 *  Created on: Dec 2, 2022
 *      Author: Little Fish
 */

#ifndef BALANCE_CONTROL_H_
#define BALANCE_CONTROL_H_

#include "main.h"

extern float YawAngle;

void UpdateYawAngle();
void InitMPU();
float Get_MPU_Temperature();

#endif /* BALANCE_CONTROL_H_ */
