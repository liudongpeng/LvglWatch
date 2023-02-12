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



/* ------------------------------ bmp280 ------------------------------ */

/**
 * @brief bmp280�ڴ�д
 * @param[in]	dev_addr
 * @param[in]	reg_addr
 * @param[in]	buf
 * @param[in]	len
 * @return
 */
int bmp280_i2c_mem_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len)
{
	return HAL_I2C_Mem_Write(&BMP280_I2C_HANDLER, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
}

/**
 * @brief bmp280�ڴ��
 * @param[in]	dev_addr
 * @param[in]	reg_addr
 * @param[out]	buf
 * @param[in]	len
 * @return
 */
int bmp280_i2c_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len)
{
	return HAL_I2C_Mem_Read(&BMP280_I2C_HANDLER, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
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
 * @brief ��ʾ��д����
 * @param[in]	data
 * @param[in]	len
 * @return
 */
int screen_spi_write(uint8_t *data, uint16_t len)
{
	return HAL_SPI_Transmit(&SCREEN_SPI_HANDLER, data, len, 100);
}

/**
 * @brief ������ʾ��RES���ŵ�ƽ
 * @param level
 */
void screen_set_res(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_RES_GPIO_Port, SCREEN_RES_Pin, level);
}

/**
 * @brief ������ʾ��DC���ŵ�ƽ
 * @param level
 */
void screen_set_dc(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_DC_GPIO_Port, SCREEN_DC_Pin, level);
}

/**
 * @brief ������ʾ��CS���ŵ�ƽ
 * @param level
 */
void screen_set_cs(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_CS_GPIO_Port, SCREEN_CS_Pin, level);
}
