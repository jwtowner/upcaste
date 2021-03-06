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

#include <up/ctime.hpp>
#include <up/cerrno.hpp>
#include <up/cstdint.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace up
{
    LIBUPCOREAPI
    int clock_settime(clockid_t clock_id, timespec const* ts) noexcept {
        FILETIME ft;
        SYSTEMTIME st;
        uint64_t usecs;
        int result;

        if ((clock_id != CLOCK_REALTIME) || !ts || (ts->tv_sec < 0) || (ts->tv_nsec < 0) || (1000000000 < ts->tv_nsec)) {
            errno = EINVAL;
            return -1;
        }

        usecs = (static_cast<uint64_t>(ts->tv_sec) * 10000000ull) + (static_cast<uint64_t>(ts->tv_nsec) / 100ull);
        ft.dwLowDateTime = static_cast<uint32_t>(usecs & 0xFFFFFFFFull);
        ft.dwHighDateTime = static_cast<uint32_t>((usecs >> 32) & 0xFFFFFFFFull);
        result = ::FileTimeToSystemTime(&ft, &st);
        if (!result) {
            set_errno_with_last_oserror();
            return -1;
        }

        result = ::SetSystemTime(&st);
        if (!result) {
            set_errno_with_last_oserror();
            return -1;
        }

        return 0;
    }
}
