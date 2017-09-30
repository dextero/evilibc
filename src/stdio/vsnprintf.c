#include "stdio.h"

#include "stdbool.h"
#include "string.h"
#include "assert.h"
#include "limits.h"

#include "format.h"

#include "internal/undefined_behavior.h"

#define _STR(x) #x
#define STR(x) _STR(x)

static size_t write_literal(char *restrict *pbuf,
                            size_t *pbuf_size,
                            const char *str,
                            size_t str_size)
{
    size_t bytes_to_copy = str_size < *pbuf_size ? str_size : *pbuf_size;
    memcpy(*pbuf, str, bytes_to_copy);
    *pbuf += bytes_to_copy;
    *pbuf_size -= bytes_to_copy;
    return str_size;
}

static size_t fill_literal(char *restrict *pbuf,
                           size_t *pbuf_size,
                           unsigned char c,
                           size_t repeat)
{
    size_t bytes_to_copy = repeat < *pbuf_size ? repeat : *pbuf_size;
    memset(*pbuf, c, bytes_to_copy);
    *pbuf += bytes_to_copy;
    *pbuf_size -= bytes_to_copy;
    return repeat;
}

static long fetch_width(struct fmt *fmt,
                        va_list *args)
{
    long min_width;

    switch (fmt->min_width) {
    case FROM_ARGUMENT:
        {
            min_width = (long)va_arg(*args, int);
            if (min_width < 0) {
                fmt->flags |= FLAG_LEFT_JUSTIFY;
                min_width = -min_width;
            }
        }
        break;
    case MISSING:
        min_width = MISSING;
        break;
    default:
        min_width = fmt->min_width;
        break;
    }

    if (min_width > (long)CONVERSION_LIMIT) {
        __evil_idb("*printf functions do not need to support writing more than "
                   "%zu bytes in a single %%* format specifier (%ld passed as "
                   "min width)", CONVERSION_LIMIT, min_width);
        min_width = CONVERSION_LIMIT;
    }

    assert(min_width == MISSING || min_width >= 0);
    return min_width;
}

static long fetch_precision(struct fmt *fmt,
                            va_list *args,
                            long default_precision) {
    long prec;

    switch (fmt->precision) {
    case FROM_ARGUMENT:
        {
            prec = (long)va_arg(*args, int);
            /*
             * > A negative precision argument is taken as if the precision were
             * > omitted.
             */
            if (prec < 0) {
                prec = default_precision;
            }
        }
        break;
    case MISSING:
        prec = default_precision;
        break;
    default:
        prec = fmt->precision;
        break;
    }

    assert(prec >= 0);
    return prec;
}


static size_t write_padded(char *restrict *pbuf,
                           size_t *pbuf_size,
                           const char *str,
                           size_t str_size,
                           int min_width,
                           int flags)
{
    if (str_size > CONVERSION_LIMIT) {
        __evil_idb("printf does not need to support writing more than %zu bytes"
                   " in a single %%* format specifier (got %zu-byte string)",
                   CONVERSION_LIMIT, str_size);
        str_size = CONVERSION_LIMIT;
    }

    size_t pad_size = 0;
    size_t bytes_to_write = str_size;

    if (min_width != MISSING) {
        assert(min_width >= 0);
        assert(min_width < (long)CONVERSION_LIMIT);

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

    unsigned pad_char = (flags & FLAG_ZERO_PAD) ? '0' : ' ';

    if (flags & FLAG_LEFT_JUSTIFY) {
        return (write_literal(pbuf, pbuf_size, str, str_size)
                + fill_literal(pbuf, pbuf_size, pad_char, pad_size));
    } else {
        return (fill_literal(pbuf, pbuf_size, pad_char, pad_size)
                + write_literal(pbuf, pbuf_size, str, str_size));
    }
}

static int write_s(char *restrict *pbuf,
                   size_t *pbuf_size,
                   const struct fmt *fmt_const,
                   va_list *args)
{
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

    return write_padded(pbuf, pbuf_size, str, str_size, min_width, fmt.flags);
}

static size_t write_signed(char *restrict *pbuf,
                           size_t *pbuf_size,
                           const struct fmt *fmt_const,
                           va_list *args)
{
    struct fmt fmt = *fmt_const;
    long min_width = fetch_width(&fmt, args);

    /*
     * > d, i   [...] The default precision is 1.
     */
    long precision = fetch_precision(&fmt, args, 1);

    long long value;
    switch (fmt.length) {
    case LENGTH_CHAR:
        value = (signed char)va_arg(*args, int);
        break;
    case LENGTH_SHORT:
        value = (short)va_arg(*args, int);
        break;
    case LENGTH_DEFAULT:
        value = va_arg(*args, int);
        break;
    case LENGTH_LONG:
        value = va_arg(*args, long);
        break;
    case LENGTH_LONG_LONG:
        value = va_arg(*args, long long);
        break;
    default:
        __evil_ub("unexpected length specifier in %%d: %.*s",
                  fmt_size(&fmt), fmt.start);
        return 0;
    }

    // TODO: OMFG, reuquired precision can make this length completely
    // arbitrary
    char tmp[sizeof(STR(LLONG_MIN))] = "";
    char *p = &tmp[sizeof(tmp) - 1];
    bool is_negative = value < 0;

    while (value) {
        *p-- = '0' + value % 10;
        value /= 10;

        assert(p >= tmp);
    }

    size_t int_str_size = &tmp[sizeof(tmp)] - p - 1;

    if (precision > 0) {
        /*
         * > The precision specifies the minimum number of digits to appear;
         * > if the value being converted can be represented in fewer digits,
         * > it is expanded with leading zeros.
         */
        while (int_str_size < (size_t)precision) {
            *p-- = '0';
            ++int_str_size;

            assert(p >= tmp);
        }
    }

    if (is_negative) {
        *p = '-';
    } else if (fmt.flags & FLAG_SIGN_REQUIRED) {
        *p = '+';
    } else if (fmt.flags & FLAG_SPACE_PREFIX) {
        *p = ' ';
    } else {
        ++p;
    }

    assert(min_width == MISSING || min_width >= 0);
    size_t width = min_width == MISSING ? int_str_size : (size_t)min_width;

    return write_padded(pbuf, pbuf_size, p, int_str_size, width, fmt.flags);
}

size_t __evil_write_formatted(char *restrict *pbuf,
                              size_t *pbuf_size,
                              int chars_written_so_far,
                              const struct fmt *fmt,
                              va_list *args)
{
    switch (fmt->type) {
    case TYPE_INVALID:
        __evil_ub("invalid format specifier: %.*s", fmt_size(fmt), fmt->start);
        return 0;
    case TYPE_SIGNED_INT:
        // TODO: other validation?
        return write_signed(pbuf, pbuf_size, fmt, args);
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
        return 0;
    case TYPE_STRING:
        switch (fmt->length) {
        case LENGTH_DEFAULT:
            return write_s(pbuf, pbuf_size, fmt, args);
        case LENGTH_LONG:
            // TODO UNIMPLEMENTED: print wchar_t *
            return 0;
        default:
            __evil_ub("unexpected length specifier in %%s: %.*s",
                      fmt_size(fmt), fmt->start);
            // TODO: print fmt->start as literal
            return 0;
        }
    case TYPE_POINTER:
        // TODO UNIMPLEMENTED
        return 0;
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
        if (*pbuf_size > 0) {
            **pbuf = '%';
            ++*pbuf;
            --*pbuf_size;

        }
        return 1;
    }

    // TODO: this should not be necessary
    __builtin_unreachable();
}

int vsnprintf(char* restrict s,
              size_t n,
              const char* restrict format,
              va_list args)
{
    const char *segment_start = format;
    size_t chars_written = 0;

    // Argh, passing a pointer to va_list parameter does not work
    // https://stackoverflow.com/a/8048892/2339636
    va_list args_copy;
    va_copy(args_copy, args);

    while (*format) {
        if (*format != '%') {
            ++format;
        } else {
            if (format != segment_start) {
                chars_written += write_literal(&s, &n, segment_start,
                                               (size_t)(format - segment_start));
            }

            struct fmt fmt;
            const char *fmt_end = __evil_parse_fmt(format, &fmt);

            chars_written += (int)__evil_write_formatted(&s, &n, chars_written,
                                                         &fmt, &args_copy);

            segment_start = fmt_end;
            format = fmt_end;
        }
    }

    va_end(args_copy);

    if (format != segment_start) {
        chars_written += write_literal(&s, &n, segment_start,
                                       (size_t)(format - segment_start));
    }

    return chars_written;
}

