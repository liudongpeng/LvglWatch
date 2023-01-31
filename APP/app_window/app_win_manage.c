//
// Created by QT on 2023/1/31.
//

#include "app_win_manage.h"
#include "page_manager.h"
#include <stdio.h>



extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */


/**
 * @brief ����������洰��
 * @return
 */
static void app_windows_create()
{
	int ret = 0;

	ret = about_window_create();  /* "����"���洰�� */
	if (ret == 0)
		printf("about_window_create() ok\n");



}

/**
 * @brief �û���������ʼ��
 * @return
 */
static void app_pages_register()
{
	int ret = 0;

	/* "����"����ע�� */
	if ((ret = page_about_register()) == 0)
		printf("page_about_register() ok\n");

	/* "����"����ע�� */
//	page_dial_register();

	/* �򿪱��� */
	if ((ret = page_push(&g_page_manager, &page_about)) == 0)
		printf("page_about push ok\n");
}

/**
 * @brief �����ʾ��ʼ��
 * @return
 */
int app_display_init()
{
	/* �������н���Ĵ��ڶ��� */
	app_windows_create();

	/* �����н���ע�ᵽ��������� */
	app_pages_register();

}