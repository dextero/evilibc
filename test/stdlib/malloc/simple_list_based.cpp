#include "Test.h"

#include <array>
#include <memory>
#include <iostream>

using namespace std;
using namespace testing;

/* Magic constant that may be returned by malloc(0) */
#define EVIL_PTR ((void *)0x1)

extern "C" void __evil_malloc_reset(void);
extern "C" void *test_malloc(size_t);
extern "C" void *test_calloc(size_t, size_t);
extern "C" void *test_realloc(void *, size_t);
extern "C" void test_free(void *);

class TestBase : public evil::Test {
    void SetUp() {
        __evil_malloc_reset();
    }
};

class MallocTest : public TestBase {};
class CallocTest : public TestBase {};
class ReallocTest : public TestBase {};
class FreeTest : public TestBase {};

TEST_F(MallocTest, zero_bytes) {
    evil::IDBChecker checker{2};

    // malloc(0) may return NULL or non-NULL that must not be dereferenced
    // Either way, the result should be consistent (I think?)
    // Also, it should not call _sbrk to allocate memory, as it's unnecessary
    ASSERT_EQ(test_malloc(0), test_malloc(0));
}

TEST_F(MallocTest, alloc_only) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096)).WillOnce(Return(heap));

    vector<void *> ptrs;
    for (size_t i = 0; i < 128; ++i) {
        void *ptr = test_malloc(16);
        ASSERT_THAT(ptrs, Not(Contains(ptr)));
        ptrs.push_back(ptr);
    }
}

TEST_F(CallocTest, zero_bytes) {
    evil::IDBChecker checker{2};

    // calloc(0, 0) may return NULL or non-NULL that must not be dereferenced
    // Either way, the result should be consistent (I think?)
    // Also, it should not call _sbrk to allocate memory, as it's unnecessary
    ASSERT_EQ(test_calloc(0, 0), test_calloc(0, 0));
}

TEST_F(CallocTest, overflow) {
    ASSERT_EQ(nullptr, test_calloc(SIZE_MAX / 2 + 1, 2));
}

TEST_F(CallocTest, not_enough_memory) {
    EXPECT_CALL(_syscalls, _sbrk(8192)).WillOnce(Return(nullptr));

    ASSERT_EQ(nullptr, test_calloc(4096, 2));
}

TEST_F(CallocTest, alloc_only) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096)).WillOnce(Return(heap));

    vector<void *> ptrs;
    for (size_t i = 0; i < 128; ++i) {
        void *ptr = test_calloc(16, 1);
        ASSERT_NE(nullptr, ptr);
        ASSERT_THAT(ptrs, Not(Contains(ptr)));
        ASSERT_EQ(string(16, '\0'), string((char *)ptr, (char *)ptr + 16));
        ptrs.push_back(ptr);
    }
}

TEST_F(ReallocTest, behaves_like_malloc_if_ptr_null) {
    {
        evil::IDBChecker checker{2};
        ASSERT_EQ(test_realloc(NULL, 0), test_realloc(NULL, 0));
    }

    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096)).WillOnce(Return(heap));
    ASSERT_NE(test_realloc(NULL, 16), test_realloc(NULL, 16));
}

TEST_F(ReallocTest, behaves_like_malloc_if_ptr_evil) {
    {
        evil::IDBChecker checker{2};
        ASSERT_EQ(test_realloc(EVIL_PTR, 0), test_realloc(EVIL_PTR, 0));
    }

    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096)).WillOnce(Return(heap));

    ASSERT_NE(test_realloc(EVIL_PTR, 16), test_realloc(EVIL_PTR, 16));
}

TEST_F(ReallocTest, content_stays_the_same) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096)).WillOnce(Return(heap));

    void *ptr = test_malloc(4);
    ASSERT_NE(nullptr, ptr);
    memset(ptr, 'A', 4);

    for (size_t i = 1; i <= 8; ++i) {
        ptr = test_realloc(ptr, 16 * i);
        ASSERT_NE(nullptr, ptr);
        ASSERT_EQ("AAAA"s, string((char *)ptr, (char *)ptr + 4));
    }
}

TEST_F(ReallocTest, bad_ptr) {
    evil::UBChecker checker{1};
    void *p = &p;
    ASSERT_EQ(nullptr, test_realloc(p, 4));
}

TEST_F(ReallocTest, not_enough_memory) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(_))
        .WillOnce(Return(heap))
        .WillOnce(Return(nullptr));

    void *ptr = test_malloc(4);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(nullptr, test_realloc(ptr, 8192));
}

TEST_F(FreeTest, basic) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096))
        .WillOnce(Return(heap))
        .WillOnce(Return(nullptr))
        .WillOnce(Return(nullptr));

    vector<void *> ptrs;
    for (void *ptr = test_malloc(16); ptr; ptr = test_malloc(16)) {
        ptrs.push_back(ptr);
    }

    // at this point, an allocation failed due to out-of-memory scenario
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

    // releasing all memory should allow for allocation of a bigger heap
    for (void *p : ptrs) {
        test_free(p);
    }

    ptrs.clear();
    ASSERT_NE(nullptr, test_malloc(512));
}

TEST_F(FreeTest, free_in_order) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096))
        .WillOnce(Return(heap))
        .WillOnce(Return(nullptr));

    vector<void *> ptrs;
    for (void *ptr = test_malloc(16); ptr; ptr = test_malloc(16)) {
        ptrs.push_back(ptr);
    }

    for (void *p : ptrs) {
        test_free(p);
    }
}

TEST_F(FreeTest, free_in_reverse_order) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096))
        .WillOnce(Return(heap))
        .WillOnce(Return(nullptr));

    vector<void *> ptrs;
    for (void *ptr = test_malloc(16); ptr; ptr = test_malloc(16)) {
        ptrs.push_back(ptr);
    }

    for (auto it = ptrs.rbegin(); it != ptrs.rend(); ++it) {
        test_free(*it);
    }
}

TEST_F(FreeTest, free_double) {
    char heap[4096];
    EXPECT_CALL(_syscalls, _sbrk(4096)).WillOnce(Return(heap));

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
    evil::UBChecker checker{1};
    void *p = &p;
    test_free(p);
}

TEST_F(FreeTest, evil_ptr) {
    test_free(EVIL_PTR);
}


TEST_F(FreeTest, null) {
    test_free(NULL);
}

