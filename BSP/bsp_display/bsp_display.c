//
// Created by QT on 2023/1/29.
//

#include "bsp_display.h"


st7789_t st; /* ST7789驱动IC对象 */
lcd_t g_lcd;  /* 显示器对象 */


/**
 * @brief 初始化显示器
 * @return
 */
int bsp_display_init()
{
	int ret = 0;

	/* 实例化ST7789驱动IC对象 */
	ret = st7789_create(&st, screen_spi_write, screen_set_res, screen_set_dc, screen_set_cs);
	if (ret < 0)
		return -1;

	/* 实例化显示器对象 */
	ret = lcd_create(&g_lcd, &st, 135, 240);
	if (ret < 0)
		return -2;

	return lcd_init(&g_lcd);
}
