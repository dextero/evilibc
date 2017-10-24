#include "Test.h"

#include <unistd.h>

using namespace ::testing;

extern "C" void __evil_init(void);
extern "C" void __evil_cleanup(void);

namespace evil {

namespace {

ssize_t do_write(int fd, const void *p, size_t n) {
    return write(fd, p, n);
}

} // namespace

Test::Test() {
    evil::SyscallsMock syscalls;

    EXPECT_CALL(syscalls, _isatty(_))
        .Times(3)
        .WillRepeatedly(Return(1));

    EXPECT_CALL(syscalls, _access(_, _))
        .WillOnce(Return(0));

    EXPECT_CALL(syscalls, _open("/dev/stdin", _, _))
        .WillOnce(Return(0));
    EXPECT_CALL(syscalls, _open("/dev/stdout", _, _))
        .WillOnce(Return(1));
    EXPECT_CALL(syscalls, _open("/dev/stderr", _, _))
        .WillOnce(Return(2));

    EXPECT_CALL(_syscalls, _write(1, _, _))
        .WillRepeatedly(Invoke(do_write));
    EXPECT_CALL(_syscalls, _write(2, _, _))
        .WillRepeatedly(Invoke(do_write));

    __evil_init();
}

Test::~Test() {
    evil::SyscallsMock syscalls;

    EXPECT_CALL(syscalls, _close(_))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(syscalls, _write(_, _, _))
        .WillRepeatedly(ReturnArg<2>());

    __evil_cleanup();
}

} // namespace evil
