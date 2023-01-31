//
// Created by QT on 2023/1/29.
//

#include "bsp_led.h"


led_t user_led;	/* 用户led */


/**
 * @brief 初始化板子上的led
 * @return
 */
int bsp_led_init()
{
	/* 实例化led对象 */
	led_create(&user_led, 0, user_led_set_level);

	led_close(&user_led);

	return 0;
}
