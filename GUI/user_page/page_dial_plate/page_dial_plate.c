//
// Created by QT on 2023/1/29.
//

#include "page_dial_plate.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "user_page_private.h"


extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */

page_t page_dial;   /* "����"���� */

static lv_obj_t *app_win;       /* "���˵�"���洰��, ���ڻ��������ؼ� */
static lv_obj_t* img_bg;        /* ����ͼƬ */
static lv_obj_t* label_bmp280;  /* ��ѹ����Ϣ */
static lv_obj_t* label_batt;    /* �����Ϣ */
static lv_obj_t* label_date;    /* ���� */
static lv_obj_t* time_cont;     /* ʱ������ */
static lv_obj_t* led_sec[2];    /* ��ָʾ�� */


static lv_obj_t* icon_run;      /* �˶�ͼ�� */
static lv_obj_t* label_step_cnt;    /* �Ʋ����� */

static lv_timer_t* timer_time_update;       /* ʱ����¶�ʱ�� */
static lv_timer_t* timer_state_bar_update;  /* ״̬�����¶�ʱ�� */



static void page_dial_bg_create();
static void page_dial_state_bar_create();
static void page_dial_label_date_create();


static void page_dial_setup();
static void page_dial_exit();
static void page_dial_event_handle(void* obj, int event);




/**
 * @brief �������̱���
 */
static void page_dial_bg_create()
{
	LV_IMG_DECLARE(img_background);

	img_bg = lv_img_create(app_win);
	lv_img_set_src(img_bg, &img_background);
	lv_obj_center(img_bg);
}

/**
 * @brief ����״̬��
 */
static void page_dial_state_bar_create()
{
	label_bmp280 = lv_label_create(app_win);
	lv_label_set_text(label_bmp280, "-- C -- kPa");
	lv_obj_align(label_bmp280, LV_ALIGN_TOP_LEFT, 0, 0);
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_white());
	lv_obj_add_style(label_bmp280, &style, 0);

	label_batt = lv_label_create(app_win);
	lv_label_set_text(label_bmp280, "#FFFFFF "LV_SYMBOL_BATTERY_EMPTY"#");
	lv_label_set_recolor(label_batt, true);
	lv_obj_align(label_batt, LV_ALIGN_TOP_RIGHT, 0, 0);

	/* ����״̬����������, �ö�ʱ����ʱ���� */
	timer_state_bar_update = lv_timer_create(NULL, 2 * 1000, NULL);
}

/**
 * @brief �������ڱ�ǩ
 */
static void page_dial_label_date_create()
{
	label_date = lv_label_create(app_win);
	lv_label_set_text(label_date, "01#FF0000 /#01 MON");
	lv_label_set_recolor(label_batt, true);
	lv_obj_align(label_date, LV_ALIGN_TOP_MID, 0, 30);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_white());
	lv_obj_add_style(label_date, &style, 0);
}

/**
 * @brief ����ʱ���ǩ
 */
static void page_dial_label_time_create()
{
}


/**
 * @brief �����ʼ��
 */
static void page_dial_setup()
{
	page_dial_bg_create();
	page_dial_state_bar_create();
	page_dial_label_date_create();
}

/**
 * @brief �����˳�
 */
static void page_dial_exit()
{

	lv_obj_clean(app_win);
}

/**
 * @brief �����¼�����
 * @param[in]	obj
 * @param[in]	event
 */
static void page_dial_event_handle(void *obj, int event)
{
	/* �а�����������ʱ, �������˵� */
	if (event == ButtonEvent_SingleClick || event == ButtonEvent_LongPressHold)
	{
		page_push(&g_page_manager, get_page_by_id(&g_page_manager, Page_MainMenu));
	}
}

/**
 * @brief ����"����"���洰��(����)
 * @return
 */
int dial_window_create()
{
	if ((app_win = lv_obj_create(lv_scr_act())) == NULL)
		return -1;

	lv_obj_set_size(app_win, 135, 240);
	lv_obj_center(app_win);
	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, 0); /* ������ʽԲ�ǻ���Ϊ ֱ�� */
	lv_style_set_border_side(&style, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
	lv_style_set_bg_color(&style, lv_color_black());    /* ���ñ���ɫΪ ��ɫ */
	lv_obj_add_style(app_win, &style, 0);

	return 0;
}

/**
 * @brief ע����浽���������
 * @return
 */
int page_dial_register()
{
	/* �����½��� */
	page_create(&page_dial, page_dial_setup, NULL, page_dial_exit, page_dial_event_handle);

	page_register(&g_page_manager, &page_dial);

	return 0;
}
