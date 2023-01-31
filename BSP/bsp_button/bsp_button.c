//
// Created by QT on 2023/1/29.
//

#include "bsp_button.h"


extern page_manager_t g_page_manager;	/* ���������, ��freertos.c�ļ��ж��� */


button_t g_btn_left;  /* ��� */
button_t g_btn_right; /* �Ҽ� */
button_t g_btn_ok;    /* ѡ��� */



static void button_event_handler(void* btn, int event);


/**
 * @brief ��ʼ�������ϵİ���
 * @return
 */
int bsp_button_init()
{
	/* ʵ������������ */
	button_create(&g_btn_left, 0, btn_left_get_level);
	button_create(&g_btn_right, 0, btn_right_get_level);
	button_create(&g_btn_ok, 0, btn_ok_get_level);

	/* ��װ�����¼�����ص����� */
	button_install_event_callback(&g_btn_left, ButtonEvent_SingleClick, button_event_handler);
	button_install_event_callback(&g_btn_right, ButtonEvent_SingleClick, button_event_handler);
	button_install_event_callback(&g_btn_ok, ButtonEvent_SingleClick, button_event_handler);
	button_install_event_callback(&g_btn_ok, ButtonEvent_DoubleClick, button_event_handler);
	button_install_event_callback(&g_btn_ok, ButtonEvent_LongPressHold, button_event_handler);

	/* �Ѱ������밴��ɨ���б��� */
	button_start(&g_btn_left);
	button_start(&g_btn_right);
	button_start(&g_btn_ok);

	/* �򿪰���ɨ��Ķ�ʱ�� */
	HAL_TIM_Base_Start_IT(&BTN_SCAN_TIM_HANDLER);

	return 0;
}

/**
 * @brief �����¼�����
 * @param[in]	btn
 * @param[in]	event
 */
static void button_event_handler(void* btn, int event)
{
	/* �������¼����ݸ����� */
	page_event_transmit(&g_page_manager, btn, event);
}
