//
// Created by QT on 2023/1/29.
//

#include "bsp_led.h"


led_t g_user_led;	/* �û�led */


/**
 * @brief ��ʼ�������ϵ�led
 * @return
 */
int bsp_led_init()
{
	/* ʵ����led���� */
	led_create(&g_user_led, 0, user_led_set_level);

	led_close(&g_user_led);

	return 0;
}
