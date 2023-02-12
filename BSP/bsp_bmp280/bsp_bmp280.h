//
// Created by summer on 2023/2/12.
//

#ifndef LVGLWATCH_BSP_BMP280_H
#define LVGLWATCH_BSP_BMP280_H


#ifdef __cplusplus
extern "C" {
#endif


#include "bsp_config.h"
#include "bmp280.h"


extern bmp280_t g_bmp280;  /* bmp280∂‘œÛ */


int bsp_bmp280_init();


#ifdef __cplusplus
}
#endif


#endif //LVGLWATCH_BSP_BMP280_H
