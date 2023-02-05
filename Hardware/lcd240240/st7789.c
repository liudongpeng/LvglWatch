//
// Created by liudongpeng on 2022/11/17.
//

#include "st7789.h"


#define ST7789_MSBLSBTO_UINT16(msb, lsb)    (((uint16_t)msb << 8) | ((uint16_t)lsb))


/**
 * @brief 设置引脚电平
 */
#define LCD_RES_SET()   st->set_res_level ? st->set_res_level(1) : NULL
#define LCD_RES_RESET() st->set_res_level ? st->set_res_level(0) : NULL

#define LCD_DC_SET()    st->set_dc_level ? st->set_dc_level(1) : NULL
#define LCD_DC_RESET()  st->set_dc_level ? st->set_dc_level(0) : NULL

#define LCD_CS_SET()    /*st->set_cs_level ? st->set_cs_level(1) : NULL*/0
#define LCD_CS_RESET()  /*st->set_cs_level ? st->set_cs_level(0) : NULL*/0


/**
 * @brief 粗糙的毫秒级延时
 * @param nms
 */
static void deley_ms(int nms)
{
	for (int i = 0; i < nms; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			for (int k = 0; k < 10000; k++);
		}
	}
}


/**
 * @brief 创建ST7789对象
 * @param[out]	st
 * @param[in]	spi_write
 * @return
 */
int st7789_create(st7789_t *st, spi_write_t spi_write, set_res_level_t set_res, set_dc_level_t set_dc,
                  set_cs_level_t set_cs)
{
	if (st == NULL)
		return -1;

	st->spi_write = spi_write;
	st->set_res_level = set_res;
	st->set_dc_level = set_dc;
	st->set_cs_level = set_cs;

	return st7789_init(st);
}

/**
 * @brief 初始化ST7789
 * @param[out]	st
 * @return
 */
int st7789_init(st7789_t *st)
{
	uint8_t cmd;
	uint8_t cmd_val[14];

	LCD_RES_RESET();
	deley_ms(1000);
	LCD_RES_SET();
	deley_ms(1000);

	if (st == NULL)
		return -1;

	/* 101 = 16bit/pixel */
	cmd = ST7789_CMD_COLMOD;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x05;
	st7789_write_data(st, cmd_val, 1);

	/* 设置VCOM Offset */
	cmd = ST7789_CMD_VCMOFSET;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x1A;
	st7789_write_data(st, cmd_val, 1);

	/* 设置页从上到下, 列从左到右, 从上到下刷新屏幕, RGB模式, 从左到右刷新屏幕 */
	cmd = ST7789_CMD_MDACTL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x00;
	st7789_write_data(st, cmd_val, 1);

	/* 门廊控制(???啥意思???) */
	cmd = ST7789_CMD_PORCTL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x05;
	cmd_val[1] = 0x05;
	cmd_val[2] = 0x00;
	cmd_val[3] = 0x33;
	cmd_val[4] = 0x33;
	st7789_write_data(st, cmd_val, 5);

	/* 控制闸门(VGL是TFT屏的负电源输入脚, VGH是正电源输入脚) */
	cmd = ST7789_CMD_GCTRL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x05;
	st7789_write_data(st, cmd_val, 1);  /* 12.2V   -10.43V */

	/* VCOM设置 */
	cmd = ST7789_CMD_VCOMS;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x3F;
	st7789_write_data(st, cmd_val, 1);

	/* LCM控制 */
	cmd = ST7789_CMD_LCMCTRL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x2C;
	st7789_write_data(st, cmd_val, 1);

	/* VDV和VRH命令使能 */
	cmd = ST7789_CMD_VDVVRHEN;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x01;
	st7789_write_data(st, cmd_val, 1);

	/* 设置VRH */
	cmd = ST7789_CMD_VRHS;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x0F;
	st7789_write_data(st, cmd_val, 1);

	/* 设置VDV */
	cmd = ST7789_CMD_VDVS;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x20;
	st7789_write_data(st, cmd_val, 1);

	/* 帧速率控制2 */
	cmd = ST7789_CMD_FRCTRL2;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x0F;
	st7789_write_data(st, cmd_val, 1);  /* 60Hz */

	/* 电源控制1 */
	cmd = ST7789_CMD_POWCTRL1;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0xA4;
	cmd_val[1] = 0xA1;
	st7789_write_data(st, cmd_val, 2);

	/* 电源控制2 */
	cmd = ST7789_CMD_POWCTRL2;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x03;
	st7789_write_data(st, cmd_val, 1);

	/* 补偿时间控制 */
	cmd = ST7789_CMD_EQCTRL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0x09;
	cmd_val[1] = 0x09;
	cmd_val[2] = 0x08;
	st7789_write_data(st, cmd_val, 3);

	/* 负电压闸门控制 */
	cmd = ST7789_CMD_PVGAMCTRL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0xD0;
	cmd_val[1] = 0x05;
	cmd_val[2] = 0x09;
	cmd_val[3] = 0x09;
	cmd_val[4] = 0x08;
	cmd_val[5] = 0x14;
	cmd_val[6] = 0x28;
	cmd_val[7] = 0x33;
	cmd_val[8] = 0x3F;
	cmd_val[9] = 0x07;
	cmd_val[10] = 0x13;
	cmd_val[11] = 0x14;
	cmd_val[12] = 0x28;
	cmd_val[13] = 0x30;
	st7789_write_data(st, cmd_val, 14);

	/* 正电压闸门控制 */
	cmd = ST7789_CMD_NVGAMCTRL;
	st7789_write_cmd(st, &cmd, 1);
	cmd_val[0] = 0xD0;
	cmd_val[1] = 0x05;
	cmd_val[2] = 0x09;
	cmd_val[3] = 0x09;
	cmd_val[4] = 0x08;
	cmd_val[5] = 0x03;
	cmd_val[6] = 0x24;
	cmd_val[7] = 0x32;
	cmd_val[8] = 0x32;
	cmd_val[9] = 0x3B;
	cmd_val[10] = 0x14;
	cmd_val[11] = 0x13;
	cmd_val[12] = 0x28;
	cmd_val[13] = 0x2F;
	st7789_write_data(st, cmd_val, 14);

	/* 开启显示反转 */
	cmd = ST7789_CMD_INVON;
	st7789_write_cmd(st, &cmd, 1);

	/* 退出睡眠模式 */
	cmd = ST7789_CMD_SLPOUT;
	st7789_write_cmd(st, &cmd, 1);
	deley_ms(1000);

	/* lcd开启显示 */
	cmd = ST7789_CMD_DISPON;
	st7789_write_cmd(st, &cmd, 1);
	deley_ms(1000);

	return 0;
}

/**
 * @brief ST7789软复位
 * @param[in]	st
 * @param[in]	spi_write
 * @return
 */
int st7789_soft_reset(st7789_t *st)
{
	uint8_t reset = ST7789_CMD_SWRESET;

	if (st == NULL)
		return -1;

	LCD_CS_RESET();
	LCD_DC_RESET();
	return st->spi_write(&reset, 1);
}

/**
 * @brief 给ST7789写命令
 * @param[in]	st
 * @param[in]	cmd_list
 * @param[in]	len
 * @return
 */
int st7789_write_cmd(st7789_t *st, uint8_t *cmd_list, uint16_t len)
{
	if (st == NULL || st->spi_write == NULL/* || cmd_list == NULL || len <= 0*/)
		return -1;

	LCD_CS_RESET();
	LCD_DC_RESET();
	st->spi_write(cmd_list, len);
	LCD_DC_SET();
	LCD_CS_SET();
	return 0;
}

/**
 * @brief 给ST7789写数据
 * @param[in]	st
 * @param[in]	data_list
 * @param[in]	len
 * @return
 */
int st7789_write_data(st7789_t *st, uint8_t *data_list, uint16_t len)
{
	if (st == NULL || st->spi_write == NULL/* || data_list == NULL || len <= 0*/)
		return -1;

	LCD_CS_RESET();
	LCD_DC_SET();
	st->spi_write(data_list, len);
	LCD_DC_RESET();
	LCD_CS_SET();
	return 0;
}
