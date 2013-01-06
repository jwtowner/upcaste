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

#ifndef UP_DETAIL_CTHREADS_C11_INL
#define UP_DETAIL_CTHREADS_C11_INL

#ifndef UP_CTHREADS_HPP
#   error "Do not include this header file directly! Instead include <up/cthreads.hpp>"
#endif

#include <threads.h>

namespace up
{
    constexpr int mtx_plain = ::mtx_plain;
	constexpr int mtx_timed = ::mtx_timed;
	constexpr int mtx_recursive = ::mtx_recursive;
    constexpr int thrd_success = ::thrd_success;
    constexpr int thrd_nomem = ::thrd_nomem;
	constexpr int thrd_timedout = ::thrd_timedout;
    constexpr int thrd_busy = ::thrd_busy;
    constexpr int thrd_error = ::thrd_error;
    using ::cnd_t;
    using ::mtx_t;
    using ::thrd_t;
    using ::thrd_start_t;
    using ::tss_t;
    using ::tss_dtor_t;
    using ::once_flag;
    using ::call_once;
    using ::cnd_broadcast;
    using ::cnd_destroy;
    using ::cnd_init;
    using ::cnd_signal;
    using ::cnd_timedwait;
    using ::cnd_wait;
    using ::mtx_destroy;
    using ::mtx_init;
    using ::mtx_lock;
    using ::mtx_timedlock;
    using ::mtx_trylock;
    using ::mtx_unlock;
    using ::thrd_create;
    using ::thrd_current;
    using ::thrd_detach;
    using ::thred_equal;
    using ::thrd_join;
    using ::thrd_sleep;
    using ::thrd_yield;
    using ::tss_create;
    using ::tss_delete;
    using ::tss_get;
    using ::tss_set;
}

#endif
