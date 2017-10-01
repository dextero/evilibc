#include "stdio.h"

#include "stdarg.h"

int fprintf(FILE* restrict stream,
            const char* restrict format,
            ...) {
    va_list list;
    va_start(list, format);
    int result = vfprintf(stream, format, list);
    va_end(list);
    return result;
}
