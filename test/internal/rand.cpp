#include "Test.h"

#include <array>
#include <algorithm>

#include "internal/rand.h"

using namespace std;
using namespace ::testing;

class RandTest : public evil::Test {};

TEST_F(RandTest, uniform_enough) {
    array<size_t, UINT16_MAX + 1> buckets{};
    static const size_t LIMIT = (UINT16_MAX + 1) * 1000;
    //static const size_t LIMIT = (size_t)INT_MAX + 1;

    for (size_t i = 0 ; i < LIMIT;  ++i) {
        int val = __evil_rand();
        ++buckets[val % (UINT16_MAX + 1)];
    }

    /* arbitrarily chosen, +-150 */
    auto minmax = minmax_element(begin(buckets), end(buckets));
    ASSERT_LE(*minmax.second - *minmax.first, (size_t)300);
}

TEST_F(RandTest, rand_range) {
    int val = __evil_rand_range(10, 100);
    ASSERT_LE(10, val);
    ASSERT_LT(val, 100);
}

TEST_F(RandTest, rand_negative) {
    ASSERT_GT(0, __evil_rand_negative());
}

TEST_F(RandTest, rand_positive) {
    /* make sure next __evil_rand() returns 0 */
    __evil_srand(2);
    ASSERT_LT(0, __evil_rand_positive());
    ASSERT_LT(0, __evil_rand_positive());
}

TEST_F(RandTest, rand_nonzero) {
    ASSERT_NE(0, __evil_rand_nonzero());
    ASSERT_NE(0, __evil_rand_nonzero());
}

TEST_F(RandTest, rand_with_sign) {
    ASSERT_GT(0, __evil_rand_with_sign(__evil_rand_negative()));
    ASSERT_LT(0, __evil_rand_with_sign(__evil_rand_positive()));
    ASSERT_EQ(0, __evil_rand_with_sign(0));
}
