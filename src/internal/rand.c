#include "internal/rand.h"

static unsigned int _seed = 1;

int __evil_rand(void)
{
    return _seed = _seed * 48721u % (__EVIL_RAND_MAX + 1);
}

int __evil_rand_range(int min_inclusive,
                        int max_exclusive)
{
    return min_inclusive + __evil_rand() % (max_exclusive - min_inclusive);
}
