/*
 * control.c
 *
 *  Created on: Dec 2, 2022
 *      Author: Little Fish
 */

#include "control.h"
#include "MPU6050/mpu6050.h"
#include "BasicInfor/debug.h"

float YawAngle;

void UpdateYawAngle()
{
	read_dmp(&mpu_pose_msg);
	YawAngle = mpu_pose_msg.yaw;
}

void InitMPU()
{
	if (w_mpu_init() != mpu_ok)
	{
		printfX("初始化 MPU6050 失败\n");
		printfX("0x%x (ID_ERROR)\r\n", w_mpu_init());
	}
	else
	{
		printfX("开始初始化 DSP\n");
		dmp_init();

		printfX("初始化 DSP 成功\n");
	}

}

float Get_MPU_Temperature()
{
	return w_mpu_read_temperature();
}
