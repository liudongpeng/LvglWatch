//
// Created by QT on 2023/1/31.
//

#include "bsp_config.h"



/* ------------------------------ led ------------------------------ */

/**
 * @brief 用户led控制
 * @param[in]	level
 */
void user_led_set_level(uint8_t level)
{
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, \
					  level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}



/* ------------------------------ bmp280 ------------------------------ */

/**
 * @brief bmp280内存写
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
 * @brief bmp280内存读
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
 * @brief 获取左按键电平接口
 * @return
 */
uint8_t btn_left_get_level()
{
	return HAL_GPIO_ReadPin(BTN_LEFT_GPIO_Port, BTN_LEFT_Pin);
}

/**
 * @brief 获取右按键电平接口
 * @return
 */
uint8_t btn_right_get_level()
{
	return HAL_GPIO_ReadPin(BTN_RIGHT_GPIO_Port, BTN_RIGHT_Pin);
}

/**
 * @brief 获取选择按键电平接口
 * @return
 */
uint8_t btn_ok_get_level()
{
	return HAL_GPIO_ReadPin(BTN_OK_GPIO_Port, BTN_OK_Pin);
}

/**
 * @brief 获取当前时间戳, 用于按键扫描
 * @return
 */
uint32_t btn_get_tick()
{
	return HAL_GetTick();
}


/* ------------------------------ display ------------------------------ */

/**
 * @brief 显示器写数据
 * @param[in]	data
 * @param[in]	len
 * @return
 */
int screen_spi_write(uint8_t *data, uint16_t len)
{
	return HAL_SPI_Transmit(&SCREEN_SPI_HANDLER, data, len, 100);
}

/**
 * @brief 设置显示器RES引脚电平
 * @param level
 */
void screen_set_res(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_RES_GPIO_Port, SCREEN_RES_Pin, level);
}

/**
 * @brief 设置显示器DC引脚电平
 * @param level
 */
void screen_set_dc(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_DC_GPIO_Port, SCREEN_DC_Pin, level);
}

/**
 * @brief 设置显示器CS引脚电平
 * @param level
 */
void screen_set_cs(uint8_t level)
{
	HAL_GPIO_WritePin(SCREEN_CS_GPIO_Port, SCREEN_CS_Pin, level);
}
