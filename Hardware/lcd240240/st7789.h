//
// Created by liudongpeng on 2022/11/17.
//

#ifndef ST7789_ST7789_H
#define ST7789_ST7789_H


#include <stdint.h>
#include <stddef.h>



/**
 * @brief ST7789�Ĵ���
 */
#define ST7789_CMD_NOP          0x00    /* �������� */
#define ST7789_CMD_SWRESET      0x01    /* �����λ */
#define ST7789_CMD_SLPIN        0x10    /* ����˯��ģʽ */
#define ST7789_CMD_SLPOUT       0x11    /* �˳�˯��ģʽ */
#define ST7789_CMD_PTLON        0x12    /* ����������ʾ */
#define ST7789_CMD_NORON        0x13    /* ����������ʾ */
#define ST7789_CMD_INVOFF       0x20    /* �ر���ʾ��ת */
#define ST7789_CMD_INVON        0x21    /* ������ʾ��ת */
#define ST7789_CMD_GAMSET       0x26
#define ST7789_CMD_DISPOFF      0x28    /* �ر���ʾ */
#define ST7789_CMD_DISPON       0x29    /* ������ʾ */
#define ST7789_CMD_CASET        0x2A    /* �����е�ַ(0~239)(��������2(4)������, ��д����, ��д����) */
#define ST7789_CMD_RASET        0x2B    /* �����е�ַ(0~240)(��������2(4)������, ��д����, ��д����) */
#define ST7789_CMD_RAMWR        0x2C    /* д�ڴ�(��������N������, ��д����, ��д����) */
#define ST7789_CMD_PTLAR        0x30    /* ���ò���������ʾģʽ��Ҫ��ʵ������(��������2(4)������, ��д����, ��д����) */
#define ST7789_CMD_TEOFF        0x34    /* �ر�˺��Ч���� */
#define ST7789_CMD_TEON         0x35    /* ����˺��Ч����(��������1������, ��д����, ��д����) */
#define ST7789_CMD_MDACTL       0x36    /* �ڴ����ݷ��ʿ���(��������1������, ��д����, ��д����) */
#define ST7789_CMD_IMOFF        0x38    /* �ر�idleģʽ */
#define ST7789_CMD_IMON         0x39    /* ����idleģʽ(��ɫ������, ��ɫ�͸�ɫʹ��֡������ÿ��R,G��B��MSB, ��ʾ8����ɫ�������) */
#define ST7789_CMD_COLMOD       0x3A    /* ����RGB��ɫģʽ(��������1������, ��д����, ��д����) */
#define ST7789_CMD_STE          0x44    /* ����˺��ɨ����(��������1(2)������, ��д����, ��д����) */
#define ST7789_CMD_WRDISBV      0x51    /* д��ʾ����ֵ(��������1������, ��д����, ��д����) */
#define ST7789_CMD_WRCTRLD      0x53    /* ������ʾ����(��������1������, ��д����, ��д����) */
#define ST7789_CMD_PORCTL       0xB2    /* ��������???(��������5������, ��д����, ��д����) */
#define ST7789_CMD_GCTRL        0xB7    /* ����բ��(��������1������, ��д����, ��д����) */
#define ST7789_CMD_VCOMS        0xBB    /* ����VCOM(��������1������, ��д����, ��д����) */
#define ST7789_CMD_LCMCTRL      0xC0    /* LCM����(��������1������, ��д����, ��д����) */
#define ST7789_CMD_VDVVRHEN     0xC2    /* VDV��VRH����ʹ��(��������2������, ��д����, ��д����) */
#define ST7789_CMD_VRHS         0xC3    /* ����VRH(��������1������, ��д����, ��д����) */
#define ST7789_CMD_VDVS         0xC4    /* ����VDV(��������1������, ��д����, ��д����) */
#define ST7789_CMD_VCMOFSET     0xC5    /* ����VCOM Offset(��������1������, ��д����, ��д����) */
#define ST7789_CMD_FRCTRL2      0xC6    /* ֡���ʿ���2(��������1������, ��д����, ��д����) */
#define ST7789_CMD_POWCTRL1     0xD0    /* ��Դ����1(��������2������, ��д����, ��д����) */
#define ST7789_CMD_PVGAMCTRL    0xE0    /* ����ѹբ�ſ���(��������14������, ��д����, ��д����) */
#define ST7789_CMD_NVGAMCTRL    0xE1    /* ����ѹբ�ſ���(��������14������, ��д����, ��д����) */
#define ST7789_CMD_POWCTRL2     0xE8    /* ��Դ����2(��������1������, ��д����, ��д����) */
#define ST7789_CMD_EQCTRL       0xE9    /* ����ʱ�����(��������3������, ��д����, ��д����) */


/**
 * @brief ST7789��Ļ����IC
 */
typedef struct st7789
{

	/**
	 * @brief �û�ʵ��spiд���ݵĽӿ�
	 * @param[in]	data
	 * @param[in]	len
	 * @return
	 */
	int (*spi_write)(uint8_t *data, uint16_t len);

	/* ����RES���ŵ�ƽ */
	void (*set_res_level)(uint8_t level);

	/* ����DC���ŵ�ƽ */
	void (*set_dc_level)(uint8_t level);

	/* ����CS���ŵ�ƽ */
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
