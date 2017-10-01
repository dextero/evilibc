#include "stdio.h"

#include "stdarg.h"

int snprintf(char* restrict s,
             size_t n,
             const char* restrict format,
             ...) {
    va_list list;
    va_start(list, format);
    int result = vsnprintf(s, n, format, list);
    va_end(list);
    return result;
}
