#include "Test.h"

#include <string>

using namespace std;

extern "C" int test_sprintf(char *, const char *, ...);

class SprintfTest : public evil::Test {};

TEST_F(SprintfTest, no_args) {
    char dst[16];

    EXPECT_EQ(11, test_sprintf(dst, "%s: %d, %%", "abc", 123));
    EXPECT_EQ("abc: 123, %"s, string(dst));
}
