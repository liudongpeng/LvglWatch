//
// Created by QT on 2023/1/29.
//

#include "page_dial_plate.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "user_page_private.h"
#include <stdio.h>
#include <stdlib.h>


extern RTC_HandleTypeDef hrtc;  /* RTC句柄 */

extern page_manager_t g_page_manager;    /* 界面管理器, 在freertos.c文件中定义 */

page_t page_dial;   /* "表盘"界面 */

static lv_obj_t *app_win;       /* "主菜单"界面窗口, 用于绘制其他控件 */
static lv_obj_t *img_bg;        /* 背景图片 */
static lv_obj_t *label_bmp280;  /* 气压计信息 */
static lv_obj_t *label_batt;    /* 电池信息 */
static lv_obj_t *label_date;    /* 日期 */

/* 两个时间标签交替使用实现上下滑动效果 */
static lv_obj_t *label_time_grp[4];
static lv_obj_t *label_time_grp_bk[4];

static lv_obj_t *time_cont;     /* 时间容器 */
static lv_obj_t *led_sec[2];    /* 秒指示灯 */

static lv_obj_t *icon_run;      /* 运动图标 */
static lv_obj_t *label_step_cnt;    /* 计步次数 */

static lv_timer_t *timer_time_update;       /* 时间更新定时器 */
static lv_timer_t *timer_state_bar_update;  /* 状态栏更新定时器 */


static RTC_DateTypeDef rtc_data;        /* 当前RTC日期 */
static RTC_TimeTypeDef rtc_time_last;   /* 上次记录的RTC时间 */
static RTC_TimeTypeDef rtc_time;        /* 当前RTC时间 */



/* ------ 测试用 ------ */
int timeTest;
static lv_obj_t *labelTimeTest;

/* ------ 测试用 ------ */



static void page_dial_bg_create();

static void page_dial_state_bar_create();

static void page_dial_label_date_create();

static void page_dial_label_time_create();

static void page_dial_led_create();

static void timer_task_state_bar_update(lv_timer_t *timer);

static void timer_task_time_update(lv_timer_t *timer);

static void date_update();

static void time_update();

static void page_dial_setup();

static void page_dial_exit();

static void page_dial_event_handle(void *obj, int event);


/**
 * @brief 创建表盘背景
 */
static void page_dial_bg_create()
{
	LV_IMG_DECLARE(img_background);

	img_bg = lv_img_create(app_win);
	lv_img_set_src(img_bg, &img_background);
	lv_obj_center(img_bg);
}

/**
 * @brief 创建状态栏
 */
static void page_dial_state_bar_create()
{
	LV_FONT_DECLARE(HandGotn_14);

	label_bmp280 = lv_label_create(app_win);
	lv_label_set_text(label_bmp280, "-- C -- kPa");
	lv_obj_align(label_bmp280, LV_ALIGN_TOP_LEFT, 0, 0);
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_text_font(&style, &HandGotn_14);
	lv_obj_add_style(label_bmp280, &style, 0);

	label_batt = lv_label_create(app_win);
	lv_label_set_text(label_batt, "#FFFFFF "LV_SYMBOL_BATTERY_2"#");
	lv_label_set_recolor(label_batt, true);
	lv_obj_align(label_batt, LV_ALIGN_TOP_RIGHT, 0, 0);

	/* 创建状态栏更新任务, 用定时器定时更新 */
	timer_state_bar_update = lv_timer_create(timer_task_state_bar_update, 2 * 1000, NULL);
}

/**
 * @brief 创建日期标签
 */
static void page_dial_label_date_create()
{
	LV_FONT_DECLARE(HandGotn_26);
	LV_FONT_DECLARE(Morganite_36);

	label_date = lv_label_create(app_win);
	lv_label_set_text(label_date, "01#FF0000 /#01 MON");
	lv_label_set_recolor(label_date, true);
	lv_obj_align(label_date, LV_ALIGN_TOP_MID, 0, 30);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_text_font(&style, &Morganite_36);
	lv_obj_add_style(label_date, &style, 0);
}

/**
 * @brief 创建时间标签
 */
static void page_dial_label_time_create()
{
	LV_FONT_DECLARE(Morganite_100);

	/* 时间标签显示窗口 */
	time_cont = lv_obj_create(app_win);
	lv_obj_set_scrollbar_mode(time_cont, LV_SCROLLBAR_MODE_OFF);  /* 关闭水平和竖直滚动条 */
	lv_obj_set_style_bg_opa(time_cont, LV_OPA_TRANSP, 0);   /* 设置背景透明 */
	lv_obj_set_size(time_cont, 130, 80);
	lv_obj_center(time_cont);

	static lv_style_t style_time_cont;
	lv_style_init(&style_time_cont);
//	lv_style_set_radius(&style_time_cont, 0); /* 设置边框弧度 */
//	lv_style_set_border_color(&style_time_cont, lv_color_make(0xff, 0, 0));
	lv_style_set_border_side(&style_time_cont, LV_BORDER_SIDE_NONE);  /* 设置边框位置为 不显示 */
	lv_style_set_pad_all(&style_time_cont, 0);    /* 设置此窗口上下左右全部padding大小为0 */
	lv_obj_add_style(time_cont, &style_time_cont, 0);


	/* 时间标签 */
	static lv_style_t style_label_time;
	lv_style_init(&style_label_time);
	lv_style_set_text_color(&style_label_time, lv_color_white());
	lv_style_set_text_font(&style_label_time, &Morganite_100);

	const static lv_coord_t x_pos[4] = {-45, -20, 20, 45};
	for (int i = 0; i < sizeof(label_time_grp) / sizeof(label_time_grp[0]); i++)
	{
		lv_obj_t *label = lv_label_create(time_cont);
		lv_label_set_text(label, "0");
		lv_obj_add_style(label, &style_label_time, 0);
		lv_obj_align(label, LV_ALIGN_CENTER, x_pos[i], 5);

		label_time_grp[i] = label;
	}

	for (int i = 0; i < sizeof(label_time_grp_bk) / sizeof(label_time_grp_bk[0]); i++)
	{
		lv_obj_t *label = lv_label_create(time_cont);
		lv_label_set_text(label, "0");
		lv_obj_add_style(label, &style_label_time, 0);
		lv_obj_align_to(label, label_time_grp[i], LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

		label_time_grp_bk[i] = label;
	}

	memset(&rtc_time_last, 0, sizeof(rtc_time_last));
	/* 创建时间更新任务 */
	timer_time_update = lv_timer_create(timer_task_time_update, 500, NULL);

//	/* ------ 测试 ------ */
//	labelTimeTest = lv_label_create(app_win);
//	lv_label_set_text(labelTimeTest, "");
//	lv_obj_align(labelTimeTest, LV_ALIGN_BOTTOM_LEFT, 0, 0);
//
//	static lv_style_t styleTest;
//	lv_style_init(&styleTest);
//	lv_style_set_text_color(&styleTest, lv_color_white());
//	lv_style_set_text_font(&styleTest, &lv_font_montserrat_14);
//	lv_obj_add_style(labelTimeTest, &styleTest, 0);
}

/**
 * @brief 创建时间标签中间的led
*/
static void page_dial_led_create()
{
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, 3);

	for (int i = 0; i < sizeof(led_sec) / sizeof(led_sec[0]); i++)
	{
		lv_obj_t *led = lv_led_create(time_cont);
		lv_led_set_color(led, lv_color_make(0xff, 0, 0));
		lv_obj_set_size(led, 8, 10);
		lv_obj_align(led, LV_ALIGN_CENTER, 0, i == 0 ? -10 : 10);
		lv_obj_add_style(led, &style, 0);

		led_sec[i] = led;
	}
}

/**
 * @brief 定时器任务栏更新任务
 * @param timer
*/
static void timer_task_state_bar_update(lv_timer_t *timer)
{
#define LV_SYMBOL_DEGREE_SIGN   "\xC2\xB0"

	/* 气压计更新 */
	lv_label_set_text_fmt(label_bmp280, "% 2dC"LV_SYMBOL_DEGREE_SIGN" %dm", 1, 1);

	/* 电池信息更新 */

}

/**
 * @brief 定时器时间更新任务
 * @param[in]   timer
*/
static void timer_task_time_update(lv_timer_t *timer)
{
	/* 更新时间标签 */
	time_update();

	/* 反转led */
	for (int i = 0; i < sizeof(led_sec) / sizeof(led_sec[0]); i++)
		lv_led_toggle(led_sec[i]);

	/* 更新日期标签 */
	date_update();
}

/**
 * @brief 日期更新
 */
static void date_update()
{
	HAL_RTC_GetDate(&hrtc, &rtc_data, RTC_FORMAT_BIN);

	const char *week[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
	int idx = rtc_data.WeekDay - 1;
	if (idx < 0 || idx > 6)
		idx = 0;
	lv_label_set_text_fmt(label_date, "%02d#FF0000 /#%02d %s", rtc_data.Month, rtc_data.Date, week[idx]);
};

/**
 * @brief 更新时间标签
 * @param[in]   cur_val
 * @param[in]   last_val
 * @param[in]   index
*/
#define LABEL_TIME_UPDATE_IF_VAL_CHANGE(cur_val, last_val, index)   \
do  \
{   \
    if ((cur_val) != (last_val))    \
    {   \
        lv_obj_t* label_now, *label_next;   \
    \
    /* 判断两个标签的相对位置, 以此来确定谁是下一个标签 */ \
    if (update_layout_and_get_obj_y(label_time_grp_bk[(index)]) > update_layout_and_get_obj_y(label_time_grp[(index)]))   \
    {   \
        label_now = label_time_grp[(index)];    \
        label_next = label_time_grp_bk[(index)];    \
    }   \
    else    \
    {   \
        label_now = label_time_grp_bk[(index)]; \
        label_next = label_time_grp[(index)];   \
    }   \
    \
    lv_label_set_text_fmt(label_now, "%d", (last_val)); \
    lv_label_set_text_fmt(label_next, "%d", (cur_val)); \
    \
    /* 把下一次要显示的标签移出显示区域 */  \
    lv_obj_align_to(label_next, label_now, LV_ALIGN_OUT_TOP_MID, 0, -10);   \
    \
    /* 计算y偏移 */ \
    lv_coord_t y_ofs = abs(update_layout_and_get_obj_y(label_now) - update_layout_and_get_obj_y(label_next));   \
    \
    /* 执行滑动动画 */    \
    LB_OBJ_START_ANIM(label_now, y, update_layout_and_get_obj_y(label_now) + y_ofs, LV_OBJ_ANIM_EXEC_TIME); \
    LB_OBJ_START_ANIM(label_next, y, update_layout_and_get_obj_y(label_next) + y_ofs, LV_OBJ_ANIM_EXEC_TIME);   \
    }                                                               \
    \
} while (0);


/**
 * @brief 时间更新
 */
static void time_update()
{
	/* 获取RTC时间 */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

	/* 分钟个位 */
	LABEL_TIME_UPDATE_IF_VAL_CHANGE(rtc_time.Minutes % 10, rtc_time_last.Minutes % 10, 3);
	/* 分钟十位 */
	LABEL_TIME_UPDATE_IF_VAL_CHANGE(rtc_time.Minutes / 10, rtc_time_last.Minutes / 10, 2);

	/* 小时个位 */
	LABEL_TIME_UPDATE_IF_VAL_CHANGE(rtc_time.Hours % 10, rtc_time_last.Hours % 10, 1);
	/* 小时十位 */
	LABEL_TIME_UPDATE_IF_VAL_CHANGE(rtc_time.Hours / 10, rtc_time_last.Hours / 10, 0);

	rtc_time_last = rtc_time;
}


/**
 * @brief 界面初始化
 */
static void page_dial_setup()
{
	lv_obj_move_foreground(app_win);

	page_dial_bg_create();
	page_dial_state_bar_create();
	page_dial_label_date_create();
	page_dial_label_time_create();
	page_dial_led_create();

}

/**
 * @brief 界面退出
 */
static void page_dial_exit()
{
	/* 关闭任务栏和时间日期更新任务 */
	lv_timer_del(timer_state_bar_update);
	lv_timer_del(timer_time_update);

	lv_obj_clean(app_win);
}

/**
 * @brief 界面事件处理
 * @param[in]	obj
 * @param[in]	event
 */
static void page_dial_event_handle(void *obj, int event)
{
	/* 有按键发生动作时, 进入主菜单 */
	if (event == ButtonEvent_SingleClick || event == ButtonEvent_LongPressHold)
	{
		page_push(&g_page_manager, Page_MainMenu);
	}
}

/**
 * @brief 创建"表盘"界面窗口(容器)
 * @return
 */
int dial_window_create()
{
	if ((app_win = lv_obj_create(lv_scr_act())) == NULL)
		return -1;

	lv_obj_set_size(app_win, APP_WIN_WIDTH(lv_scr_act()), APP_WIN_HEIGHT(lv_scr_act()));
	lv_obj_center(app_win);
	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* 关闭水平和竖直滚动条 */

	static lv_style_t style;
	lv_style_init(&style);
//	lv_style_set_radius(&style, 0); /* 设置样式圆角弧度为 直角 */
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
int page_dial_register()
{
	if (page_register(&g_page_manager, Page_Dial, page_dial_setup, NULL,
	                  page_dial_exit, page_dial_event_handle) < 0)
	{
		printf("page_dial_register() failed\n");

		return -1;
	}

	return 0;
}
