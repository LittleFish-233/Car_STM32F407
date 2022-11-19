/*
 * bsp_uart.c
 *
 *  Created on: Nov 15, 2022
 *      Author: Little Fish
 */

#include "bsp_uart.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//串口句柄
UART_HandleTypeDef *UART_Handles[] = { &huart1, &huart2 };
USART_TypeDef *UART_Types[] = { USART1, USART2 };

//接收数据缓存
uint8_t RxData[UART_Number][40];
uint8_t RxData_Buffer[UART_Number];
//数据索引
uint8_t RxData_Index[UART_Number];
//1 未读 0 已读
uint8_t RxData_Flag[UART_Number];

void Init_USART()
{
	//打开串口接收中断
	for (int var = 0; var < UART_Number; ++var)
	{
		HAL_UART_Receive_IT(UART_Handles[var], &RxData_Buffer[var], 1);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t index = 0;
	//判断串口序号
	for (int var = 0; var < UART_Number; ++var)
	{
		if (huart->Instance == UART_Types[var])
		{
			index = var;
		}
	}

	//判断结束字符
	if (RxData_Buffer[index] == '\0' || RxData_Buffer[index] == '#' || RxData_Buffer[index] == '\n')
	{
		RxData[index][RxData_Index[index]++] = '\0';
		RxData_Flag[index] = RX_UnRead;
	}
	else
	{
		//未读 则清零
		if (RxData_Flag[index] == RX_UnRead)
		{
			RxData_Flag[index] = RX_NoData;
			RxData_Index[index] = RX_NoData;
		}

		//复制数据
		RxData[index][RxData_Index[index]++] = RxData_Buffer[index];
	}

	//打开串口接收中断
	HAL_UART_Receive_IT(UART_Handles[index], &RxData_Buffer[index], 1);
}

//清除未读标志
void CleanRxData(uint8_t index)
{

	RxData_Flag[index] = 0;
	RxData_Index[index] = 0;
}

void printf_Base(const char *TX_String)
{
	//HAL_UART_Transmit(UART_Handles[Printf_UART], (uint8_t*) TX_String, strlen(TX_String), 0xFFFF);
}

