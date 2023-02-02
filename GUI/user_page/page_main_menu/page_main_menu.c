//
// Created by QT on 2023/2/1.
//

#include "page_main_menu.h"
#include "bsp.h"
#include "lvgl_obj_util.h"
#include "user_page_private.h"
#include <stdio.h>


/**
 * @brief ����ͼ����Դ
 */
LV_IMG_DECLARE(icon_info)
LV_IMG_DECLARE(icon_light)
LV_IMG_DECLARE(icon_mountain)
LV_IMG_DECLARE(icon_stop_watch)
LV_IMG_DECLARE(icon_theme)
LV_IMG_DECLARE(icon_time_cfg)

extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */

page_t page_main_menu;    /* "���˵�"����, �Դ������н������ */

static lv_obj_t *app_win;   /* "���˵�"���洰��, ���ڻ��������ؼ� */

static lv_obj_t *label_title;   /* �����ǩ */
static lv_obj_t *line_title;    /* ����ָ��� */

static lv_obj_t *icon_disp;  /* img�ؼ�, ������ʾͼ�� */
static lv_obj_t *icon_cont; /* ͼ������ */


/**
 * @brief ͼ�������Ϣ����
 */
typedef struct icon
{
	const void *icon_data;  /* ͼ������ */
	const char *icon_name;  /* ͼ������ */
	lv_obj_t *img_icon;     /* img�ؼ�, ������ʾͼ�� */
	uint8_t page_id;        /* ����id */
} icon_t;

/* ͼ�꼯��, ��������ͼ����� */
static icon_t icon_grp[] = {
	{.icon_data = &icon_stop_watch, .icon_name = "StopWatch",   .page_id = Page_StopWatch}, /* ͣ�� */
	{.icon_data = &icon_mountain,   .icon_name = "Altitude",    .page_id = Page_Altitude},  /* ���θ߶� */
	{.icon_data = &icon_light,      .icon_name = "BackLight",   .page_id = Page_BackLight}, /* ���� */
	{.icon_data = &icon_time_cfg,   .icon_name = "TimeCfg",     .page_id = Page_TimeCfg},   /* ʱ������ */
	{.icon_data = &icon_info,       .icon_name = "About",       .page_id = Page_About},     /* ���� */
};


#define ICON_INTERVAL   20  /* ͼ���� */
#define ICON_SIZE       50  /* ͼ���С */

static uint8_t icon_idx_cur;    /* ��ǰͼ������ */

/* ͼ�����������ֵ */
#define ICON_COUNT  (sizeof(icon_grp) / sizeof(icon_t))
#define ICON_INDEX_MAX  (ICON_COUNT - 1)


/**
 * @brief ����ͼ�꼯��
 */
static void icon_grp_create()
{
	/* ����ͼ����ʾ���� */
	icon_disp = lv_obj_create(app_win);
	lv_obj_set_size(icon_disp, ICON_SIZE + 20, APP_WIN_HEIGHT(app_win) - lv_obj_get_y(line_title) - 20);
	lv_obj_align(icon_disp, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	lv_obj_set_scrollbar_mode(icon_disp, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */

	/* ����ͼ����ʾ���ڶ���͸�� */
	static lv_style_t style_icon_disp;
	lv_style_init(&style_icon_disp);
	lv_style_set_opa(&style_icon_disp, LV_OPA_TRANSP);
	lv_obj_add_style(icon_disp, &style_icon_disp, 0);

	/* ��ͼƬ����ƴ������, ͼƬ��������Ļ���»��� */
	icon_cont = lv_obj_create(icon_disp);
	lv_obj_set_size(icon_cont, lv_obj_get_width(icon_disp), (ICON_SIZE + ICON_INTERVAL) * ICON_COUNT);
	lv_obj_set_y(icon_cont, lv_obj_get_height(icon_cont));
	lv_obj_add_style(icon_cont, &style_icon_disp, 0);

	for (int i = 0; i < ICON_COUNT; i++)
	{
		lv_obj_t *icon = lv_img_create(icon_cont);
		icon_grp[i].img_icon = icon;
		lv_img_set_src(icon, icon_grp[i].icon_data);
		lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 0);

		/* ����ÿ��ͼ���ƫ����, ������ͼ����ϵ������ΰڷ� */
		int ofs_y = (ICON_SIZE - lv_obj_get_height(icon)) / 2;
		lv_obj_set_y(icon, (ICON_SIZE + ICON_INTERVAL) * i + ofs_y);
	}
}

/**
 * @brief ����������
 */
static void title_create()
{
	label_title = lv_label_create(app_win);
	/* Ĭ�Ͽ�ѡ�ڶ���ͼ��, �����ڴ˴���ʾ�ڶ���ͼ������� */
	lv_label_set_text_static(label_title, icon_grp[1].icon_name);
	lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_color(&style_label, lv_color_white());
	lv_style_set_text_font(&style_label, &lv_font_montserrat_20);
	lv_obj_add_style(label_title, &style_label, 0);


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
 * @brief �ƶ���ѡ�е�ͼ��
 * @param[in]	idx
 */
static void icon_grp_move_focus(uint8_t idx)
{
	if (idx > ICON_INDEX_MAX)
		return;

	/* �ı������������˵�� */
	lv_label_set_text_static(label_title, icon_grp[idx].icon_name);

	/* ����Ŀ��yλ�� */
	int tar_y = -(ICON_SIZE + ICON_INTERVAL) * (idx - 1);

	/* ����ͼ�곤ͼ��Ŀ��λ�� */
	LB_OBJ_START_ANIM(icon_cont, y, tar_y, LV_OBJ_ANIM_EXEC_TIME);
}

/**
 * @brief �����ƶ�ͼ�꼯��
 * @param[in]	dir	1 �����ƶ�,
 * 					-1 �����ƶ�
 */
static void icon_grp_move(int8_t dir)
{
	/* ����ͼ������ */
	int tmp = icon_idx_cur;
	tmp += dir;
	if (tmp < 0)
		tmp = ICON_INDEX_MAX;
	if (tmp > ICON_INDEX_MAX)
		tmp = 0;
	icon_idx_cur = tmp;

	/* �ƶ�����ǰѡ�е�ͼ�� */
	icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief ����img�ؼ�ʵ�ֹ�����ӰЧ��
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
 * @brief ���˵������ʼ��
 */
static void page_main_menu_setup()
{
	title_create();
	icon_grp_create();
//	img_shadow_create();

//	icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief ���˵������˳�
 */
static void page_main_menu_exit()
{
	/* ͼ��ȫ������ */
	LB_OBJ_START_ANIM(icon_cont, y, lv_obj_get_height(icon_disp) + ICON_SIZE, LV_OBJ_ANIM_EXEC_TIME);


	lv_obj_clean(app_win);
}

/**
 * @brief ���˵������ϵİ����¼�����
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
				/* ˫��ȷ�ϰ�ť, �˳���ǰҳ�� */
				page_pop(&g_page_manager);
				break;

			case ButtonEvent_SingleClick:
				/* ����ȷ�ϰ�ť, ����ѡ�е�ҳ�� */
				id = icon_grp[icon_idx_cur].page_id;
				page_push(&g_page_manager, get_page_by_id(&g_page_manager, id));
				break;

			default:
				break;
		}
	}
	else
	{
		/* �����򳤰� ����(����)����, ͼ�����»�����ѡ�� */
		if (event == ButtonEvent_SingleClick || event == ButtonEvent_LongPressHold)
		{
			if (btn == &g_btn_left)
				icon_grp_move(-1);
			if (btn == &g_btn_right)
				icon_grp_move(1);
		}
	}
}

/**
 * @brief ����"���˵�"���洰��(����)
 * @return
 */
int main_menu_window_create()
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

	printf("main_menu_window_create ok\n");

	return 0;
}

/**
 * @brief ���˵�����ע�ᵽ���������
 * @return
 */
int page_main_menu_register()
{
	/* �����½���, ��ע�ᵽ��������� */
	page_create(&page_main_menu, page_main_menu_setup, NULL, page_main_menu_exit, page_main_menu_event_handle);
	page_register(&g_page_manager, &page_main_menu);

	return 0;
}
