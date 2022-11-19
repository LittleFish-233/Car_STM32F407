/*
 * bsp_fatfs.c
 *
 *  Created on: Nov 18, 2022
 *      Author: Little Fish
 */

#include "bsp_fatfs.h"
#include "BasicInfor/debug.h"

FATFS fs; /* FatFs文件系统对象 */
FIL file; /* 文件对象 */
FRESULT f_res; /* 文件操作结果 */

void Init_FATFS()
{
	FATFS_LinkDriver(&SD_Driver, SDPath);

	//在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
	f_res = f_mount(&fs, (TCHAR const*) SDPath, 1);
	printf_fatfs_error(f_res);
}

/**
 * @brief  打印输出信息
 * @param  无
 * @retval 无
 */
void printf_fatfs_error(FRESULT fresult)
{
	switch (fresult)
	{
		case FR_OK:
			printfX("》操作成功。\r\n");
			break;
		case FR_DISK_ERR:
			printfX("！！硬件输入输出驱动出错。\r\n");
			break;
		case FR_INT_ERR:
			printfX("！！断言错误。\r\n");
			break;
		case FR_NOT_READY:
			printfX("！！物理设备无法工作。\r\n");
			break;
		case FR_NO_FILE:
			printfX("！！无法找到文件。\r\n");
			break;
		case FR_NO_PATH:
			printfX("！！无法找到路径。\r\n");
			break;
		case FR_INVALID_NAME:
			printfX("！！无效的路径名。\r\n");
			break;
		case FR_DENIED:
		case FR_EXIST:
			printfX("！！拒绝访问。\r\n");
			break;
		case FR_INVALID_OBJECT:
			printfX("！！无效的文件或路径。\r\n");
			break;
		case FR_WRITE_PROTECTED:
			printfX("！！逻辑设备写保护。\r\n");
			break;
		case FR_INVALID_DRIVE:
			printfX("！！无效的逻辑设备。\r\n");
			break;
		case FR_NOT_ENABLED:
			printfX("！！无效的工作区。\r\n");
			break;
		case FR_NO_FILESYSTEM:
			printfX("！！无效的文件系统。\r\n");
			break;
		case FR_MKFS_ABORTED:
			printfX("！！因函数参数问题导致f_mkfs函数操作失败。\r\n");
			break;
		case FR_TIMEOUT:
			printfX("！！操作超时。\r\n");
			break;
		case FR_LOCKED:
			printfX("！！文件被保护。\r\n");
			break;
		case FR_NOT_ENOUGH_CORE:
			printfX("！！长文件名支持获取堆空间失败。\r\n");
			break;
		case FR_TOO_MANY_OPEN_FILES:
			printfX("！！打开太多文件。\r\n");
			break;
		case FR_INVALID_PARAMETER:
			printfX("！！参数无效。\r\n");
			break;
	}
}
