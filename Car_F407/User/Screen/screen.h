/*
 * screen.h
 *
 *  Created on: Nov 16, 2022
 *      Author: Little Fish
 */

#ifndef SCREEN_SCREEN_H_
#define SCREEN_SCREEN_H_

//屏幕类型 0 OLED 1 LCD
#define SCREEN_TYPE 1

//是否使用Flash
//#define SCREEN_USE_FLASH
//
//是否使用文件系统
#define SCREEN_USE_FATFS

#if SCREEN_TYPE==0
#define USE_OLED
#else
#define USE_LCD
#endif

#include "main.h"

#if defined USE_OLED
#include "oled.h"
#else
#include "lcd.h"
#endif

void Screen_Init();
void Screen_ShowStringLine(uint8_t line, char *chr, uint8_t size);
void Screen_ShowPicture(unsigned char x0, unsigned char y0, uint16_t length, uint16_t width, const unsigned char BMP[]);
void Screen_ShowPicture_Num(uint8_t a, uint8_t b);
void Screen_Clear();

void Screen_Close();

#if defined SCREEN_USE_FLASH
void Screen_ShowFlashImage(uint32_t ReadAddr, uint32_t Size);
#endif
#if defined SCREEN_USE_FATFS
void Screen_ShowFileImage(const char *fileName);
#endif
#endif /* SCREEN_SCREEN_H_ */
