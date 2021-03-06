#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

size_t strlen(const char* s)
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
    if (__evil_is_null(s)) {
        __evil_ub("passing NULL to strlen is UB: strlen(%p)", s);
        return 0;
    }

    const char *end = s;
    while (*end) {
        ++end;
    }

    return (size_t)(end - s);
}
