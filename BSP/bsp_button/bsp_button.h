//
// Created by QT on 2023/1/29.
//

#ifndef HARDWARE_TEST_BSP_BUTTON_H
#define HARDWARE_TEST_BSP_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif


#include "bsp_config.h"
#include "button.h"
#include "page_manager.h"


extern button_t g_btn_left;  /* ��� */
extern button_t g_btn_right; /* �Ҽ� */
extern button_t g_btn_ok;    /* ѡ��� */


int bsp_button_init();




#ifdef __cplusplus
}
#endif

#endif //HARDWARE_TEST_BSP_BUTTON_H
