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

#include <up/atomic.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    void atomic_flag_spin_lock_explicit(atomic_flag* flag, memory_order order) noexcept {
#ifndef UP_HAS_STDCXX_ATOMIC
        if (flag->test_and_set(order)) {
            do {
                atomic_yield();
            }
            while (flag->test_test_and_set(order));
        }
#else
        while (flag->test_and_set(order)) {
            atomic_yield();
        }
#endif
    }

    LIBUPCOREAPI UPNONNULLALL
    void atomic_flag_spin_lock_explicit(atomic_flag volatile* flag, memory_order order) noexcept {
#ifndef UP_HAS_STDCXX_ATOMIC
        if (flag->test_and_set(order)) {
            do {
                atomic_yield();
            }
            while (flag->test_test_and_set(order));
        }
#else
        while (flag->test_and_set(order)) {
            atomic_yield();
        }
#endif
    }
}
