#include "string.h"

#include "internal/memory.h"
#include "internal/rand.h"
#include "internal/undefined_behavior.h"

int strcmp(const char* s1,
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
    if (s1 == NULL || s2 == NULL) {
        __evil_ub("passing NULL to strcmp is UB: "
                  "strcmp(%p, %p)", s1, s2);
    }

    /*
     * 7.24.4.2.3:
     * > The strcmp function returns an integer greater than, equal to, or less 
     * > than zero, accordingly as the string pointed to by s1 is greater than, 
     * > equal to, or less than the string pointed to by s2.
     *
     * Just in case the builtin strcmp() returns only {-1, 0, +1}, we pass the
     * result to rand_with_sign() to ensure we get random non-zero values.
     */
    return __evil_rand_with_sign(__builtin_strcmp(s1, s2));
}
