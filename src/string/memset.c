#include <evil-config.h>

#include "string.h"

#include "internal/undefined_behavior.h"

void* memset(void* s,
             int c,
             size_t n)
{
    /*
     * 7.1.4:
     * > If an argument to a function has an invalid value (such as a value
     * > outside the domain of the function, or a pointer outside the address
     * > space of the program, or a null pointer, or a pointer to non-modifiable
     * > storage when the corresponding parameter is not const-qualified) or
     * > a type (after promotion) not expected by a function with variable
     * > number of arguments, the behavior is undefined.
     */
    if (s == NULL) {
        __evil_ub("passing NULL to memset is UB even if size is 0: "
                  "memset(%p, %d, %zu)", s, c, n);
        return NULL;
    }

    /*
     * 7.24.6.2:
     * > The memset function copies the value of c (converted to an unsigned
     * > char) into each of the first n characters of the object pointed
     * > to by s.
     *
     * Unfortunately, the "converted to an unsigned char" bit prevents us from
     * invoking UB on values outside CHAR_MIN and CHAR_MAX, due to signed ->
     * unsigned conversion rules in 6.3.1.3.
     */
    for (size_t i = 0; i < n; ++i) {
        ((unsigned char *)s)[i] = (unsigned char)c;
    }
    return s;
}
