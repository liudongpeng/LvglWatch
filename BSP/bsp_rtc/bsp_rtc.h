//
// Created by summer on 2023/2/6.
//

#ifndef LVGLWATCH_BSP_RTC_H
#define LVGLWATCH_BSP_RTC_H




#include "bsp_config.h"


/**
 * @brief RTC±¸·Ý¼Ä´æÆ÷
 */
#define RTC_BKREG_YEAR  RTC_BKP0R
#define RTC_BKREG_MONTH RTC_BKP1R
#define RTC_BKREG_DAY   RTC_BKP2R
#define RTC_BKREG_WEEK  RTC_BKP3R
#define RTC_BKREG_HOUR  RTC_BKP4R
#define RTC_BKREG_MIN   RTC_BKP5R
#define RTC_BKREG_SEC   RTC_BKP6R


int bsp_rtc_init();

void bsp_rtc_date_backup();
void bsp_rtc_time_backup();

int bsp_rtc_date_restore();
int bsp_rtc_time_restore();




#endif //LVGLWATCH_BSP_RTC_H
