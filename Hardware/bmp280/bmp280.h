/**
  * @file    bmp280.h
  * @author  LiuDongPeng
  * @version V0.1
  * @date    2022-12-25
  * @brief   BMP280气压传感器模块头文件
  *
  * @attention
  * 使用此bmp280模块, 需要用户实现通过I2C总线读写器件寄存器的接口, 接口函数格式为:
  * int i2c_mem_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);
  * int i2c_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);
  *
  * @changelog
  * 2023-1-5	修改变量, 函数等的命名风格为小写字母加下划线组合
  */

#ifndef F103_TEST_BMP280_H
#define F103_TEST_BMP280_H


#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif



#define BMP280_I2C_ADDRESS  0x76
#define BMP280_CHIPID       0x58
#define BMP280_SOFTRESET    0xB6

/**
 * @brief BMP280的寄存器
 */
enum bmp280_register
{
	BMP280_REGISTER_DIG_T1 = 0x88,
	BMP280_REGISTER_DIG_T2 = 0x8A,
	BMP280_REGISTER_DIG_T3 = 0x8C,

	BMP280_REGISTER_DIG_P1 = 0x8E,
	BMP280_REGISTER_DIG_P2 = 0x90,
	BMP280_REGISTER_DIG_P3 = 0x92,
	BMP280_REGISTER_DIG_P4 = 0x94,
	BMP280_REGISTER_DIG_P5 = 0x96,
	BMP280_REGISTER_DIG_P6 = 0x98,
	BMP280_REGISTER_DIG_P7 = 0x9A,
	BMP280_REGISTER_DIG_P8 = 0x9C,
	BMP280_REGISTER_DIG_P9 = 0x9E,

	BMP280_REGISTER_CHIPID = 0xD0,
	BMP280_REGISTER_VERSION = 0xD1,
	BMP280_REGISTER_SOFTRESET = 0xE0,

	BMP280_REGISTER_CAL26 = 0xE1, // R calibration stored in 0xE1-0xF0

	BMP280_REGISTER_CONTROL = 0xF4,
	BMP280_REGISTER_CONFIG = 0xF5,
	BMP280_REGISTER_PRESSUREDATA = 0xF7,
	BMP280_REGISTER_TEMPDATA = 0xFA,
};

/*! @name Calibration parameters' relative position */
#define BMP280_DIG_T1_LSB_POS   0
#define BMP280_DIG_T1_MSB_POS   1
#define BMP280_DIG_T2_LSB_POS   2
#define BMP280_DIG_T2_MSB_POS   3
#define BMP280_DIG_T3_LSB_POS   4
#define BMP280_DIG_T3_MSB_POS   5
#define BMP280_DIG_P1_LSB_POS   6
#define BMP280_DIG_P1_MSB_POS   7
#define BMP280_DIG_P2_LSB_POS   8
#define BMP280_DIG_P2_MSB_POS   9
#define BMP280_DIG_P3_LSB_POS   10
#define BMP280_DIG_P3_MSB_POS   11
#define BMP280_DIG_P4_LSB_POS   12
#define BMP280_DIG_P4_MSB_POS   13
#define BMP280_DIG_P5_LSB_POS   14
#define BMP280_DIG_P5_MSB_POS   15
#define BMP280_DIG_P6_LSB_POS   16
#define BMP280_DIG_P6_MSB_POS   17
#define BMP280_DIG_P7_LSB_POS   18
#define BMP280_DIG_P7_MSB_POS   19
#define BMP280_DIG_P8_LSB_POS   20
#define BMP280_DIG_P8_MSB_POS   21
#define BMP280_DIG_P9_LSB_POS   22
#define BMP280_DIG_P9_MSB_POS   23

typedef struct
{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;

	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
} bmp280_calib_data;


/**
 * @brief bmp280对象结构体定义
 */
typedef struct bmp280
{

	/**
	 * @brief 用户实现I2C写字节的接口
	 * @param[in]	dev_addr	器件的I2C地址
	 * @param[in]	reg_addr	寄存器地址
	 * @param[in]	buf			要写入的数据
	 * @param[in]	len			要写入的数据长度
	 * @return
	 */
	int (*i2c_mem_write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);

	/**
	 * @brief 用户实现I2C读字节的接口
	 * @param[in]	dev_addr	器件的I2C地址
	 * @param[in]	reg_addr	寄存器地址
	 * @param[out]	buf			要读取的数据
	 * @param[in]	len			要读取的数据长度
	 * @return
	 */
	int (*i2c_mem_read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);

} bmp280_t;


typedef int (*i2c_mem_write_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);
typedef int (*i2c_mem_read_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);

/* 合并高字节和低字节为uint16_t类型 */
#define BMP280_MSBLSB_TO_U16(msb, lsb)    (((uint16_t)msb << 8) | ((uint16_t)lsb))


/**
 * @brief 提供给用户的API
 */

int bmp280_create(bmp280_t *bmp280, i2c_mem_write_t i2c_write, i2c_mem_read_t i2c_read);

int bmp280_init(bmp280_t *bmp280);

int bmp280_get_chipid(bmp280_t *bmp280);

int bmp280_softreset(bmp280_t *bmp280);

float bmp280_get_temperature(bmp280_t *bmp280);

float bmp280_get_pressure(bmp280_t *bmp280);


#ifdef __cplusplus
}
#endif

#endif //F103_TEST_BMP280_H
