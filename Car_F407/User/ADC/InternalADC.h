/*
 * InternalADC.h
 *
 *  Created on: Nov 15, 2022
 *      Author: Little Fish
 */

#ifndef ADC_INTERNALADC_H_
#define ADC_INTERNALADC_H_

#include "main.h"
#include "adc.h"

uint16_t Get_ADCChannelValue(ADC_HandleTypeDef *hadc, uint32_t channel);

#endif /* ADC_INTERNALADC_H_ */
