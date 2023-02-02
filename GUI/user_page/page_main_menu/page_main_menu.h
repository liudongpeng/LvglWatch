//
// Created by QT on 2023/2/1.
//

#ifndef LVGLWATCH_PAGE_MAIN_MENU_H
#define LVGLWATCH_PAGE_MAIN_MENU_H

#ifdef __cplusplus
extern "C" {
#endif


#include "page_manager.h"


extern page_t page_main_menu;    /* "主菜单"界面, 以此来进行界面管理 */


int main_menu_window_create();
int page_main_menu_register();


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_PAGE_MAIN_MENU_H
