#include "Test.h"

#include <string>

#include "../../include/stdarg.h"

using namespace std;
using namespace testing;

#define EVIL_IOFBF (-0xfbf)
extern "C" int test_setvbuf(FILE *, char *, int, size_t);
extern "C" int test_vfprintf(FILE *, const char *, va_list);
extern "C" int test_fflush(FILE *);
extern "C" FILE *__evil_stdout;

namespace {

int vfprintf_wrapper(FILE *f, const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    int result = test_vfprintf(f, fmt, list);
    va_end(list);
    return result;
}

} // namespace

class VfprintfTest : public evil::Test {};

MATCHER_P(BufferMatches, str, "") {
    return memcmp(str.c_str(), arg, str.size()) == 0;
}

TEST_F(VfprintfTest, success) {
    char stdout_buffer[4096];
    EXPECT_EQ(0, test_setvbuf(__evil_stdout, stdout_buffer,
                              EVIL_IOFBF, sizeof(stdout_buffer)));

    static string OUTPUT = "abc: 123, %, foo";

    EXPECT_CALL(_syscalls, _write(1, BufferMatches(OUTPUT),
                                  (ssize_t) OUTPUT.size()))
        .WillOnce(Return(OUTPUT.size()));

    EXPECT_EQ(OUTPUT.size(),
              vfprintf_wrapper(__evil_stdout, "%s: %d, %%, foo", "abc", 123));
    EXPECT_EQ(0, test_fflush(__evil_stdout));
}

TEST_F(VfprintfTest, failing_write) {
    {
        EXPECT_CALL(_syscalls, _write(1, _, _))
            .WillOnce(Return(-1));

        EXPECT_LE(vfprintf_wrapper(__evil_stdout, "x%s", ""),
                  0);
        EXPECT_EQ(0, test_fflush(__evil_stdout));

        Mock::VerifyAndClear(&_syscalls);
    }

    {
        EXPECT_CALL(_syscalls, _write(1, _, _))
            .WillOnce(Return(-1));

        EXPECT_LE(vfprintf_wrapper(__evil_stdout, "%s", "x"),
                  0);
        EXPECT_EQ(0, test_fflush(__evil_stdout));

        Mock::VerifyAndClear(&_syscalls);
    }

    {
        EXPECT_CALL(_syscalls, _write(1, _, _))
            .WillOnce(Return(-1));

        EXPECT_LE(vfprintf_wrapper(__evil_stdout, "%sx", ""),
                  0);
        EXPECT_EQ(0, test_fflush(__evil_stdout));

        Mock::VerifyAndClear(&_syscalls);
    }
}
