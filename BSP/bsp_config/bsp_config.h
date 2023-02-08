//
// Created by QT on 2023/1/31.
//

#ifndef LVGLWATCH_BSP_CONFIG_H
#define LVGLWATCH_BSP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

#include "rtc.h"
#include "spi.h"
#include "i2c.h"
#include "tim.h"

#include "led.h"
#include "button.h"
#include "bmp280.h"
#include "lcd.h"


/* ------------------------------ led ------------------------------ */
void user_led_set_level(uint8_t level);


/* ------------------------------ button ------------------------------ */
#define BTN_SCAN_TIM TIM4            /* 按键扫描定时器 */
#define BTN_SCAN_TIM_HANDLER htim4   /* 按键扫描定时器句柄 */

uint8_t btn_left_get_level();
uint8_t btn_right_get_level();
uint8_t btn_ok_get_level();
uint32_t btn_get_tick();


/* ------------------------------ display ------------------------------ */
#define SCREEN_SPI    SPI1            /* 屏幕SPI接口 */
#define SCREEN_SPI_HANDLER    hspi1   /* 屏幕SPI接口句柄 */

#define LVGL_TICK_TIM TIM3          /* lvgl tick定时器 */
#define LVGL_TICK_TIM_HANDLE  htim3 /* lvgl tick定时器句柄 */

int screen_spi_write(uint8_t *data, uint16_t len);
void screen_set_res(uint8_t level);
void screen_set_dc(uint8_t level);
void screen_set_cs(uint8_t level);


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_BSP_CONFIG_H
