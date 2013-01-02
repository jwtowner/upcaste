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

#ifndef UP_DETAIL_CTIME_POSIX_INL
#define UP_DETAIL_CTIME_POSIX_INL

#ifndef UP_CTIME_HPP
#   error "Do not include this header file directly! Instead include <up/ctime.hpp>"
#endif

#ifndef TIME_UTC
#   define TIME_UTC 1
#endif

namespace up
{
    using ::timespec;

#if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)) || defined(__USE_POSIX199309)
    using ::clockid_t;
    using ::clock_getres;
    using ::clock_gettime;
    using ::clock_settime;
#else
#   error "No POSIX time extensions compatability layer for target platform."
#endif

#ifndef UP_HAS_STDC_TIMESPEC_GET
    inline UPALWAYSINLINE
    int timespec_get(timespec* ts, int) noexcept {
        return clock_gettime(CLOCK_REALTIME, ts);
    }
#else
    using ::timespec_get;
#endif
}

#endif
