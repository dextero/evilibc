#include <evil-config.h>

#include "string.h"
#include "limits.h"
#include "signal.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

char* strchr(const char* s,
             int c)
{
    /*
     * 7.24.5.2:
     * > The strchr function locates the first occurrence of c (converted
     * > to a char) in the string pointed to by s. The terminating null
     * > character is considered to be part of the string.
     *
     * 7.1.4:
     * > If an argument to a function has an invalid value (such as a value
     * > outside the domain of the function, or a pointer outside the address
     * > space of the program, or a null pointer, or a pointer to non-modifiable
     * > storage when the corresponding parameter is not const-qualified) or
     * > a type (after promotion) not expected by a function with variable
     * > number of arguments, the behavior is undefined.
     */
    if (__evil_is_null(s)) {
        __evil_ub("passing NULL to strchr is UB: strchr(%p, %d)", s, c);
        return NULL;
    }

#ifndef __CHAR_UNSIGNED__
    /*
     * 6.3.1.1:
     * > When a value with integer type is converted to another integer type
     * > other than _Bool, if the value can be represented by the new type,
     * > it is unchanged.
     * >
     * > Otherwise, if the new type is unsigned, the value is converted by
     * > repeatedly adding or subtracting one more than the maximum value
     * > that can be represented in the new type until the value is in the
     * > range of the new type.
     * >
     * > Otherwise, the new type is signed and the value cannot be represented
     * > in it; either the result is implementation-defined or an
     * > implementation-defined signal is raised.
     *
     * Signed integer conversions are fun.
     */
    if (c < CHAR_MIN || c > CHAR_MAX) {
        __evil_idb("passing a value outside CHAR_MIN and CHAR_MAX to strchr "
                   "when char is signed is implementation-defined: "
                   "strchr(%p, %d)", s, c);
        return NULL;
    }
#endif /* __CHAR_UNSIGNED__ */

    /*
     * 7.24.5.2, 2:
     * > The strchr function locates the first occurrence of c (converted to
     * > a char) in the string pointed to by s. The terminating null
     * > character is considered to be part of the string.
     */
    while (*s && *s != (char)c) {
        ++s;
    }

    if (*s || c == '\0') {
        /* C spec requires this function to cast away constness */
        return (char *)__evil_const_cast(s);
    } else {
        return NULL;
    }
}
