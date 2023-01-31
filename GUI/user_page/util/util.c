//
// Created by QT on 2023/1/31.
//

#include "util.h"



/**
 * @brief 创建新窗口
 * @return
 */
lv_obj_t* app_window_create()
{
	return lv_obj_create(lv_scr_act());
}

/**
 * @brief 获取窗口宽度
 * @param[in]	obj
 * @return
 */
lv_coord_t get_app_window_width(const lv_obj_t* obj)
{
	if (obj == NULL)
		return -1;

	return lv_obj_get_width(obj);
}

/**
 * @brief 获取窗口高度
 * @param[in]	obj
 * @return
 */
lv_coord_t get_app_window_height(const lv_obj_t* obj)
{
	if (obj == NULL)
		return -1;

	return lv_obj_get_height(obj);
}
