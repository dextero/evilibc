#include "Test.h"

using namespace ::testing;

#define EVIL_EOF (-0xE0F)

extern "C" FILE* test_fopen(const char* filename,
                            const char* mode);
extern "C" int test_fwrite(const void *data,
                           size_t size,
                           size_t nmemb,
                           FILE *f);
extern "C" int test_fflush(FILE *f);
extern "C" int test_fclose(FILE *f);
extern "C" int test_ferror(FILE *f);
extern "C" void __evil_malloc_reset(void);

class FflushTest : public evil::Test {
public:
    char _heap[4096];
    FILE *_f;

    virtual void SetUp() {
        evil::Test::SetUp();

        EXPECT_CALL(_syscalls, _isatty(_))
            .WillOnce(Return(0));
        EXPECT_CALL(_syscalls, _sbrk(4096))
            .WillOnce(Return(_heap));
        EXPECT_CALL(_syscalls, _open(_, _, _))
            .WillOnce(Return(3));

        _f = test_fopen("", "w");
        ASSERT_NE(nullptr, _f);
    }

    virtual void TearDown() {
        EXPECT_CALL(_syscalls, _close(3))
            .WillOnce(Return(0));
        EXPECT_EQ(0, test_fclose(_f));

        __evil_malloc_reset();
        evil::Test::TearDown();
    }
};

TEST_F(FflushTest, null) {
    evil::UBChecker ub{1};
    EXPECT_EQ(EVIL_EOF, test_fflush(nullptr));
}

TEST_F(FflushTest, nothing_to_do) {
    EXPECT_EQ(0, test_fflush(_f));
}

TEST_F(FflushTest, failed_write) {
    const char data[] = "foo";
    EXPECT_EQ(sizeof(data), test_fwrite(data, sizeof(data), 1, _f));

    EXPECT_CALL(_syscalls, _write(3, _, 4))
        .WillOnce(Return(-1));

    EXPECT_EQ(EVIL_EOF, test_fflush(_f));
    EXPECT_NE(0, test_ferror(_f));
}

TEST_F(FflushTest, short_write) {
    const char data[] = "foo";
    EXPECT_EQ(sizeof(data), test_fwrite(data, sizeof(data), 1, _f));

    EXPECT_CALL(_syscalls, _write(3, _, 4))
        .WillOnce(Return(2));

    EXPECT_EQ(EVIL_EOF, test_fflush(_f));
    EXPECT_NE(0, test_ferror(_f));
}
