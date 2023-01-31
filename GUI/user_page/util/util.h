//
// Created by QT on 2023/1/31.
//

#ifndef LVGLWATCH_UTIL_H
#define LVGLWATCH_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"


#define APP_WIN_WIDTH(obj)  get_app_window_width(obj)
#define APP_WIN_HEIGHT(obj) get_app_window_height(obj)



lv_obj_t* app_window_create();

lv_coord_t get_app_window_width(const lv_obj_t* obj);
lv_coord_t get_app_window_height(const lv_obj_t* obj);


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_UTIL_H
