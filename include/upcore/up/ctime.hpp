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

#ifndef UP_CTIME_HPP
#define UP_CTIME_HPP

#include <up/prolog.hpp>

#ifndef UP_HAS_STDC_TIME
#   error "Header file unavailable! Target configuration, compiler, or platform does not support timing!"
#endif

#include <ctime>

namespace up
{
    using ::std::clock_t;
    using ::std::time_t;
    using ::std::tm;
    using ::std::asctime;
    using ::std::clock;
    using ::std::ctime;
    using ::std::difftime;
    using ::std::gmtime;
    using ::std::localtime;
    using ::std::mktime;
    using ::std::strftime;
    using ::std::time;
}

#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
#   include <up/detail/ctime_msvc.inl>
#elif UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_POSIX
#   include <up/detail/ctime_posix.inl>
#else
#   error "No time extensions compatability layer for target platform."
#endif

namespace up
{
    extern LIBUPCOREAPI UPNONNULLALL
    int timespec_add(timespec const* ts1, timespec const* ts2, timespec* result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int timespec_subtract(timespec const* ts1, timespec const* ts2, timespec* result) noexcept;

    inline UPALWAYSINLINE UPNONNULLALL
    double timespec_to_seconds(timespec const* ts) noexcept {
        return ts->tv_sec + ts->tv_nsec / 1000000000.0;
    }

    inline UPALWAYSINLINE UPNONNULLALL
    double timespec_diff(timespec const* ts1, timespec const* ts2) noexcept {
        timespec diff;
        timespec_subtract(ts1, ts2, &diff);
        return timespec_to_seconds(&diff);
    }
}

#endif
