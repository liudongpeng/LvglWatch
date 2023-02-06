//
// Created by liudongpeng on 2022/11/17.
//

#ifndef ST7789_LCD240240_H
#define ST7789_LCD240240_H


#include "st7789.h"


/* RES�������� */
//#define LCD_RES_RCC_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
//#define LCD_RES_GPIO_PORT       GPIOC
//#define LCD_RES_GPIO_PIN        GPIO_PIN_1
//#define LCD_RES_SET()           HAL_GPIO_WritePin(LCD_RES_GPIO_PORT, LCD_RES_GPIO_PIN, GPIO_PIN_SET)
//#define LCD_RES_RESET()         HAL_GPIO_WritePin(LCD_RES_GPIO_PORT, LCD_RES_GPIO_PIN, GPIO_PIN_RESET)

/* DC�������� */
//#define LCD_DC_RCC_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
//#define LCD_DC_GPIO_PORT        GPIOG
//#define LCD_DC_GPIO_PIN         GPIO_PIN_0
//#define LCD_DC_SET()            HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_GPIO_PIN, GPIO_PIN_SET)
//#define LCD_DC_RESET()          HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_GPIO_PIN, GPIO_PIN_RESET)

/* BLK�������� */
//#define LCD_CS_RCC_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
//#define LCD_CS_GPIO_PORT        GPIOG
//#define LCD_CS_GPIO_PIN         GPIO_PIN_1
//#define LCD_CS_SET()            HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, GPIO_PIN_SET)
//#define LCD_CS_RESET()          HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, GPIO_PIN_RESET)


#define     RED          0XF800	  //��ɫ
#define     GREEN        0X07E0	  //��ɫ
#define     BLUE         0X001F	  //��ɫ


/* ������ɫ��ʽ */
#define RGB_FORMAT    565


/**
 * @brief lcd��Ļ����
 */
typedef struct lcd
{
	st7789_t* drv;  /* ST7789����IC */

	int width;      /* ��Ļ��� */
	int height;     /* ��Ļ�߶� */

} lcd_t;


/**
 * @brief lcd������ɫ
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
