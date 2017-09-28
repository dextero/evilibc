#include "stdio/file.h"

void __evil_init(void) {
    for (size_t i = 0;
            i < sizeof(__evil_open_files) / sizeof(__evil_open_files[0]);
            ++i) {
        file_dealloc(&__evil_open_files[i]);
    }
}
