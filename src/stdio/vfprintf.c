#include "stdio.h"

#include "stdbool.h"
#include "string.h"
#include "assert.h"
#include "limits.h"

#include "internal/undefined_behavior.h"

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
#define DEFAULT (-2)
#define FROM_ARGUMENT (-3)

struct fmt {
    const char *start;
    const char *end;
    int flags;
    int min_width; // negative: either MISSING, DEFAULT or FROM_ARGUMENT
    int precision; // negative: either MISSING, DEFAULT or FROM_ARGUMENT
    enum length length;
    enum type type;
};

static int fmt_size(const struct fmt *fmt) {
    assert(fmt->end - fmt->start <= INT_MAX);
    return (int)(fmt->end - fmt->start);
}

static const char *parse_flags(const char *fmt,
                               int *out_flags) {
    bool flags_done = false;
    while (!flags_done) {
        switch (*fmt) {
        case '-':
            *out_flags |= FLAG_LEFT_JUSTIFY;
            break;
        case '+':
            *out_flags |= FLAG_SIGN_REQUIRED;
            break;
        case ' ':
            *out_flags |= FLAG_SPACE_PREFIX;
            break;
        case '#':
            *out_flags |= FLAG_ALTERNATIVE_FORM;
            break;
        case '0':
            *out_flags |= FLAG_ZERO_PAD;
            break;
        default:
            flags_done = true;
            break;
        }

        ++fmt;
    }

    return fmt - 1;
}

static const char *parse_width(const char *fmt,
                               int *out_size) {
    if (*fmt == '*') {
        *out_size = FROM_ARGUMENT;
        return fmt + 1;
    } else if (*fmt < '0' || '0' < *fmt) {
        *out_size = MISSING;
        return fmt;
    }

    while ('0' <= *fmt && *fmt <= '9') {
        assert(*out_size < (INT_MAX - (*fmt - '0')) / 10);

        *out_size *= 10;
        *out_size += *fmt - '0';
    }

    return fmt;
}

static const char *parse_precision(const char *fmt,
                                   int *out_precision) {
    if (*fmt != '.') {
        *out_precision = MISSING;
        return fmt;
    }

    *out_precision = 0;
    const char *result = parse_width(fmt + 1, out_precision);

    if (*out_precision == DEFAULT) {
        /*
         * 7.21.6.1.4:
         * > if only the period is specified, the precision is taken as zero.
         */
        *out_precision = 0;
    }

    return result;
}
                                       
static const char *parse_length(const char *fmt,
                                enum length *out_length) {
    switch (fmt[0]) {
    case 'h':
        if (fmt[1] == 'h') {
            *out_length = LENGTH_CHAR;
            return fmt + 2;
        } else {
            *out_length = LENGTH_SHORT;
            return fmt + 1;
        }
    case 'l':
        if (fmt[1] == 'l') {
            *out_length = LENGTH_LONG_LONG;
            return fmt + 2;
        } else {
            *out_length = LENGTH_LONG;
            return fmt + 1;
        }
    case 'j':
        *out_length = LENGTH_INTMAX;
        return fmt + 1;
    case 'z':
        *out_length = LENGTH_SIZE;
        return fmt + 1;
    case 't':
        *out_length = LENGTH_PTRDIFF;
        return fmt + 1;
    case 'L':
        *out_length = LENGTH_LONG_DOUBLE;
        return fmt + 1;
    default:
        *out_length = LENGTH_DEFAULT;
        return fmt;
    }
}

static const char *parse_type(const char *fmt,
                              enum type *out_type) {
    switch (*fmt) {
    case 'd':
    case 'i':
        *out_type = TYPE_SIGNED_INT;
        break;
    case 'o':
        *out_type = TYPE_UNSIGNED_INT_OCTAL;
        break;
    case 'u':
        *out_type = TYPE_UNSIGNED_INT;
        break;
    case 'x':
        *out_type = TYPE_UNSIGNED_INT_HEX_LOWER;
        break;
    case 'X':
        *out_type = TYPE_UNSIGNED_INT_HEX_UPPER;
        break;
    case 'f':
        *out_type = TYPE_DOUBLE_DECIMAL_LOWER;
        break;
    case 'F':
        *out_type = TYPE_DOUBLE_DECIMAL_UPPER;
        break;
    case 'e':
        *out_type = TYPE_DOUBLE_EXPONENTIAL_LOWER;
        break;
    case 'E':
        *out_type = TYPE_DOUBLE_EXPONENTIAL_UPPER;
        break;
    case 'g':
        *out_type = TYPE_DOUBLE_ANY_LOWER;
        break;
    case 'G':
        *out_type = TYPE_DOUBLE_ANY_UPPER;
        break;
    case 'a':
        *out_type = TYPE_DOUBLE_HEX_EXPONENTIAL_LOWER;
        break;
    case 'A':
        *out_type = TYPE_DOUBLE_HEX_EXPONENTIAL_UPPER;
        break;
    case 'c':
        *out_type = TYPE_INT_AS_UNSIGNED_CHAR;
        break;
    case 's':
        *out_type = TYPE_STRING;
        break;
    case 'p':
        *out_type = TYPE_POINTER;
        break;
    case 'n':
        *out_type = TYPE_NUM_CHARS_WRITTEN;
        break;
    case '%':
        *out_type = TYPE_PERCENT;
        break;
    default:
        *out_type = TYPE_INVALID;
        break;
    }

    return fmt + 1;
}

static const char *parse_fmt(const char *format,
                             struct fmt *out) {
    memset(out, 0, sizeof(*out));

    out->start = format;
    format = parse_flags(format, &out->flags);
    format = parse_width(format, &out->min_width);
    format = parse_precision(format, &out->precision);
    format = parse_length(format, &out->length);
    format = parse_type(format, &out->type);
    out->end = format;

    return format;
}

/*
 * 7.21.6.1.15:
 * > The number of characters that can be produced by any single conversion
 * > shall be at least 4095.
 *
 * TODO: this should probably be malloc'd
 */
static char g_conversion_buffer[4096];

static long fetch_width(struct fmt *fmt,
                        va_list *args) {
    long min_width;

    switch (fmt->min_width) {
    case FROM_ARGUMENT:
        {
            int width_arg = va_arg(*args, int);
            if (width_arg < 0) {
                fmt->flags |= FLAG_LEFT_JUSTIFY;
                min_width = -(long)width_arg;
            } else {
                min_width = (long) width_arg;
            }
        }
        break;
    case MISSING:
        min_width = MISSING;
        break;
    case DEFAULT:
        min_width = 0;
        break;
    default:
        min_width = fmt->min_width;
        break;
    }

    if (min_width >= (long)sizeof(g_conversion_buffer)) {
        __evil_idb("printf does not need to support writing more than %zu bytes"
                   " in a single %%* format specifier (%ld passed as min width)",
                   sizeof(g_conversion_buffer) - 1, min_width);
        min_width = (long)(sizeof(g_conversion_buffer) - 1);
    }

    assert(min_width == MISSING || min_width >= 0);
    return min_width;
}

static int print_s(FILE *stream,
                   const struct fmt *fmt_const,
                   va_list *args) {
    struct fmt fmt = *fmt_const;
    long min_width = fetch_width(&fmt, args);

    const char *str = va_arg(*args, const char *);
    if (!str) {
        __evil_ub("passing NULL as printf argument for %%s is UB");
        str = "(null)";
    }

    size_t str_size;
    if (min_width == MISSING) {
        str_size = strlen(str);
    } else {
        assert(min_width >= 0);
        // str may have a null-terminator early
        str_size = min_width;
        for (long i = 0; i < min_width; ++i) {
            if (str[i] == '\0') {
                str_size = (size_t)i;
                break;
            }
        }
    }

    if (str_size > sizeof(g_conversion_buffer) - 1) {
        __evil_idb("printf does not need to support writing more than %zu bytes"
                   " in a single %%* format specifier (got %zu-byte string)",
                   sizeof(g_conversion_buffer) - 1, str_size);
        str_size = sizeof(g_conversion_buffer) - 1;
    }

    size_t pad_size = 0;
    size_t bytes_to_write = str_size;
    if (min_width != MISSING) {
        assert(min_width >= 0);
        assert(min_width < (long)sizeof(g_conversion_buffer));

        /*
         * > If the converted value has fewer characters than the field
         * > width, it is padded with spaces (by default) on the left (or
         * > right, if the left adjustment flag, described later, has been
         * > given) to the field width.
         */
        bytes_to_write = (size_t)min_width;
        if (str_size > bytes_to_write) {
            bytes_to_write = str_size;
        }

        pad_size = (size_t)min_width - str_size;
    }

    if (fmt.flags & FLAG_LEFT_JUSTIFY) {
        memcpy(&g_conversion_buffer[0], str, str_size);
        memset(&g_conversion_buffer[str_size], ' ', pad_size);
    } else {
        memset(&g_conversion_buffer[0], ' ', pad_size);
        memcpy(&g_conversion_buffer[pad_size], str, str_size);
    }

    return fwrite(g_conversion_buffer, bytes_to_write, 1, stream);
}

static int print_formatted(FILE *stream,
                           int chars_written_so_far,
                           const struct fmt *fmt,
                           va_list *args) {
    switch (fmt->type) {
    case TYPE_INVALID:
        __evil_ub("invalid format specifier: %.*s", fmt_size(fmt), fmt->start);
        return -1;
    case TYPE_SIGNED_INT:
    case TYPE_UNSIGNED_INT:
    case TYPE_UNSIGNED_INT_OCTAL:
    case TYPE_UNSIGNED_INT_HEX_LOWER:
    case TYPE_UNSIGNED_INT_HEX_UPPER:
    case TYPE_DOUBLE_DECIMAL_LOWER:
    case TYPE_DOUBLE_DECIMAL_UPPER:
    case TYPE_DOUBLE_EXPONENTIAL_LOWER:
    case TYPE_DOUBLE_EXPONENTIAL_UPPER:
    case TYPE_DOUBLE_ANY_LOWER:
    case TYPE_DOUBLE_ANY_UPPER:
    case TYPE_DOUBLE_HEX_EXPONENTIAL_LOWER:
    case TYPE_DOUBLE_HEX_EXPONENTIAL_UPPER:
    case TYPE_INT_AS_UNSIGNED_CHAR:
        // TODO UNIMPLEMENTED
        return -1;
    case TYPE_STRING:
        switch (fmt->length) {
        case LENGTH_DEFAULT:
            return print_s(stream, fmt, args);
        case LENGTH_LONG:
            // TODO UNIMPLEMENTED: print wchar_t *
            return -1;
        default:
            __evil_ub("unexpected length specifier in %%s: %.*s",
                      fmt_size(fmt), fmt->start);
            // TODO: print fmt->start as literal
            return -1;
        }
    case TYPE_POINTER:
        // TODO UNIMPLEMENTED
        return -1;
    case TYPE_NUM_CHARS_WRITTEN:
        if (fmt->flags != 0
                || fmt->min_width != MISSING
                || fmt->precision != MISSING
                || fmt->length != LENGTH_DEFAULT) {
            __evil_ub("specifying any flags to %%n is UB: %.*s",
                      fmt_size(fmt), fmt->start);
        }
        *va_arg(*args, int*) = chars_written_so_far;
        return 0;
    case TYPE_PERCENT:
        if (fmt->flags != 0
                || fmt->min_width != MISSING
                || fmt->precision != MISSING
                || fmt->length != LENGTH_DEFAULT) {
            __evil_ub("specifying any flags to %%%% is UB: %.*s",
                      fmt_size(fmt), fmt->start);
        }
        return fputc('%', stream) == EOF ? 0 : 1;
    }

    // TODO: this should not be necessary
    __builtin_unreachable();
}

int vfprintf(FILE* restrict stream,
             const char* restrict format,
             va_list args) {
    const char *segment_start = format;
    const char *segment_end = segment_start;
    int chars_written = 0;

    while (*format) {
        if (*format != '%') {
            ++format;
        } else {
            if (format != segment_start) {
                chars_written += fwrite(segment_start, segment_end - segment_start, 1, stream);
            }

            struct fmt fmt;
            const char *fmt_end = parse_fmt(++format, &fmt);

            // Argh, passing a pointer to va_list parameter does not work
            // https://stackoverflow.com/a/8048892/2339636
            va_list args_copy;
            va_copy(args_copy, args);
            int result = print_formatted(stream, chars_written, &fmt, &args_copy);
            va_end(args_copy);

            if (result < 0) {
                return result; // TODO: can we proceed anyway?
            } else {
                chars_written += result;
            }

            segment_start = fmt_end;
            break;
        }
    }

    return chars_written;
}

