#include <evil-config.h>

#include "internal/rand.h"

static unsigned int _seed = 1;

void __evil_srand(unsigned int seed)
{
    assert(seed != 0);
    _seed = seed;
}

int __evil_rand(void)
{
    /*
     * Linear Feedback Shift Register with 31-bit cycle
     * https://en.wikipedia.org/wiki/Linear-feedback_shift_register
     * http://www.onarm.com/forum/3202/
     */
    unsigned int lsb = _seed & 1;
    _seed >>= 1;
    if (lsb) {
        _seed ^= 0x54d4d555;
    }

    assert(_seed != 0);
    return (int)((_seed - 1) & __EVIL_RAND_MAX);
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
