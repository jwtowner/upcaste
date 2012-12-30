//
//  Upcaste Performance Libraries
//  Copyright (C) 2012 Jesse W. Towner
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
    int clock_getres(clockid_t clock_id, timespec* res) noexcept {
        if (clock_id == CLOCK_MONOTONIC) {
            int64_t frequency;
            int result;

            result = ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
            if (!result) {
                set_errno_with_last_oserror();
                return -1;
            }

            if (res) {
                res->tv_sec = static_cast<time_t>(1 / frequency);
                res->tv_nsec = static_cast<long>((1000000000ll / frequency) % 1000000000ll);
            }

            return 0;
        }
        else if ((CLOCK_REALTIME <= clock_id) && (clock_id <= CLOCK_THREAD_CPUTIME_ID)) {
            if (res) {
                res->tv_sec = 0;
                res->tv_nsec = 100;
            }
            return 0;
        }
        else {
            errno = EINVAL;
            return -1;
        }
    }
}
