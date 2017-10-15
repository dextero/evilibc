#ifndef __EVIL_TEST_TEST_H
#define __EVIL_TEST_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "UBChecker.h"
#include "IDBChecker.h"

namespace evil {

class Test : public ::testing::Test
{
    UBChecker _disallow_ub;
    IDBChecker _disallow_idb;
};

} // namespace evil

#endif // __EVIL_TEST_TEST_H
