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

#include <up/prolog.hpp>

#if !defined(UP_HAS_STDC_THREADS) && defined(UP_HAS_POSIX_THREADS)

#include <up/cthreads.hpp>
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI
    int mtx_init(mtx_t* mtx, int type) noexcept {
        constexpr int mask = mtx_plain | mtx_timed | mtx_recursive;
        pthread_mutexattr_t attr;
        int res;
        
        if (((type | mask) != mask) || ((type & mask) == 0)) {
            return thrd_error;
        }

        res = ::pthread_mutexattr_init(&attr);
        if (!res) {
            return thrd_error;
        }

        if (type & mtx_recursive) {
            verify(0 == ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
        }
#if defined(UP_HAS_POSIX_PTHREAD_MUTEX_TIMEDLOCK) && defined(UP_HAS_POSIX_PTHREAD_MUTEX_TIMED_NP)
        else if (type & mtx_timed) {
            verify(0 == ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_TIMED_NP));
        }
#endif
        else {
#if defined(UP_DEBUG) && defined(UP_HAS_POSIX_PTHREAD_MUTEX_ERRORCHECK)
            verify(0 == ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));
#elif defined(UP_HAS_POSIX_PTHREAD_MUTEX_ADAPTIVE_NP)
            verify(0 == ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ADAPTIVE_NP));
#else
            verify(0 == ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL));
#endif
        }

        res = ::pthread_mutex_init(mtx, &attr);
        verify(0 == pthread_mutexattr_destroy(&attr));
        return !res ? thrd_success : thrd_error;
    }
}

#endif
