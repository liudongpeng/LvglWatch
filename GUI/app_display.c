//
// Created by QT on 2023/2/2.
//

#include "app_display.h"
#include "page_manager.h"
#include "lvgl.h"

#include "user_page_private.h"
#include "page_about.h"
#include "page_dial_plate.h"
#include "page_main_menu.h"

#include <stdio.h>


extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */


/**
 * @brief ����������洰��
 * @return
 */
static int app_windows_create()
{
	/* ����"����"���洰�� */
	if (dial_window_create() < 0)
	{
		printf("dial_window_create() failed"DEBUG_ERROR_INFO);
		return -1;
	}

	/* ����"���˵�"���洰�� */
	if (main_menu_window_create() < 0)
	{
		printf("main_menu_window_create() failed"DEBUG_ERROR_INFO);
		return -2;
	}

	/* ����"����"���洰�� */
	if (about_window_create() < 0)
	{
		printf("about_window_create() failed"DEBUG_ERROR_INFO);
		return -1;
	}

	return 0;
}

/**
 * @brief �û���������ʼ��
 * @return
 */
static int app_pages_register()
{
	/* ע��"����"���� */
	if (page_dial_register() < 0)
	{
		printf("page_dial_register() failed"DEBUG_ERROR_INFO);
		return -1;
	}

	/* ע��"���˵�"���� */
	if (page_main_menu_register() < 0)
	{
		printf("page_main_menu_register() failed"DEBUG_ERROR_INFO);
		return -2;
	}

	/* ע��"����"���� */
	if (page_about_register() < 0)
	{
		printf("page_about_register() failed"DEBUG_ERROR_INFO);
		return -3;
	}
	printf("page_about id = %d\n", page_about.page_id);

//	printf("page_about id = %d\n", page_main_menu.page_id);
//	printf("page_about addr = %p\n", &page_main_menu);
//	printf("page_about addr = %p\n", get_page_by_id(&g_page_manager, Page_MainMenu));



	/* ��"����"���� */
	if (page_push(&g_page_manager, get_page_by_id(&g_page_manager, Page_Dial)) < 0)
	{
		printf("page_push() page_dial failed"DEBUG_ERROR_INFO);
		return -3;
	}
}


/**
 * @brief �����ʾ��ʼ��
 * @return
 */
int app_display_init()
{
	/* �������н���Ĵ��� */
	if (app_windows_create() < 0)
	{
		printf("app_windows_create() failed"DEBUG_ERROR_INFO);
		return -1;
	}

	/* �����н���ע�ᵽ��������� */
	if (app_pages_register() < 0)
	{
		printf("app_pages_register() failed"DEBUG_ERROR_INFO);
		return -2;
	}

	return 0;
}


/**
 * @brief �����ʾ����
 */
void app_display_update()
{
	lv_task_handler();
	page_run(&g_page_manager);
}
