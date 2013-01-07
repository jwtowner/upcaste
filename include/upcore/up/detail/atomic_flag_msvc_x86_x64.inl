//
//  Upcaste Performance Libraries
//  Copyright (C) 2012-2013 Jesse W. Towner
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef UP_DETAIL_ATOMIC_FLAG_MSVC_X86_X64_INL
#define UP_DETAIL_ATOMIC_FLAG_MSVC_X86_X64_INL

#ifndef UP_ATOMIC_HPP
#   error "Do not include this file directly, instead include <up/atomic.hpp>"
#endif

#ifndef ATOMIC_FLAG_INIT
#   define ATOMIC_FLAG_INIT 0
#endif

#define UP_DETAIL_ATOMIC_FLAG_OPERATIONS(Volatile) \
    UPALWAYSINLINE \
    void clear(memory_order order = memory_order_seq_cst) Volatile noexcept { \
        if (order != memory_order_seq_cst) { \
            _ReadWriteBarrier(); \
            state_ = 0; \
        } \
        else { \
            _InterlockedExchange8(&state_, 0); \
        } \
    } \
    UPALWAYSINLINE \
    bool test_and_set(memory_order = memory_order_seq_cst) Volatile noexcept { \
        return _InterlockedCompareExchange8(&state_, 1, 0) == 1; \
    } \
    UPALWAYSINLINE \
    bool test_test_and_set(memory_order order = memory_order_seq_cst) Volatile noexcept { \
        if (order != memory_order_seq_cst) { \
            char result = state_; \
            _ReadWriteBarrier(); \
            if (result == 1) { \
                return true; \
            } \
        } \
        return test_and_set(order); \
    }

namespace up
{
    struct LIBUPCOREAPI atomic_flag
    {
        UPNONCOPYABLE(atomic_flag);

    public:
        
        UPDEFAULTCTOR(atomic_flag);
        UPALWAYSINLINE UPCONSTEXPR atomic_flag(bool state) noexcept : state_(state ? 1 : 0) { }
        UP_DETAIL_ATOMIC_FLAG_OPERATIONS(UP_DETAIL_NOT_VOLATILE)
        UP_DETAIL_ATOMIC_FLAG_OPERATIONS(UP_DETAIL_VOLATILE)

    private:

        char volatile state_;
    };
}

#undef UP_DETAIL_ATOMIC_FLAG_OPERATIONS

#endif
