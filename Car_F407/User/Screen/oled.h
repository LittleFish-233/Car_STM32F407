#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include <Screen/font.h>

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64
//-----------------OLED IIC端口定义----------------

//#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET);//SCL
//#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET);
//
//#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET);//SDA
//#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET);

#define OLED_SCLK_Clr() ;//SCL
#define OLED_SCLK_Set() ;

#define OLED_SDIN_Clr() ;//SDA
#define OLED_SDIN_Set() ;

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define   LINE(x)                       ((x) * (1))

//OLED控制用函数
void OLED_Clear();
//自定义函数
void OLED_InitX(const unsigned char BMP[]);
void OLED_ShowStringLine(uint8_t line, char *chr);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, const unsigned char BMP[]);

#endif

