#include "Test.h"

#include <string>

using namespace std;

extern "C" char *test_strcpy(char *, const char *);

class StrcpyTest : public evil::Test {};

TEST_F(StrcpyTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strcpy(NULL, ""));
    }
    {
        evil::UBChecker checker{1};
        string buf = "";
        ASSERT_EQ(&buf[0], test_strcpy(&buf[0], NULL));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strcpy(NULL, NULL));
    }
}

TEST_F(StrcpyTest, overlapping_right) {
    evil::UBChecker checker{1};
    string buf = "1234";
    ASSERT_EQ(&buf[1], test_strcpy(&buf[1], &buf[0]));
    ASSERT_EQ("1234", buf);
}

TEST_F(StrcpyTest, overlapping_left) {
    evil::UBChecker checker{1};
    string buf = "1234";
    ASSERT_EQ(&buf[0], test_strcpy(&buf[0], &buf[1]));
    ASSERT_EQ("1234", buf);
}

TEST_F(StrcpyTest, basic) {
    string src = "1234";
    char dst[src.size() + 1];
    memset(dst, 1, sizeof(dst)); // make sure there's no nullbytes

    ASSERT_EQ(&dst[0], test_strcpy(&dst[0], &src[0]));
    ASSERT_EQ(src, string(dst));
}
