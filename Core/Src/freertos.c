/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "bsp.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "app_display.h"
#include "page_manager.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


page_manager_t g_page_manager;    /* 界面管理器 */


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
		.name = "defaultTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityLow,
};
/* Definitions for display_flush_task */
osThreadId_t display_flush_taskHandle;
const osThreadAttr_t display_flush_task_attributes = {
		.name = "display_flush_task",
		.stack_size = 500 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for usart1_mutex */
osMutexId_t usart1_mutexHandle;
const osMutexAttr_t usart1_mutex_attributes = {
		.name = "usart1_mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void test()
{
	lv_obj_t *btn = lv_btn_create(lv_scr_act());
	lv_obj_set_size(btn, 60, 30);
	lv_obj_center(btn);
}

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void start_display_flush_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
	/* USER CODE BEGIN Init */
	int ret = 0;

	printf("enter MX_FREERTOS_Init()\n");

//	bsp_rtc_init();
	bsp_button_init();
	bsp_led_init();
	bsp_display_init();
	lcd_clear(&g_lcd);

	/* lvgl及显示驱动初始化 */
	lv_init();
	lv_port_disp_init();
	HAL_TIM_Base_Start_IT(&LVGL_TICK_TIM_HANDLE);

	/* 界面管理器初始化 */
	page_manager_create(&g_page_manager, 10, 10);

	/* 软件界面显示初始化 */
	app_display_init();


//	HAL_Delay(2000);
//
//
//	/* RGB565 */
//	uint8_t buf[(240 * 20) * 2] = {0};
//	memset(buf, 0xFF, (240 * 20) * 2);
//	for (int i = 0; i < (240 * 20) * 2; i++)
//	{
//		buf[i] = 0xF8;
//		buf[++i] = 0x00;
//	}
//	lcd_draw_area(&g_lcd, 0, 0, 240, 20, buf);
//
//	HAL_Delay(2000);
//
//	for (int i = 0; i < (240 * 20) * 2; i++)
//	{
//		buf[i] = 0x0F;
//		buf[++i] = 0xE0;
//	}
//	lcd_draw_area(&g_lcd, 0, 60, 240, 20, buf);
//
//
//	HAL_Delay(2000);
//
//	for (int i = 200; i < 220; i++)
//	{
//		for (int j = 0; j < 240; j++)
//		{
//			lcd_draw_point(&g_lcd, j, i, 0x001F);
//		}
//	}


	/* FIXME: hello world */


	/* USER CODE END Init */
	/* Create the mutex(es) */
	/* creation of usart1_mutex */
	usart1_mutexHandle = osMutexNew(&usart1_mutex_attributes);

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* creation of display_flush_task */
	display_flush_taskHandle = osThreadNew(start_display_flush_task, NULL, &display_flush_task_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;)
	{
		led_open(&g_user_led);
		HAL_Delay(500);
		led_close(&g_user_led);
		HAL_Delay(500);

		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_start_display_flush_task */
/**
* @brief 显示器刷新任务
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_display_flush_task */
void start_display_flush_task(void *argument)
{
	/* USER CODE BEGIN start_display_flush_task */
	/* Infinite loop */
	for (;;)
	{
		app_display_update();

		osDelay(1);
	}
	/* USER CODE END start_display_flush_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief 定时器中断回调
 * @param[in]	htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* 按键扫描中断 */
	if (htim->Instance == BTN_SCAN_TIM)
	{
		button_ticks();
	}

	/* lvgl tick */
	if (htim->Instance == LVGL_TICK_TIM)
	{
		lv_tick_inc(1);
	}
}

/* USER CODE END Application */

