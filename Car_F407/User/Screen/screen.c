/*
 * screen.c
 *
 *  Created on: Nov 16, 2022
 *      Author: Little Fish
 */
#include "screen.h"
#include "image.h"
#include "BasicInfor/debug.h"
#include "FATFS/bsp_fatfs.h"
#include "Flash/W25QXX.h"
#include "fatfs.h"
#include "bmp.h"
#include "BasicInfor/delay.h"

#define LCD_File_Disp_Delay_us 40
#define ImageBufferLength 115208
uint8_t ImageBuffer[ImageBufferLength];
char ImageNameString[40];

void Screen_Init()
{
#if defined USE_OLED
	OLED_InitX(PuppySauce);
#else
	LCD_Init();
#if defined SCREEN_USE_FATFS
	Screen_ShowPicture_Num(0, 0);
	HAL_Delay(2000);
#endif
	Screen_Clear();
#endif
}

void Screen_ShowPicture(unsigned char x0, unsigned char y0, uint16_t length, uint16_t width, const unsigned char BMP[])
{
#if defined USE_OLED
	OLED_DrawBMP(x0, y0, x0 + length, y0 + width, BMP);
#else
	LCD_Show_Image(x0, y0, length, width, BMP);
#endif
}

void Screen_ShowStringLine(uint8_t line, char *chr, uint8_t size)
{
#if defined USE_OLED
	OLED_ShowStringLine(line, chr);
#else
	LCD_ShowStringLineX(line, chr, size);
#endif
}

void Screen_Clear()
{
#if defined USE_OLED
	OLED_Clear();
#else
	LCD_Clear(BLACK);
#endif
}

void Screen_ShowPicture_Num(uint8_t a, uint8_t b)
{
#if defined USE_OLED
	switch (a)
	{
		case 0:
			Screen_ShowPicture(31, 0, 64, 8, PuppySauce);
			break;
		default:
			break;
	}
#else
	switch (a)
	{
		case 0:
#if defined SCREEN_USE_FATFS
			Screen_ShowFileImage("Images/PuppySauce.bmp");
			//Screen_ShowFileImage("test17.bmp");
#else
			LCD_Show_Image(0, 0, 240, 240, PuppySauce_16Color);
#endif
			break;
		case 1:
			sprintf(ImageNameString, "Images/miku/image%03d.bmp", b);
			Screen_ShowFileImage(ImageNameString);
			break;
		default:
			break;
	}
#endif
}

void Screen_Close()
{
#if defined USE_OLED

#else
	LCD_Close();
#endif
}
#if defined SCREEN_USE_FLASH
void Screen_ShowFlashImage(uint32_t ReadAddr, uint32_t Size)
{

	char String_Line[40];

	sprintf(String_Line, "读取 地址：%lX 长度：%d 图片%s\r\n", ReadAddr, (int) Size, BSP_W25Qx_Read(ImageBuffer, ReadAddr, Size) == W25Qx_OK ? "成功" : "失败");
	printfX(String_Line);

	Screen_ShowPicture(0, 0, 240, 240, ImageBuffer);

}
#endif

#if defined SCREEN_USE_FATFS

//单次显示行数 越多速度越快
#define  readRowNum 15
BYTE pColorData[960 * readRowNum]; /* 一行真彩色数据缓存 240 * 2 = 480 */
uint8_t ImageTransform_Buffer[480 * readRowNum];

void Screen_ShowFileImage(const char *fileName)
{
	//uint32_t time = uwTick;

	int i, j, k;
	int width, height, l_width;

	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;
	unsigned int read_num;
	f_res = f_open(&file, fileName, FA_OPEN_EXISTING | FA_READ);
	/*-------------------------------------------------------------------------------------------------------*/
	if (f_res == FR_OK)
	{
		//printfX("打开文件成功\r\n");

		/* 读取文件头信息  两个字节*/
		f_read(&file, &fileType, sizeof(WORD), &read_num);

		/* 判断是不是bmp文件 "BM"*/
		if (fileType != 0x4d42)
		{
			printfX("这不是一个 .bmp 文件!\r\n");
			//			return;
		}
		else
		{
			//printfX("这是一个 .bmp 文件\r\n");
		}

		/* 读取BMP文件头信息*/
		f_read(&file, &bitHead, sizeof(tagBITMAPFILEHEADER), &read_num);
		//showBmpHead(&bitHead);

		/* 读取位图信息头信息 */
		f_read(&file, &bitInfoHead, sizeof(BITMAPINFOHEADER), &read_num);
		//showBmpInforHead(&bitInfoHead);
	}
	else
	{
		printfX("打开文件失败!错误代码：f_res = %d \r\n", f_res);
	}
	/*-------------------------------------------------------------------------------------------------------*/
	width = 240;
	height = 240;
	uint16_t temp = 0;
	int row, m;
	/* 计算位图的实际宽度并确保它为32的倍数	*/
	l_width = WIDTHBYTES(width * bitInfoHead.biBitCount);

	if (l_width > 720)
	{
		printfX("\n 本图片太大(%d)，无法在液晶屏上显示 (<=240)\n", l_width);
	}

	for (i = 0; i < height / readRowNum; i++)
	{
		/*从文件的后面读起，BMP文件的原始图像方向为右下角到左上角*/
		f_lseek(&file, bitHead.bfOffBits + (height - (i + 1) * readRowNum) * l_width);
		/* 读取一行bmp的数据到数组pColorData里面 */
		f_read(&file, pColorData, l_width * readRowNum, &read_num);

		for (m = 0; m < readRowNum; m++)
		{
			for (j = 0; j < width; j++) 											   //一行有效信息
			{
				k = j * 3 + m * width * 3;																	 //一行中第K个像素的起点
				row = readRowNum - m - 1;
				temp = RGB24TORGB16(pColorData[k + 2], pColorData[k + 1], pColorData[k]);
				ImageTransform_Buffer[row * 480 + j * 2] = temp >> 8;
				ImageTransform_Buffer[row * 480 + j * 2 + 1] = temp;

				/* 使用 image2lcd 软件生成16位BMP图像时，
				 * 内部格式: GGGB BBBB XRRR RRGG
				 * LCD格式：RRRR RGGG GGGB BBBB
				 * 需要依据上诉格式进行转换*/

//				k = j * 2 + m * width * 2;																	 //一行中第K个像素的起点
//				row = readRowNum - m - 1;
//				ImageTransform_Buffer[row * 480 + j * 2] = ((pColorData[k + 1] << 1) | pColorData[k] >> 7);
//				ImageTransform_Buffer[row * 480 + j * 2 + 1] = (pColorData[k] & 0x1F) | ((pColorData[k] & 0xE0) << 1);
			}
		}

		LCD_Show_Image(0, i * readRowNum, 240, readRowNum, ImageTransform_Buffer);
		delay_us(LCD_File_Disp_Delay_us);
	}
	// 不再读写，关闭文件 //
	f_close(&file);
	// 不再使用，取消挂载 //
	//f_res = f_mount(NULL, (TCHAR const*) SDPath, 1);

	// 注销一个FatFS设备：SD卡 //
	//FATFS_UnLinkDriver(SDPath);

	//printfX("总耗时：%.d ms\n\n", (int )(uwTick - time));
}
#endif
