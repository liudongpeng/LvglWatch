//
// Created by QT on 2023/1/31.
//

#ifndef LVGLWATCH_UTIL_H
#define LVGLWATCH_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"


/**
 * @brief 获取窗口宽高
 */
#define APP_WIN_WIDTH(obj)  update_layout_and_get_obj_width(obj)
#define APP_WIN_HEIGHT(obj) update_layout_and_get_obj_height(obj)


/**
 * @brief lvgl对象当前y坐标
 */
#define LV_OBJ_CUR_Y    y


/**
 * @brief lvgl对象执行动画
 */
#define LB_OBJ_START_ANIM(obj, attr, tar, time) \
do                                              \
{                                               \
    static lv_anim_t anim;                      \
                                                \
    lv_obj_add_anim(                            \
    (obj),                                      \
    (&anim),                                    \
    ((lv_anim_exec_xcb_t)lv_obj_set_##attr),    \
    (update_layout_and_get_obj_##attr((obj))),  \
    (tar),                                      \
    (time),                                     \
    (NULL),                                     \
    (NULL));                                    \
} while (0);


lv_obj_t *app_window_create();

lv_coord_t update_layout_and_get_obj_width(const lv_obj_t *obj);
lv_coord_t update_layout_and_get_obj_height(const lv_obj_t *obj);

lv_coord_t update_layout_and_get_obj_y(const lv_obj_t *obj);

void lv_obj_add_anim(lv_obj_t *obj, lv_anim_t *anim, lv_anim_exec_xcb_t anim_exec_cb,
                     int start, int end, int time, lv_anim_ready_cb_t anim_ready_cb,
                     lv_anim_path_cb_t anim_path_cb);


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_UTIL_H
