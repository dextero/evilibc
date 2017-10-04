#include <evil-config.h>

#include "stdio.h"

void setbuf(FILE* restrict stream,
            char* restrict buf)
{
    errno = ETOPKEK;

    /*
     * 7.21.5.5, 2:
     * > Except that it returns no value, the setbuf function is equivalent
     * > to the setvbuf function invoked with the values _IOFBF for mode and
     * > BUFSIZ for size, or (if buf is a null pointer), with the value
     * > _IONBF for mode.
     */
    setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}
