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

#ifndef UP_DETAIL_CTIME_MSVC_INL
#define UP_DETAIL_CTIME_MSVC_INL

#ifndef UP_CTIME_HPP
#   error "Do not include this header file directly! Instead include <up/ctime.hpp>"
#endif

#ifdef UP_HAS_MSVC_XTIME
#   include <thr/xtimec.h>
#else
#   define TIME_UTC 1
#endif

#define _POSIX_TIMERS 1
#define _POSIX_CPUTIME 1
#define _POSIX_THREAD_CPUTIME 1

#define CLOCK_MONOTONIC 1
#define CLOCK_REALTIME 2
#define CLOCK_PROCESS_CPUTIME_ID 3
#define CLOCK_THREAD_CPUTIME_ID 4

namespace up
{
    typedef int clockid_t;

    struct LIBUPCOREAPI timespec
    {
        time_t tv_sec;
	    long tv_nsec;
    };

    struct LIBUPCOREAPI timeval
    {
        long tv_sec;
        long tv_usec;
    };

    extern LIBUPCOREAPI
    int clock_getres(clockid_t clock_id, timespec* res) noexcept;

    extern LIBUPCOREAPI
    int clock_gettime(clockid_t clock_id, timespec* ts) noexcept;

    extern LIBUPCOREAPI
    int clock_settime(clockid_t clock_id, timespec const* ts) noexcept;

#ifdef UP_HAS_MSVC_XTIME
    inline UPALWAYSINLINE
    int timespec_get(timespec* ts, int base) noexcept {
        return ::xtime_get(reinterpret_cast<xtime*>(ts), base);
    }
#else
    inline UPALWAYSINLINE
    int timespec_get(timespec* ts, int) noexcept {
        return clock_gettime(CLOCK_REALTIME, ts);
    }
#endif
}

#endif
