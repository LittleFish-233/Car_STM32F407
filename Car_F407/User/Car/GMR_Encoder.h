/*
 * GMR_Encoder.h
 *
 *  Created on: 2022年11月12日
 *      Author: Little Fish
 */

#ifndef CAR_GMR_ENCODER_H_
#define CAR_GMR_ENCODER_H_

#include "common.h"
#include "tim.h"

extern float Motor_Actual_Speeds[4];

void EnableAllMotorEncoders();
void UpdateAllMotorSpeed();

#endif /* CAR_GMR_ENCODER_H_ */
