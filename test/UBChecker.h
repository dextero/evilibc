#ifndef __EVIL_TEST_UB_CHECKER_H
#define __EVIL_TEST_UB_CHECKER_H

#include <cstddef>
#include <cstdio>

#include <vector>

#include <gtest/gtest.h>

#include "test_utils.h"

namespace evil {

struct UBHandlerContext
{
    evil_ub_handler_t *handler;
    void *handler_data;
};

class UBChecker
{
    static std::vector<UBHandlerContext> _handler_ctx_stack;
        
    size_t _ub_count = 0;
    size_t _expected_ub_count;

    static void update_handler()
    {
        __evil_set_ub_handler(_handler_ctx_stack.back().handler,
                              _handler_ctx_stack.back().handler_data);
    }

public:
    UBChecker(size_t expected_ub_count = 0)
        : _expected_ub_count(expected_ub_count)
    {
        auto handler = [](void *self_,
                          const char *reason_fmt,
                          va_list reason_args) {
            UBChecker *self = (UBChecker *)self_;
            if (self->_ub_count >= self->_expected_ub_count) {
                fprintf(stderr, "unexpected UB triggered: ");
                vfprintf(stderr, reason_fmt, reason_args);
            }

            EXPECT_LT(self->_ub_count, self->_expected_ub_count);
            ++self->_ub_count;
        };

        _handler_ctx_stack.emplace_back(UBHandlerContext{handler, this});
        update_handler();
    }

    ~UBChecker()
    {
        if (_ub_count != _expected_ub_count) {
            fprintf(stderr, "UB hander was not called, but it should\n");
        }

        EXPECT_EQ(_expected_ub_count, _ub_count);
        _handler_ctx_stack.pop_back();
        update_handler();
    }
};

} // namespace evil

#endif // __EVIL_TEST_UB_CHECKER_H
