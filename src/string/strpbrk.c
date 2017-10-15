#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

char* strpbrk(const char* s1,
              const char* s2)
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
    if (__evil_is_null(s1) || __evil_is_null(s2)) {
        __evil_ub("passing NULL to strpbrk is UB: strpbrk(%p, %p)", s1, s2);
        return NULL;
    }

    while (*s1 != '\0') {
        if (strchr(s2, *s1)) {
            /* C spec requires this function to cast away constness */
            return (char *)__evil_const_cast(s1);
        }
        ++s1;
    }

    return NULL;
}
