/*
 * tool.c
 *
 *  Created on: Nov 17, 2022
 *      Author: Little Fish
 */
#include "tool.h"
#include "Flash/W25QXX.h"
#include "ADC/InternalADC.h"
#include "debug.h"

//测试
#define Data_Lenth 0x100
uint8_t wData[Data_Lenth];   //写缓存数组
uint8_t rData[Data_Lenth];   //读缓存数组
uint8_t ID[4];          //设备ID缓存数组
uint32_t i;

//获取电量
float GetBatteryLevel()
{
	return Get_ADCChannelValue(&hadc1, ADC_CHANNEL_4) * 3.3 * 11 / 4096;
}

//获取内部温度
float GetInternalTemperature()
{
	return ((Get_ADCChannelValue(&hadc1, ADC_CHANNEL_TEMPSENSOR) * 3.3 / 4096) - 0.76) / 0.0025 + 25;
}

void Test_W25QXX()
{
	//显示缓存
	char String_Line[40];

	printfX("\r\n SPI-W25Qxx Example \r\n\r\n");

	/*-Step1- 验证设备ID  ************************************************Step1*/
	BSP_W25Qx_Init();
	BSP_W25Qx_Read_ID(ID);
	//第一位厂商ID固定0xEF,第二位设备ID根据容量不同,具体为：
	//W25Q16为0x14、32为0x15、40为0x12、64为0x16、80为0x13、128为0x17
	if ((ID[0] != 0xEF) | (ID[1] != 0x14))
	{
		printfX("something wrong in Step1 \r\n");
	}
	else
	{
		printfX(" W25Qxx ID is : ");
		for (i = 0; i < 2; i++)
		{
			sprintf(String_Line, "0x%02X ", ID[i]);
			printfX(String_Line);
		}printfX("\r\n");
	}

	/*-Step2- 擦除块  ************************************************Step2*/
	if (BSP_W25Qx_Erase_Block(0) == W25Qx_OK)
		printfX(" QSPI Erase Block OK!\r\n");
	else
		printfX("something wrong in Step2\r\n");

	/*-Step3- 写数据  ************************************************Step3*/
	for (i = 0; i < Data_Lenth; i++)
	{
		wData[i] = i;
		rData[i] = 0;
	}

	if (BSP_W25Qx_Write(wData, 0x00, Data_Lenth) == W25Qx_OK)
		printfX(" QSPI Write OK!\r\n");
	else
		printfX("something wrong in Step3\r\n");
	/*-Step4- 读数据  ************************************************Step4*/

	if (BSP_W25Qx_Read(rData, 0x00, Data_Lenth) == W25Qx_OK)
		printfX(" QSPI Read ok\r\n\r\n");
	else
		printfX("something wrong in Step4\r\n");

	printfX("QSPI Read Data : \r\n");

	for (i = Data_Lenth - 1; i > Data_Lenth - 0x100; i--)
	{
		sprintf(String_Line, "0x%02X  ", rData[i]);
		printfX(String_Line);
	}

	printfX("\r\n\r\n");
	/*-Step5- 数据对比  ************************************************Step5*/

	if (memcmp(wData, rData, 0x100) == 0)
		printfX(" W25Q64FV QuadSPI Test OK\r\n");
	else
		printfX(" W25Q64FV QuadSPI Test False\r\n");
	
}
