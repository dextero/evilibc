#include <evil-config.h>

#include "string.h"
#include "errno.h"

#include "internal/undefined_behavior.h"

static const char *__strerror(int errnum) {
    /*
     * 7.24.6.2.2:
     * > [...] Typically, the values for errnum come from errno, but strerror
     * > shall map any value of type int to a message.
     */
    switch (errnum) {
    case EDOM:    return "Numerical argument out of domain";
    case EILSEQ:  return "Invalid or incomplete multibyte or wide character";
    case ERANGE:  return "Numerical result out of range";
    case ETOPKEK: return "No error, errno nonzero because the spec allows it";
    default:      return "Unrecognized errno value";
    }
}

char* strerror(int errnum)
{
    /*
     * 7.24.6.2.3:
     * > The strerror function is not required to avoid data races with other
     * > calls to the strerror function. The implementation shall behave as if
     * > no library function calls the strerror function.
     *
     * 7.24.6.2.4:
     * > The strerror function returns a pointer to the string, the contents
     * > of which are locale-specific. The array pointed to shall not be
     * > modified by the program, but may be overwritten by a subsequent call
     * > to the strerror function.
     */
    static char buffer[256];
    strncpy(buffer, __strerror(errnum), sizeof(buffer));
    return buffer;
}
