#ifndef __EVILIBC_STDIO_FORMAT_H
#define __EVILIBC_STDIO_FORMAT_H

#include <assert.h>
#include <limits.h>
#include <stdbool.h>

/*
 * 7.21.6.1.15:
 * > The number of characters that can be produced by any single conversion
 * > shall be at least 4095.
 */
#define CONVERSION_LIMIT ((size_t)4095U)

enum flag {
    FLAG_LEFT_JUSTIFY = 1 << 0,
    FLAG_SIGN_REQUIRED = 1 << 1,
    FLAG_SPACE_PREFIX = 1 << 2,
    FLAG_ALTERNATIVE_FORM = 1 << 3,
    FLAG_ZERO_PAD = 1 << 4,
};

enum type {
    TYPE_INVALID,
    TYPE_SIGNED_INT,
    TYPE_UNSIGNED_INT,
    TYPE_UNSIGNED_INT_OCTAL,
    TYPE_UNSIGNED_INT_HEX_LOWER,
    TYPE_UNSIGNED_INT_HEX_UPPER,
    TYPE_DOUBLE_DECIMAL_LOWER,
    TYPE_DOUBLE_DECIMAL_UPPER,
    TYPE_DOUBLE_EXPONENTIAL_LOWER,
    TYPE_DOUBLE_EXPONENTIAL_UPPER,
    TYPE_DOUBLE_ANY_LOWER,
    TYPE_DOUBLE_ANY_UPPER,
    TYPE_DOUBLE_HEX_EXPONENTIAL_LOWER,
    TYPE_DOUBLE_HEX_EXPONENTIAL_UPPER,
    TYPE_INT_AS_UNSIGNED_CHAR,
    TYPE_STRING,
    TYPE_POINTER,
    TYPE_NUM_CHARS_WRITTEN,
    TYPE_PERCENT,
};

enum length {
    LENGTH_DEFAULT,
    LENGTH_CHAR,
    LENGTH_SHORT,
    LENGTH_LONG,
    LENGTH_LONG_LONG,
    LENGTH_INTMAX,
    LENGTH_SIZE,
    LENGTH_PTRDIFF,
    LENGTH_LONG_DOUBLE,
};

#define MISSING (-1)
#define FROM_ARGUMENT (-2)

struct fmt {
    const char *start;
    const char *end;
    int flags;
    int min_width; // negative: either MISSING, DEFAULT or FROM_ARGUMENT
    int precision; // negative: either MISSING, DEFAULT or FROM_ARGUMENT
    enum length length;
    enum type type;
};

static inline int fmt_size(const struct fmt *fmt) {
    assert(fmt->end - fmt->start <= INT_MAX);
    return (int)(fmt->end - fmt->start);
}

static inline bool fmt_has_any_modifiers(const struct fmt *fmt) {
    return fmt->flags != 0
        || fmt->min_width != MISSING
        || fmt->precision != MISSING
        || fmt->length != LENGTH_DEFAULT;
}

/**
 * @param[in]  format Format string, starting with '%'.
 * @param[out] out    Parsed format specifier.
 *
 * @returns pointer to the first character succeeding parsed format specifier.
 *          If @p format does not start with '%' character, @p format is returned
 *          and @p out is left unmodified.
 *      
 */
const char *__evil_parse_fmt(const char *format,
                             struct fmt *out);

/**
 * @param[inout] pbuf                 Pointer to the buffer to put formatted
 *                                    string in. After a successful call, it is
 *                                    advanced so that <c>*pbuf</c> points to
 *                                    the end of formatted string.
 * @param[inout] pbuf_size            Number of bytes available in @p pbuf .
 *                                    After a successful call, it is reduced by
 *                                    the number of bytes actually written to
 *                                    the buffer.
 * @param[in]    chars_written_so_far Number of bytes written so far, used to
 *                                    handle the %n format specifier.
 * @param[in]    fmt                  Parsed format specifier to write.
 * @param[inout] pargs                <c>va_list</c> object used to fetch
 *                                    data from.
 *
 * @returns Number of bytes the formatted data would require if @p pbuf was
 *          big enough. This may be more than @p pbuf_size, which means @p pbuf
 *          was fully filled, but some data was discarded due to it being too
 *          small.
 */
size_t __evil_write_formatted(char *restrict *pbuf,
                              size_t *pbuf_size,
                              int chars_written_so_far,
                              const struct fmt *fmt,
                              va_list *pargs);

#endif // __EVILIBC_STDIO_FORMAT_H
