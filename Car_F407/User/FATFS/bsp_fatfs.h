/*
 * bsp_fatfs.h
 *
 *  Created on: Nov 18, 2022
 *      Author: Little Fish
 */

#ifndef FATFS_BSP_FATFS_H_
#define FATFS_BSP_FATFS_H_

#include "main.h"
#include "fatfs.h"

extern FATFS fs; /* FatFs文件系统对象 */
extern FIL file; /* 文件对象 */
extern FRESULT f_res; /* 文件操作结果 */

void printf_fatfs_error(FRESULT fresult);
void Init_FATFS();
#endif /* FATFS_BSP_FATFS_H_ */
