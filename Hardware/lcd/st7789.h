//
// Created by liudongpeng on 2022/11/17.
//

#ifndef ST7789_ST7789_H
#define ST7789_ST7789_H


#include <stdint.h>
#include <stddef.h>



/**
 * @brief ST7789寄存器
 */
#define ST7789_CMD_NOP          0x00    /* 不做操作 */
#define ST7789_CMD_SWRESET      0x01    /* 软件复位 */
#define ST7789_CMD_SLPIN        0x10    /* 进入睡眠模式 */
#define ST7789_CMD_SLPOUT       0x11    /* 退出睡眠模式 */
#define ST7789_CMD_PTLON        0x12    /* 开启部分显示 */
#define ST7789_CMD_NORON        0x13    /* 开启正常显示 */
#define ST7789_CMD_INVOFF       0x20    /* 关闭显示反转 */
#define ST7789_CMD_INVON        0x21    /* 开启显示反转 */
#define ST7789_CMD_GAMSET       0x26
#define ST7789_CMD_DISPOFF      0x28    /* 关闭显示 */
#define ST7789_CMD_DISPON       0x29    /* 开启显示 */
#define ST7789_CMD_CASET        0x2A    /* 设置列地址(0~239)(此命令有2(4)个参数, 先写命令, 再写数据) */
#define ST7789_CMD_RASET        0x2B    /* 设置行地址(0~240)(此命令有2(4)个参数, 先写命令, 再写数据) */
#define ST7789_CMD_RAMWR        0x2C    /* 写内存(此命令有N个参数, 先写命令, 再写数据) */
#define ST7789_CMD_PTLAR        0x30    /* 设置部分区域显示模式下要现实的区域(此命令有2(4)个参数, 先写命令, 再写数据) */
#define ST7789_CMD_TEOFF        0x34    /* 关闭撕裂效果线 */
#define ST7789_CMD_TEON         0x35    /* 开启撕裂效果线(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_MDACTL       0x36    /* 内存数据访问控制(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_IMOFF        0x38    /* 关闭idle模式 */
#define ST7789_CMD_IMON         0x39    /* 开启idle模式(颜色表达减少, 主色和副色使用帧记忆中每个R,G和B的MSB, 显示8个颜色深度数据) */
#define ST7789_CMD_COLMOD       0x3A    /* 设置RGB颜色模式(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_STE          0x44    /* 设置撕裂扫描线(此命令有1(2)个参数, 先写命令, 再写数据) */
#define ST7789_CMD_WRDISBV      0x51    /* 写显示亮度值(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_WRCTRLD      0x53    /* 控制显示亮度(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_PORCTL       0xB2    /* 控制门廊???(此命令有5个参数, 先写命令, 再写数据) */
#define ST7789_CMD_GCTRL        0xB7    /* 控制闸门(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_VCOMS        0xBB    /* 设置VCOM(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_LCMCTRL      0xC0    /* LCM控制(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_VDVVRHEN     0xC2    /* VDV和VRH命令使能(此命令有2个参数, 先写命令, 再写数据) */
#define ST7789_CMD_VRHS         0xC3    /* 设置VRH(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_VDVS         0xC4    /* 设置VDV(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_VCMOFSET     0xC5    /* 设置VCOM Offset(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_FRCTRL2      0xC6    /* 帧速率控制2(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_POWCTRL1     0xD0    /* 电源控制1(此命令有2个参数, 先写命令, 再写数据) */
#define ST7789_CMD_PVGAMCTRL    0xE0    /* 负电压闸门控制(此命令有14个参数, 先写命令, 再写数据) */
#define ST7789_CMD_NVGAMCTRL    0xE1    /* 正电压闸门控制(此命令有14个参数, 先写命令, 再写数据) */
#define ST7789_CMD_POWCTRL2     0xE8    /* 电源控制2(此命令有1个参数, 先写命令, 再写数据) */
#define ST7789_CMD_EQCTRL       0xE9    /* 补偿时间控制(此命令有3个参数, 先写命令, 再写数据) */


/**
 * @brief ST7789屏幕驱动IC
 */
typedef struct st7789
{

	/**
	 * @brief 用户实现spi写数据的接口
	 * @param[in]	data
	 * @param[in]	len
	 * @return
	 */
	int (*spi_write)(uint8_t *data, uint16_t len);

	/* 设置RES引脚电平 */
	void (*set_res_level)(uint8_t level);

	/* 设置DC引脚电平 */
	void (*set_dc_level)(uint8_t level);

	/* 设置CS引脚电平 */
	void (*set_cs_level)(uint8_t level);

} st7789_t;


typedef void (*set_res_level_t)(uint8_t level);
typedef void (*set_dc_level_t)(uint8_t level);
typedef void (*set_cs_level_t)(uint8_t level);
typedef int (*spi_write_t)(uint8_t *data, uint16_t len);


int st7789_create(st7789_t *st, spi_write_t spi_write, set_res_level_t set_res, set_dc_level_t set_dc,
                  set_cs_level_t set_cs);

int st7789_init(st7789_t *st);

int st7789_soft_reset(st7789_t *st);

int st7789_write_cmd(st7789_t *st, uint8_t *cmd_list, uint16_t len);

int st7789_write_data(st7789_t *st, uint8_t *data_list, uint16_t len);


#endif //ST7789_ST7789_H
