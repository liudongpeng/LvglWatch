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
 * @brief lvglͼƬ����
 */
typedef struct lvgl_img
{
	lv_obj_t* img;  /* ͼƬ���� */
	char* name;     /* ͼƬ���� */

	struct lvgl_img* prev;
	struct lvgl_img* next;
} lvgl_img_t;


/**
 * @brief ��ʾͼƬ�Ĵ���
 */
typedef struct lvgl_img_disp_window
{
	lv_obj_t* win;

	lvgl_img_t* img_list;

} lvgl_img_win_t;




/**
 * @brief ����Ĭ����ʽ��ͼƬ��ʾ����, ֻ��ʾһ��ͼƬ
 * @param[in]	parent
 * @param[in]	w
 * @param[in]	h
 * @return
 */
lvgl_img_win_t* lvgl_img_win_create(lv_obj_t* parent, lv_coord_t w, lv_coord_t h);


/**
 * @brief ��ȡlvgl_img_win_t�����еĴ��ڶ���
 * @param[in]	img_win
 * @return
*/
lv_obj_t* lvgl_img_win_get_win(lvgl_img_win_t* img_win);

/**
 * @brief ��ȡlvgl_img_win_t�����е�ͼƬ�б�
 * @param img_win
 * @return
 */
lvgl_img_t* lvgl_img_win_get_list(lvgl_img_win_t* img_win);

/**
 * @brief ��ͼƬ�б����ӵ�ͼƬ��ʾ����
 * @param[in]	img_win
 * @param[in]	img_list
 * @return
 */
int lvgl_link_img_list_to_img_win(lvgl_img_win_t* img_win, lvgl_img_t* img_list);



/**
 * @brief ��ȡlvgl_img_t�����е�ͼƬ����
 * @param[in]	img
 * @return
 */
lv_obj_t* lvgl_img_get_obj(lvgl_img_t* img);

/**
 * @brief ����img�б�
 * @return
 */
lvgl_img_t* lvgl_img_list_create();

/**
 * @brief ���ͼƬ
 * @param[in]	list
 * @param[in]	img
 * @param[in]	name
 * @return
 */
int lvgl_img_list_insert(lvgl_img_t* list, lv_obj_t* img, char* name);

/**
 * @brief �������Ʋ���ͼƬ����
 * @param[in]	list
 * @param[in]	name
 * @return
 */
lvgl_img_t* lvgl_img_find(lvgl_img_t* list, char* name);

/**
 * @brief ��ָ��ͼƬ�Ƶ�ǰ̨
 * @param[in]	img_win
 * @param[in]	name
 * @return
*/
int lvgl_img_move_foreground(lvgl_img_win_t* img_win, char* name);





#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_LVGL_IMG_LIST_H
