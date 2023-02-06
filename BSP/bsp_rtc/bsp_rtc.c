//
// Created by summer on 2023/2/6.
//

#include "bsp_rtc.h"


/**
 * @brief RTC��ʼ��
 * @return
 */
int bsp_rtc_init()
{
	static uint8_t first_programme = 1;

	/* ���ϵͳ�ǵ�һ����¼�̼� */
	if (first_programme)
	{
		/* ��RTC�����ж�ȡ��ʼ���õ����ں�ʱ�� */
		do
		{}
		while (0);

		first_programme = 0;
	}
	else
	{
		/* �ӱ��ݼĴ����ж�ȡʱ���������� */
		bsp_rtc_date_restore();
		bsp_rtc_time_restore();
	}

	return 0;
}

/**
 * @brief RTC���ڱ���
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
 * @brief RTCʱ�䱸��
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
 * @brief RTC���ڻָ�
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
 * @brief RTCʱ��ָ�
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
