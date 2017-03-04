#ifndef __EVILIBC_INTERNAL_RAND_H
#define __EVILIBC_INTERNAL_RAND_H

#define __EVIL_RAND_MAX 0x7FFFFFFF

int __evil_rand(void);

int __evil_rand_range(int min_inclusive,
                      int max_inclusive);

#endif /* __EVILIBC_INTERNAL_RAND_H */
