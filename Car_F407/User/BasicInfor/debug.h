/*
 * debug.h
 *
 *  Created on: Nov 18, 2022
 *      Author: Little Fish
 */

#ifndef BASICINFOR_DEBUG_H_
#define BASICINFOR_DEBUG_H_

#include "main.h"
#include "UART/bsp_uart.h"
#include "fatfs.h"

//发送缓存
extern char Printf_Buffer[2000];
//重定义
#define printfX(FORMAT,...) (sprintf(Printf_Buffer,FORMAT,##__VA_ARGS__),printf_Base(Printf_Buffer))

#endif /* BASICINFOR_DEBUG_H_ */
