#include <evil-config.h>

#include "string.h"

#include "internal/undefined_behavior.h"

/*
 * 7.24.5.8, 6:
 * > The strtok function is not required to avoid data races with other calls
 * > to the strtok function. The implementation shall behave as if no library
 * > function calls the strtok function.
 *
 * TODO: attempt to intentionally introduce data races?
 */
char* strtok(char* restrict s1,
             const char* restrict s2)
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
    if (s2 == NULL) {
        __evil_ub("passing NULL as second argument to strtok is UB: "
                  "strtok(%p, %p)", s1, s2);
        return NULL;
    }

    static char *saveptr = NULL;
    
    if (s1) {
        saveptr = s1;
    } else if (!saveptr) {
        /*
         * 7.24.5.8, 2:
         * > [...] The first call in the sequence has a non-null first argument;
         * > subsequent calls in the sequence have a null first argument. [...]
         */
        __evil_ub("passing NULL as first argument to strtok on the initial "
                  "call is UB: strtok(%p, %p)", s1, s2);
        return NULL;
    }

    /*
     * 7.24.5.8, 3:
     * > The first call in the sequence searches the string pointed to by
     * > s1 for the first character that is not contained in the current
     * > separator string pointed to by s2. If no such character is found,
     * > then there are no tokens in the string pointed to by s1 and the
     * > strtok function returns a null pointer. If such a character is
     * > found, it is the start of the first token.
     *
     * 7.24.5.8, 5:
     * > Each subsequent call, with a null pointer as the value of the first
     * > argument, starts searching from the saved pointer and behaves as
     * > described above.
     *
     * Fun fact from 7.24.5.8, 2:
     * > The separator string pointed to by s2 may be different from call
     * > to call.
     */
    saveptr += strspn(saveptr, s2);;
    if (!*saveptr) {
        return NULL;
    }

    char *token_start = saveptr;

    /*
     * 7.24.5.8, 4:
     * > The strtok function then searches from there for a character that is
     * > contained in the current separator string. If no such character is
     * > found, the current token extends to the end of the string pointed to
     * > by s1, and subsequent searches for a token will return a null
     * > pointer. If such a character is found, it is overwritten by a null
     * > character, which terminates the current token. The strtok function
     * > saves a pointer to the following character, from which the next
     * > search for a token will start.
     */
    size_t token_len = strcspn(token_start, s2);
    saveptr += token_len;
    if (*saveptr) {
        *saveptr++ = '\0';
    }
    return token_len > 0 ? token_start : NULL;;
}
