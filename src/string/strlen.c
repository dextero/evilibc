#include "string.h"

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
    if (s) {
        __evil_ub("passing NULL to strlen is UB: strlen(%p)", s);
    }

    return __builtin_strlen(s);
}
