#include "led.h"

//呼吸灯
uint8_t LED1_DirInc;
uint16_t LED1_Pulsewidth;

//周期调用 更新呼吸灯
void Periodic_Update_BreathingLights()
{
	if (LED1_DirInc == 0) //脉宽递增
	{
		LED1_Pulsewidth++;
		if (LED1_Pulsewidth >= 999)
		{
			LED1_DirInc = 1; //脉宽递减
		}
	}
	else
	{
		LED1_Pulsewidth--;
		if (LED1_Pulsewidth <= 1)
		{
			LED1_DirInc = 0; //脉宽递增
		}
	}

	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, LED1_Pulsewidth); //设置CCR的
}
