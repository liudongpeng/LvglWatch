//
// Created by QT on 2023/1/29.
//

#ifndef HARDWARE_TEST_BSP_LED_H
#define HARDWARE_TEST_BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif


#include "bsp_config.h"
#include "led.h"


extern led_t user_led;	/* ”√ªßled */


int bsp_led_init();


#ifdef __cplusplus
}
#endif

#endif //HARDWARE_TEST_BSP_LED_H
