//
// Created by QT on 2023/1/29.
//

#ifndef HARDWARE_TEST_BSP_DISPLAY_H
#define HARDWARE_TEST_BSP_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif


#include "bsp_config.h"
#include "lcd.h"


extern lcd_t g_lcd;  /* œ‘ æ∆˜∂‘œÛ */


int bsp_display_init();



#ifdef __cplusplus
}
#endif

#endif //HARDWARE_TEST_BSP_DISPLAY_H
