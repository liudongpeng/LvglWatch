//
// Created by QT on 2023/2/2.
//

#include "debug_print.h"
#include <string.h>


int debug_print_info(const char* fmt, ...)
{
	char buf[4096] = "INFO: ";
	va_list arg;

	va_start(arg, fmt);
	vsprintf(buf + strlen(buf), fmt, arg);
	va_end(arg);

	sprintf(buf + strlen(buf) - 1, "\t%s | %s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
	printf(buf);

	return 0;
}