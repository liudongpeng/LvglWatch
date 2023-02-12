//
// Created by summer on 2023/2/6.
//

#include "bsp_rtc.h"


/**
 * @brief RTC初始化
 * @return
 */
int bsp_rtc_init()
{
	static uint8_t first_programme = 1;

	/* 如果系统是第一次烧录固件 */
	if (first_programme)
	{
		/* 从RTC对象中读取初始设置的日期和时间 */
		do
		{}
		while (0);

		first_programme = 0;
	}
	else
	{
		/* 从备份寄存器中读取时间日期数据 */
		bsp_rtc_date_restore();
		bsp_rtc_time_restore();
	}

	return 0;
}

/**
 * @brief 获取RTC时间
 * @param[out]	time
 */
void bsp_rtc_get_time(RTC_TimeTypeDef* time)
{
	RTC_DateTypeDef tmp;

	HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &tmp, RTC_FORMAT_BIN);
}

/**
 * @brief 获取RTC日期
 * @param[out]	date
 */
void bsp_rtc_get_date(RTC_DateTypeDef* date)
{
	RTC_TimeTypeDef tmp;

	HAL_RTC_GetTime(&hrtc, &tmp, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, date, RTC_FORMAT_BIN);
}

/**
 * @brief 获取RTC星期
 * @param[out]	date
 */
void bsp_rtc_get_week(RTC_DateTypeDef* date)
{
	RTC_TimeTypeDef tmp;

	HAL_RTC_GetTime(&hrtc, &tmp, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, date, RTC_FORMAT_BIN);
}

/**
 * @brief 设置RTC时间
 * @param[in]	time
 */
void bsp_rtc_set_time(RTC_TimeTypeDef* time)
{
	HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);
}

/**
 * @brief 设置RTC日期
 * @param[in]	date
 */
void bsp_rtc_set_date(RTC_DateTypeDef* date)
{
	HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);
}


/**
 * @brief RTC日期备份
 * @return
 */
void bsp_rtc_date_backup()
{
	RTC_DateTypeDef rtc_date;

	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_YEAR, rtc_date.Year);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_MONTH, rtc_date.Month);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_DAY, rtc_date.Date);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_WEEK, rtc_date.WeekDay);
}

/**
 * @brief RTC时间备份
 * @return
 */
void bsp_rtc_time_backup()
{
	RTC_TimeTypeDef rtc_time;

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_HOUR, rtc_time.Hours);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_MIN, rtc_time.Minutes);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKREG_SEC, rtc_time.Seconds);
}

/**
 * @brief RTC日期恢复
 * @return
 */
int bsp_rtc_date_restore()
{
	RTC_DateTypeDef rtc_date;

	rtc_date.Year = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_YEAR);
	rtc_date.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_MONTH);
	rtc_date.Date = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_DAY);
	rtc_date.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_WEEK);

	return HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
}

/**
 * @brief RTC时间恢复
 * @return
 */
int bsp_rtc_time_restore()
{
	RTC_TimeTypeDef rtc_time;

	rtc_time.Hours = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_HOUR);
	rtc_time.Minutes = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_MIN);
	rtc_time.Seconds = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKREG_SEC);

	return HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
}
