//
// Created by QT on 2023/1/29.
//

#include "page_about.h"
#include "bsp.h"
#include "util.h"
#include <stdio.h>


extern page_manager_t g_page_manager;    /* 界面管理器, 在freertos.c文件中定义 */

page_t page_about;    /* "关于"界面 */

static lv_obj_t *app_win;   /* "关于"界面窗口, 界面中的元素在其上面绘制 */

static lv_obj_t *label_title;   /* 标题标签 */
static lv_obj_t *line_title;    /* 标题线条 */

static lv_obj_t *img_logo;  /* 图片logo */

static lv_obj_t *label_author;  /* 作者 */
static lv_obj_t *label_fw_info; /* 固件信息 */



static void title_create(const char *text);
static void img_create();
static void label_info_create();

static void page_about_setup();
static void page_about_exit();
static void page_about_event_handle(void *obj, int event);



/**
 * @brief 创建界面标题
 * @param[in]	text
 */
static void title_create(const char *text)
{
	label_title = lv_label_create(app_win);
	lv_label_set_text_static(label_title, text);
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_label;


	line_title = lv_line_create(app_win);
	lv_point_t line_points[2] = {{0,                      0},
	                             {APP_WIN_WIDTH(app_win), 0}};
	lv_line_set_points(line_title, line_points, 2);
	lv_obj_align_to(line_title, label_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

	static lv_style_t style_line;
}

/**
 * @brief 创建界面图片
 */
static void img_create()
{
	LV_IMG_DECLARE(img_watchx_logo)

	img_logo = lv_img_create(app_win);
	lv_img_set_src(img_logo, &img_watchx_logo);
//	lv_obj_align_to(img_logo, line_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	lv_obj_center(img_logo);
}

/**
 * @brief 创建信息标签
 */
static void label_info_create()
{
	/* 作者标签控件 */
	label_author = lv_label_create(app_win);
	lv_label_set_text_static(label_author, "- DEVELOPER -");
	lv_obj_align(label_author, LV_ALIGN_BOTTOM_MID, 0, -30);

	/* 固件信息标签控件 */
	label_fw_info = lv_label_create(app_win);
	lv_label_set_text_static(label_fw_info, WATCH_NAME" "WATCH_SW_VER"\n"__DATE__);
	lv_obj_align(label_fw_info, LV_ALIGN_BOTTOM_MID, 0, 0);

	printf("child cnt = %lu\n", lv_obj_get_child_cnt(app_win));
	printf("app_win = %p\n", app_win);
}

/**
 * @brief 界面初始化
 */
static void page_about_setup()
{
//	lv_obj_move_foreground(app_win);

	title_create("About");
	img_create();
	label_info_create();
}

/**
 * @brief 界面退出
 */
static void page_about_exit()
{
	lv_obj_clean(app_win);
}

/**
 * @brief 界面事件处理
 * @param[in]	obj
 * @param[in]	event
 */
static void page_about_event_handle(void *obj, int event)
{
	if (obj == &g_btn_ok)
	{
		if (event == ButtonEvent_SingleClick)
		{
			/* 现在显示的是"关于"界面, 在此界面时, 只需要做退出此界面的操作 */
			page_pop(&g_page_manager);
		}
	}
}

/**
 * @brief 创建"关于"界面窗口(容器)
 * @return
 */
int about_window_create()
{
	if ((app_win = lv_obj_create(lv_scr_act())) == NULL)
		return -1;

	lv_obj_set_size(app_win, 135, 240);
	lv_obj_center(app_win);

	return 0;
}

/**
 * @brief 注册界面到界面管理器
 * @return
 */
int page_about_register()
{
	/* 创建新界面, 并注册到界面管理器 */
	page_create(&page_about, page_about_setup, NULL, page_about_exit, page_about_event_handle);
	page_register(&g_page_manager, &page_about);

	return 0;
}
