#include "Test.h"

using namespace ::testing;

extern "C" FILE* test_fopen(const char* filename,
                            const char* mode);
extern "C" int test_fwrite(const void *data,
                           size_t size,
                           size_t nmemb,
                           FILE *f);
extern "C" int test_fclose(FILE *f);

class FwriteTest : public evil::Test {
public:
    FILE *_f;

    virtual void SetUp() {
        evil::Test::SetUp();

        EXPECT_CALL(_syscalls, _isatty(_))
            .WillOnce(Return(1));
        EXPECT_CALL(_syscalls, _open(_, _, _))
            .WillOnce(Return(3));

        _f = test_fopen("", "w");
        ASSERT_NE(nullptr, _f);
    }

    virtual void TearDown() {
        EXPECT_CALL(_syscalls, _close(3))
            .WillOnce(Return(0));
        EXPECT_EQ(0, test_fclose(_f));

        evil::Test::TearDown();
    }
};

TEST_F(FwriteTest, zero) {
    ASSERT_EQ(0, test_fwrite("", 0, 1, _f));
    ASSERT_EQ(0, test_fwrite("", 1, 0, _f));
}

TEST_F(FwriteTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(0, test_fwrite(nullptr, 0, 0, _f));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(0, test_fwrite("", 0, 0, nullptr));
    }
}

TEST_F(FwriteTest, first_block_failed) {
    EXPECT_CALL(_syscalls, _write(_, _, 1))
        .Times(6)
        .WillOnce(Return(-1))
        .WillRepeatedly(Return(1));

    ASSERT_EQ(2, test_fwrite("123456", 2, 3, _f));
}
