//
// Created by liudongpeng on 2022/11/17.
//

#ifndef ST7789_LCD240240_H
#define ST7789_LCD240240_H


#include "st7789.h"


/* RES引脚配置 */
//#define LCD_RES_RCC_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
//#define LCD_RES_GPIO_PORT       GPIOC
//#define LCD_RES_GPIO_PIN        GPIO_PIN_1
//#define LCD_RES_SET()           HAL_GPIO_WritePin(LCD_RES_GPIO_PORT, LCD_RES_GPIO_PIN, GPIO_PIN_SET)
//#define LCD_RES_RESET()         HAL_GPIO_WritePin(LCD_RES_GPIO_PORT, LCD_RES_GPIO_PIN, GPIO_PIN_RESET)

/* DC引脚配置 */
//#define LCD_DC_RCC_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
//#define LCD_DC_GPIO_PORT        GPIOG
//#define LCD_DC_GPIO_PIN         GPIO_PIN_0
//#define LCD_DC_SET()            HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_GPIO_PIN, GPIO_PIN_SET)
//#define LCD_DC_RESET()          HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_GPIO_PIN, GPIO_PIN_RESET)

/* BLK引脚配置 */
//#define LCD_CS_RCC_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
//#define LCD_CS_GPIO_PORT        GPIOG
//#define LCD_CS_GPIO_PIN         GPIO_PIN_1
//#define LCD_CS_SET()            HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, GPIO_PIN_SET)
//#define LCD_CS_RESET()          HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, GPIO_PIN_RESET)


#define     RED          0XF800	  //红色
#define     GREEN        0X07E0	  //绿色
#define     BLUE         0X001F	  //蓝色


/* 像素颜色格式 */
#define RGB_FORMAT    565


/**
 * @brief lcd屏幕对象
 */
typedef struct lcd
{
	st7789_t* drv;  /* ST7789驱动IC */

	int width;      /* 屏幕宽度 */
	int height;     /* 屏幕高度 */

} lcd_t;


/**
 * @brief lcd像素颜色
 */
typedef uint16_t lcd_color_t;
//typedef union
//{
//	struct pixel_color
//	{
//		uint16_t b: 5;
//		uint16_t g: 6;
//		uint16_t r: 5;
//	} color;
//	uint16_t full;
//} lcd_color_t;



int lcd_create(lcd_t* lcd, st7789_t* st, int width, int height);
int lcd_init(lcd_t* lcd);
int lcd_soft_reset(lcd_t* lcd);
void lcd_clear(lcd_t* lcd);
int lcd_full(lcd_t* lcd, lcd_color_t color);
int lcd_draw_point(lcd_t* lcd, int x, int y, lcd_color_t color);
int lcd_set_window_area(lcd_t *lcd, int x, int y, int width, int height);
int lcd_draw_area(lcd_t* lcd, int x, int y, int width, int height, uint8_t* buf);

int lcd_set_pixel_color(lcd_color_t* pixel_color, uint16_t color);

int lcd_flush_page_cache(lcd_t* lcd);

#endif //ST7789_LCD240240_H
