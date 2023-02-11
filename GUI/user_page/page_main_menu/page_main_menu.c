//
// Created by QT on 2023/2/1.
//

#include "page_main_menu.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "lvgl_img_list.h"
#include "user_page_private.h"
#include <stdio.h>


/**
 * @brief 声明图标资源
 */
LV_IMG_DECLARE(icon_stopwatch_white_64x64)
LV_IMG_DECLARE(icon_mountain_white_64x64)
LV_IMG_DECLARE(icon_brightness_white_64x64)
LV_IMG_DECLARE(icon_setting_white_64x64)
LV_IMG_DECLARE(icon_butterfly_white_64x64)
LV_IMG_DECLARE(icon_about_white_64x64)

LV_IMG_DECLARE(icon_stopwatch_red_64x64)
LV_IMG_DECLARE(icon_mountain_red_64x64)
LV_IMG_DECLARE(icon_brightness_red_64x64)
LV_IMG_DECLARE(icon_setting_red_64x64)
LV_IMG_DECLARE(icon_butterfly_red_64x64)
LV_IMG_DECLARE(icon_about_red_64x64)


extern page_manager_t g_page_manager;    /* 界面管理器, 在freertos.c文件中定义 */
page_t page_main_menu;    /* "主菜单"界面, 以此来进行界面管理 */

static lv_obj_t *app_win;      /* "主菜单"界面窗口, 用于绘制其他控件 */
static lv_obj_t *label_title;  /* 标题标签 */
static lv_obj_t *line_title;   /* 标题分割线 */
static lv_obj_t *icon_disp;    /* img控件, 用于显示图标 */
static lv_obj_t *icon_cont;    /* 图标容器 */

static uint8_t icon_idx_cur;   /* 当前图标索引 */


/**
 * @brief 图标对象信息定义
 */
typedef struct icon_info
{
	const void *icon_data_normal;  /* 正常图标数据 */
	const void *icon_data_select;  /* 选中图标数据 */

	const char *icon_name;  /* 图标名称 */

	lvgl_img_win_t *icon;   /* 图标窗口 */

	uint8_t page_id;        /* 界面id */
} icon_t;

/* 图标集合, 保存所有图标对象 */
static icon_t icon_grp[] = {
	{.icon_data_normal = &icon_stopwatch_white_64x64, .icon_data_select = &icon_stopwatch_red_64x64,
		.icon_name = "StopWatch", .page_id = Page_StopWatch},   /* 停表 */
	{.icon_data_normal = &icon_mountain_white_64x64, .icon_data_select = &icon_mountain_red_64x64,
		.icon_name = "Altitude", .page_id = Page_Altitude}, /* 海拔高度 */
	{.icon_data_normal = &icon_brightness_white_64x64, .icon_data_select = &icon_brightness_red_64x64,
		.icon_name = "BackLight", .page_id = Page_BackLight},   /* 亮度 */
	{.icon_data_normal = &icon_setting_white_64x64, .icon_data_select = &icon_setting_red_64x64,
		.icon_name = "TimeConfig", .page_id = Page_TimeCfg},   /* 时间设置 */
	{.icon_data_normal = &icon_butterfly_white_64x64, .icon_data_select = &icon_butterfly_red_64x64,
		.icon_name = "Theme", .page_id = Page_Theme},   /* 时间设置 */
	{.icon_data_normal = &icon_about_white_64x64, .icon_data_select = &icon_about_red_64x64,
		.icon_name = "About", .page_id = Page_About},   /* 关于 */
};

#define ICON_INTERVAL   20  /* 图标间隔 */
#define ICON_SIZE       64  /* 图标大小 */
#define ICON_COUNT      (sizeof(icon_grp) / sizeof(icon_t))
#define ICON_INDEX_MAX  (ICON_COUNT - 1)


static void page_main_menu_icon_grp_create();
static void page_main_menu_title_create();
static void page_main_menu_icon_grp_move_focus(uint8_t idx);
static void page_main_menu_icon_grp_move(int8_t dir);
static void page_main_menu_img_shadow_create();

static void page_main_menu_setup();
static void page_main_menu_exit();
static void page_main_menu_event_handle(void *btn, int event);


/**
 * @brief 创建标题栏
 */
static void page_main_menu_title_create()
{
	LV_FONT_DECLARE(HandGotn_20);

	/* 创建标题标签 */
	label_title = lv_label_create(app_win);
	/* 默认框选第二个图标, 所以在此处显示第二个图标的名称 */
	lv_label_set_text_static(label_title, icon_grp[1].icon_name);
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_color(&style_label, lv_color_white());
	lv_style_set_text_font(&style_label, &HandGotn_20);
	lv_obj_add_style(label_title, &style_label, 0);

	/* 创建标题下面的分隔线 */
	static lv_point_t line_points[2];
	line_points[1].x = APP_WIN_WIDTH(app_win) - 1;
	line_title = lv_line_create(app_win);
	lv_line_set_points(line_title, line_points, 2);
	lv_obj_align_to(line_title, label_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_color(&style_line, lv_color_make(0xff, 0, 0));
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_line_rounded(&style_line, true);
	lv_obj_add_style(line_title, &style_line, 0);
}

/**
 * @brief 创建图标集合
 */
static void page_main_menu_icon_grp_create()
{
	/* 创建图标显示窗口 */
	icon_disp = lv_obj_create(app_win);
	lv_obj_set_size(icon_disp, ICON_SIZE + 20,
	                APP_WIN_HEIGHT(app_win) - update_layout_and_get_obj_y(line_title) - 20);
	lv_obj_set_scrollbar_mode(icon_disp, LV_SCROLLBAR_MODE_OFF);  /* 关闭水平和竖直滚动条 */
	lv_obj_set_style_bg_opa(icon_disp, LV_OPA_TRANSP, 0);   /* 设置背景透明 */
	lv_obj_align_to(icon_disp, line_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

	static lv_style_t style_icon_disp;
	lv_style_init(&style_icon_disp);
	lv_style_set_pad_all(&style_icon_disp, 0);      /* 设置此窗口上下左右全部padding大小为0 */
	lv_style_set_radius(&style_icon_disp, 0);       /* 设置边框弧度 */
	lv_style_set_border_width(&style_icon_disp, 0); /* 设置边框宽度为0 */
	lv_style_set_border_side(&style_icon_disp, LV_BORDER_SIDE_NONE);  /* 设置边框位置为 不显示 */
	lv_obj_add_style(icon_disp, &style_icon_disp, 0);


	/* 把图片竖向拼接起来, 图片可以在屏幕上下滑动 */
	icon_cont = lv_obj_create(icon_disp);
	lv_obj_set_style_bg_opa(icon_cont, LV_OPA_TRANSP, 0);   /* 设置对象背景透明 */
	lv_obj_set_scrollbar_mode(icon_cont, LV_SCROLLBAR_MODE_OFF);  /* 关闭水平和竖直滚动条 */
	lv_obj_set_size(icon_cont, update_layout_and_get_obj_width(icon_disp),
	                (ICON_SIZE + ICON_INTERVAL) * ICON_COUNT - ICON_INTERVAL);
	lv_obj_align_to(icon_cont, icon_disp, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_icon_cont;
	lv_style_init(&style_icon_cont);
	lv_style_set_pad_all(&style_icon_cont, 0);    /* 设置此窗口上下左右全部padding大小为0 */
	lv_style_set_border_color(&style_icon_cont, lv_color_make(0xff, 0, 0));
	lv_style_set_radius(&style_icon_cont, 0); /* 设置边框弧度 */
	lv_style_set_border_width(&style_icon_cont, 0); /* 设置边框宽度为0 */
	lv_style_set_border_side(&style_icon_cont, LV_BORDER_SIDE_NONE);  /* 设置边框位置为 不显示 */
	lv_obj_add_style(icon_cont, &style_icon_cont, 0);

	/* 在icon_cont上依次创建每个图标窗口, 每个窗口中有两个图标, 分为白色和红色两个样式 */
	for (int i = 0; i < ICON_COUNT; i++)
	{
		lvgl_img_win_t* img_wim = lvgl_img_win_create(icon_cont, ICON_SIZE, ICON_SIZE);
		lv_obj_align(lvgl_img_win_get_win(img_wim), LV_ALIGN_TOP_MID, 0, 0);

		lv_obj_t* icon_normal = lv_img_create(lvgl_img_win_get_win(img_wim));
		lv_img_set_src(icon_normal, icon_grp[i].icon_data_normal);
		lv_obj_align(icon_normal, LV_ALIGN_TOP_MID, 0, 0);
		lvgl_img_list_insert(lvgl_img_win_get_list(img_wim), icon_normal, "normal");

		lv_obj_t* icon_select = lv_img_create(lvgl_img_win_get_win(img_wim));
		lv_img_set_src(icon_select, icon_grp[i].icon_data_select);
		lv_obj_align(icon_select, LV_ALIGN_TOP_MID, 0, 0);
		lvgl_img_list_insert(lvgl_img_win_get_list(img_wim), icon_select, "select");

		/* 把normal图标移到前台 */
		lv_obj_move_foreground(lvgl_img_get_obj(lvgl_img_find(lvgl_img_win_get_list(img_wim), "normal")));

		/* 设置每个图标显示窗口的位置 */
		lv_obj_set_y(lvgl_img_win_get_win(img_wim), (ICON_SIZE + ICON_INTERVAL) * i);

		icon_grp[i].icon = img_wim;
	}
}

/**
 * @brief 利用img控件实现滚动阴影效果
 */
static void page_main_menu_img_shadow_create()
{
	LV_IMG_DECLARE(img_shadow_up)
	LV_IMG_DECLARE(img_shadow_down)

	lv_obj_t *img_up = lv_img_create(icon_disp);
	lv_img_set_src(img_up, &img_shadow_up);
	lv_obj_align(img_up, LV_ALIGN_TOP_MID, 0, 0);

	lv_obj_t *img_down = lv_img_create(icon_disp);
	lv_img_set_src(img_down, &img_shadow_down);
	lv_obj_align(img_down, LV_ALIGN_BOTTOM_MID, 0, 0);
}

/**
 * @brief 移动到选中的图标
 * @param[in]	idx
 */
static void page_main_menu_icon_grp_move_focus(uint8_t idx)
{
	static uint8_t last_idx;

	if (idx > ICON_INDEX_MAX)
		return;

	/* 改变标题栏的文字说明 */
	lv_label_set_text_static(label_title, icon_grp[idx].icon_name);

	/* 把上次选择的图标换为对应的白色图片 */
	lv_obj_move_foreground(lvgl_img_get_obj(
		lvgl_img_find(lvgl_img_win_get_list(icon_grp[last_idx].icon), "normal")));

	/* 把当前图标图片换为对应的为红色图片 */
	lv_obj_move_foreground(lvgl_img_get_obj(
		lvgl_img_find(lvgl_img_win_get_list(icon_grp[idx].icon), "select")));

	/* 计算目标y位置 */
	int tar_y = -(ICON_SIZE + ICON_INTERVAL) * (idx - 1) - ICON_INTERVAL;

	/* 滑动图标长图到目标位置 */
	LV_OBJ_START_ANIM(icon_cont, y, tar_y, LVGL_OBJ_ANIM_EXEC_TIME);

	/* 记录上次的图标索引 */
	last_idx = idx;
}

/**
 * @brief 上下移动图标集合
 * @param[in]	dir	1 向上移动,
 * 					-1 向下移动
 */
static void page_main_menu_icon_grp_move(int8_t dir)
{
	/* 更新图标索引 */
	int tmp = icon_idx_cur;
	tmp += dir;
	if (tmp < 0)
		tmp = ICON_INDEX_MAX;
	if (tmp > ICON_INDEX_MAX)
		tmp = 0;
	icon_idx_cur = tmp;

	/* 移动到当前选中的图标 */
	page_main_menu_icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief 主菜单界面初始化
 */
static void page_main_menu_setup()
{
	printf("enter page_main_menu_setup()\n");

	lv_obj_move_foreground(app_win);

	icon_idx_cur = 0;

	page_main_menu_title_create();
	page_main_menu_icon_grp_create();
	page_main_menu_img_shadow_create();

	page_main_menu_icon_grp_move_focus(icon_idx_cur);

	printf("leave page_main_menu_setup()\n");
}

/**
 * @brief 主菜单界面退出
 */
static void page_main_menu_exit()
{
	printf("enter page_main_menu_exit()\n");

	/* 图标全部滑出 */
	LV_OBJ_START_ANIM(icon_cont, y, update_layout_and_get_obj_height(icon_disp) + ICON_SIZE, LVGL_OBJ_ANIM_EXEC_TIME);

	/* 界面延时, 延时期间保持界面刷新 */
//	lvgl_page_delay(LVGL_OBJ_ANIM_EXEC_TIME);

	lv_obj_clean(app_win);

	printf("leave page_main_menu_exit()\n\n");
}

/**
 * @brief 主菜单界面上的按键事件处理
 * @param[in]	btn
 * @param[in]	event
 */
static void page_main_menu_event_handle(void *btn, int event)
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
				if (g_page_manager.page_list[id].page_setup != NULL)
					page_push(&g_page_manager, id);
				break;

			default:
				break;
		}
	}
	else
	{
		/* 单击或长按 上下(左右)按键, 图标上下滑动并选中 */
		if (event == ButtonEvent_SingleClick || event == ButtonEvent_LongPressHold)
		{
			if (btn == &g_btn_left)
				page_main_menu_icon_grp_move(-1);
			if (btn == &g_btn_right)
				page_main_menu_icon_grp_move(1);
		}
	}
}

/**
 * @brief 创建"主菜单"界面窗口(容器)
 * @return
 */
int main_menu_window_create()
{
	if ((app_win = lv_obj_create(lv_scr_act())) == NULL)
		return -1;

	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* 关闭水平和竖直滚动条 */
	lv_obj_set_size(app_win, APP_WIN_WIDTH(lv_scr_act()), APP_WIN_HEIGHT(lv_scr_act()));
	lv_obj_center(app_win);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, 0); /* 设置样式圆角弧度为 直角 */
	lv_style_set_border_side(&style, LV_BORDER_SIDE_NONE);  /* 设置边框位置为 不显示 */
	lv_style_set_bg_color(&style, lv_color_black());    /* 设置背景色为 黑色 */
	lv_style_set_pad_all(&style, 0);    /* 设置此窗口上下左右全部padding大小为0 */
	lv_obj_add_style(app_win, &style, 0);

	return 0;
}

/**
 * @brief 主菜单界面注册到界面管理器
 * @return
 */
int page_main_menu_register()
{
	/* 创建新界面, 并注册到界面管理器 */
	if (page_register(&g_page_manager, Page_MainMenu, page_main_menu_setup, NULL,
	                  page_main_menu_exit, page_main_menu_event_handle) < 0)
	{
		printf("page_main_menu_register() failed\n");

		return -1;
	}

	return 0;
}
