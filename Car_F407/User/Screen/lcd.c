#include <Screen/lcd.h>
#include "BasicInfor/delay.h"

#define SPI_DMA_Max_Length 32768
#define SPI_DMA_Delay_us 180
/**
 * @brief    SPI 发送字节函数
 * @param    TxData 要发送的数据
 * @param    size   发送数据的字节大小
 * @return  0:写入成功,其他:写入失败
 */
uint8_t SPI_WriteByte(uint8_t *TxData, uint16_t size)
{
	return HAL_SPI_Transmit(LCD_SPI, TxData, size, 1000);
	//return HAL_SPI_Transmit_DMA(LCD_SPI, TxData, size);

}
void SPI_WriteByte_DMA(uint8_t *TxData, uint16_t size)
{
	//return HAL_SPI_Transmit(LCD_SPI, TxData, size, 1000);
	HAL_SPI_Transmit_DMA(LCD_SPI, TxData, size);

}
/**
 * @brief   写命令到LCD
 * @param   cmd —— 需要发送的命令
 * @return  none
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
	LCD_WR_RS(0);
	SPI_WriteByte(&cmd, 1);
}

/**
 * @brief   写数据到LCD
 * @param   dat —— 需要发送的数据
 * @return  none
 */
static void LCD_Write_Data(uint8_t dat)
{
	LCD_WR_RS(1);
	SPI_WriteByte(&dat, 1);
}

/**
 * @brief  写16位的数据（两个字节）到LCD
 * @param   dat —— 需要发送的16bit数据
 * @return  none
 */
void LCD_Write_2Bytes(const uint16_t dat)
{
	uint8_t data[2] = { 0 };

	data[0] = dat >> 8;
	LCD_Write_Data(data[0]);
	data[1] = dat;
	LCD_Write_Data(data[1]);

	//SPI_WriteByte((uint8_t*) data, 2);
}

/**
 * @breif   打开LCD显示背光
 * @param   none
 * @return  none
 */
void LCD_DisplayOn(void)
{
	LCD_PWR(1);
}
/**
 * @brief   关闭LCD显示背光
 * @param   none
 * @return  none
 */
void LCD_DisplayOff(void)
{
	LCD_PWR(0);
}

/**
 * @brief   设置数据写入LCD显存区域
 * @param   x1,y1   —— 起点坐标
 * @param   x2,y2   —— 终点坐标
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	/* 指定X方向操作区域 */
	LCD_Write_Cmd(0x2a);
	LCD_Write_Data(x1 >> 8);
	LCD_Write_Data(x1);
	LCD_Write_Data(x2 >> 8);
	LCD_Write_Data(x2);

	/* 指定Y方向操作区域 */
	LCD_Write_Cmd(0x2b);
	LCD_Write_Data(y1 >> 8);
	LCD_Write_Data(y1);
	LCD_Write_Data(y2 >> 8);
	LCD_Write_Data(y2);

	/* 发送该命令，LCD开始等待接收显存数据 */
	LCD_Write_Cmd(0x2C);
}

#define LCD_TOTAL_BUF_SIZE  (240*240*2)
#define LCD_Buf_Size 1152
static uint8_t lcd_buf[LCD_Buf_Size];
/**
 * @brief   以一种颜色清空LCD屏
 * @param   color —— 清屏颜色(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
	uint16_t i, j;
	uint8_t data[2] = { 0 };  //color是16bit的，每个像素点需要两个字节的显存

	/* 将16bit的color值分开为两个单独的字节 */
	data[0] = color >> 8;
	data[1] = color;

	/* 显存的值需要逐字节写入 */
	for (j = 0; j < LCD_Buf_Size / 2; j++)
	{
		lcd_buf[j * 2] = data[0];
		lcd_buf[j * 2 + 1] = data[1];
	}
	/* 指定显存操作地址为全屏幕 */
	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);
	/* 指定接下来的数据为数据 */
	LCD_WR_RS(1);
	/* 将显存缓冲区的数据全部写入缓冲区 */
	for (i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
	{
		SPI_WriteByte(lcd_buf, (uint16_t) LCD_Buf_Size);
	}
}
/**
 * @brief   LCD初始化
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
	/* 初始化和LCD通信的引脚 */
	/* 复位LCD */
	LCD_PWR(0);
	LCD_RST(0);
	HAL_Delay(100);
	LCD_RST(1);

	HAL_Delay(120);

	/* 关闭睡眠模式 */
	LCD_Write_Cmd(0x11);
	HAL_Delay(120);

	/* 开始设置显存扫描模式，数据格式等 */
	LCD_Write_Cmd(0x36);
	LCD_Write_Data(0x00);
	/* RGB 5-6-5-bit格式  */
	LCD_Write_Cmd(0x3A);
	LCD_Write_Data(0x65);
	/* porch 设置 */
	LCD_Write_Cmd(0xB2);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x33);
	LCD_Write_Data(0x33);
	/* VGH设置 */
	LCD_Write_Cmd(0xB7);
	LCD_Write_Data(0x72);
	/* VCOM 设置 */
	LCD_Write_Cmd(0xBB);
	LCD_Write_Data(0x3D);
	/* LCM 设置 */
	LCD_Write_Cmd(0xC0);
	LCD_Write_Data(0x2C);
	/* VDV and VRH 设置 */
	LCD_Write_Cmd(0xC2);
	LCD_Write_Data(0x01);
	/* VRH 设置 */
	LCD_Write_Cmd(0xC3);
	LCD_Write_Data(0x19);
	/* VDV 设置 */
	LCD_Write_Cmd(0xC4);
	LCD_Write_Data(0x20);
	/* 普通模式下显存速率设置 60Mhz */
	LCD_Write_Cmd(0xC6);
	LCD_Write_Data(0x0F);
	/* 电源控制 */
	LCD_Write_Cmd(0xD0);
	LCD_Write_Data(0xA4);
	LCD_Write_Data(0xA1);
	/* 电压设置 */
	LCD_Write_Cmd(0xE0);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2B);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x54);
	LCD_Write_Data(0x4C);
	LCD_Write_Data(0x18);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x23);
	/* 电压设置 */
	LCD_Write_Cmd(0xE1);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x44);
	LCD_Write_Data(0x51);
	LCD_Write_Data(0x2F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x20);
	LCD_Write_Data(0x23);
	/* 显示开 */
	LCD_Write_Cmd(0x21);
	LCD_Write_Cmd(0x29);

	/* 清屏为白色 */
	LCD_Clear(WHITE);

	/*打开显示*/
	LCD_PWR(1);
}

/**
 * @brief		显示一个ASCII码字符
 * @param   x,y		显示起始坐标
 * @param   ch		需要显示的字符
 * @param   size	字体大小(支持16/24/32号字体)
 * @return  none
 * @note		需要font.h字库文件的支持
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{
	uint32_t i = 0, j = 0;
	uint8_t temp = 0;
	uint8_t size = 0;
	uint8_t t = 0;

	/* 检测显示是否会越界 */
	if ((x > (LCD_Width - font_size / 2)) || (y > (LCD_Height - font_size)))
		return;

	/* 根据字符大小设置显存操作区域 */
	LCD_Address_Set(x, y, x + font_size / 2 - 1, y + font_size - 1);

	/* 计算字符在字库中的偏移值*/
	uint8_t charIndex = ch - ' ';

	/* 显示16号/32号字体 */
	if ((font_size == 16) || (font_size == 32))
	{
		/* 计算字体一个字符对应点阵集所占的字节数 */
		size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

		for (i = 0; i < size; i++)
		{
			if (font_size == 16)
				temp = F16X8[charIndex][i];	//调用1608字体
			else if (font_size == 32)
				temp = F32X16[charIndex][i];	//调用3216字体
			else
				return;			//没有的字库

			for (j = 0; j < 8; j++)
			{
				if (temp & 0x80)
					LCD_Write_2Bytes(font_color);
				else
					LCD_Write_2Bytes(back_color);

				temp <<= 1;
			}
		}
	}
	/* 显示12号字体 */
	else if (font_size == 12)
	{
		/* 计算字体一个字符对应点阵集所占的字节数 */
		size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

		for (i = 0; i < size; i++)
		{
			temp = F12X6[charIndex][i];

			for (j = 0; j < 6; j++)
			{
				if (temp & 0x80)
					LCD_Write_2Bytes(font_color);
				else
					LCD_Write_2Bytes(back_color);

				temp <<= 1;
			}
		}
	}
	/* 显示24号字体 */
	else if (font_size == 24)
	{
		/* 计算字体一个字符对应点阵集所占的字节数 */
		size = (font_size * 16) / 8;

		for (i = 0; i < size; i++)
		{
			temp = F24X12[charIndex][i];
			if (i % 2 == 0)
				t = 8;
			else
				t = 4;
			for (j = 0; j < t; j++)
			{
				if (temp & 0x80)
					LCD_Write_2Bytes(font_color);
				else
					LCD_Write_2Bytes(back_color);

				temp <<= 1;
			}
		}
	}
	/* 其余字体 */
	else
		return;
}

/**
 * @brief		显示一个ASCII码字符串
 * @param   x,y		显示起始坐标
 * @param   str		需要显示的字符串
 * @param   size	字体大小(支持16/24/32号字体)
 * @return  none
 * @note		1. 需要font.h字库文件的支持
 * 					2. 超过指定width不显示超过的字符
 */
void LCD_ShowCharStr(uint16_t x, uint16_t y, uint8_t max_width, char *str, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{

	max_width += x;

	while ((*str <= '~') && (*str >= ' '))	//判断是否非法字符
	{
		if (x >= max_width)
		{
			//x方向越界，结束
			break;
		}

		LCD_ShowChar(x, y, *str, back_color, font_color, font_size);
		x += font_size / 2;
		str++;
	}
}

/**
 * @brief	带颜色画点函数
 * @param   x,y	—— 画点坐标
 * @return  none
 */
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_Address_Set(x, y, x, y);
	LCD_Write_2Bytes(color);
}

/**
 * @brief		带颜色画线函数(直线、斜线)
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 * @return  none
 */
void LCD_Draw_ColorLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t i = 0;
	int16_t delta_x = 0, delta_y = 0;
	int8_t incx = 0, incy = 0;
	uint16_t distance = 0;
	uint16_t t = 0;
	uint16_t x = 0, y = 0;
	uint16_t x_temp = 0, y_temp = 0;

	if (y1 == y2)
	{
		/* 快速画水平线 */
		LCD_Address_Set(x1, y1, x2, y2);

		for (i = 0; i < x2 - x1; i++)
		{
			lcd_buf[2 * i] = color >> 8;
			lcd_buf[2 * i + 1] = color;
		}

		LCD_WR_RS(1);
		SPI_WriteByte(lcd_buf, (x2 - x1) * 2);
		return;
	}
	else
	{
		/* 画斜线（Bresenham算法） */
		/* 计算两点之间在x和y方向的间距，得到画笔在x和y方向的步进值 */
		delta_x = x2 - x1;
		delta_y = y2 - y1;
		if (delta_x > 0)
		{
			//斜线(从左到右)
			incx = 1;
		}
		else if (delta_x == 0)
		{
			//垂直斜线(竖线)
			incx = 0;
		}
		else
		{
			//斜线(从右到左)
			incx = -1;
			delta_x = -delta_x;
		}
		if (delta_y > 0)
		{
			//斜线(从左到右)
			incy = 1;
		}
		else if (delta_y == 0)
		{
			//水平斜线(水平线)
			incy = 0;
		}
		else
		{
			//斜线(从右到左)
			incy = -1;
			delta_y = -delta_y;
		}

		/* 计算画笔打点距离(取两个间距中的最大值) */
		if (delta_x > delta_y)
		{
			distance = delta_x;
		}
		else
		{
			distance = delta_y;
		}

		/* 开始打点 */
		x = x1;
		y = y1;
		//第一个点无效，所以t的次数加一
		for (t = 0; t <= distance + 1; t++)
		{
			LCD_Draw_ColorPoint(x, y, color);

			/* 判断离实际值最近的像素点 */
			x_temp += delta_x;
			if (x_temp > distance)
			{
				//x方向越界，减去距离值，为下一次检测做准备
				x_temp -= distance;
				//在x方向递增打点
				x += incx;

			}
			y_temp += delta_y;
			if (y_temp > distance)
			{
				//y方向越界，减去距离值，为下一次检测做准备
				y_temp -= distance;
				//在y方向递增打点
				y += incy;
			}
		}
	}
}

/**
 * @breif	带颜色画矩形函数
 * @param   x1,y1 —— 矩形起始点
 * @param	x2,y2 —— 矩形终点
 * @param	color	—— 颜色
 * @retval	none
 */
void LCD_Draw_ColorRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	LCD_Draw_ColorLine(x1, y1, x2, y1, color);
	LCD_Draw_ColorLine(x1, y1, x1, y2, color);
	LCD_Draw_ColorLine(x1, y2, x2, y2, color);
	LCD_Draw_ColorLine(x2, y1, x2, y2, color);
}

/**
 * @breif	带颜色画圆函数
 * @param   x1,x2 —— 圆心坐标
 * @param	r —— 半径
 * @param	color —— 颜色
 * @retval	none
 */
void LCD_Draw_ColorCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
	/* Bresenham画圆算法 */
	int16_t a = 0, b = r;
	int16_t d = 3 - (r << 1);		//算法决策参数

	/* 如果圆在屏幕可见区域外，直接退出 */
	if (x - r < 0 || x + r > LCD_Width || y - r < 0 || y + r > LCD_Height)
		return;

	/* 开始画圆 */
	while (a <= b)
	{
		LCD_Draw_ColorPoint(x - b, y - a, color);
		LCD_Draw_ColorPoint(x + b, y - a, color);
		LCD_Draw_ColorPoint(x - a, y + b, color);
		LCD_Draw_ColorPoint(x - b, y - a, color);
		LCD_Draw_ColorPoint(x - a, y - b, color);
		LCD_Draw_ColorPoint(x + b, y + a, color);
		LCD_Draw_ColorPoint(x + a, y - b, color);
		LCD_Draw_ColorPoint(x + a, y + b, color);
		LCD_Draw_ColorPoint(x - b, y + a, color);
		a++;

		if (d < 0)
			d += 4 * a + 6;
		else
		{
			d += 10 + 4 * (a - b);
			b--;
		}

		LCD_Draw_ColorPoint(x + a, y + b, color);
	}
}
/**
 * @brief		显示图片函数
 * @param   x,y		起点坐标
 * @param   width	图片宽度
 * @param   height	图片高度
 * @param   p —— 图片缓存数据起始地址
 * @note		Image2Lcd取模方式：C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
 * @return  none
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
	uint32_t img_size = width * height * 2;		//图片所占字节数
	uint32_t remain_size = img_size;									//图片每次发送后剩余的字节数
	uint8_t i = 0;
	uint8_t flag = 0;

	/* 错误检测 */
	if (x + width > LCD_Width || y + height > LCD_Height)
	{
		return;
	}

	LCD_Address_Set(x, y, x + width - 1, y + height - 1);

	LCD_WR_RS(1);

	/* SPI每次最大发送2^16 = 65536个数据,图片最大大小为240*240*2 = 115200，会超过此大小，所以设计循环发送算法 */
	for (i = 0; i <= img_size / SPI_DMA_Max_Length; i++)
	{
		if (flag != 0)
		{
			//非首次进入 等待传输完成
			delay_us(SPI_DMA_Delay_us);
		}
		if (remain_size / SPI_DMA_Max_Length >= 1)
		{
			SPI_WriteByte_DMA((uint8_t*) p, SPI_DMA_Max_Length - 1);
			p += SPI_DMA_Max_Length - 1;
			remain_size -= SPI_DMA_Max_Length - 1;

		}
		else
		{
			SPI_WriteByte_DMA((uint8_t*) p, remain_size % (SPI_DMA_Max_Length - 1));
		}

		flag = 1;
	}
}

uint16_t LCD_FontColor = WHITE;
uint16_t LCD_BackgroundColor = BLACK;

//显示一行字符串
void LCD_ShowStringLineX(uint8_t line, char *chr, uint8_t size)
{
	LCD_ShowCharStr(0, line * size, LCD_Width, chr, LCD_BackgroundColor, LCD_FontColor, size);
}

//自定义初始化过程
void LCD_InitX(const unsigned char BMP[])
{
//	LCD_Init();
//	LCD_Clear(BLACK);
//
//	LCD_Show_Image(0, 0, LCD_Width, LCD_Height, BMP);
//	HAL_Delay(2000);
//	LCD_Clear(BLACK);
}

void LCD_Close()
{

	LCD_Write_Cmd(0x53);
	LCD_Write_Cmd(0x28);
	LCD_Write_Cmd(0x10);
	LCD_PWR(0);
	LCD_RST(0);
}
