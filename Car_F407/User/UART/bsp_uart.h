/*
 * bsp_uart.h
 *
 *  Created on: Nov 15, 2022
 *      Author: Little Fish
 */

#ifndef UART_BSP_UART_H_
#define UART_BSP_UART_H_

#include "main.h"
#include "usart.h"

#define UART_Number 2
#define Printf_UART UART_2

typedef enum
{
	UART_1, UART_2
} UART_Name;

typedef enum
{
	RX_NoData, RX_UnRead
} UART_RX_State;

extern uint8_t RxData[UART_Number][40];
extern uint8_t RxData_Flag[UART_Number];

void Init_USART();
void CleanRxData(uint8_t UART_Name);
void printf_Base(const char *TX_String);

#endif /* UART_BSP_UART_H_ */
