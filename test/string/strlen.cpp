#include "Test.h"

extern "C" size_t test_strlen(const char *);

class StrlenTest : public evil::Test {};

TEST_F(StrlenTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(0, test_strlen(NULL));
    }
}

TEST_F(StrlenTest, basic) {
    ASSERT_EQ(0, test_strlen(""));
    ASSERT_EQ(3, test_strlen("abc"));
    ASSERT_EQ(1, test_strlen("a\0c"));
}
