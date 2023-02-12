//
// Created by summer on 2023/2/11.
//

#include "page_time_setting.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "lvgl_img_list.h"
#include "user_page_private.h"
#include <stdio.h>


extern page_manager_t g_page_manager;   /* ���������, ��freertos.c�ļ��ж��� */
page_t page_time_setting;   /* "ʱ������"���� */


static RTC_TimeTypeDef rtc_time;
static RTC_DateTypeDef rtc_date;

static lv_obj_t *app_win;   /* ���洰��, ���ڻ��������ؼ� */

static lv_obj_t *label_title;   /* ���� */
static lv_obj_t *line_title;

static lv_obj_t *time_disp; /* ʱ�䲿�� */
static lv_obj_t *label_time;
static lv_obj_t *time_disp_anim;

static lv_obj_t *date_disp; /* ���ڲ��� */
static lv_obj_t *label_year;
static lv_obj_t *label_date;
static lv_obj_t *label_week;
static lv_obj_t *date_disp_anim;

static int8_t cur_cfg_item;  /* ��ǰ���õ���Ŀ */
static bool cfg_changed = false;    /* �����Ƿ�ı� */




static void page_time_setting_title_create();

static void page_time_setting_time_date_disp_create();

static void page_time_setting_time_date_label_create();

static void page_time_setting_label_time_update(uint8_t item);
static void page_time_setting_label_date_update(uint8_t item);
static void page_time_setting_label_week_update();
static void page_time_setting_label_year_update(uint8_t item);

static void page_time_setting_label_title_update();

static void page_time_setting_time_cfg_val_update(int8_t opt);

static void page_time_setting_setup();
static void page_time_setting_exit();
static void page_time_setting_event_handle(void *btn, int event);


/**
 * @brief ����������
 */
static void page_time_setting_title_create()
{
	LV_FONT_DECLARE(HandGotn_20);

	/* ���������ǩ */
	label_title = lv_label_create(app_win);
	lv_label_set_text_static(label_title, "TimeConfig");
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_color(&style_label, lv_color_white());
	lv_style_set_text_font(&style_label, &HandGotn_20);
	lv_obj_add_style(label_title, &style_label, 0);

	/* ������������ķָ��� */
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
 * @brief ����ʱ������ڵ���ʾ����
 */
static void page_time_setting_time_date_disp_create()
{
	LV_FONT_DECLARE(HandGotn_14);

	/* ����ʱ��������ʾ���� */
	static lv_style_t style_disp;
	lv_style_init(&style_disp);
	lv_style_set_bg_opa(&style_disp, LV_OPA_TRANSP);
	lv_style_set_radius(&style_disp, 10);
	lv_style_set_border_width(&style_disp, 1);
	lv_style_set_border_color(&style_disp, lv_color_make(0xff, 0, 0));
	lv_style_set_border_opa(&style_disp, LV_OPA_COVER);
	lv_style_set_pad_all(&style_disp, 0);   /* ���ô˴�����������ȫ��padding��СΪ0 */

	time_disp = lv_obj_create(app_win);
	lv_obj_set_scrollbar_mode(time_disp, LV_SCROLLBAR_MODE_OFF);    /* �ر�ˮƽ����ֱ������ */
	lv_obj_set_size(time_disp, 128, 42);
	lv_obj_align_to(time_disp, line_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	lv_obj_add_style(time_disp, &style_disp, LV_PART_MAIN);

	date_disp = lv_obj_create(app_win);
	lv_obj_set_scrollbar_mode(date_disp, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
	lv_obj_set_size(date_disp, 128, 120);
	lv_obj_align_to(date_disp, time_disp, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	lv_obj_add_style(date_disp, &style_disp, LV_PART_MAIN);


	/* ������ǩ */
	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_font(&style_label, &HandGotn_14);
	lv_style_set_text_color(&style_label, lv_color_white());
	lv_style_set_bg_color(&style_label, lv_color_black());
	lv_style_set_bg_opa(&style_label, LV_OPA_COVER);

	lv_obj_t *label1 = lv_label_create(app_win);
	lv_label_set_text_static(label1, " TIME ");
	lv_obj_align_to(label1, time_disp, LV_ALIGN_OUT_TOP_MID, 0, 10);
	lv_obj_add_style(label1, &style_label, LV_PART_MAIN);

	lv_obj_t *label2 = lv_label_create(app_win);
	lv_label_set_text_static(label2, " DATE ");
	lv_obj_align_to(label2, date_disp, LV_ALIGN_OUT_TOP_MID, 0, 10);
	lv_obj_add_style(label2, &style_label, LV_PART_MAIN);
}

/**
 * @brief ����ʱ�����ڱ�ǩ
 */
static void page_time_setting_time_date_label_create()
{
	LV_FONT_DECLARE(HandGotn_26);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_font(&style, &HandGotn_26);
	lv_style_set_text_color(&style, lv_color_white());

	/* ��ȡ��ǰRTCʱ�� */
	bsp_rtc_get_time(&rtc_time);
	/* ����ʱ���ǩ */
	label_time = lv_label_create(time_disp);
	lv_label_set_recolor(label_time, true);
	lv_obj_center(label_time);
	lv_obj_add_style(label_time, &style, LV_PART_MAIN);
	page_time_setting_label_time_update(0); /* ����ʱ���ǩ */
//	printf("test1, %s|%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);

	/* ��ȡ���� */
	bsp_rtc_get_date(&rtc_date);
	/* �����������ܱ�ǩ */
	label_week = lv_label_create(date_disp);
	lv_label_set_recolor(label_week, true);
	lv_obj_align(label_week, LV_ALIGN_BOTTOM_MID, 0, -10);
	lv_obj_add_style(label_week, &style, LV_PART_MAIN);
	page_time_setting_label_week_update();  /* �������ڱ�ǩ */
//	printf("test2, %s|%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);

	label_year = lv_label_create(date_disp);
	lv_label_set_recolor(label_year, true);
	lv_obj_align(label_year, LV_ALIGN_TOP_MID, 0, 10);
	lv_obj_add_style(label_year, &style, LV_PART_MAIN);
	page_time_setting_label_year_update(0); /* ������ݱ�ǩ */
//	printf("test3, %s|%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);

	label_date = lv_label_create(date_disp);
	lv_label_set_recolor(label_date, true);
	lv_obj_center(label_date);
	lv_obj_add_style(label_date, &style, LV_PART_MAIN);
	page_time_setting_label_date_update(0); /* �������ڱ�ǩ */
//	printf("test4, %s|%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
}

/**
 * @brief ����ʱ���ǩ
 * @param[in]	item: ʱ0 ��1 ��2
 */
static void page_time_setting_label_time_update(uint8_t item)
{
	const char *fmt[] = {
			"%02d:%02d:%02d",
			"#FF0000 %02d#:%02d:%02d",
			"%02d:#FF0000 %02d#:%02d",
			"%02d:%02d:#FF0000 %02d#",
	};

	if (item > 3)
		item = 0;

	lv_label_set_text_fmt(label_time, fmt[item], rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
}

/**
 * @brief �������ڱ�ǩ
 * @param[in]	item: ��0 ��1
 */
static void page_time_setting_label_date_update(uint8_t item)
{
	const char *fmt[] = {
			"%02d-%02d",
			"#FF0000 %02d#-%02d",
			"%02d-#FF0000 %02d#"
	};

	if (item > 2)
		item = 0;

	lv_label_set_text_fmt(label_date, fmt[item], rtc_date.Month, rtc_date.Date);

	page_time_setting_label_week_update();  /* �������� */
}

/**
 * @brief �������ڱ�ǩ
 */
static void page_time_setting_label_week_update()
{
	const char *week[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
	int8_t idx;

	/* �������� */
	bsp_rtc_get_week(&rtc_date);

	idx = rtc_date.WeekDay - 1;
	if (idx < 0 || idx > 6)
		idx = 0;

	lv_label_set_text(label_week, week[idx]);
}

/**
 * @brief ������ݱ�ǩ
 */
static void page_time_setting_label_year_update(uint8_t item)
{
	const char *fmt = item ? "#FF0000 20%02d#" : "20%02d";

	lv_label_set_text_fmt(label_year, fmt, rtc_date.Year);

	page_time_setting_label_week_update();  /* �������� */
}

/**
 * @brief ���±����ǩ
 */
static void page_time_setting_label_title_update()
{
	const char *title[] = {
			"TimeConfig",
			"Hours",
			"Minutes",
			"Seconds",
			"Year",
			"Month",
			"Date",
			"Week"
	};

	const char* text;
	if (cur_cfg_item == 0)
		text = cfg_changed ? "Save?" : title[cur_cfg_item];
	else
		text = title[cur_cfg_item];

	lv_label_set_text(label_title, text);
}

/**
 * @brief ʱ������ֵ����
 * @param[in]	opt: 1��һ, -1��һ, 0�޲���
 */
static void page_time_setting_time_cfg_val_update(int8_t opt)
{
	int8_t val;  /* ���з�����, ������� */

	page_time_setting_label_title_update();

	if (cur_cfg_item != RTC_Item_None && opt != 0)
		cfg_changed = true;

	/* ʱ������ */
	if (cur_cfg_item >= RTC_Item_Hour && cur_cfg_item <= RTC_Item_Second)
	{
		switch (cur_cfg_item)
		{
			case 1:
				val = (int8_t)(rtc_time.Hours + opt);
				if (val < 0 )
					val = 23;
				if (val > 23)
					val = 0;
				rtc_time.Hours = val;
				break;

			case 2:
				val = (int8_t)(rtc_time.Minutes + opt);
				if (val < 0)
					val = 59;
				if (val > 59)
					val = 0;
				rtc_time.Minutes = val;
				break;

			case 3:
				val = (int8_t)(rtc_time.Seconds + opt);
				if (val < 0)
					val = 59;
				if (val > 59)
					val = 0;
				rtc_time.Seconds = val;
				break;

			default:
				break;
		}
		bsp_rtc_set_time(&rtc_time);    /* ÿ�θ��Ķ���¼һ�� */
		page_time_setting_label_time_update(cur_cfg_item);
	}
	else
		page_time_setting_label_time_update(0);

	/* ������� */
	if (cur_cfg_item == RTC_Item_Year)
	{
		val = (int8_t)(rtc_date.Year + opt);
		if (val < 0)
			val = 99;
		if (val > 99)
			val = 0;
		rtc_date.Year = val;
		bsp_rtc_set_date(&rtc_date);    /* ÿ�θ��Ķ���¼һ�� */
		page_time_setting_label_year_update(1);
	}
	else
		page_time_setting_label_year_update(0);

	/* �������� */
	if (cur_cfg_item >= RTC_Item_Month && cur_cfg_item <= RTC_Item_Date)
	{
		if (cur_cfg_item == 5)
		{
			val = (int8_t)(rtc_date.Month + opt);
			if (val < 0)
				val = 12;
			if (val > 12)
				val = 0;
			rtc_date.Month = val;
		}
		else
		{
			val = (int8_t)(rtc_date.Date + opt);
			if (val < 0)
				val = 31;
			if (val > 31)
				val = 0;
			rtc_date.Date = val;
		}
		bsp_rtc_set_date(&rtc_date);    /* ÿ�θ��Ķ���¼һ�� */
		page_time_setting_label_date_update(cur_cfg_item - 4);
	}
	else
		page_time_setting_label_date_update(0);
}

/**
 * @brief "ʱ������"�����ʼ��
 */
static void page_time_setting_setup()
{
	printf("enter page_time_setting_setup()\n");

	lv_obj_move_foreground(app_win);

	cur_cfg_item = 0;
	cfg_changed = false;

	page_time_setting_title_create();
	printf("test1\n");
	page_time_setting_time_date_disp_create();
	printf("test2\n");
	page_time_setting_time_date_label_create();

	printf("leave page_time_setting_setup()\n\n");
}

/**
 * @brief "ʱ������"�����˳�
 */
static void page_time_setting_exit()
{
	printf("enter page_time_setting_exit()\n");

	lv_obj_clean(app_win);

	printf("leave page_time_setting_exit()\n");
}

/**
 * @brief "ʱ������"�����ϵİ����¼�����
 * @param[in]	btn
 * @param[in]	event
 */
static void page_time_setting_event_handle(void *btn, int event)
{
	if (btn == &g_btn_ok)
	{
		switch (event)
		{
			case ButtonEvent_DoubleClick:
				if (cur_cfg_item == 0 && cfg_changed)
				{
					bsp_rtc_set_time(&rtc_time);
					bsp_rtc_set_date(&rtc_date);
				}
				page_pop(&g_page_manager);
				break;

			case ButtonEvent_SingleClick:
				cur_cfg_item++;
				if (cur_cfg_item < 0)
					cur_cfg_item = 6;
				if (cur_cfg_item > 6)
					cur_cfg_item = 0;
				page_time_setting_time_cfg_val_update(0);
				break;

			default:
				break;
		}
	}

	/* �ϼ����¼�, �Ӽ�ֵ */
	if (event == ButtonEvent_SingleClick || event == ButtonEvent_LongPressHold)
	{
		if (btn == &g_btn_left)
			page_time_setting_time_cfg_val_update(1);
		if (btn == &g_btn_right)
			page_time_setting_time_cfg_val_update(-1);
	}
}


/**
 * @brief ����"ʱ������"���洰��(����)
 * @return
 */
int time_setting_window_create()
{
	if ((app_win = lv_obj_create(lv_scr_act())) == NULL)
		return -1;

	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
	lv_obj_set_size(app_win, APP_WIN_WIDTH(lv_scr_act()), APP_WIN_HEIGHT(lv_scr_act()));
	lv_obj_center(app_win);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, 0); /* ������ʽԲ�ǻ���Ϊ ֱ�� */
	lv_style_set_border_side(&style, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
	lv_style_set_bg_color(&style, lv_color_black());    /* ���ñ���ɫΪ ��ɫ */
	lv_style_set_pad_all(&style, 0);    /* ���ô˴�����������ȫ��padding��СΪ0 */
	lv_obj_add_style(app_win, &style, 0);

	return 0;
}

/**
 * @brief ���˵�����ע�ᵽ���������
 * @return
 */
int page_time_setting_register()
{
	/* �����½���, ��ע�ᵽ��������� */
	if (page_register(&g_page_manager, Page_TimeCfg, page_time_setting_setup, NULL,
	                  page_time_setting_exit, page_time_setting_event_handle) < 0)
	{
		printf("page_time_setting_register() failed\n");

		return -1;
	}

	return 0;
}
