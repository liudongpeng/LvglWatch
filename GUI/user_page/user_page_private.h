//
// Created by QT on 2023/2/1.
//

#ifndef LVGLWATCH_USER_PAGE_PRIVATE_H
#define LVGLWATCH_USER_PAGE_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif



#define LV_OBJ_ANIM_EXEC_TIME   200 /* lvgl对象动画执行时长 */



/**
 * @brief 各个用户界面的id
 */
enum user_page_id
{
	Page_None = 0,  /* 保留 */

	Page_Dial,      /* 表盘 */
	Page_MainMenu,  /* 主菜单 */

	Page_About,     /* 关于 */

	Page_TimeCfg,   /* 时间设置 */
	Page_BackLight, /* 亮度 */
	Page_StopWatch, /* 停表 */
	Page_Altitude,  /* 海拔高度 */

	Page_Max,   /* 保留 */
};




#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_USER_PAGE_PRIVATE_H
