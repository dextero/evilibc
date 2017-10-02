#include <evil-config.h>

#include "internal/undefined_behavior.h"

#include "stdarg.h"
#include "stdio.h"

void __evil_ub(const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
}

void __evil_idb(const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
}
