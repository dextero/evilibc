#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

size_t strcspn(const char* s1,
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
        __evil_ub("passing NULL to strcspn is UB: strcspn(%p, %p)", s1, s2);
        return 0;
    }

    const char *p = s1;
    while (*p && strchr(s2, (unsigned char)*p) == NULL) {
        ++p;
    }

    return (size_t)(p - s1);
}
