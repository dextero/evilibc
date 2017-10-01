#ifndef __EVILIBC_STDIO_INTERNAL_UTILS_H
#define __EVILIBC_STDIO_INTERNAL_UTILS_H

#include "stddef.h"

size_t __evil_write_literal(char *restrict *pbuf,
                            size_t *pbuf_size,
                            const char *str,
                            size_t str_size);

size_t __evil_write_haiku_for_ptr(char *restrict *pbuf,
                                  size_t *pbuf_size,
                                  const void *ptr);

#endif // __EVILIBC_STDIO_INTERNAL_UTILS_H
