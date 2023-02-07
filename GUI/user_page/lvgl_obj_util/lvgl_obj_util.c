//
// Created by QT on 2023/1/31.
//

#include "lvgl_obj_util.h"


/**
 * @brief 创建新窗口
 * @return
 */
lv_obj_t *app_window_create()
{
	return lv_obj_create(lv_scr_act());
}

/**
 * @brief 获取窗口宽度
 * @param[in]	obj
 * @return
 */
lv_coord_t update_layout_and_get_obj_width(const lv_obj_t *obj)
{
	if (obj == NULL)
		return 0;

	lv_obj_update_layout(lv_scr_act());

	return lv_obj_get_width(obj);
}

/**
 * @brief 获取窗口高度
 * @param[in]	obj
 * @return
 */
lv_coord_t update_layout_and_get_obj_height(const lv_obj_t *obj)
{
	if (obj == NULL)
		return 0;

	lv_obj_update_layout(lv_scr_act());

	return lv_obj_get_height(obj);
}

/**
 * @brief 更新obj对象所在屏幕, 并获取对象y坐标
 * @param[in]	obj
 * @return
 */
lv_coord_t update_layout_and_get_obj_y(const lv_obj_t *obj)
{
	if (obj == NULL)
		return 0;

	lv_obj_update_layout(lv_scr_act());

	return lv_obj_get_y(obj);
}

/**
 * @brief 给lvgl对象添加动画效果
 * @param[out]	obj
 * @param[in]	anim
 * @param[in]	anim_exec_cb
 * @param[in]	start
 * @param[in]	end
 * @param[in]	time
 * @param[in]	anim_ready_cb
 * @param[in]	anim_path_cb
 */
void lv_obj_add_anim(lv_obj_t *obj, lv_anim_t *anim, lv_anim_exec_xcb_t anim_exec_cb,
                     int start, int end, int time, lv_anim_ready_cb_t anim_ready_cb,
                     lv_anim_path_cb_t anim_path_cb)
{
	lv_anim_t tmp;

	if (obj == NULL)
		return;

	if (anim == NULL)
	{
		lv_anim_init(&tmp);
		anim = &tmp;
	}

	lv_anim_set_exec_cb(anim, anim_exec_cb);
	lv_anim_set_path_cb(anim, anim_path_cb == NULL ? lv_anim_path_ease_out : anim_path_cb);
	lv_anim_set_ready_cb(anim, anim_ready_cb);
	lv_anim_set_values(anim, start, end);
	lv_anim_set_time(anim, time);
	lv_anim_set_var(anim, obj);
	lv_anim_start(anim);
}
