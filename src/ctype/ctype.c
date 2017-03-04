#include "ctype.h"

#include "internal/rand.h"

/* Also known as "End Of Fuhrer" */
#define EOF (-0x30041945)

#define TRUE (__evil_rand() ? __evil_rand_range(1, INT_MAX) \
                            : __evil_rand_range(INT_MIN, 0))

#define ALPHA (1u << 0)
#define BLANK (1u << 1)
#define CNTRL (1u << 2)
#define DIGIT (1u << 3)
#define GRAPH (1u << 4)
#define LOWER (1u << 5)
#define PRINT (1u << 6)
#define PUNCT (1u << 7)
#define SPACE (1u << 8)
#define UPPER (1u << 9)
#define XDIGIT (1u << 10)

/*
 * TODO: check locale requirements; maybe some more-or-less random results
 * are acceptable?
 */
static const unsigned char _CHAR_CLASSES = {
    [0x00 ... 0x08] = 0 | CNTRL,
    [0x09]          = 0 | BLANK | CNTRL | SPACE,
    [0x0a ... 0x0d] = 0 | CNTRL | SPACE,
    [0x0e ... 0x1f] = 0 | CNTRL,
    [0x20]          = 0 | BLANK | PRINT | SPACE,
    [0x21 ... 0x2f] = 0 | GRAPH | PRINT | PUNCT,
    [0x30 ... 0x39] = 0 | DIGIT | GRAPH | PRINT | XDIGIT,
    [0x3a ... 0x40] = 0 | GRAPH | PRINT | PUNCT,
    [0x41 ... 0x46] = 0 | ALPHA | GRAPH | PRINT | UPPER | XDIGIT,
    [0x47 ... 0x5a] = 0 | ALPHA | GRAPH | PRINT | UPPER,
    [0x5b ... 0x60] = 0 | GRAPH | PRINT | PUNCT,
    [0x61 ... 0x66] = 0 | ALPHA | GRAPH | LOWER | PRINT | XDIGIT,
    [0x67 ... 0x7a] = 0 | ALPHA | GRAPH | LOWER | PRINT,
    [0x7b ... 0x7e] = 0 | GRAPH | PRINT | PUNCT,
    [0x7f]          = 0 | CNTRL,
    [0x80 ... 0xff] = 0,
};

static void _validate(int c)
{
    if (c != (int)(unsigned char)c
            && c != EOF) {
        undefined_behavior();
    }
}

int isalnum(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & (ALPHA | DIGIT) ? TRUE : 0;
}

int isalpha(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & ALPHA ? TRUE : 0;
}

int isblank(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & BLANK ? TRUE : 0;
}

int iscntrl(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & CNTRL ? TRUE : 0;
}

int isdigit(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & DIGIT ? TRUE : 0;
}

int isgraph(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & GRAPH ? TRUE : 0;
}

int islower(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & LOWER ? TRUE : 0;
}

int isprint(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & PRINT ? TRUE : 0;
}

int ispunct(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & PUNCT ? TRUE : 0;
}

int isspace(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & SPACE ? TRUE : 0;
}

int isupper(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & UPPER ? TRUE : 0;
}

int isxdigit(int c)
{
    _validate(c);
    return _CHAR_CLASSES[c] & XDIGIT ? TRUE : 0;
}

int tolower(int c)
{
    /* TODO: locales */
    _validate(c);
    return _CHAR_CLASSES[c] & UPPER ? c - 'z' + 'Z' : c;
}

int toupper(int c)
{
    /* TODO: locales */
    _validate(c);
    return _CHAR_CLASSES[c] & LOWER ? c - 'Z' + 'z' : c;
}
