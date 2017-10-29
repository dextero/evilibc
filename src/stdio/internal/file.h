#ifndef __EVILIBC_STDIO_FILE_H
#define __EVILIBC_STDIO_FILE_H

#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

enum __evil_file_flag {
    READ = (1 << 0),
    WRITE = (1 << 1),
    APPEND = (1 << 2),
    EXCLUSIVE = (1 << 3),
    BINARY = (1 << 4),
    TRUNCATE = (1 << 5),
};

struct __evil_file {
    int fd;
    int file_flags;
    int bufmode;
    bool error;
    bool eof;
    bool buffer_needs_free;
    bool can_swap_buffer;
    char *buffer;
    size_t buffer_off;
    size_t buffer_cap;
};

extern FILE __evil_open_files[FOPEN_MAX];

static inline void file_set_buffer(FILE *restrict stream,
                                   char *buffer,
                                   size_t buffer_size,
                                   bool buffer_needs_free) {
    if (stream->buffer_needs_free) {
        free(stream->buffer);
    }

    stream->buffer = buffer;
    stream->buffer_off = 0;
    stream->buffer_cap = buffer_size;
    stream->buffer_needs_free = buffer_needs_free;
}


static inline FILE *file_alloc(void) {
    for (size_t i = 0; i < FOPEN_MAX; ++i) {
        if (__evil_open_files[i].fd < 0) {
            return &__evil_open_files[i];
        }
    }
    return NULL;
}

static inline void file_dealloc(FILE *f) {
    file_set_buffer(f, NULL, 0, false);
    f->fd = -1;
}

#endif // __EVILIBC_STDIO_FILE_H
