/**
  * @file    bmp280.c
  * @author  LiuDongPeng
  * @version V0.1
  * @date    2022-12-25
  * @brief   BMP280��ѹ������ģ��Դ�ļ�
  *
  * @attention
  * ʹ�ô�bmp280ģ��, ��Ҫ�û�ʵ��ͨ��I2C���߶�д�����Ĵ����Ľӿ�, �ӿں�����ʽΪ:
  * int i2c_mem_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);
  * int i2c_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);
  *
  * @changelog
  * 2023-1-5	�޸ı���, �����ȵ��������ΪСд��ĸ���»������
  */

#include "bmp280.h"

#include <stdio.h>

/* ���� */
int32_t t_fine;
/* ����У׼���� */
static bmp280_calib_data gBmp280Calib;


/**
 * @brief ��ȡУ׼����
 * @param[in] bmp280
 * @return
 */
static int bmp280_read_calib(bmp280_t *bmp280)
{
	uint8_t reg[24] = {0};
	uint8_t ret;


	if (bmp280 == NULL || bmp280->i2c_mem_read == NULL)
		return -1;

	ret = bmp280->i2c_mem_read(BMP280_I2C_ADDRESS, BMP280_REGISTER_DIG_T1, reg, 24);
	if (ret != 0)
		return ret;

	gBmp280Calib.dig_T1 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_T1_MSB_POS], reg[BMP280_DIG_T1_LSB_POS]);
	gBmp280Calib.dig_T2 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_T2_MSB_POS], reg[BMP280_DIG_T2_LSB_POS]);
	gBmp280Calib.dig_T3 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_T3_MSB_POS], reg[BMP280_DIG_T3_LSB_POS]);

	gBmp280Calib.dig_P1 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P1_MSB_POS], reg[BMP280_DIG_P1_LSB_POS]);
	gBmp280Calib.dig_P2 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P2_MSB_POS], reg[BMP280_DIG_P2_LSB_POS]);
	gBmp280Calib.dig_P3 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P3_MSB_POS], reg[BMP280_DIG_P3_LSB_POS]);
	gBmp280Calib.dig_P4 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P4_MSB_POS], reg[BMP280_DIG_P4_LSB_POS]);
	gBmp280Calib.dig_P5 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P5_MSB_POS], reg[BMP280_DIG_P5_LSB_POS]);
	gBmp280Calib.dig_P6 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P6_MSB_POS], reg[BMP280_DIG_P6_LSB_POS]);
	gBmp280Calib.dig_P7 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P7_MSB_POS], reg[BMP280_DIG_P7_LSB_POS]);
	gBmp280Calib.dig_P8 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P8_MSB_POS], reg[BMP280_DIG_P8_LSB_POS]);
	gBmp280Calib.dig_P9 = BMP280_MSBLSB_TO_U16(reg[BMP280_DIG_P9_MSB_POS], reg[BMP280_DIG_P9_LSB_POS]);

	return 0;
}


/**
 * @brief ����bmp280����ṹ���Ա
 * @param[out]	bmp280
 * @param[in]	i2c_write
 * @param[in]	i2c_read
 * @return
 */
int bmp280_create(bmp280_t *bmp280, i2c_mem_write_t i2c_write, i2c_mem_read_t i2c_read)
{
	if (bmp280 == NULL || i2c_write == NULL || i2c_read == NULL)
		return -1;

	/* ����bmp280������I2C��д�ӿ� */
	bmp280->i2c_mem_write = i2c_write;
	bmp280->i2c_mem_read = i2c_read;

	return bmp280_init(bmp280);
}

/**
 * @brief bmp280������ʼ��
 * @param[in] bmp280
 * @return
 */
int bmp280_init(bmp280_t *bmp280)
{
	/* 001 : 16 bit / 0.0050 ��C
	 * 111 : 20 bit / 0.16 Pa
	 *  11 : Normal mode
	 */
	uint8_t reg = 0xE7;

	if (bmp280 == NULL)
		return -1;

	/* ��ȡоƬID */
	if (bmp280_get_chipid(bmp280) != BMP280_CHIPID)
		return -2;

	/* ����ctrl_meas�Ĵ��� */
	if (bmp280->i2c_mem_write(BMP280_I2C_ADDRESS, BMP280_REGISTER_CONTROL, &reg, 1) != 0)
		return -3;

	/* ��ȡУ׼���� */
	if (bmp280_read_calib(bmp280) != 0)
		return -4;

	return 0;
}

/**
 * @brief ������ID
 * @param[in]	bmp280
 * @return
 */
int bmp280_get_chipid(bmp280_t *bmp280)
{
	uint8_t ret = 0;
	uint8_t chipid = 0;

	if (bmp280 == NULL || bmp280->i2c_mem_read == NULL)
		return -1;

	printf("------------------------test------------------------\n");

	if (bmp280->i2c_mem_read(BMP280_I2C_ADDRESS, BMP280_REGISTER_CHIPID, &chipid, 1) != 0)
		return -2;

	return chipid;
}

/**
 * @brief bmp280��λ
 * @param[in]	bmp280
 * @return
 */
int bmp280_softreset(bmp280_t *bmp280)
{
	uint8_t ret = 0;
	uint8_t softreset = BMP280_SOFTRESET;

	if (bmp280 == NULL || bmp280->i2c_mem_write == NULL)
		return -1;

	ret = bmp280->i2c_mem_write(BMP280_I2C_ADDRESS, BMP280_REGISTER_SOFTRESET, &softreset, 1);
	if (ret != 0)
		return ret;

	return 0;
}

/**
 * @brief ���¶�ֵ
 * @param[in]	bmp280
 * @return
 */
float bmp280_get_temperature(bmp280_t *bmp280)
{
	int32_t var1, var2, adc_T;
	uint8_t reg[3];
	uint8_t ret;

	if (bmp280 == NULL || bmp280->i2c_mem_read == NULL)
		return -1;

	ret = bmp280->i2c_mem_read(BMP280_I2C_ADDRESS, BMP280_REGISTER_TEMPDATA, reg, 3);
	if (ret != 0)
		return ret;

	adc_T = BMP280_MSBLSB_TO_U16(reg[0], reg[1]);
	adc_T <<= 8;
	adc_T |= reg[2];
	adc_T >>= 4;

	var1 = ((((adc_T >> 3) - ((int32_t) gBmp280Calib.dig_T1 << 1))) *
	        ((int32_t) gBmp280Calib.dig_T2)) >> 11;

	var2 = (((((adc_T >> 4) - ((int32_t) gBmp280Calib.dig_T1)) *
	          ((adc_T >> 4) - ((int32_t) gBmp280Calib.dig_T1))) >> 12) *
	        ((int32_t) gBmp280Calib.dig_T3)) >> 14;

	t_fine = var1 + var2;

	return (float) ((t_fine * 5 + 128) >> 8) / 100;
}

/**
 * @brief ����ѹֵ
 * @param[in]	bmp280
 * @return
 */
float bmp280_get_pressure(bmp280_t *bmp280)
{
	int64_t var1, var2, p;
	int32_t adc_P;
	uint8_t reg[3];
	uint8_t ret;

	if (bmp280 == NULL || bmp280->i2c_mem_read == NULL)
		return -1;

	ret = bmp280->i2c_mem_read(BMP280_I2C_ADDRESS, BMP280_REGISTER_PRESSUREDATA, reg, 3);
	if (ret != 0)
		return ret;

	adc_P = BMP280_MSBLSB_TO_U16(reg[0], reg[1]);
	adc_P <<= 8;
	adc_P |= reg[2];
	adc_P >>= 4;

	var1 = ((int64_t) t_fine) - 128000;
	var2 = var1 * var1 * (int64_t) gBmp280Calib.dig_P6;
	var2 = var2 + ((var1 * (int64_t) gBmp280Calib.dig_P5) << 17);
	var2 = var2 + (((int64_t) gBmp280Calib.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t) gBmp280Calib.dig_P3) >> 8) +
	       ((var1 * (int64_t) gBmp280Calib.dig_P2) << 12);
	var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) gBmp280Calib.dig_P1) >> 33;

	if (var1 == 0)
	{
		return 0;  // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t) gBmp280Calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t) gBmp280Calib.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t) gBmp280Calib.dig_P7) << 4);
	return (float) p / 256;
}
