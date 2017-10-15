#include "Test.h"

#include <string>

using namespace std;

extern "C" int test_snprintf(char *, size_t, const char *, ...);

class SnprintfTest : public evil::Test {};

TEST_F(SnprintfTest, no_args) {
    char dst[sizeof("abc")];

    ASSERT_EQ(3, test_snprintf(dst, sizeof(dst), "abc"));
    ASSERT_EQ("abc"s, string(dst));
}

TEST_F(SnprintfTest, format_string) {
    char dst[sizeof("abcAB")];

    ASSERT_EQ(5, test_snprintf(dst, sizeof(dst), "AB%s", "abc"));
    ASSERT_EQ("ABabc"s, string(dst));

    ASSERT_EQ(5, test_snprintf(dst, sizeof(dst), "A%sB", "abc"));
    ASSERT_EQ("AabcB"s, string(dst));

    ASSERT_EQ(5, test_snprintf(dst, sizeof(dst), "%sAB", "abc"));
    ASSERT_EQ("abcAB"s, string(dst));
}
