/**
 * @file lv_port_disp_templ.c
 *
 */


#include "lv_port_disp.h"
#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "bsp.h"



#define LV_HOR_RES_MAX  135
#define LV_VER_RES_MAX  240
#define DISP_BUF_SIZE   (LV_HOR_RES_MAX * 10)



static lv_color_t lv_disp_buf1[DISP_BUF_SIZE];
static lv_color_t lv_disp_buf2[DISP_BUF_SIZE];
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;  /*Descriptor of a display driver*/



static void disp_spi_dma_send(void* buf, uint32_t size);
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);



/**
 * @brief spi dma 发送数据
 * @param[in]	buf
 * @param[in]	size
 */
static void disp_spi_dma_send(void* buf, uint32_t size)
{
	HAL_SPI_Transmit_DMA(&SCREEN_SPI_HANDLER, (uint8_t*)buf, size);
}


void lv_port_disp_init(void)
{
	/*Initialize the display buffer*/
	lv_disp_draw_buf_init(&draw_buf, lv_disp_buf1, lv_disp_buf2, DISP_BUF_SIZE);

	/* 初始化lvgl驱动 */
	lv_disp_drv_init(&disp_drv);    /*Basic initialization*/

	/* 设置屏幕大小 */
	disp_drv.hor_res = (lv_coord_t)g_lcd.width;
	disp_drv.ver_res = (lv_coord_t)g_lcd.height;

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
void ending_exchange(uint16_t* a)
{
	printf("before: %#x, ", *a);
	uint8_t lsb = *a;
	*a <<= 8;
	*a &= lsb;
	printf("after: %#x\n", *a);
}


/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	int w = area->x2 - area->x1 + 1;
	int h = area->y2 - area->y1 + 1;
	uint32_t data_size = w * h * sizeof(lv_color_t);

//	uint8_t* data = pvPortMalloc(data_size);
//	if (data == NULL)
//	{
//		printf("空间开辟失败, 刷屏失败\n");
//		return;
//	}
//	memcpy(data, color_p, data_size);
//
//	printf("data_size = %lu, ", data_size);
//	int count = data_size >> 1;
//	for (int i = 0; i < count; i++)
//		ending_exchange((uint16_t*)(color_p + i));
//
//	/* 设置显示区域 */
//	lcd_set_window_area(&g_lcd, area->x1, area->y1, w, h);
//	/* DMA发送请求 */
//	disp_spi_dma_send(color_p, data_size);
//
//	vPortFree(data);


	int32_t x, y;
	for(y = area->y1; y <= area->y2; y++) {
		for(x = area->x1; x <= area->x2; x++) {
			lcd_draw_point(&g_lcd, x, y, (lcd_color_t)color_p->full);
			color_p++;
		}
	}
	lv_disp_flush_ready(disp_drv);

	/*IMPORTANT!!!
	 *Inform the graphics library that you are ready with the flushing*/

}


/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SCREEN_SPI)
	{
		if (hspi->hdmatx->Instance == DMA1_Stream4)
		{
			lv_disp_flush_ready(&disp_drv);
		}
	}
}
