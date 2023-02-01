//
// Created by QT on 2023/2/1.
//

#include "page_main_menu.h"
#include "bsp.h"
#include "util.h"
#include "user_page_private.h"
#include <stdio.h>


/**
 * @brief 声明图标资源
 */
LV_IMG_DECLARE(icon_info)
LV_IMG_DECLARE(icon_light)
LV_IMG_DECLARE(icon_mountain)
LV_IMG_DECLARE(icon_stop_watch)
LV_IMG_DECLARE(icon_theme)
LV_IMG_DECLARE(icon_time_cfg)

extern page_manager_t g_page_manager;    /* 界面管理器, 在freertos.c文件中定义 */

page_t page_main_menu;    /* "主菜单"界面, 以此来进行界面管理 */

static lv_obj_t *app_win;   /* "主菜单"界面窗口, 用于绘制其他控件 */

static lv_obj_t *label_title;   /* 标题标签 */
static lv_obj_t *line_title;    /* 标题分割线 */

static lv_obj_t *img_icon;  /* img控件, 用于显示图标 */
static lv_obj_t *icon_cont; /* 图标容器 */


/**
 * @brief 图标对象信息定义
 */
typedef struct icon
{
	const void *icon_data;  /* 图标数据 */
	const char *icon_name;  /* 图标描述 */
	lv_obj_t *img_icon; /* img控件, 用来显示图标 */
	uint8_t page_id;    /* 界面id */
} icon_t;

/* 图标集合, 保存所有图标对象 */
static icon_t icon_grp[] = {
	{.icon_data = &icon_info, .icon_name = "About", .page_id = 1},  /* 关于 */
};


#define ICON_INTERVAL   20  /* 图标间隔 */
#define ICON_SIZE       50  /* 图标大小 */

static uint8_t icon_idx_cur;    /* 当前图标索引 */

/* 图标索引的最大值 */
#define ICON_COUNT  (sizeof(icon_grp) / sizeof(icon_t))
#define ICON_INDEX_MAX  (ICON_COUNT - 1)


/**
 * @brief 创建图标集合
 */
static void icon_grp_create()
{
	/* 创建图标显示窗口 */
	img_icon = lv_obj_create(app_win);
	lv_obj_set_size(img_icon, ICON_SIZE + 20, APP_WIN_HEIGHT(app_win) - lv_obj_get_y(line_title) - 20);

	/* 把图片竖向拼接起来, 图片可以在屏幕上下滑动 */
	icon_cont = lv_obj_create(img_icon);
	lv_obj_set_size(icon_cont, lv_obj_get_width(img_icon), (ICON_SIZE + ICON_INTERVAL) * ICON_COUNT);
	lv_obj_set_y(icon_cont, lv_obj_get_height(icon_cont));

	for (int i = 0; i < ICON_COUNT; i++)
	{
		lv_obj_t *icon = lv_img_create(icon_cont);
		icon_grp[i].img_icon = icon;
		lv_img_set_src(icon, icon_grp[i].icon_data);
		lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 0);

		/* 计算每个图标的偏移量, 把所有图标从上到下依次摆放 */
		int ofs_y = (ICON_SIZE - lv_obj_get_height(icon)) / 2;
		lv_obj_set_y(icon, (ICON_SIZE + ICON_INTERVAL) * i + ofs_y);
	}
}

/**
 * @brief 创建标题栏
 */
static void title_create()
{
	label_title = lv_label_create(app_win);
	/* 默认框选第二个图标, 所以在此处显示第二个图标的名称 */
	lv_label_set_text_static(label_title, icon_grp[1].icon_name);
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_label;


	line_title = lv_line_create(app_win);
	lv_point_t line_points[2] = {{0,                      0},
	                             {APP_WIN_WIDTH(app_win), 0}};
	lv_line_set_points(line_title, line_points, 2);
	lv_obj_align_to(line_title, label_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_line_rounded(&style_line, true);
}

/**
 * @brief 移动到选中的图标
 * @param[in]	idx
 */
static void icon_grp_move_focus(uint8_t idx)
{
	if (idx > ICON_INDEX_MAX)
		return;

	/* 改变标题栏的文字说明 */
	lv_label_set_text_static(label_title, icon_grp[idx].icon_name);

	/* 计算目标y位置 */
	int tar_y = -(ICON_SIZE + ICON_INTERVAL) * (idx - 1);

	/* 滑动图标长图到目标位置 */

}

/**
 * @brief 上下移动图标集合
 * @param[in]	dir	1 向上移动,
 * 					-1 向下移动
 */
static void icon_grp_move(int8_t dir)
{
	/* 在限定范围内移动 */


	/* 移动到当前选中的图标 */
	icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief 利用img控件实现滚动阴影效果
 */
static void img_shadow_create()
{
	LV_IMG_DECLARE(img_shadow_up)
	LV_IMG_DECLARE(img_shadow_down)

	lv_obj_t *img_up = lv_img_create(icon_cont);
	lv_img_set_src(img_up, &img_shadow_up);
	lv_obj_align(img_up, LV_ALIGN_TOP_MID, 0, 0);

	lv_obj_t *img_down = lv_img_create(icon_cont);
	lv_img_set_src(img_down, &img_shadow_down);
	lv_obj_align(img_down, LV_ALIGN_BOTTOM_MID, 0, 0);
}

/**
 * @brief 主菜单界面初始化
 */
static void page_main_menu_setup()
{
	title_create();
	icon_grp_create();
	img_shadow_create();

	icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief 主菜单界面退出
 */
static void page_main_menu_exit()
{


	lv_obj_clean(app_win);
}

/**
 * @brief 主菜单界面上的按键事件处理
 * @param[in]	btn
 * @param[in]	event
 */
static void page_main_menu_event(void *btn, int event)
{
	uint8_t id;

	if (btn == &g_btn_ok)
	{
		switch (event)
		{
			case ButtonEvent_DoubleClick:
				/* 双击确认按钮, 退出当前页面 */
				page_pop(&g_page_manager);
				break;

			case ButtonEvent_SingleClick:
				/* 单击确认按钮, 进入选中的页面 */
				id = icon_grp[icon_idx_cur].page_id;
				page_push(&g_page_manager, get_page_by_id(&g_page_manager, id));
				break;

			default:
				break;
		}
	}
}



