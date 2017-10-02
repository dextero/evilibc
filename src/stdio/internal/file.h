#ifndef __EVILIBC_STDIO_FILE_H
#define __EVILIBC_STDIO_FILE_H

#include "stdbool.h"
#include "stdio.h"

enum __evilibc_file_flag {
    READ = (1 << 0),
    WRITE = (1 << 1),
    APPEND = (1 << 2),
    EXCLUSIVE = (1 << 3),
    BINARY = (1 << 4),
    TRUNCATE = (1 << 5),
};

struct __evilibc_file {
    int fd;
    int file_flags;
    int bufmode;
    bool error;
    bool eof;
};

extern FILE __evil_open_files[FOPEN_MAX];

static inline FILE *file_alloc(void) {
    for (size_t i = 0; i < FOPEN_MAX; ++i) {
        if (__evil_open_files[i].fd < 0) {
            return &__evil_open_files[i];
        }
    }
    return NULL;
}

static inline void file_dealloc(FILE *f) {
    f->fd = -1;
}

#endif // __EVILIBC_STDIO_FILE_H