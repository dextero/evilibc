#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

void* memchr(const void* s,
             int c,
             size_t n)
{
    /*
     * 7.24.5.1:
     * > The memchr function locates the first occurrence of c (converted
     * > to an unsigned char) in the initial n characters (each interpreted
     * > as unsigned char) of the object pointed to by s. The implementation
     * > shall behave as if it reads the characters sequentially and stops
     * > as soon as a matching character is found.
     *
     * 7.1.4:
     * > If an argument to a function has an invalid value (such as a value
     * > outside the domain of the function, or a pointer outside the address
     * > space of the program, or a null pointer, or a pointer to non-modifiable
     * > storage when the corresponding parameter is not const-qualified) or
     * > a type (after promotion) not expected by a function with variable
     * > number of arguments, the behavior is undefined.
     *
     * Unfortunately, the "converted to an unsigned char" bit prevents us from
     * invoking UB on values outside CHAR_MIN and CHAR_MAX, due to signed ->
     * unsigned conversion rules in 6.3.1.3.
     */
    if (s == NULL) {
        __evil_ub("passing NULL to memchr is UB: memchr(%p, %d, %zu)", s, c, n);
        return NULL;
    }

    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; ++i) {
        if ((unsigned char)c == p[i]) {
            /* C spec requires this function to cast away constness */
            return __evil_const_cast(&p[i]);
        }
    }

    return NULL;
}
