//
// Created by QT on 2023/1/31.
//

#include "app_win_manage.h"
#include "page_manager.h"
#include <stdio.h>



extern page_manager_t g_page_manager;    /* 界面管理器, 在freertos.c文件中定义 */


/**
 * @brief 创建软件界面窗口
 * @return
 */
static void app_windows_create()
{
	int ret = 0;

	ret = about_window_create();  /* "关于"界面窗口 */
	if (ret == 0)
		printf("about_window_create() ok\n");



}

/**
 * @brief 用户软件界面初始化
 * @return
 */
static void app_pages_register()
{
	int ret = 0;

	/* "关于"界面注册 */
	if ((ret = page_about_register()) == 0)
		printf("page_about_register() ok\n");

	/* "表盘"界面注册 */
//	page_dial_register();

	/* 打开表盘 */
	if ((ret = page_push(&g_page_manager, &page_about)) == 0)
		printf("page_about push ok\n");
}

/**
 * @brief 软件显示初始化
 * @return
 */
int app_display_init()
{
	/* 创建所有界面的窗口对象 */
	app_windows_create();

	/* 将所有界面注册到界面管理器 */
	app_pages_register();

}