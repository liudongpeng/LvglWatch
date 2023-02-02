//
// Created by QT on 2023/2/2.
//

#ifndef LVGLWATCH_DEBUG_PRINT_H
#define LVGLWATCH_DEBUG_PRINT_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdarg.h>


#define DEBUG_INFO(msg) debug_print_info(msg, ...)

int debug_print_info(const char* fmt, ...);



#ifdef __cplusplus
}
#endif

#endif //LVGLWATCH_DEBUG_PRINT_H
