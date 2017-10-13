#include "Test.h"

#include <string>
#include <limits>

using namespace std;

extern "C" char *test_strerror(int);

class StrerrorTest : public evil::Test {};

TEST_F(StrerrorTest, resuses_buffer) {
    char *result = test_strerror(0);
    ASSERT_NE(nullptr, result);

    for (int n = 0; n < 0x1000; ++n) {
        ASSERT_EQ(result, test_strerror(n));
    }

    ASSERT_EQ(result, test_strerror(numeric_limits<int>::min()));
    ASSERT_EQ(result, test_strerror(numeric_limits<int>::max()));
}
