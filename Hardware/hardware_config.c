////
//// Created by liu on 2023/1/19.
////
//
//#include "hardware_config.h"
//
//#if 0
//
///* ------------------------------ led ------------------------------ */
//
///**
// * @brief 板载led控制
// * @param[in]   level
// */
//void board_led_set_level(uint8_t level)
//{
//    HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin,
//                      level ? GPIO_PIN_SET : GPIO_PIN_RESET);
//}
//
///**
// * @brief 用户led控制
// * @param[in]	level
// */
//void user_led_set_level(uint8_t level)
//{
//    HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin,
//                      level ? GPIO_PIN_SET : GPIO_PIN_RESET);
//}
//
//
//
///* ------------------------------ button ------------------------------ */
//
///**
// * @brief 获取按键电平接口
// * @return
// */
//uint8_t btn_left_get_level()
//{
//    return HAL_GPIO_ReadPin(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin);
//}
//
///**
// * @brief 获取当前时间戳
// * @return
// */
//uint32_t btn_get_tick()
//{
//    return HAL_GetTick();
//}
//
//
////void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
////{
////	if (htim->Instance == TIM4)
////	{
////		button_ticks();
////	}
////}
//
///* ------------------------------ oled12864 ------------------------------ */
//
///**
// * @brief 写ssd1306的接口
// * @param[in]	data
// * @param[in]	len
// * @return
// */
//int oled_spi_write(uint8_t *data, uint16_t len)
//{
//	return HAL_SPI_Transmit(&hspi1, data, len, 1000);
//}
//
///**
// * @brief 设置oled res引脚电平
// * @param level
// */
//void oled_set_res(uint8_t level)
//{
//	HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, level);
//}
//
///**
// * @brief 设置oled dc引脚电平
// * @param level
// */
//void oled_set_dc(uint8_t level)
//{
//	HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, level);
//}
//
///**
// * @brief 设置oled cs引脚电平
// * @param level
// */
//void oled_set_cs(uint8_t level)
//{
//	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, level);
//}
//
//
//#endif