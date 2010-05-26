#ifndef __MY_PRINTF__H
#define __MY_PRINTF__H

#include <stdarg.h>

extern void outbyte (char byte);

#define SIZE_OF_BUF 16

int my_printf(const char *fmt, ...);

#endif
