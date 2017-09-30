#include "format.h"

#include "stdbool.h"
#include "string.h"

#define DEFAULT INT_MIN

static const char *parse_flags(const char *fmt,
                               int *out_flags)
{
    while (true) {
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
            return fmt;
        }

        ++fmt;
    }
}

static const char *parse_width(const char *fmt,
                               int *out_size)
{
    if (*fmt == '*') {
        *out_size = FROM_ARGUMENT;
        return fmt + 1;
    } else if (*fmt < '0' || '9' < *fmt) {
        *out_size = MISSING;
        return fmt;
    }

    while ('0' <= *fmt && *fmt <= '9') {
        assert(*out_size < (INT_MAX - (*fmt - '0')) / 10);

        *out_size *= 10;
        *out_size += *fmt - '0';
        ++fmt;
    }

    return fmt;
}

static const char *parse_precision(const char *fmt,
                                   int *out_precision)
{
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
                                enum length *out_length)
{
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
                              enum type *out_type)
{
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

const char *__evil_parse_fmt(const char *format,
                             struct fmt *out)
{
    if (*format != '%') {
        return format;
    }

    memset(out, 0, sizeof(*out));

    out->start = format++;
    format = parse_flags(format, &out->flags);
    format = parse_width(format, &out->min_width);
    format = parse_precision(format, &out->precision);
    format = parse_length(format, &out->length);
    format = parse_type(format, &out->type);
    out->end = format;

    return format;
}
