#include "stdio.h"

#include "stdarg.h"
#include "stdint.h"

int sprintf(char* restrict s,
            const char* restrict format,
            ...) {
    va_list list;
    va_start(list, format);
    int result = vsnprintf(s, SIZE_MAX, format, list);
    va_end(list);
    return result;
}
