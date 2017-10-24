#ifndef __EVIL_TEST_MEMORY_POOL_H
#define __EVIL_TEST_MEMORY_POOL_H

#include <array>

namespace evil {

class MemoryPool {
public:
    virtual char *sbrk(ptrdiff_t increment) = 0;
};

template <size_t N>
class SizedMemoryPool : public MemoryPool {
    std::array<char, N> _pool;
    char *_brk;

public:
    SizedMemoryPool()
        : _pool(),
          _brk(&_pool[0])
    {}

    virtual char *sbrk(ptrdiff_t increment) {
        char *new_brk = _brk + increment;
        if (new_brk > _pool.end()) {
            return NULL;
        }

        char *old_brk = _brk;
        _brk = new_brk;
        return old_brk;
    }
};

} // namespace evil

#endif // __EVIL_TEST_MEMORY_POOL_H
