//
// Created by QT on 2023/1/29.
//

#include "page_about.h"
#include "bsp.h"
#include "util.h"
#include <stdio.h>


extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */

page_t page_about;    /* "����"���� */

static lv_obj_t *app_win;   /* "����"���洰��, �����е�Ԫ������������� */

static lv_obj_t *label_title;   /* �����ǩ */
static lv_obj_t *line_title;    /* �������� */

static lv_obj_t *img_logo;  /* ͼƬlogo */

static lv_obj_t *label_author;  /* ���� */
static lv_obj_t *label_fw_info; /* �̼���Ϣ */



static void title_create(const char *text);
static void img_create();
static void label_info_create();

static void page_about_setup();
static void page_about_exit();
static void page_about_event_handle(void *obj, int event);



/**
 * @brief �����������
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
 * @brief ��������ͼƬ
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
 * @brief ������Ϣ��ǩ
 */
static void label_info_create()
{
	/* ���߱�ǩ�ؼ� */
	label_author = lv_label_create(app_win);
	lv_label_set_text_static(label_author, "- DEVELOPER -");
	lv_obj_align(label_author, LV_ALIGN_BOTTOM_MID, 0, -30);

	/* �̼���Ϣ��ǩ�ؼ� */
	label_fw_info = lv_label_create(app_win);
	lv_label_set_text_static(label_fw_info, WATCH_NAME" "WATCH_SW_VER"\n"__DATE__);
	lv_obj_align(label_fw_info, LV_ALIGN_BOTTOM_MID, 0, 0);

	printf("child cnt = %lu\n", lv_obj_get_child_cnt(app_win));
	printf("app_win = %p\n", app_win);
}

/**
 * @brief �����ʼ��
 */
static void page_about_setup()
{
//	lv_obj_move_foreground(app_win);

	title_create("About");
	img_create();
	label_info_create();
}

/**
 * @brief �����˳�
 */
static void page_about_exit()
{
	lv_obj_clean(app_win);
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

	return 0;
}

/**
 * @brief ע����浽���������
 * @return
 */
int page_about_register()
{
	/* �����½���, ��ע�ᵽ��������� */
	page_create(&page_about, page_about_setup, NULL, page_about_exit, page_about_event_handle);
	page_register(&g_page_manager, &page_about);

	return 0;
}
