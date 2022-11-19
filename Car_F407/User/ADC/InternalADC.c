/*
 * InternalADC.c
 *
 *  Created on: Nov 15, 2022
 *      Author: Little Fish
 */

#include "InternalADC.h"

uint16_t Get_ADCChannelValue(ADC_HandleTypeDef *hadc, uint32_t channel)
{
	ADC_ChannelConfTypeDef ADC_ChanConf;

	ADC_ChanConf.Channel = channel;
	ADC_ChanConf.Rank = 1;
	ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_15CYCLES;

	HAL_ADC_ConfigChannel(hadc, &ADC_ChanConf);

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 10);   //等待转换完成，50为最大等待时间，单位为ms
	return (uint16_t) HAL_ADC_GetValue(hadc);
}
