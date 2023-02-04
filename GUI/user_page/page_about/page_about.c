//
// Created by QT on 2023/1/29.
//

#include "page_about.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "user_page_private.h"
#include <stdio.h>


extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */

page_t page_about;    /* "����"����, �Դ������н������ */

static lv_obj_t *app_win;   /* "����"���洰��, ���ڻ��������ؼ� */

static lv_obj_t *label_title;   /* �����ǩ */
static lv_obj_t *line_title;    /* ����ָ��� */

static lv_obj_t *img_logo;  /* ͼƬlogo */

static lv_obj_t *label_author;  /* ���� */
static lv_obj_t *label_fw_info; /* �̼���Ϣ */


static void page_about_title_create(const char *text);
static void page_about_img_create();
static void page_about_label_info_create();

static void page_about_setup();
static void page_about_exit();
static void page_about_event_handle(void *obj, int event);


/**
 * @brief �����������
 * @param[in]	text
 */
static void page_about_title_create(const char *text)
{
	LV_FONT_DECLARE(HandGotn_20);

	/* ������ǩ */
	label_title = lv_label_create(app_win);
	lv_label_set_text_static(label_title, text);
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	/* ���ñ�ǩ��ʽ */
	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_color(&style_label, lv_color_white());
	lv_style_set_text_font(&style_label, &HandGotn_20);
	lv_obj_add_style(label_title, &style_label, 0);

	/* ������ǩ�µķָ��� */
	static lv_point_t line_points[2];
	line_points[1].x = APP_WIN_WIDTH(app_win) - 1;
	line_title = lv_line_create(app_win);
	lv_line_set_points(line_title, line_points, 2);
	lv_obj_align_to(line_title, label_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

	/* ���÷ָ�����ʽ */
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_color(&style_line, lv_color_make(0xff, 0, 0));
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_line_rounded(&style_line, true);
	lv_obj_add_style(line_title, &style_line, 0);
}

/**
 * @brief ��������ͼƬ
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
 * @brief ������Ϣ��ǩ
 */
static void page_about_label_info_create()
{
	LV_FONT_DECLARE(HandGotn_14);

	/* ���߱�ǩ�ؼ� */
	label_author = lv_label_create(app_win);
	lv_label_set_text_static(label_author, "- DEVELOPER -");
	lv_obj_align(label_author, LV_ALIGN_BOTTOM_MID, 0, -30);
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_text_font(&style, &HandGotn_14);
	lv_obj_add_style(label_author, &style, 0);

	/* �̼���Ϣ��ǩ�ؼ� */
	label_fw_info = lv_label_create(app_win);
	lv_label_set_text_static(label_fw_info, WATCH_NAME" "WATCH_SW_VER"\n"__DATE__);
	lv_obj_align(label_fw_info, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_add_style(label_fw_info, &style, 0);
}

/**
 * @brief �����ʼ��
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
 * @brief �����˳�
 */
static void page_about_exit()
{
	lv_obj_clean(app_win);

	printf("leave page_about_exit()\n\n");
}

/**
 * @brief �����¼�����
 * @param[in]	obj
 * @param[in]	event
 */
static void page_about_event_handle(void *obj, int event)
{
	if (obj == &g_btn_ok)
	{
		if (event == ButtonEvent_SingleClick)
		{
			/* ������ʾ����"����"����, �ڴ˽���ʱ, ֻ��Ҫ���˳��˽���Ĳ��� */
			page_pop(&g_page_manager);
		}
	}
}

/**
 * @brief ����"����"���洰��(����)
 * @return
 */
int about_window_create()
{
	if ((app_win = lv_obj_create(lv_scr_act())) == NULL)
		return -1;

	lv_obj_set_size(app_win, 135, 240);
	lv_obj_center(app_win);
	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, 0); /* ������ʽԲ��Ϊ ֱ�� */
	lv_style_set_border_side(&style, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
	lv_style_set_bg_color(&style, lv_color_black());    /* ���ñ���ɫΪ ��ɫ */
	lv_style_set_pad_all(&style, 0);    /* ������������ȫ��padding��СΪ0 */
	lv_obj_add_style(app_win, &style, 0);

	return 0;
}

/**
 * @brief ע����浽���������
 * @return
 */
int page_about_register()
{
	/* �����½���, ��ע�ᵽ��������� */
	if (page_register(&g_page_manager, Page_About, page_about_setup, NULL,
					  page_about_exit, page_about_event_handle) < 0)
	{
		printf("page_about register failed\n");
		return -1;
	}

	printf("leave page_about_register(), cur_page_exec = %p\n\n", g_page_manager.page_list[Page_About].page_exit);

	return 0;
}
