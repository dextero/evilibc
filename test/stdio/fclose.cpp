#include "Test.h"

using namespace ::testing;

#define EVIL_EOF (-0xE0F)

extern "C" FILE* test_fopen(const char* filename,
                            const char* mode);
extern "C" int test_fwrite(const void *data,
                           size_t size,
                           size_t nmemb,
                           FILE *f);
extern "C" int test_fclose(FILE *f);

class FcloseTest : public evil::Test {};

TEST_F(FcloseTest, null) {
    evil::UBChecker ub{1};
    EXPECT_EQ(EVIL_EOF, test_fclose(nullptr));
}

TEST_F(FcloseTest, failing_close) {
    EXPECT_CALL(_syscalls, _isatty(_))
        .WillOnce(Return(1));
    EXPECT_CALL(_syscalls, _access(_, _))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open(_, _, _))
        .WillOnce(Return(3));

    FILE *f = test_fopen("", "r");
    EXPECT_NE(nullptr, f);

    EXPECT_CALL(_syscalls, _close(3))
        .WillOnce(Return(-1));

    EXPECT_EQ(EVIL_EOF, test_fclose(f));
}

TEST_F(FcloseTest, failing_flush) {
    EXPECT_CALL(_syscalls, _isatty(_))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open(_, _, _))
        .WillOnce(Return(3));

    FILE *f = test_fopen("", "w");
    test_fwrite("a", 1, 1, f);

    EXPECT_CALL(_syscalls, _write(_, _, _))
        .WillOnce(Return(-1));
    EXPECT_CALL(_syscalls, _close(3))
        .WillOnce(Return(0));

    EXPECT_EQ(EVIL_EOF, test_fclose(f));
}
