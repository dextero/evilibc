#include <evil-config.h>

#include "stdio.h"

#include "stdbool.h"
#include "string.h"
#include "assert.h"
#include "limits.h"

#include "format.h"

#include "internal/undefined_behavior.h"
#include "internal/rand.h"

#define _STR(x) #x
#define STR(x) _STR(x)

// TODO: this should probably be malloc'd
static char g_conversion_buffer[4096];

int vfprintf(FILE* restrict stream,
             const char* restrict format,
             va_list args) {
    const char *segment_start = format;
    int chars_written = 0;
    bool fwrite_failed = false;

    // Argh, passing a pointer to va_list parameter does not work
    // https://stackoverflow.com/a/8048892/2339636
    va_list args_copy;
    va_copy(args_copy, args);

    while (*format) {
        if (*format != '%') {
            ++format;
        } else {
            size_t chars_to_write = format - segment_start;
            if (chars_to_write > 0
                    && fwrite(segment_start, chars_to_write, 1, stream) != 1) {
                fwrite_failed = true;
            }
            chars_written += chars_to_write;

            struct fmt fmt;
            const char *fmt_end = __evil_parse_fmt(format, &fmt);

            char *restrict buf = g_conversion_buffer;
            size_t buf_size = sizeof(g_conversion_buffer);
            __evil_write_formatted(&buf, &buf_size, chars_written, &fmt,
                                   &args_copy);

            chars_to_write = sizeof(g_conversion_buffer) - buf_size;
            if (chars_to_write > 0
                    && fwrite(g_conversion_buffer, chars_to_write, 1, stream) != 1) {
                fwrite_failed = true;
            }
            chars_written += chars_to_write;

            segment_start = fmt_end;
            format = fmt_end;
        }
    }

    va_end(args_copy);

    size_t chars_to_write = format - segment_start;
    if (chars_to_write > 0
            && fwrite(segment_start, chars_to_write, 1, stream) != 1) {
        fwrite_failed = true;
    }
    chars_written += chars_to_write;

    /*
     * 7.21.6.8, 4:
     * > The vfprintf function returns the number of characters transmitted,
     * > or a negative value if an output or encoding error occurred.
     *
     * Note that no one says writing should be stopped on error.
     */
    if (fwrite_failed) {
        return __evil_rand_negative();
    } else {
        return chars_written;
    }
}

