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

#ifndef UP_DETAIL_CTHREADS_XTHREADS_INL
#define UP_DETAIL_CTHREADS_XTHREADS_INL

#ifndef UP_CTHREADS_HPP
#   error "Do not include this header file directly! Instead include <up/cthreads.hpp>"
#endif

#include <thr/xthreads.h>

#define ONCE_FLAG_INIT _ONCE_FLAG_INIT
#define TSS_DTOR_ITERATIONS _TSS_DTOR_ITERATIONS

namespace up
{
    constexpr int thrd_success = ::_Thrd_success;
    constexpr int thrd_nomem = ::_Thrd_nomem;
	constexpr int thrd_timedout = ::_Thrd_timedout;
    constexpr int thrd_busy = ::_Thrd_busy;
    constexpr int thrd_error = ::_Thrd_error;
    typedef ::_Thrd_t thrd_t;
    typedef ::_Thrd_start_t thrd_start_t;
    inline UPALWAYSINLINE int thrd_create(thrd_t* thr, thrd_start_t func, void* arg) noexcept { return ::_Thrd_create(thr, func, arg); }
    inline UPALWAYSINLINE int thrd_detach(thrd_t thr) noexcept { return ::_Thrd_detach(thr); }
    inline UPALWAYSINLINE int thrd_join(thrd_t thr, int* res) noexcept { return ::_Thrd_join(thr, res); }
    inline UPALWAYSINLINE int thrd_equal(thrd_t thr0, thrd_t thr1) noexcept { return ::_Thrd_equal(thr0, thr1); }
    inline UPALWAYSINLINE int thrd_lt(thrd_t thr0, thrd_t thr1) { return ::_Thrd_lt(thr0, thr1); }
    inline UPALWAYSINLINE UPPURE thrd_t thrd_current() noexcept { return ::_Thrd_current(); }
    inline UPALWAYSINLINE UPNORETURN void thrd_exit(int res) noexcept { ::_Thrd_exit(res); }
    inline UPALWAYSINLINE void thrd_yield() noexcept { return ::_Thrd_yield(); }
    inline UPALWAYSINLINE void thrd_sleep(timespec const* duration) noexcept { ::_Thrd_sleep(reinterpret_cast<xtime const*>(duration)); }
    extern LIBUPCOREAPI int thrd_sleep(timespec const* duration, timespec* remaining) noexcept;

    typedef ::_Once_flag once_flag;
    inline UPALWAYSINLINE void call_once(once_flag* flag, void (UPCDECL *func)(void)) noexcept { ::_Call_once(flag, func); }

    constexpr int mtx_plain = ::_Mtx_plain;
	constexpr int mtx_try = ::_Mtx_try;
	constexpr int mtx_timed = ::_Mtx_timed;
	constexpr int mtx_recursive = ::_Mtx_recursive;
    typedef ::_Mtx_t mtx_t;
    inline UPALWAYSINLINE void mtx_destroy(mtx_t* mtx) noexcept { ::_Mtx_destroy(mtx); }
    inline UPALWAYSINLINE int mtx_init(mtx_t* mtx, int type) noexcept { return ::_Mtx_init(mtx, type); }
    inline UPALWAYSINLINE int mtx_lock(mtx_t* mtx) noexcept { return ::_Mtx_lock(mtx); }
    inline UPALWAYSINLINE int mtx_trylock(mtx_t* mtx) noexcept { return ::_Mtx_trylock(mtx); }
    inline UPALWAYSINLINE int mtx_timedlock(mtx_t* UPRESTRICT mtx, timespec const* UPRESTRICT ts) noexcept { return ::_Mtx_timedlock(mtx, reinterpret_cast<xtime const*>(ts)); }
    inline UPALWAYSINLINE int mtx_unlock(mtx_t* mtx) noexcept { return ::_Mtx_unlock(mtx); }

    typedef ::_Cnd_t cnd_t;
    inline UPALWAYSINLINE void cnd_destroy(cnd_t* cond) noexcept { ::_Cnd_destroy(cond); }
    inline UPALWAYSINLINE int cnd_init(cnd_t* cond) noexcept { return ::_Cnd_init(cond); }
    inline UPALWAYSINLINE int cnd_broadcast(cnd_t* cond) noexcept { return ::_Cnd_broadcast(cond); }
    inline UPALWAYSINLINE int cnd_signal(cnd_t* cond) noexcept { return ::_Cnd_signal(cond); }
    inline UPALWAYSINLINE int cnd_wait(cnd_t* UPRESTRICT cond, mtx_t* UPRESTRICT mtx) noexcept { return ::_Cnd_wait(cond, mtx); }
    inline UPALWAYSINLINE int cnd_timedwait(cnd_t* UPRESTRICT cond, mtx_t* UPRESTRICT mtx, timespec const* UPRESTRICT ts) noexcept { return ::_Cnd_timedwait(cond, mtx, reinterpret_cast<xtime const*>(ts)); }

    typedef ::_Tss_t tss_t;
    typedef ::_Tss_dtor_t tss_dtor_t;
    inline UPALWAYSINLINE int tss_create(tss_t* key, tss_dtor_t dtor) noexcept { return ::_Tss_create(key, dtor); }
    inline UPALWAYSINLINE void tss_delete(tss_t key) noexcept { ::_Tss_delete(key); }
    inline UPALWAYSINLINE int tss_set(tss_t key, void* val) noexcept { return ::_Tss_set(key, val); }
    inline UPALWAYSINLINE void* tss_get(tss_t key) noexcept { return ::_Tss_get(key); }
}

#endif
