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
#define BTN_SCAN_TIM TIM4            /* ����ɨ�趨ʱ�� */
#define BTN_SCAN_TIM_HANDLER htim4   /* ����ɨ�趨ʱ����� */

uint8_t btn_left_get_level();
uint8_t btn_right_get_level();
uint8_t btn_ok_get_level();
uint32_t btn_get_tick();


/* ------------------------------ display ------------------------------ */
#define SCREEN_SPI    SPI1            /* ��ĻSPI�ӿ� */
#define SCREEN_SPI_HANDLER    hspi1   /* ��ĻSPI�ӿھ�� */

#define LVGL_TICK_TIM TIM3          /* lvgl tick��ʱ�� */
#define LVGL_TICK_TIM_HANDLE  htim3 /* lvgl tick��ʱ����� */

int screen_spi_write(uint8_t *data, uint16_t len);
void screen_set_res(uint8_t level);
void screen_set_dc(uint8_t level);
void screen_set_cs(uint8_t level);


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_BSP_CONFIG_H
