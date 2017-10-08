#include <evil-config.h>

#include "stdarg.h"
#include "stdio.h"

#include "evilibc.h"

#include "internal/undefined_behavior.h"

evil_ub_handler_t *__evil_ub_handler = NULL;
void *__evil_ub_handler_data = NULL;

evil_ub_handler_t *__evil_idb_handler = NULL;
void *__evil_idb_handler_data = NULL;

void __evil_ub(const char* fmt, ...)
{
    if (!__evil_ub_handler) {
        __evil_ub_handler = (evil_ub_handler_t *)vfprintf;
        __evil_ub_handler_data = stderr;
    }

    va_list list;
    va_start(list, fmt);
    __evil_ub_handler(__evil_ub_handler_data, fmt, list);
    va_end(list);
}

void __evil_idb(const char* fmt, ...)
{
    if (!__evil_idb_handler) {
        __evil_idb_handler = (evil_idb_handler_t *)vfprintf;
        __evil_idb_handler_data = stderr;
    }

    va_list list;
    va_start(list, fmt);
    __evil_idb_handler(__evil_idb_handler, fmt, list);
    va_end(list);
}
