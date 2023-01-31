//
// Created by QT on 2023/1/29.
//

#include "bsp_button.h"


extern page_manager_t g_page_manager;	/* 界面管理器, 在freertos.c文件中定义 */


button_t g_btn_left;  /* 左键 */
button_t g_btn_right; /* 右键 */
button_t g_btn_ok;    /* 选择键 */



static void button_event_handler(void* btn, int event);


/**
 * @brief 初始化板子上的按键
 * @return
 */
int bsp_button_init()
{
	/* 实例化按键对象 */
	button_create(&g_btn_left, 0, btn_left_get_level);
	button_create(&g_btn_right, 0, btn_right_get_level);
	button_create(&g_btn_ok, 0, btn_ok_get_level);

	/* 安装按键事件处理回调函数 */
	button_install_event_callback(&g_btn_left, ButtonEvent_SingleClick, button_event_handler);
	button_install_event_callback(&g_btn_right, ButtonEvent_SingleClick, button_event_handler);
	button_install_event_callback(&g_btn_ok, ButtonEvent_SingleClick, button_event_handler);
	button_install_event_callback(&g_btn_ok, ButtonEvent_DoubleClick, button_event_handler);
	button_install_event_callback(&g_btn_ok, ButtonEvent_LongPressHold, button_event_handler);

	/* 把按键加入按键扫描列表中 */
	button_start(&g_btn_left);
	button_start(&g_btn_right);
	button_start(&g_btn_ok);

	/* 打开按键扫描的定时器 */
	HAL_TIM_Base_Start_IT(&BTN_SCAN_TIM_HANDLER);

	return 0;
}

/**
 * @brief 按键事件处理
 * @param[in]	btn
 * @param[in]	event
 */
static void button_event_handler(void* btn, int event)
{
	/* 将按键事件传递给界面 */
	page_event_transmit(&g_page_manager, btn, event);
}
