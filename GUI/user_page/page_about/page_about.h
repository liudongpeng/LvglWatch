//
// Created by QT on 2023/1/29.
//

#ifndef HARDWARE_TEST_PAGE_ABOUT_H
#define HARDWARE_TEST_PAGE_ABOUT_H

#ifdef __cplusplus
extern "C" {
#endif


#include "page_manager.h"


extern page_t page_about;	/* "关于"界面 */


#define WATCH_NAME  "WatchX"
#define WATCH_HW_VER    "0.1"
#define WATCH_SW_VER    "0.1"


int about_window_create();
int page_about_register();



#ifdef __cplusplus
}
#endif

#endif //HARDWARE_TEST_PAGE_ABOUT_H
