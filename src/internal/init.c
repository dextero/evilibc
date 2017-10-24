#include <evil-config.h>

#include "stdio.h"
#include "evilibc.h"

#include "stdio/internal/file.h"

void __evil_init(void) {
    stdin = fopen("/dev/stdin", "r");
    stdout = fopen("/dev/stdout", "a");
    stderr = fopen("/dev/stderr", "a");
}

void __evil_cleanup(void) {
    __evil_malloc_reset();
    __evil_fclose_all();
}
