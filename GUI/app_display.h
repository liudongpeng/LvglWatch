//
// Created by QT on 2023/2/2.
//

#ifndef LVGLWATCH_DISPLAY_H
#define LVGLWATCH_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief ��ӡ������Ϣʱ, ��ʾ�������ڵ��ļ�, ����, �к�
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
