#include "Test.h"

#include <string>

using namespace std;
using namespace std::literals;

extern "C" char *test_strtok(char *, const char *);

class StrtokTest : public evil::Test {};

TEST_F(StrtokTest, null_str) {
    evil::UBChecker checker{1};
    ASSERT_EQ(NULL, test_strtok(NULL, " "));
}

TEST_F(StrtokTest, null_delim) {
    evil::UBChecker checker{1};
    string s = "a";
    ASSERT_EQ(NULL, test_strtok(&s[0], NULL));
}

TEST_F(StrtokTest, empty_str_and_delim) {
    string s = "";
    ASSERT_EQ(NULL, test_strtok(&s[0], ""));
}

TEST_F(StrtokTest, delim_only) {
    string s = "a";
    ASSERT_EQ(NULL, test_strtok(&s[0], "a"));
}

TEST_F(StrtokTest, multiple_delim_only) {
    string s = "aa";
    ASSERT_EQ(NULL, test_strtok(&s[0], "a"));
}

TEST_F(StrtokTest, one_token) {
    string s = "a";
    ASSERT_EQ(&s[0], test_strtok(&s[0], "b"));
    ASSERT_EQ(NULL, test_strtok(NULL, "b"));
}

TEST_F(StrtokTest, two_tokens) {
    string s = "a b";
    ASSERT_EQ(&s[0], test_strtok(&s[0], " "));
    ASSERT_EQ(&s[2], test_strtok(NULL, " "));
    ASSERT_EQ("a\0b"s, s);
}

TEST_F(StrtokTest, duplicate_delim) {
    string s = "a b";
    ASSERT_EQ(&s[0], test_strtok(&s[0], "  "));
    ASSERT_EQ(&s[2], test_strtok(NULL, "  "));
    ASSERT_EQ("a\0b"s, s);
}

TEST_F(StrtokTest, changing_delim) {
    string s = "a b\tc";
    ASSERT_EQ(&s[0], test_strtok(&s[0], " "));
    ASSERT_EQ(&s[2], test_strtok(NULL, "\t"));
    ASSERT_EQ(&s[4], test_strtok(NULL, "\n"));
    ASSERT_EQ(NULL, test_strtok(NULL, " "));
    ASSERT_EQ("a\0b\0c"s, s);
}

TEST_F(StrtokTest, multiple_delims) {
    string s = "a b\tc";
    ASSERT_EQ(&s[0], test_strtok(&s[0], " \t"));
    ASSERT_EQ(&s[2], test_strtok(NULL, " \t"));
    ASSERT_EQ(&s[4], test_strtok(NULL, " \t"));
    ASSERT_EQ(NULL, test_strtok(NULL, " \t"));
    ASSERT_EQ("a\0b\0c"s, s);
}
