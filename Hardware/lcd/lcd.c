//
// Created by liudongpeng on 2022/11/17.
//

#include "lcd.h"
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "bsp_config.h"

/**
 * @brief 把屏幕分成8页, 由上到下0~7, 每页30行
 */
#define LCD_PAGE_NUM    8



/**
 * @brief lcd缓存, 30行
 */
//static uint8_t s_lcd_cache[135 * 120 * sizeof(uint16_t)];



static uint16_t endian_exchange_u16(uint16_t* a);

static int lcd_write_cmd(lcd_t* lcd, uint8_t* cmd_list, uint16_t len);
static int lcd_write_data(lcd_t* lcd, uint8_t* data_list, uint16_t len);


/**
 * @brief 创建lcd对象
 * @param[out]	lcd
 * @param[in]	width
 * @param[in]	height
 * @return
 */
int lcd_create(lcd_t* lcd, st7789_t* st, int width, int height)
{
	if (lcd == NULL || st == NULL || width <= 0 || height <= 0)
		return -1;

	lcd->drv = st;
	lcd->width = width;
	lcd->height = height;

	return lcd_init(lcd);
}

/**
 * @brief lcd初始化
 * @param[in]	lcd
 * @return
 */
int lcd_init(lcd_t* lcd)
{
	if (lcd == NULL)
		return -1;

	return st7789_init(lcd->drv);
}

/**
 * @brief 把颜色数据转换为lcd像素颜色格式
 * @param[out]	pixel_color
 * @param[in]	color
 * @return
 */
int lcd_set_pixel_color(lcd_color_t* pixel_color, uint16_t color)
{
	if (pixel_color == NULL)
		return -1;



	return 0;
}

/**
 * @brief lcd清屏
 * @param[in]	lcd
 */
void lcd_clear(lcd_t* lcd)
{
	uint8_t cmd = ST7789_CMD_RAMWR;

	if (lcd == NULL)
		return;

	lcd_set_window_area(lcd, 0, 0, lcd->width, lcd->height);

	/* 内存写, 发送数据从MCU到帧内存(发送其他任何一个命令将打断帧写操作) */
//	lcd_write_cmd(lcd, &cmd, 1);
//	memset(s_lcd_cache, 0xFF, sizeof(s_lcd_cache));
//	for (int i = 0; i < LCD_PAGE_NUM; i++)
//	{
//		lcd_flush_page_cache(lcd);
//	}

	for (int i = 0; i < lcd->height; i++)
	{
		for (int j = 0; j < lcd->width; j++)
		{
			uint16_t empty = 0xFFFF;
			lcd_draw_point(lcd, j, i, (lcd_color_t)empty);

		}
	}
}

/**
 * @brief 全屏填充
 * @param[in]	lcd
 * @param[in]	color
 * @return
 */
int lcd_full(lcd_t* lcd, lcd_color_t color)
{
	if (lcd == NULL)
		return -1;

	for (int y = 0; y < lcd->height; y++)
	{
		for (int x = 0; x < lcd->width; x++)
		{
			lcd_draw_point(lcd, x, y, color);
		}
	}

	return 0;
}

/**
 * @brief 绘制一个点
 * @param[in]	lcd
 * @param[in]	x
 * @param[in]	y
 * @return
 */
int lcd_draw_point(lcd_t* lcd, int x, int y, lcd_color_t color)
{
	uint8_t cmd = ST7789_CMD_RAMWR;
	uint8_t val[2] = {color >> 8, color};

	if (lcd == NULL)
		return -1;

	lcd_set_window_area(lcd, x, y, 1, 1);

	/* 进行内存写操作 */
//	lcd_write_cmd(lcd, &cmd, 1);

//	val[0] = color >> 8;
//	val[1] = color;
	return lcd_write_data(lcd, val, 2);

//	HAL_SPI_Transmit_DMA(&hspi2, val, 2);
//	while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);

	return 0;
}

/**
 * @brief 写命令
 * @param[in]	lcd
 * @param[in]	cmd_list
 * @param[in]	len
 * @return
 */
static int lcd_write_cmd(lcd_t* lcd, uint8_t* cmd_list, uint16_t len)
{
	if (lcd == NULL)
		return -1;

	return st7789_write_cmd(lcd->drv, cmd_list, len);
}

/**
 * @brief 写数据
 * @param[in]	lcd
 * @param[in]	data_list
 * @param[in]	len
 * @return
 */
static int lcd_write_data(lcd_t* lcd, uint8_t* data_list, uint16_t len)
{
	if (lcd == NULL || data_list == NULL || len <= 0)
		return -1;

	return st7789_write_data(lcd->drv, data_list, len);
}

/**
 * @brief lcd软件复位
 * @param[in]	lcd
 * @return
 */
int lcd_soft_reset(lcd_t* lcd)
{
	if (lcd == NULL)
		return -1;

	return st7789_soft_reset(lcd->drv);
}

/**
 * @brief 设置lcd显示区域
 * @param[in]	x
 * @param[in]	y
 * @param[in]	width
 * @param[in]	height
 * @return
 */
int lcd_set_window_area(lcd_t *lcd, int x, int y, int width, int height)
{
	uint8_t cmd;
	uint8_t cmd_list[4];
	int x1 = x + width, y1 = y + height;

	if (lcd == NULL)
		return -1;

	/* 设置列起始地址和结束地址 */
	cmd = ST7789_CMD_CASET;
	lcd_write_cmd(lcd, &cmd, 1);
	x += 52;
	cmd_list[0] = x >> 8;
	cmd_list[1] = x;
	x1 += 51;
	cmd_list[2] = x1 >> 8;
	cmd_list[3] = x1;
	lcd_write_data(lcd, cmd_list, 4);

	/* 设置行起始地址和结束地址 */
	cmd = ST7789_CMD_RASET;
	lcd_write_cmd(lcd, &cmd, 1);
//	y += 40;
	cmd_list[0] = y >> 8;
	cmd_list[1] = y;
//	y1 += 40;
	cmd_list[2] = y1 >> 8;
	cmd_list[3] = y1;
	lcd_write_data(lcd, cmd_list, 4);

	cmd = ST7789_CMD_RAMWR;
	return lcd_write_cmd(lcd, &cmd, 1);
}

/**
 * @brief 大小端互转
 * @param[in]	num
 * @return
 */
static uint16_t endian_exchange_u16(uint16_t* num)
{
	printf("before: %#x, ", *num);
	uint8_t lsb = (*num >> 8);
	*num <<= 8;
	*num |= lsb;
	printf("after: %#x\n", *num);

	return *num;
}

/**
 * @brief 把lvgl颜色转换为lcd颜色
 * @param[in,out] lv_color
 * @return
 */
static lcd_color_t lv_color_convert(uint16_t* lv_color)
{

}


/**
 *
 * @param a
 * @param b
 */
static void swap_u8(uint8_t* a, uint8_t* b)
{
	uint8_t tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * @brief 区域绘制
 * @param[in]	lcd
 * @param[in]	x
 * @param[in]	y
 * @param[in]	width
 * @param[in]	height
 * @param[in]	buf
 * @return
 */
int lcd_draw_area(lcd_t* lcd, int x, int y, int width, int height, uint8_t* buf)
{
	if (lcd == NULL)
		return -1;

	if (width <= 0 || height <= 0)
		return -2;

	uint32_t byte_num = width * height * sizeof(uint16_t);
//	memset(s_lcd_cache, 0, sizeof(s_lcd_cache));
//	memcpy(s_lcd_cache, buf, byte_num);
//
//	uint32_t idx = byte_num - 1;
//	for (uint32_t i = 0; i < idx; i += 2)
//	{
//		swap_u8(&s_lcd_cache[i], &s_lcd_cache[i + 1]);
//	}

	lcd_set_window_area(lcd, x, y, width, height);
	return lcd_write_data(lcd, buf, byte_num);
}

/**
 * @brief 刷新页缓冲
 * @return
 */
int lcd_flush_page_cache(lcd_t* lcd)
{
	int byteToWrite = 0;
	int remain = 0;
	int linePixelSize;
	int ofs = 0;

	if (lcd == NULL)
		return -1;

//	remain = sizeof(s_lcd_cache);
	linePixelSize = lcd->width * sizeof(lcd_color_t);

	/* 循环发送写入缓存中的数据 */
	byteToWrite = linePixelSize;
	while (remain > 0)
	{
		if (remain < linePixelSize)
			byteToWrite = remain;

//		lcd_write_data(lcd, (uint8_t*)s_lcd_cache + ofs, byteToWrite);

		ofs += byteToWrite;
		remain -= byteToWrite;
	}
}


/**
 * @brief 全屏填充
 */
//void LcdFull(int color)
//{
//	int index = 0;
//	int lineCount = 0;
//
//	/* 设置显示区域 */
////	LcdSetWindowArea(0, LCD_SCREEN_WIDTH - 1, 0 , LCD_SCREEN_HEIGHT - 1);
//	lcd_set_window_area(0, 0, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT);
//
//	/* 内存写, 发送数据从MCU到帧内存(发送其他任何一个命令将打断帧写操作) */
//	lcd_write_cmd(ST7789_CMD_RAMWR);
//	for (int row = 0; row < LCD_SCREEN_HEIGHT; row++)
//	{
//		for (int col = 0; col < LCD_SCREEN_WIDTH; col++)
//		{
//			LcdSetPixel(&gLcdCache[index++], color);
//
//			/*LcdWriteByte(color>>8);
//			LcdWriteByte(color);*/
//		}
//		lineCount++;
//
//		if (lineCount >= 30)
//		{
//			index = 0;
//			lineCount = 0;
//			LcdFlushPageCache();
//		}
//	}
//}

/**
 * @brief 刷新lcd页缓存
 */
//void LcdFlushPageCache()
//{
//	/*int byteToWrite = 0;
//	int remain = sizeof(gLcdCache);
//	int linePixelSize = SCREEN_WIDTH * sizeof(pixel_t);
//
//	*//* 循环发送写入缓存中的数据 *//*
//	byteToWrite = linePixelSize;
//	while (remain > 0)
//	{
//		if (remain < linePixelSize)
//			byteToWrite = remain;
//
//		HAL_SPI_Transmit(&SPI_INTERFACE, (uint8_t*)gLcdCache, byteToWrite, 100);
//
//		remain -= byteToWrite;
//	}*/
//
//
//	LCD_DC_SET();
//	HAL_SPI_Transmit(&SPI_INTERFACE, (uint8_t*)gLcdCache, sizeof(gLcdCache), 1000);
//
//
//	/*HAL_SPI_Transmit_DMA(&SPI_INTERFACE, (uint8_t*)gLcdCache, sizeof(gLcdCache));
//	while (__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TC3) != SET);
//	__HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_TC3);*/
//}
