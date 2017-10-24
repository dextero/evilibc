#include "Test.h"

#include <string>

using namespace std;
using namespace testing;

#define EVIL_FOPEN_MAX 8

extern "C" void __evil_init(void);
extern "C" void __evil_fclose_all(void);

extern "C" FILE* test_fopen(const char* filename,
                            const char* mode);

class FopenTest : public evil::Test {
    virtual void EnableStandardIOStreams() {}
};

TEST_F(FopenTest, invalid_mode) {
    {
        evil::UBChecker ub{1};
        ASSERT_EQ(nullptr, test_fopen("path", nullptr));
    }
    {
        evil::UBChecker ub{1};
        ASSERT_EQ(nullptr, test_fopen("path", ""));
    }
    {
        evil::UBChecker ub{1};
        ASSERT_EQ(nullptr, test_fopen("path", "?"));
    }
    {
        evil::UBChecker ub{1};
        ASSERT_EQ(nullptr, test_fopen("path", "+"));
    }
    {
        evil::UBChecker ub{1};
        ASSERT_EQ(nullptr, test_fopen("path", "b"));
    }
}

TEST_F(FopenTest, read_inaccessible) {
    EXPECT_CALL(_syscalls, _access("path", EVIL_R_OK))
        .WillOnce(Return(-1));

    ASSERT_EQ(nullptr, test_fopen("path", "r"));
}

TEST_F(FopenTest, open_failed) {
    EXPECT_CALL(_syscalls, _access("path", _))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open("path", _, _))
        .WillOnce(Return(-1));

    ASSERT_EQ(nullptr, test_fopen("path", "r"));
}

TEST_F(FopenTest, buffered) {
    EXPECT_CALL(_syscalls, _isatty(_))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _access("path", _))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open("path", _, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "r"));
}

TEST_F(FopenTest, file_limit) {
    EXPECT_CALL(_syscalls, _isatty(_))
        .WillRepeatedly(Return(1));
    EXPECT_CALL(_syscalls, _access("path", _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(_syscalls, _open("path", _, _))
        .WillRepeatedly(Return(3));

    for (size_t i = 0; i < EVIL_FOPEN_MAX; ++i) {
        ASSERT_NE(nullptr, test_fopen("path", "r"));
    }
    ASSERT_EQ(nullptr, test_fopen("path", "r"));
}

class TerminalFopenTest : public FopenTest {
    virtual void SetUp() {
        EXPECT_CALL(_syscalls, _isatty(_))
            .WillOnce(Return(1));
        FopenTest::SetUp();
    }
};

TEST_F(TerminalFopenTest, mode_r) {
    EXPECT_CALL(_syscalls, _access("path", EVIL_R_OK))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open("path", EVIL_O_RDONLY, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "r"));
}

TEST_F(TerminalFopenTest, mode_w) {
    EXPECT_CALL(_syscalls, _open("path", EVIL_O_WRONLY | EVIL_O_TRUNC | EVIL_O_CREAT, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "w"));
}

TEST_F(TerminalFopenTest, mode_r_plus) {
    EXPECT_CALL(_syscalls, _access("path", EVIL_R_OK))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open("path", EVIL_O_RDWR, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "r+"));
}

TEST_F(TerminalFopenTest, mode_a) {
    EXPECT_CALL(_syscalls, _open("path", EVIL_O_WRONLY | EVIL_O_APPEND | EVIL_O_CREAT, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "a"));
}

TEST_F(TerminalFopenTest, mode_rx) {
    EXPECT_CALL(_syscalls, _access("path", EVIL_R_OK))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open("path", EVIL_O_RDONLY | EVIL_O_EXCL, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "rx"));
}

TEST_F(TerminalFopenTest, mode_rb) {
    EXPECT_CALL(_syscalls, _access("path", EVIL_R_OK))
        .WillOnce(Return(0));
    EXPECT_CALL(_syscalls, _open("path", EVIL_O_RDONLY, _))
        .WillOnce(Return(3));

    ASSERT_NE(nullptr, test_fopen("path", "rb"));
}
