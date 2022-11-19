/*
 * delay.c
 *
 *  Created on: Nov 18, 2022
 *      Author: Little Fish
 */

#include "delay.h"

void delay_us(uint32_t us)
{
	uint32_t delay = (HAL_RCC_GetHCLKFreq() / 1000000 * us);
	while (delay--)
		__NOP();
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(u16 nms)
{
	HAL_Delay(nms);
}
