//
// Created by QT on 2023/1/31.
//

#include "util.h"



/**
 * @brief �����´���
 * @return
 */
lv_obj_t* app_window_create()
{
	return lv_obj_create(lv_scr_act());
}

/**
 * @brief ��ȡ���ڿ��
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
 * @brief ��ȡ���ڸ߶�
 * @param[in]	obj
 * @return
 */
lv_coord_t get_app_window_height(const lv_obj_t* obj)
{
	if (obj == NULL)
		return -1;

	return lv_obj_get_height(obj);
}
