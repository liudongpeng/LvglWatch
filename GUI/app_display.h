//
// Created by QT on 2023/2/2.
//

#ifndef LVGLWATCH_DISPLAY_H
#define LVGLWATCH_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief 打印错误信息时, 显示错误所在的文件, 函数, 行号
 *
 * @example printf("malloc failed"DEBUG_ERROR_INFO);
 */
#define DEBUG_ERROR_INFO \
	"\t%s | %s(%d)\n", __FILE__, __FUNCTION__, __LINE__



int app_display_init();
void app_display_update();


#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_DISPLAY_H
