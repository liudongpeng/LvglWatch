/**
 * @file lv_port_disp_templ.c
 *
 */


#include "lv_port_disp.h"
#include <stdio.h>
#include <stdbool.h>

#include "bsp.h"



static lv_disp_drv_t disp_drv;  /*Descriptor of a display driver*/




static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);


void lv_port_disp_init(void)
{
	/* 创建缓存 */
	static lv_disp_draw_buf_t draw_buf_dsc_1;
	static lv_color_t buf_1[MY_DISP_HOR_RES * 10];  /*A buffer for 10 rows*/
	lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);  /*Initialize the display buffer*/

	/* 初始化lvgl驱动 */
	lv_disp_drv_init(&disp_drv);    /*Basic initialization*/

	/* 设置屏幕大小 */
	disp_drv.hor_res = (lv_coord_t)g_lcd.width;
	disp_drv.ver_res = (lv_coord_t)g_lcd.height;

	/*Used to copy the buffer's content to the display*/
	disp_drv.flush_cb = disp_flush;

	/*Set a display buffer*/
	disp_drv.draw_buf = &draw_buf_dsc_1;

	/*Finally register the driver*/
	lv_disp_drv_register(&disp_drv);
}


volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
	disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
	disp_flush_enabled = false;
}


void change_lvcolor(uint16_t* data)
{
	uint16_t lsb5 = *data & 0x001F;
	uint16_t msb5 = *data & 0xF800;

	*data &= (msb5 >> 11);
	*data &= (lsb5 << 11);
}


/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	int w = area->x2 - area->x1 + 1;
	int h = area->y2 - area->y1 + 1;
	uint16_t size = w * h * sizeof(lv_color_t);

	if (disp_flush_enabled)
	{
		/*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
		int32_t x;
		int32_t y;
        for(y = area->y1; y <= area->y2; y++) {
            for(x = area->x1; x <= area->x2; x++) {
                /*Put a pixel to the display. For example:*/
                /*put_px(x, y, *color_p)*/

				lcd_draw_point(&g_lcd, x, y, (lcd_color_t)color_p->full);
                color_p++;
            }
        }


//		uint16_t* p = (uint16_t*)color_p;
//		for (int i = 0; i < w * h; i++)
//			change_lvcolor(&p[i]);



//		lcd_set_window_area(area->x1, area->y1, w, h);
//		LcdWriteCmd(ST7789_CMD_RAMWR);
//		LCD_DC_SET();
//	    HAL_SPI_Transmit(&hspi2, (uint8_t*)(color_p), size, 1000);
//		HAL_SPI_Transmit_DMA(&hspi2, (uint8_t *) color_p, size);
	}

	/*IMPORTANT!!!
	 *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}


/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI2)
	{
//		lv_disp_flush_ready(&disp_drv);
		printf("cb\n");
	}
}
