/*
 * statusControl.h
 *
 *  Created on: 2022年11月21日
 *      Author: Little Fish
 */

#ifndef BASICINFOR_STATUSCONTROL_H_
#define BASICINFOR_STATUSCONTROL_H_

#include "main.h"

extern uint8_t StatusControl_RuningFlag;
void Periodic_Status_Control();
void Status_Control_Clear();

#endif /* BASICINFOR_STATUSCONTROL_H_ */
