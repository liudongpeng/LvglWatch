//
// Created by QT on 2023/1/29.
//

#include "page_about.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "user_page_private.h"
#include <stdio.h>


extern page_manager_t g_page_manager;    /* 界面管理器, 在freertos.c文件中定义 */

page_t page_about;    /* "关于"界面, 以此来进行界面管理 */

static lv_obj_t *app_win;   /* "关于"界面窗口, 用于绘制其他控件 */

static lv_obj_t *label_title;   /* 标题标签 */
static lv_obj_t *line_title;    /* 标题分割线 */

static lv_obj_t *img_logo;  /* 图片logo */

static lv_obj_t *label_author;  /* 作者 */
static lv_obj_t *label_fw_info; /* 固件信息 */


static void page_about_title_create(const char *text);
static void page_about_img_create();
static void page_about_label_info_create();

static void page_about_setup();
static void page_about_exit();
static void page_about_event_handle(void *obj, int event);


/**
 * @brief 创建界面标题
 * @param[in]	text
 */
static void page_about_title_create(const char *text)
{
	LV_FONT_DECLARE(HandGotn_20);

	/* 创建标签 */
	label_title = lv_label_create(app_win);
	lv_label_set_text_static(label_title, text);
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	/* 设置标签样式 */
	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_color(&style_label, lv_color_white());
	lv_style_set_text_font(&style_label, &HandGotn_20);
	lv_obj_add_style(label_title, &style_label, 0);

	/* 创建标签下的分隔线 */
	static lv_point_t line_points[2];
	line_points[1].x = APP_WIN_WIDTH(app_win) - 1;
	line_title = lv_line_create(app_win);
	lv_line_set_points(line_title, line_points, 2);
	lv_obj_align_to(line_title, label_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

	/* 设置分割线样式 */
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_color(&style_line, lv_color_make(0xff, 0, 0));
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_line_rounded(&style_line, true);
	lv_obj_add_style(line_title, &style_line, 0);
}

/**
 * @brief 创建界面图片
 */
static void page_about_img_create()
{
	LV_IMG_DECLARE(img_watchx_logo)

	img_logo = lv_img_create(app_win);
	lv_img_set_src(img_logo, &img_watchx_logo);
	lv_obj_align_to(img_logo, line_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	lv_obj_center(img_logo);
}

/**
 * @brief 创建信息标签
 */
static void page_about_label_info_create()
{
	LV_FONT_DECLARE(HandGotn_14);

	/* 作者标签控件 */
	label_author = lv_label_create(app_win);
	lv_label_set_text_static(label_author, "- DEVELOPER -");
	lv_obj_align(label_author, LV_ALIGN_BOTTOM_MID, 0, -30);
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_text_font(&style, &HandGotn_14);
	lv_obj_add_style(label_author, &style, 0);

	/* 固件信息标签控件 */
	label_fw_info = lv_label_create(app_win);
	lv_label_set_text_static(label_fw_info, WATCH_NAME" "WATCH_SW_VER"\n"__DATE__);
	lv_obj_align(label_fw_info, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_add_style(label_fw_info, &style, 0);
}

/**
 * @brief 界面初始化
 */
static void page_about_setup()
{
	lv_obj_move_foreground(app_win);

	page_about_title_create("About");
	page_about_img_create();
	page_about_label_info_create();

	printf("leave page_about_setup()\n\n");
}

/**
 * @brief 界面退出
 */
static void page_about_exit()
{
	lv_obj_clean(app_win);

	printf("leave page_about_exit()\n\n");
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
	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* 关闭水平和竖直滚动条 */

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, 0); /* 设置样式圆角为 直角 */
	lv_style_set_border_side(&style, LV_BORDER_SIDE_NONE);  /* 设置边框位置为 不显示 */
	lv_style_set_bg_color(&style, lv_color_black());    /* 设置背景色为 黑色 */
	lv_style_set_pad_all(&style, 0);    /* 设置上下左右全部padding大小为0 */
	lv_obj_add_style(app_win, &style, 0);

	return 0;
}

/**
 * @brief 注册界面到界面管理器
 * @return
 */
int page_about_register()
{
	/* 创建新界面, 并注册到界面管理器 */
	if (page_register(&g_page_manager, Page_About, page_about_setup, NULL,
					  page_about_exit, page_about_event_handle) < 0)
	{
		printf("page_about register failed\n");
		return -1;
	}

	printf("leave page_about_register(), cur_page_exec = %p\n\n", g_page_manager.page_list[Page_About].page_exit);

	return 0;
}
