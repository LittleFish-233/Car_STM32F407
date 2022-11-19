/*
 * common.h
 *
 *  Created on: Nov 12, 2022
 *      Author: Little Fish
 */

#ifndef CAR_COMMON_H_
#define CAR_COMMON_H_

#include "main.h"

//电机数量
#define Motor_Number 4

typedef enum
{
	Motor_Forward, Motor_Backward, Motor_Stop
} Motor_State;

/*
 * 电机名称
 * 1   2
 * |   |
 * 3   4
 */
typedef enum
{
	Motor_1, Motor_2, Motor_3, Motor_4
} Motor_Name;

#endif /* CAR_COMMON_H_ */
