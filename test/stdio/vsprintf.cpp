#include "Test.h"

#include <string>

#include "../../include/stdarg.h"

using namespace std;

extern "C" int test_vsprintf(char *, const char *, ...);

namespace {

int vsprintf_wrapper(char *buf, const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    int result = test_vsprintf(buf, fmt, list);
    va_end(list);
    return result;
}

} // namespace

class VsprintfTest : public evil::Test {};

TEST_F(VsprintfTest, no_args) {
    char dst[16];

    EXPECT_EQ(11, vsprintf_wrapper(dst, "%s: %d, %%", "abc", 123));
    EXPECT_EQ("abc: 123, %"s, string(dst));
}
