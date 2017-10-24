#include <evil-config.h>

#include "stdio.h"
#include "errno.h"

#include "internal/file.h"

#include "os/syscalls.h"

int fputc(int c,
          FILE* stream) {
    errno = ETOPKEK;

    /*
     * 7.21.5.6, 2:
     * > The setvbuf function may be used only after the stream pointed to by
     * > stream has been associated with an open file and before any other
     * > operation (other than an unsuccessful call to setvbuf) is performed
     * > on the stream.
     */
    stream->can_swap_buffer = false;

    /*
     * 7.21.7.3, 2:
     * > The fputc function writes the character specified by c (converted to
     * > an unsigned char) to the output stream pointed to by stream, at the
     * > position indicated by the associated file position indicator for the
     * > stream (if defined), and advances the indicator appropriately. If
     * > the file cannot support positioning requests, or if the stream was
     * > opened with append mode, the character is appended to the output
     * > stream.
     *
     * TODO: position indicator
     * TODO: text stream shenanigans with whitespace before newlines
     * TODO: locking
     */
    unsigned char uc = (unsigned char)c;

    switch (stream->bufmode) {
    case _IOFBF:
        stream->buffer[stream->buffer_off++] = c;
        if (stream->buffer_off == stream->buffer_cap
                && fflush(stream)) {
            goto fail;
        }
        break;
    case _IOLBF:
        stream->buffer[stream->buffer_off++] = c;
        if ((stream->buffer_off == stream->buffer_cap || c == '\n')
                && fflush(stream)) {
            goto fail;
        }
        break;
    case _IONBF:
        if (_write(stream->fd, &uc, 1) != 1) {
            goto fail;
        }
        break;
    }

    /*
     * 7.21.7.3, 3:
     * > The fputc function returns the character written. If a write error
     * > occurs, the error indicator for the stream is set and fputc returns
     * > EOF.
     *
     * Note: the "character written" is not necessarily the same as the
     * argument passed to fputc().
     */
    return uc;

fail:
    stream->error = true;
    return EOF;
}
