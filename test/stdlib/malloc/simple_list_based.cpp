#include "Test.h"

#include <array>
#include <memory>
#include <iostream>

using namespace std;
using namespace testing;

namespace {

class MemoryPool {
public:
    virtual char *sbrk(ptrdiff_t increment) = 0;
};

MemoryPool *g_pool;

template <size_t N>
class SizedMemoryPool : public MemoryPool {
    array<char, N> _pool;
    char *_brk;

    MemoryPool *_prev_pool;

public:
    SizedMemoryPool()
        : _pool(),
          _brk(&_pool[0]) {
        g_pool = this;
    }

    ~SizedMemoryPool() {
        g_pool = nullptr;
    }

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

} // namespace

extern "C" void *_sbrk(ptrdiff_t increment) {
    return g_pool->sbrk(increment);
}

extern "C" void __evil_malloc_reset(void);
extern "C" void *test_malloc(size_t);
extern "C" void test_free(void *);

class MallocTest : public evil::Test {
    void SetUp() {
        __evil_malloc_reset();
    }
};

class FreeTest : public evil::Test {
    void SetUp() {
        __evil_malloc_reset();
    }
};

TEST_F(MallocTest, zero_bytes) {
    evil::IDBChecker checker{2};

    // malloc(0) may return NULL or non-NULL that must not be dereferenced
    // Either way, the result should be consistent (I think?)
    // Also, it should not call _sbrk to allocate memory, as it's unnecessary
    ASSERT_EQ(test_malloc(0), test_malloc(0));
}

TEST_F(MallocTest, alloc_only) {
    SizedMemoryPool<4096> pool;

    vector<void *> ptrs;
    for (size_t i = 0; i < 128; ++i) {
        void *ptr = test_malloc(16);
        ASSERT_THAT(ptrs, Not(Contains(ptr)));
        ptrs.push_back(ptr);
    }
}

TEST_F(FreeTest, basic) {
    SizedMemoryPool<4096> pool;

    vector<void *> ptrs;
    for (void *ptr = test_malloc(16); ptr; ptr = test_malloc(16)) {
        ptrs.push_back(ptr);
    }

    // at this point, an allocation failed due to out-of-memory scenario
    ASSERT_EQ(nullptr, test_malloc(16));
    test_free(ptrs.back());
    ptrs.pop_back();

    // this should allow allocationg some memory
    ptrs.push_back(test_malloc(16));
    ASSERT_NE(nullptr, ptrs.back());

    // it should work the same when the oldest pointer is released
    test_free(ptrs[0]);
    ptrs[0] = test_malloc(16);
    ASSERT_NE(nullptr, ptrs[0]);
    ASSERT_EQ(nullptr, test_malloc(16));

    // releasing all memory should allow for allocation of a bigger chunk
    for (void *p : ptrs) {
        test_free(p);
    }

    ptrs.clear();
    ASSERT_NE(nullptr, test_malloc(512));
}

TEST_F(FreeTest, free_in_order) {
    SizedMemoryPool<4096> pool;

    vector<void *> ptrs;
    for (void *ptr = test_malloc(16); ptr; ptr = test_malloc(16)) {
        ptrs.push_back(ptr);
    }

    for (void *p : ptrs) {
        test_free(p);
    }
}

TEST_F(FreeTest, free_in_reverse_order) {
    SizedMemoryPool<4096> pool;

    vector<void *> ptrs;
    for (void *ptr = test_malloc(16); ptr; ptr = test_malloc(16)) {
        ptrs.push_back(ptr);
    }

    for (auto it = ptrs.rbegin(); it != ptrs.rend(); ++it) {
        test_free(*it);
    }
}

TEST_F(FreeTest, free_double) {
    SizedMemoryPool<4096> pool;

    void *a = test_malloc(16);
    void *b = test_malloc(16);

    test_free(a);
    {
        evil::UBChecker checker{1};
        test_free(a);
    }
    test_free(b);
    {
        evil::UBChecker checker{1};
        test_free(b);
    }
}

TEST_F(FreeTest, invalid_ptr) {
    SizedMemoryPool<4096> pool;
    evil::UBChecker checker{1};

    void *p = &p;
    test_free(p);
}

