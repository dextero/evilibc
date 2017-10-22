#ifndef __EVIL_TEST_TEST_H
#define __EVIL_TEST_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "UBChecker.h"
#include "IDBChecker.h"
#include "SyscallsMock.h"

namespace evil {

class Test : public ::testing::Test
{
    UBChecker _disallow_ub;
    IDBChecker _disallow_idb;

protected:
    evil::SyscallsMock _syscalls;
};

} // namespace evil

#endif // __EVIL_TEST_TEST_H
