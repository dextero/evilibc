#include "stdio.h"

#include "stdbool.h"
#include "string.h"
#include "assert.h"
#include "limits.h"

#include "format.h"

#include "internal/undefined_behavior.h"

#define _STR(x) #x
#define STR(x) _STR(x)

// TODO: this should probably be malloc'd
static char g_conversion_buffer[4096];

int vfprintf(FILE* restrict stream,
             const char* restrict format,
             va_list args) {
    const char *segment_start = format;
    int chars_written = 0;

    // Argh, passing a pointer to va_list parameter does not work
    // https://stackoverflow.com/a/8048892/2339636
    va_list args_copy;
    va_copy(args_copy, args);

    while (*format) {
        if (*format != '%') {
            ++format;
        } else {
            // TODO: errors
            chars_written += fwrite(segment_start,
                                    format - segment_start, 1, stream);

            struct fmt fmt;
            const char *fmt_end = __evil_parse_fmt(format, &fmt);

            char *restrict buf = g_conversion_buffer;
            size_t buf_size = sizeof(g_conversion_buffer);
            __evil_write_formatted(&buf, &buf_size, chars_written, &fmt,
                                   &args_copy);

            // TODO: errors
            chars_written += fwrite(g_conversion_buffer,
                                    sizeof(g_conversion_buffer) - buf_size,
                                    1, stream);

            segment_start = fmt_end;
            format = fmt_end;
        }
    }

    va_end(args_copy);

    if (format != segment_start) {
        chars_written += fwrite(segment_start, format - segment_start, 1,
                                stream);
    }

    return chars_written;
}

