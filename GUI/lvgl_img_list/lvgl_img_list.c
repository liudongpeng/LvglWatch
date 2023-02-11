//
// Created by QT on 2023/2/9.
//

#include "lvgl_img_list.h"

#include <stdio.h>


static lvgl_img_t* lvgl_img_create(lv_obj_t* img, char* name);
static lv_obj_t* lvgl_img_single_disp_win_create(lv_obj_t* parent, lv_coord_t w, lv_coord_t h);



/**
 * @brief 创建img对象
 * @param[in]	img
 * @param[in]	name
 * @return
 */
static lvgl_img_t* lvgl_img_create(lv_obj_t* img, char* name)
{
	lvgl_img_t* img_desc;

	if (img == NULL)
		return NULL;

	if ((img_desc = lv_mem_alloc(sizeof(lvgl_img_t))) == NULL)
		return NULL;

	img_desc->img = img;
	img_desc->name = name;

	return img_desc;
}



/**
 * @brief 创建默认样式的图片显示窗口, 只显示一张图片
 * @param[in]	parent
 * @param[in]	w
 * @param[in]	h
 * @return
 */
static lv_obj_t* lvgl_img_single_disp_win_create(lv_obj_t* parent, lv_coord_t w, lv_coord_t h)
{
	lv_obj_t* win = lv_obj_create(parent);
	lv_obj_set_scrollbar_mode(win, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(win, w, h);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_border_width(&style, 0);
	lv_style_set_radius(&style, 0);
	lv_style_set_bg_color(&style, lv_color_black());
	lv_style_set_pad_all(&style, 0);
	lv_obj_add_style(win, &style, 0);

	return win;
}


/**
 * @brief 创建默认样式的图片显示窗口, 只显示一张图片
 * @param[in]	parent
 * @param[in]	w
 * @param[in]	h
 * @return
 */
lvgl_img_win_t* lvgl_img_win_create(lv_obj_t* parent, lv_coord_t w, lv_coord_t h)
{
	lvgl_img_win_t* img_wim;

	if (parent == NULL || w <= 0 || h <= 0)
		return NULL;

	if ((img_wim = lv_mem_alloc(sizeof(lvgl_img_win_t))) == NULL)
		return NULL;
	lv_memset_00(img_wim, sizeof(lvgl_img_win_t));

	if ((img_wim->win = lvgl_img_single_disp_win_create(parent, w, h)) == NULL)
		return NULL;

	if ((img_wim->img_list = lvgl_img_list_create()) == NULL)
		return NULL;

	return img_wim;
}


/**
 * @brief 获取lvgl_img_win_t对象中的窗口对象
 * @param[in]	img_win
 * @return
*/
lv_obj_t* lvgl_img_win_get_win(lvgl_img_win_t* img_win)
{
	if (img_win == NULL)
		return NULL;

	return img_win->win;
}

/**
 * @brief 获取lvgl_img_win_t对象中的图片列表
 * @param img_win
 * @return
 */
lvgl_img_t* lvgl_img_win_get_list(lvgl_img_win_t* img_win)
{
	if (img_win == NULL)
		return NULL;

	return img_win->img_list;
}

/**
 * @brief 把图片裂变链接到图片显示窗口
 * @param[in]	img_win
 * @param[in]	img_list
 * @return
 */
int lvgl_link_img_list_to_img_win(lvgl_img_win_t* img_win, lvgl_img_t* img_list)
{
	if (img_win == NULL || img_list == NULL)
		return -1;

	img_win->img_list = img_list;

	return 0;
}



/**
 * @brief 获取lvgl_img_t对象中的图片本体
 * @param[in]	img
 * @return
 */
lv_obj_t* lvgl_img_get_obj(lvgl_img_t* img)
{
	if (img == NULL)
		return NULL;

	return img->img;
}

/**
 * @brief 创建img列表
 * @return
 */
lvgl_img_t* lvgl_img_list_create()
{
	lvgl_img_t* list;

	if ((list = lv_mem_alloc(sizeof(lvgl_img_t))) == NULL)
		return NULL;

	lv_memset_00(list, sizeof(lvgl_img_t));

	return list;
}

/**
 * @brief 添加图片
 * @param[in]	list
 * @param[in]	img
 * @param[in]	name
 * @return
 */
int lvgl_img_list_insert(lvgl_img_t* list, lv_obj_t* img, char* name)
{
	lvgl_img_t* img_desc;

	if (img == NULL)
		return -1;

	if ((img_desc = lvgl_img_create(img, name)) == NULL)
		return -2;

	img_desc->next = list->next;
	list->next = img_desc;

	return 0;
}

/**
 * @brief 根据名称查找图片对象
 * @param[in]	list
 * @param[in]	name
 * @return
 */
lvgl_img_t* lvgl_img_find(lvgl_img_t* list, char* name)
{
	lvgl_img_t* cur;

	if (list == NULL)
		return NULL;

	/* 跳过头节点开始寻找 */
	cur = list->next;
	while (cur != NULL)
	{
		if (strcmp(cur->name, name) == 0)
			return cur;

		cur = cur->next;
	}

	return NULL;
}

/**
 * @brief 把指定图片移到前台
 * @param[in]	img_win
 * @param[in]	name
 * @return
*/
int lvgl_img_move_foreground(lvgl_img_win_t* img_win, char* name)
{
	lvgl_img_t* img;

	if (img_win == NULL)
		return -1;

	if ((img = lvgl_img_find(lvgl_img_win_get_list(img_win), name)) == NULL)
		return -2;

	lv_obj_move_foreground(lvgl_img_get_obj(img));

	return 0;
}

