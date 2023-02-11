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
 * @brief ����ͼ����Դ
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


extern page_manager_t g_page_manager;    /* ���������, ��freertos.c�ļ��ж��� */
page_t page_main_menu;    /* "���˵�"����, �Դ������н������ */

static lv_obj_t *app_win;      /* "���˵�"���洰��, ���ڻ��������ؼ� */
static lv_obj_t *label_title;  /* �����ǩ */
static lv_obj_t *line_title;   /* ����ָ��� */
static lv_obj_t *icon_disp;    /* img�ؼ�, ������ʾͼ�� */
static lv_obj_t *icon_cont;    /* ͼ������ */

static uint8_t icon_idx_cur;   /* ��ǰͼ������ */


/**
 * @brief ͼ�������Ϣ����
 */
typedef struct icon_info
{
	const void *icon_data_normal;  /* ����ͼ������ */
	const void *icon_data_select;  /* ѡ��ͼ������ */

	const char *icon_name;  /* ͼ������ */

	lvgl_img_win_t *icon;   /* ͼ�괰�� */

	uint8_t page_id;        /* ����id */
} icon_t;

/* ͼ�꼯��, ��������ͼ����� */
static icon_t icon_grp[] = {
	{.icon_data_normal = &icon_stopwatch_white_64x64, .icon_data_select = &icon_stopwatch_red_64x64,
		.icon_name = "StopWatch", .page_id = Page_StopWatch},   /* ͣ�� */
	{.icon_data_normal = &icon_mountain_white_64x64, .icon_data_select = &icon_mountain_red_64x64,
		.icon_name = "Altitude", .page_id = Page_Altitude}, /* ���θ߶� */
	{.icon_data_normal = &icon_brightness_white_64x64, .icon_data_select = &icon_brightness_red_64x64,
		.icon_name = "BackLight", .page_id = Page_BackLight},   /* ���� */
	{.icon_data_normal = &icon_setting_white_64x64, .icon_data_select = &icon_setting_red_64x64,
		.icon_name = "TimeConfig", .page_id = Page_TimeCfg},   /* ʱ������ */
	{.icon_data_normal = &icon_butterfly_white_64x64, .icon_data_select = &icon_butterfly_red_64x64,
		.icon_name = "Theme", .page_id = Page_Theme},   /* ʱ������ */
	{.icon_data_normal = &icon_about_white_64x64, .icon_data_select = &icon_about_red_64x64,
		.icon_name = "About", .page_id = Page_About},   /* ���� */
};

#define ICON_INTERVAL   20  /* ͼ���� */
#define ICON_SIZE       64  /* ͼ���С */
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
 * @brief ����������
 */
static void page_main_menu_title_create()
{
	LV_FONT_DECLARE(HandGotn_20);

	/* ���������ǩ */
	label_title = lv_label_create(app_win);
	/* Ĭ�Ͽ�ѡ�ڶ���ͼ��, �����ڴ˴���ʾ�ڶ���ͼ������� */
	lv_label_set_text_static(label_title, icon_grp[1].icon_name);
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
 * @brief ����ͼ�꼯��
 */
static void page_main_menu_icon_grp_create()
{
	/* ����ͼ����ʾ���� */
	icon_disp = lv_obj_create(app_win);
	lv_obj_set_size(icon_disp, ICON_SIZE + 20,
	                APP_WIN_HEIGHT(app_win) - update_layout_and_get_obj_y(line_title) - 20);
	lv_obj_set_scrollbar_mode(icon_disp, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
	lv_obj_set_style_bg_opa(icon_disp, LV_OPA_TRANSP, 0);   /* ���ñ���͸�� */
	lv_obj_align_to(icon_disp, line_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

	static lv_style_t style_icon_disp;
	lv_style_init(&style_icon_disp);
	lv_style_set_pad_all(&style_icon_disp, 0);      /* ���ô˴�����������ȫ��padding��СΪ0 */
	lv_style_set_radius(&style_icon_disp, 0);       /* ���ñ߿򻡶� */
	lv_style_set_border_width(&style_icon_disp, 0); /* ���ñ߿���Ϊ0 */
	lv_style_set_border_side(&style_icon_disp, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
	lv_obj_add_style(icon_disp, &style_icon_disp, 0);


	/* ��ͼƬ����ƴ������, ͼƬ��������Ļ���»��� */
	icon_cont = lv_obj_create(icon_disp);
	lv_obj_set_style_bg_opa(icon_cont, LV_OPA_TRANSP, 0);   /* ���ö��󱳾�͸�� */
	lv_obj_set_scrollbar_mode(icon_cont, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
	lv_obj_set_size(icon_cont, update_layout_and_get_obj_width(icon_disp),
	                (ICON_SIZE + ICON_INTERVAL) * ICON_COUNT - ICON_INTERVAL);
	lv_obj_align_to(icon_cont, icon_disp, LV_ALIGN_TOP_MID, 0, 0);

	static lv_style_t style_icon_cont;
	lv_style_init(&style_icon_cont);
	lv_style_set_pad_all(&style_icon_cont, 0);    /* ���ô˴�����������ȫ��padding��СΪ0 */
	lv_style_set_border_color(&style_icon_cont, lv_color_make(0xff, 0, 0));
	lv_style_set_radius(&style_icon_cont, 0); /* ���ñ߿򻡶� */
	lv_style_set_border_width(&style_icon_cont, 0); /* ���ñ߿���Ϊ0 */
	lv_style_set_border_side(&style_icon_cont, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
	lv_obj_add_style(icon_cont, &style_icon_cont, 0);

	/* ��icon_cont�����δ���ÿ��ͼ�괰��, ÿ��������������ͼ��, ��Ϊ��ɫ�ͺ�ɫ������ʽ */
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

		/* ��normalͼ���Ƶ�ǰ̨ */
		lv_obj_move_foreground(lvgl_img_get_obj(lvgl_img_find(lvgl_img_win_get_list(img_wim), "normal")));

		/* ����ÿ��ͼ����ʾ���ڵ�λ�� */
		lv_obj_set_y(lvgl_img_win_get_win(img_wim), (ICON_SIZE + ICON_INTERVAL) * i);

		icon_grp[i].icon = img_wim;
	}
}

/**
 * @brief ����img�ؼ�ʵ�ֹ�����ӰЧ��
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
 * @brief �ƶ���ѡ�е�ͼ��
 * @param[in]	idx
 */
static void page_main_menu_icon_grp_move_focus(uint8_t idx)
{
	static uint8_t last_idx;

	if (idx > ICON_INDEX_MAX)
		return;

	/* �ı������������˵�� */
	lv_label_set_text_static(label_title, icon_grp[idx].icon_name);

	/* ���ϴ�ѡ���ͼ�껻Ϊ��Ӧ�İ�ɫͼƬ */
	lv_obj_move_foreground(lvgl_img_get_obj(
		lvgl_img_find(lvgl_img_win_get_list(icon_grp[last_idx].icon), "normal")));

	/* �ѵ�ǰͼ��ͼƬ��Ϊ��Ӧ��Ϊ��ɫͼƬ */
	lv_obj_move_foreground(lvgl_img_get_obj(
		lvgl_img_find(lvgl_img_win_get_list(icon_grp[idx].icon), "select")));

	/* ����Ŀ��yλ�� */
	int tar_y = -(ICON_SIZE + ICON_INTERVAL) * (idx - 1) - ICON_INTERVAL;

	/* ����ͼ�곤ͼ��Ŀ��λ�� */
	LV_OBJ_START_ANIM(icon_cont, y, tar_y, LVGL_OBJ_ANIM_EXEC_TIME);

	/* ��¼�ϴε�ͼ������ */
	last_idx = idx;
}

/**
 * @brief �����ƶ�ͼ�꼯��
 * @param[in]	dir	1 �����ƶ�,
 * 					-1 �����ƶ�
 */
static void page_main_menu_icon_grp_move(int8_t dir)
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
	page_main_menu_icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief ���˵������ʼ��
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
 * @brief ���˵������˳�
 */
static void page_main_menu_exit()
{
	printf("enter page_main_menu_exit()\n");

	/* ͼ��ȫ������ */
	LV_OBJ_START_ANIM(icon_cont, y, update_layout_and_get_obj_height(icon_disp) + ICON_SIZE, LVGL_OBJ_ANIM_EXEC_TIME);

	/* ������ʱ, ��ʱ�ڼ䱣�ֽ���ˢ�� */
//	lvgl_page_delay(LVGL_OBJ_ANIM_EXEC_TIME);

	lv_obj_clean(app_win);

	printf("leave page_main_menu_exit()\n\n");
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
				if (g_page_manager.page_list[id].page_setup != NULL)
					page_push(&g_page_manager, id);
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
				page_main_menu_icon_grp_move(-1);
			if (btn == &g_btn_right)
				page_main_menu_icon_grp_move(1);
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
int page_main_menu_register()
{
	/* �����½���, ��ע�ᵽ��������� */
	if (page_register(&g_page_manager, Page_MainMenu, page_main_menu_setup, NULL,
	                  page_main_menu_exit, page_main_menu_event_handle) < 0)
	{
		printf("page_main_menu_register() failed\n");

		return -1;
	}

	return 0;
}
