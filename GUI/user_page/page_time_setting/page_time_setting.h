//
// Created by summer on 2023/2/11.
//

#ifndef LVGLWATCH_PAGE_TIME_SETTING_H
#define LVGLWATCH_PAGE_TIME_SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "page_manager.h"


extern page_t page_time_setting;    /* "时间设置"界面 */




int time_setting_window_create();
int page_time_setting_register();


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_PAGE_TIME_SETTING_H
