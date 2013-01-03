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

#ifndef UP_DETAIL_CTHREADS_PTHREADS_INL
#define UP_DETAIL_CTHREADS_PTHREADS_INL

#ifndef UP_CTHREADS_HPP
#   error "Do not include this header file directly! Instead include <up/cthreads.hpp>"
#endif

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <errno.h>

#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS

namespace up
{
    constexpr int thrd_success = 0;
    constexpr int thrd_busy = 1;
    constexpr int thrd_error = 2;
    constexpr int thrd_nomem = 3;
	constexpr int thrd_timedout = 4;
    typedef ::pthread_t thrd_t;
    typedef int (UPCDECL* thrd_start_t)(void*);
    inline UPALWAYSINLINE int thrd_create(thrd_t* thr, thrd_start_t func, void* arg) noexcept { return ::pthread_create(thr, 0, (void*(UPCDECL*)(void*))func, arg) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE int thrd_detach(thrd_t thr) noexcept { return ::pthread_detach(thr) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE int thrd_join(thrd_t thr, int* res) noexcept { void* p; if (::pthread_join(thr, &p)) { return thrd_error; } if (res) { *res = (long)p; } return thrd_success; }
    inline UPALWAYSINLINE int thrd_equal(thrd_t thr0, thrd_t thr1) noexcept { return ::pthread_equal(thr0, thr1); }
    inline UPALWAYSINLINE int thrd_lt(thrd_t thr0, thrd_t thr1) { return !::pthread_equal(thr0, thr1) && (thr0 < thr1); }
    inline UPALWAYSINLINE UPPURE thrd_t thrd_current() noexcept { return ::pthread_self(); }
    inline UPALWAYSINLINE UPNORETURN void thrd_exit(int res) noexcept { ::pthread_exit((void*)((long)res)); }
    inline UPALWAYSINLINE void thrd_yield() noexcept { ::sched_yield(); }
    extern LIBUPCOREAPI void thrd_sleep(timespec const* duration) noexcept;
    inline UPALWAYSINLINE int thrd_sleep(timespec const* UPRESTRICT duration, timespec* UPRESTRICT remaining) noexcept { return ::nanosleep(duration, remaining) ? thrd_error : thrd_success; }

    typedef ::pthread_once_t once_flag;
    inline UPALWAYSINLINE void call_once(once_flag* flag, void (UPCDECL *func)(void)) noexcept { ::pthread_once(flag, func); }

    constexpr int mtx_plain = 0;
	constexpr int mtx_try = 1;
	constexpr int mtx_timed = 2;
	constexpr int mtx_recursive = 4;
    typedef ::pthread_mutex_t mtx_t;
    extern LIBUPCOREAPI int mtx_init(mtx_t* mtx, int type) noexcept;
    inline UPALWAYSINLINE void mtx_destroy(mtx_t* mtx) noexcept { ::pthread_mutex_destroy(mtx); }
    inline UPALWAYSINLINE int mtx_lock(mtx_t* mtx) noexcept { int r = ::pthread_mutex_lock(mtx); return !r ? thrd_success : ((r == EDEADLK) ? thrd_busy : thrd_error);  }
    inline UPALWAYSINLINE int mtx_trylock(mtx_t* mtx) noexcept { int r = ::pthread_mutex_trylock(mtx); return !r ? thrd_success : ((r == EBUSY) ? thrd_busy : thrd_error); }
    inline UPALWAYSINLINE int mtx_timedlock(mtx_t* UPRESTRICT mtx, timespec const* UPRESTRICT ts) noexcept { int r = ::pthread_mutex_timedlock(mtx, ts); return !r ? thrd_success : ((r == EBUSY) ? thrd_busy : thrd_error); }
    inline UPALWAYSINLINE int mtx_unlock(mtx_t* mtx) noexcept { return ::pthread_mutex_unlock(mtx) ? thrd_error : thrd_success; }

    typedef ::pthread_cond_t cnd_t;
    inline UPALWAYSINLINE int cnd_init(cnd_t* cond) noexcept { return ::pthread_cond_init(cond, 0) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE void cnd_destroy(cnd_t* cond) noexcept { ::pthread_cond_destroy(cond); }
    inline UPALWAYSINLINE int cnd_broadcast(cnd_t* cond) noexcept { return ::pthread_cond_broadcast(cond) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE int cnd_signal(cnd_t* cond) noexcept { return ::pthread_cond_signal(cond) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE int cnd_wait(cnd_t* UPRESTRICT cond, mtx_t* UPRESTRICT mtx) noexcept { return ::pthread_cond_wait(cond, mtx) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE int cnd_timedwait(cnd_t* UPRESTRICT cond, mtx_t* UPRESTRICT mtx, timespec const* UPRESTRICT ts) noexcept { int r = ::pthread_cond_timedwait(cond, mtx, ts); return !r ? thrd_success : ((r == ETIMEDOUT) ? thrd_timedout : thrd_error); }

    typedef ::pthread_key_t tss_t;
    typedef void (UPCDECL* tss_dtor_t)(void*);
    inline UPALWAYSINLINE int tss_create(tss_t* key, tss_dtor_t dtor) noexcept { return ::pthread_key_create(key, dtor) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE void tss_delete(tss_t key) noexcept { ::pthread_key_delete(key); }
    inline UPALWAYSINLINE int tss_set(tss_t key, void* val) noexcept { return ::pthread_setspecific(key, val) ? thrd_error : thrd_success; }
    inline UPALWAYSINLINE void* tss_get(tss_t key) noexcept { return ::pthread_getspecific(key); }
}

#endif

