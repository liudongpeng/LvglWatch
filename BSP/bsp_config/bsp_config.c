//
// Created by QT on 2023/1/31.
//

#include "bsp_config.h"



/* ------------------------------ led ------------------------------ */

/**
 * @brief �û�led����
 * @param[in]	level
 */
void user_led_set_level(uint8_t level)
{
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, \
					  level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


/* ------------------------------ button ------------------------------ */

/**
 * @brief ��ȡ�󰴼���ƽ�ӿ�
 * @return
 */
uint8_t btn_left_get_level()
{
	return HAL_GPIO_ReadPin(BTN_LEFT_GPIO_Port, BTN_LEFT_Pin);
}

/**
 * @brief ��ȡ�Ұ�����ƽ�ӿ�
 * @return
 */
uint8_t btn_right_get_level()
{
	return HAL_GPIO_ReadPin(BTN_RIGHT_GPIO_Port, BTN_RIGHT_Pin);
}

/**
 * @brief ��ȡѡ�񰴼���ƽ�ӿ�
 * @return
 */
uint8_t btn_ok_get_level()
{
	return HAL_GPIO_ReadPin(BTN_OK_GPIO_Port, BTN_OK_Pin);
}

/**
 * @brief ��ȡ��ǰʱ���, ���ڰ���ɨ��
 * @return
 */
uint32_t btn_get_tick()
{
	return HAL_GetTick();
}


/* ------------------------------ display ------------------------------ */

/**
 * @brief
 * @param[in]	data
 * @param[in]	len
 * @return
 */
int screen_spi_write(uint8_t *data, uint16_t len)
{
	return HAL_SPI_Transmit(&SCREEN_SPI_HANDLER, data, len, 1000);
}

/**
 * @brief
 * @param level
 */
void screen_set_res(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_RES_GPIO_Port, SCREEN_RES_Pin, level);
}

/**
 * @brief
 * @param level
 */
void screen_set_dc(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_DC_GPIO_Port, SCREEN_DC_Pin, level);
}

/**
 * @brief
 * @param level
 */
void screen_set_cs(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_CS_GPIO_Port, SCREEN_CS_Pin, level);
}
