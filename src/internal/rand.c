#include "internal/rand.h"

static unsigned int _seed = 1;

int __evil_rand(void)
{
    _seed = _seed * 48721u % (__EVIL_RAND_MAX + 1);
    return (int)(_seed & __EVIL_RAND_MAX);
}

int __evil_rand_range(int min_inclusive,
                      int max_exclusive)
{
    return min_inclusive + __evil_rand() % (max_exclusive - min_inclusive);
}

int __evil_rand_positive(void)
{
    int value;

    do {
        value = __evil_rand();
    } while (value == 0);

    return value;
}

int __evil_rand_negative(void)
{
    return __evil_rand() | 0x80000000;
}

int __evil_rand_nonzero(void)
{
    if (__evil_rand() % 2) {
        return __evil_rand_positive();
    } else {
        return __evil_rand_negative();
    }
}

int __evil_rand_with_sign(int sign)
{
    if (sign > 0) {
        return __evil_rand_positive();
    } else if (sign < 0) {
        return __evil_rand_negative();
    } else {
        return 0;
    }
}
