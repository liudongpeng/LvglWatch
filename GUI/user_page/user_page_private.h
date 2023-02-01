//
// Created by QT on 2023/2/1.
//

#ifndef LVGLWATCH_USER_PAGE_PRIVATE_H
#define LVGLWATCH_USER_PAGE_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief �����û������id
 */
enum user_page_id
{
	Page_None = 0,  /* ���� */

	Page_Dial,      /* ���� */
	Page_MainMenu,  /* ���˵� */
	Page_TimeCfg,   /* ʱ������ */
	Page_BackLight, /* ���� */
	Page_StopWatch, /* ͣ�� */
	Page_Altitude,  /* ���θ߶� */
	Page_About,     /* ���� */

	Page_Max,   /* ���� */
};




#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_USER_PAGE_PRIVATE_H