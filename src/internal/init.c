#include <evil-config.h>

#include "stdio.h"

#include "stdio/internal/file.h"

void __evil_init(void) {
    stdin = fopen("/dev/stdin", "r");
    stdout = fopen("/dev/stdout", "a");
    stderr = fopen("/dev/stderr", "a");
}
