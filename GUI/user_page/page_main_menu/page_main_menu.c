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

static lv_obj_t *app_win;      /* "���˵�"���洰��, ���ڻ��������ؼ� */
static lv_obj_t *label_title;  /* �����ǩ */
static lv_obj_t *line_title;   /* ����ָ��� */
static lv_obj_t *icon_disp;    /* img�ؼ�, ������ʾͼ�� */
static lv_obj_t *icon_cont;    /* ͼ������ */

static volatile uint8_t icon_idx_cur;   /* ��ǰͼ������ */


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

	lv_obj_update_layout(app_win);
	printf("in title_create(), update_layout_and_get_obj_y(label_title) = %d\n",
		   update_layout_and_get_obj_y(label_title));
	printf("in title_create(), update_layout_and_get_obj_y(line_title) = %d\n",
	       update_layout_and_get_obj_y(line_title));
}

/**
 * @brief ����ͼ�꼯��
 */
static void page_main_menu_icon_grp_create()
{
	/* ����ͼ����ʾ���� */
    icon_disp = lv_obj_create(app_win);
    lv_obj_set_size(icon_disp, ICON_SIZE + 20 + 40,
					APP_WIN_HEIGHT(app_win) - update_layout_and_get_obj_y(line_title) - 20);
    lv_obj_set_scrollbar_mode(icon_disp, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
    lv_obj_set_style_bg_opa(icon_disp, LV_OPA_TRANSP, 0);   /* ���ñ���͸�� */
	lv_obj_align(icon_disp, LV_ALIGN_BOTTOM_MID, 0, 10);

    static lv_style_t style_icon_disp;
    lv_style_init(&style_icon_disp);
    lv_style_set_radius(&style_icon_disp, 0); /* ���ñ߿򻡶� */
//    lv_style_set_border_side(&style_icon_disp, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
	lv_style_set_pad_all(&style_icon_disp, 0);    /* ���ô˴�����������ȫ��padding��СΪ0 */
    lv_obj_add_style(icon_disp, &style_icon_disp, 0);


    /* ��ͼƬ����ƴ������, ͼƬ��������Ļ���»��� */
    icon_cont = lv_obj_create(icon_disp);
    lv_obj_set_style_bg_opa(icon_cont, LV_OPA_TRANSP, 0);   /* ���ö��󱳾�͸�� */
    lv_obj_set_scrollbar_mode(icon_cont, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
    lv_obj_center(icon_cont);
    lv_obj_set_size(icon_cont, /*lv_obj_get_width(icon_disp)*/70, (ICON_SIZE + ICON_INTERVAL) * ICON_COUNT);
    lv_obj_set_y(icon_cont, update_layout_and_get_obj_height(icon_disp));

    static lv_style_t style_icon_cont;
    lv_style_init(&style_icon_cont);
    lv_style_set_border_color(&style_icon_cont, lv_color_make(0xff, 0, 0));
    lv_style_set_radius(&style_icon_cont, 0); /* ���ñ߿򻡶� */
//    lv_style_set_border_side(&style_icon_cont, LV_BORDER_SIDE_NONE);  /* ���ñ߿�λ��Ϊ ����ʾ */
//	lv_style_set_pad_all(&style_icon_cont, 0);    /* ���ô˴�����������ȫ��padding��СΪ0 */
    lv_obj_add_style(icon_cont, &style_icon_cont, 0);

    for (int i = 0; i < ICON_COUNT; i++)
    {
        lv_obj_t* icon = lv_img_create(icon_cont);
        lv_img_set_src(icon, icon_grp[i].icon_data);
        lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 0);

        /* ����ÿ��ͼ���ƫ����, ������ͼ����ϵ������ΰڷ� */
        int ofs_y = (ICON_SIZE - update_layout_and_get_obj_height(icon)) / 2;
        lv_obj_set_y(icon, ((ICON_SIZE + ICON_INTERVAL) * i) + ofs_y);

	    icon_grp[i].img_icon = icon;
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
	if (idx > ICON_INDEX_MAX)
	{
		return;
	}

	/* �ı������������˵�� */
	lv_label_set_text_static(label_title, icon_grp[idx].icon_name);

	/* ����Ŀ��yλ�� */
	int tar_y = -(ICON_SIZE + ICON_INTERVAL) * (idx - 1);
	printf("in icon_grp_move_focus(), idx = %d\n", idx);

	/* ����ͼ�곤ͼ��Ŀ��λ�� */
	LB_OBJ_START_ANIM(icon_cont, y, tar_y, LV_OBJ_ANIM_EXEC_TIME);
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
	lv_obj_move_foreground(app_win);

	page_main_menu_title_create();
	page_main_menu_icon_grp_create();
	page_main_menu_img_shadow_create();

	page_main_menu_icon_grp_move_focus(icon_idx_cur);
}

/**
 * @brief ���˵������˳�
 */
static void page_main_menu_exit()
{
	/* ͼ��ȫ������ */
	LB_OBJ_START_ANIM(icon_cont, y, update_layout_and_get_obj_height(icon_disp) + ICON_SIZE, LV_OBJ_ANIM_EXEC_TIME);

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
				printf("in main_menu, cur_icon_idx = %d\n", icon_idx_cur);
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

	lv_obj_set_size(app_win, APP_WIN_WIDTH(lv_scr_act()), APP_WIN_HEIGHT(lv_scr_act()));
	lv_obj_set_scrollbar_mode(app_win, LV_SCROLLBAR_MODE_OFF);  /* �ر�ˮƽ����ֱ������ */
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
