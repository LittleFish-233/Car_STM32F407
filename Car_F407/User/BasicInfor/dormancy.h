/*
 * dormancy.h
 *
 *  Created on: Nov 15, 2022
 *      Author: Little Fish
 */

#ifndef BASICINFOR_DORMANCY_H_
#define BASICINFOR_DORMANCY_H_

#include <Screen/screen.h>
#include "main.h"
#include "tim.h"

extern int16_t Dormancy_Count;

void Clean_Dormancy_Count();
void Periodic_Dormancy_Counter();

#endif /* BASICINFOR_DORMANCY_H_ */
