//
// Created by QT on 2023/2/9.
//

#ifndef LVGLWATCH_LVGL_IMG_LIST_H
#define LVGLWATCH_LVGL_IMG_LIST_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"


/**
 * @brief lvgl图片对象
 */
typedef struct lvgl_img
{
	lv_obj_t* img;  /* 图片对象 */
	char* name;     /* 图片名称 */

	struct lvgl_img* prev;
	struct lvgl_img* next;
} lvgl_img_t;


/**
 * @brief 显示图片的窗口
 */
typedef struct lvgl_img_disp_window
{
	lv_obj_t* win;

	lvgl_img_t* img_list;

} lvgl_img_win_t;




/**
 * @brief 创建默认样式的图片显示窗口, 只显示一张图片
 * @param[in]	parent
 * @param[in]	w
 * @param[in]	h
 * @return
 */
lvgl_img_win_t* lvgl_img_win_create(lv_obj_t* parent, lv_coord_t w, lv_coord_t h);


/**
 * @brief 获取lvgl_img_win_t对象中的窗口对象
 * @param[in]	img_win
 * @return
*/
lv_obj_t* lvgl_img_win_get_win(lvgl_img_win_t* img_win);

/**
 * @brief 获取lvgl_img_win_t对象中的图片列表
 * @param img_win
 * @return
 */
lvgl_img_t* lvgl_img_win_get_list(lvgl_img_win_t* img_win);

/**
 * @brief 把图片列表链接到图片显示窗口
 * @param[in]	img_win
 * @param[in]	img_list
 * @return
 */
int lvgl_link_img_list_to_img_win(lvgl_img_win_t* img_win, lvgl_img_t* img_list);



/**
 * @brief 获取lvgl_img_t对象中的图片本体
 * @param[in]	img
 * @return
 */
lv_obj_t* lvgl_img_get_obj(lvgl_img_t* img);

/**
 * @brief 创建img列表
 * @return
 */
lvgl_img_t* lvgl_img_list_create();

/**
 * @brief 添加图片
 * @param[in]	list
 * @param[in]	img
 * @param[in]	name
 * @return
 */
int lvgl_img_list_insert(lvgl_img_t* list, lv_obj_t* img, char* name);

/**
 * @brief 根据名称查找图片对象
 * @param[in]	list
 * @param[in]	name
 * @return
 */
lvgl_img_t* lvgl_img_find(lvgl_img_t* list, char* name);

/**
 * @brief 把指定图片移到前台
 * @param[in]	img_win
 * @param[in]	name
 * @return
*/
int lvgl_img_move_foreground(lvgl_img_win_t* img_win, char* name);





#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_LVGL_IMG_LIST_H
