#include "Test.h"

#include <string>

using namespace std;

extern "C" char *test_strncpy(char *, const char *, size_t);

class StrncpyTest : public evil::Test {};

TEST_F(StrncpyTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strncpy(NULL, "", 0));
    }
    {
        evil::UBChecker checker{1};
        string buf = "";
        ASSERT_EQ(&buf[0], test_strncpy(&buf[0], NULL, 0));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strncpy(NULL, NULL, 0));
    }
}

TEST_F(StrncpyTest, overlapping_right) {
    evil::UBChecker checker{1};
    string buf = "1234";
    ASSERT_EQ(&buf[1], test_strncpy(&buf[1], &buf[0], buf.size() + 1));
    ASSERT_EQ("1234", buf);
}

TEST_F(StrncpyTest, overlapping_left) {
    evil::UBChecker checker{1};
    string buf = "1234";
    ASSERT_EQ(&buf[0], test_strncpy(&buf[0], &buf[1], buf.size() + 1));
    ASSERT_EQ("1234", buf);
}

TEST_F(StrncpyTest, basic) {
    string src = "1234";
    char dst[src.size() + 1];
    memset(dst, 1, sizeof(dst)); // make sure there's no nullbytes

    ASSERT_EQ(&dst[0], test_strncpy(&dst[0], &src[0], sizeof(dst)));
    ASSERT_EQ(src, string(dst));
}

TEST_F(StrncpyTest, destination_too_small) {
    string src = "1234";
    char dst[src.size() + 1];
    memset(dst, 'A', sizeof(dst)); // make sure there's no nullbytes

    ASSERT_EQ(&dst[0], test_strncpy(&dst[0], &src[0], 2));
    ASSERT_EQ("12AAA"s, string(&dst[0], &dst[sizeof(dst)]));
}

TEST_F(StrncpyTest, destination_too_big) {
    string src = "12";
    char dst[src.size() + 3];
    memset(dst, 'A', sizeof(dst)); // make sure there's no nullbytes

    ASSERT_EQ(&dst[0], test_strncpy(&dst[0], &src[0], sizeof(dst)));
    ASSERT_EQ("12\0\0\0"s, string(&dst[0], &dst[sizeof(dst)]));
}
