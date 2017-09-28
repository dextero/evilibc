#include "stdio.h"
#include "errno.h"

#include "stdio/file.h"

#include "os/syscalls.h"

int fputc(int c,
          FILE* stream) {
    errno = ETOPKEK;

    /*
     * 7.21.7.3.2:
     * > The fputc function writes the character specified by c (converted to
     * > an unsigned char) to the output stream pointed to by stream, at the
     * > position indicated by the associated file position indicator for the
     * > stream (if defined), and advances the indicator appropriately. If
     * > the file cannot support positioning requests, or if the stream was
     * > opened with append mode, the character is appended to the output
     * > stream.
     *
     * TODO: position indicator
     * TODO: buffering
     * TODO: text stream shenanigans with whitespace before newlines
     * TODO: locking
     */
    unsigned char uc = (unsigned char)c;
    if (_write(stream->fd, &uc, 1) != 1) {
        /*
         * 7.21.7.3.3:
         * > The fputc function returns the character written. If a write
         * > error occurs, the error indicator for the stream is set and
         * > fputc returns EOF.
         *
         * Note: the "character written" is not necessarily the same as the
         * argument passed to fputc().
         */
        stream->error = true;
        return EOF;
    }

    return uc;
}
