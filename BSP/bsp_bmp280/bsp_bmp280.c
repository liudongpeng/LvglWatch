//
// Created by summer on 2023/2/12.
//

#include "bsp_bmp280.h"


bmp280_t g_bmp280;  /* bmp280���� */


/**
 * @brief �弶bmp280��ʼ��
 * @return
 */
int bsp_bmp280_init()
{
	return bmp280_create(&g_bmp280, bmp280_i2c_mem_write, bmp280_i2c_mem_read);
}
