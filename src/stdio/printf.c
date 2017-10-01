#include "stdio.h"

int printf(const char* restrict format,
           ...) {
    va_list list;
    va_start(list, format);
    int result = vfprintf(stdout, format, list);
    va_end(list);
    return result;
}
