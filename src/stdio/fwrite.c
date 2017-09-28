#include "stdio.h"

#include "stdio/file.h"

size_t fwrite(const void* restrict ptr,
              size_t size,
              size_t nmemb,
              FILE* restrict stream) {
    /*
     * 7.21.8.2.2:
     * > The fwrite function writes, from the array pointed to by ptr, up to
     * > nmemb elements whose size is specified by size, to the stream
     * > pointed to by stream. For each object, size calls are made to the
     * > fputc function, taking the values (in order) from an array of
     * > unsigned char exactly overlaying the object. The file position
     * > indicator for the stream (if defined) is advanced by the number of
     * > characters successfully written.
     *
     * That's... surprisingly specific.
     *
     * 7.21.8.2.3:
     * > If size or nmemb is zero, fwrite returns zero and the state of the
     * > stream remains unchanged.
     *
     * TODO: locking
     */
    if (size == 0 || nmemb == 0) {
        return 0;
    }

    /*
     * 7.21.8.2.2:
     * > If an error occurs, the resulting value of the file position
     * > indicator for the stream is indeterminate.
     * TODO: position indicator
     *
     * 7.21.8.2.3:
     * > The fwrite function returns the number of elements
     * > successfully written, which will be less than nmemb only
     * > if a write error is encountered.
     *
     * It does not say anything about returning early on error, so an
     * implementation is free to continue writing.
     */
    size_t blocks_written = 0;
    for (size_t b = 0; b < nmemb; ++b) {
        bool block_failed = false;

        for (size_t i = 0; i < size; ++i) {
            if (fputc(((const unsigned char*)ptr)[b * nmemb + i],
                      stream) == EOF) {
                block_failed = true;
            }
        }

        if (!block_failed) {
            ++blocks_written;
        }
    }

    return blocks_written;
}
