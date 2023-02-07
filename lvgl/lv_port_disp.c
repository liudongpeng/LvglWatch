/**
 * @file lv_port_disp_templ.c
 *
 */


#include "lv_port_disp.h"
#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "bsp.h"
#include "bsp_config.h"


#define LV_HOR_RES_MAX  135
#define LV_VER_RES_MAX  240
#define DISP_BUF_SIZE   (LV_HOR_RES_MAX * 60)


static uint8_t buf[DISP_BUF_SIZE * 2];

static lv_color_t lv_disp_buf1[DISP_BUF_SIZE];
static lv_color_t lv_disp_buf2[DISP_BUF_SIZE];
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;  /*Descriptor of a display driver*/



static void disp_spi_dma_send(void *buf, uint32_t size);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);


/**
 * @brief spi dma 发送数据
 * @param[in]	buf
 * @param[in]	size
 */
static void disp_spi_dma_send(void *buf, uint32_t size)
{
	HAL_SPI_Transmit_DMA(&SCREEN_SPI_HANDLER, (uint8_t *)buf, size);
}


void lv_port_disp_init(void)
{
	/*Initialize the display buffer*/
	lv_disp_draw_buf_init(&draw_buf, lv_disp_buf1, lv_disp_buf2, DISP_BUF_SIZE);

	/* 初始化lvgl驱动 */
	lv_disp_drv_init(&disp_drv);    /*Basic initialization*/

	/* 设置屏幕大小 */
	disp_drv.hor_res = (lv_coord_t) g_lcd.width;
	disp_drv.ver_res = (lv_coord_t) g_lcd.height;

	/*Used to copy the buffer's content to the display*/
	disp_drv.flush_cb = disp_flush;

	/*Set a display buffer*/
	disp_drv.draw_buf = &draw_buf;

	/*Finally register the driver*/
	lv_disp_drv_register(&disp_drv);
}


/**
 * @brief 大小端互转
 * @param[in]	a
 */
void endian_exchange_u16(uint16_t *a)
{
	uint8_t lsb = (*a >> 8);
	*a <<= 8;
	*a |= lsb;
}


/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
//	int w = area->x2 - area->x1 + 1;
//	int h = area->y2 - area->y1 + 1;
//	uint32_t data_size = w * h * sizeof(lv_color_t);

//	uint16_t *data = pvPortMalloc(data_size);
//	if (data == NULL)
//	{
//		printf("空间开辟失败, 刷屏失败\n");
//		return;
//	}

//	memcpy(buf, color_p, data_size);
//
//	uint32_t count = data_size / 2;
//	for (uint32_t i = 0; i < count; i++)
//		endian_exchange_u16(buf + i);
//
//	/* 设置显示区域 */
//	lcd_set_window_area(&g_lcd, area->x1, area->y1, w, h);
//	/* DMA发送请求 */
//	g_lcd.drv->set_dc_level(1);
//	disp_spi_dma_send(buf, data_size);
////	vPortFree(data);
//	lv_disp_flush_ready(disp_drv);


//	int32_t x, y;
//	for(y = area->y1; y <= area->y2; y++) {
//		for(x = area->x1; x <= area->x2; x++) {
//			lcd_draw_point(&g_lcd, x, y, (lcd_color_t)color_p->full);
//			color_p++;
//		}
//	}

	int w = area->x2 - area->x1 + 1;
	int h = area->y2 - area->y1 + 1;
	uint32_t data_size = w * h * sizeof(lv_color_t);


	memcpy(buf, color_p, data_size);

	uint32_t count = data_size / 2;
	for (uint32_t i = 0; i < count; i++)
		endian_exchange_u16((uint16_t*)buf + i);

	lcd_draw_area(&g_lcd, area->x1, area->y1, w, h, (uint8_t*)buf);
	lv_disp_flush_ready(disp_drv);


	/*IMPORTANT!!!
	 *Inform the graphics library that you are ready with the flushing*/

}


///**
//  * @brief  Tx Transfer completed callback.
//  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
//  *               the configuration information for SPI module.
//  * @retval None
//  */
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//	if (hspi->Instance == SCREEN_SPI)
//	{
//		/* tell lvgl that flushing is done */
//		lv_disp_flush_ready(&disp_drv);
//		g_lcd.drv->set_cs_level(1);
//
//	}
//}

/**
 * @brief DMA传输完成回调
 * @param _hdma
 */
void lvgl_flush_cb(DMA_HandleTypeDef *_hdma)
{
	lv_disp_flush_ready(&disp_drv);
}
