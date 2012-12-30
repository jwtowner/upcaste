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

#include <up/cthreads.hpp>
#include <up/cerrno.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if NTDDI_VERSION >= NTDDI_VISTA
#   define TICK_COUNT unsigned long long
#   define TICK_COUNT_MAX ULLONG_MAX
#   define GET_TICK_COUNT ::GetTickCount64
#else
#   define TICK_COUNT unsigned long
#   define TICK_COUNT_MAX ULONG_MAX
#   define GET_TICK_COUNT ::GetTickCount
#endif

namespace up
{
    extern LIBUPCOREAPI
    int thrd_sleep(timespec const* duration, timespec* remaining) noexcept {
        TICK_COUNT elapsed_time, cur_time, prev_time;
        unsigned long time_remaining, wait_result;
        int result;

        if (!duration && (duration->tv_sec < 0) && (duration->tv_nsec < 0) && (duration->tv_nsec > 1000000000)) {
            errno = EINVAL;
            return -1;
        }

        if (duration->tv_sec >= static_cast<time_t>(INFINITE / 1000)) {
            time_remaining = INFINITE;
        }
        else {
            time_remaining = static_cast<unsigned long>(duration->tv_sec * 1000 + ((duration->tv_nsec + 999999) / 1000000));
        }

        prev_time = GET_TICK_COUNT();
        wait_result = ::SleepEx(time_remaining, 1);
        if (wait_result == 0) {
            time_remaining = 0;
            result = 0;
        }
        else {
            set_errno_with_last_oserror();
            result = -1;
            if (time_remaining != INFINITE) {
                cur_time = GET_TICK_COUNT();
                elapsed_time = (cur_time < prev_time) ? (cur_time + (TICK_COUNT_MAX - prev_time) + 1) : (cur_time - prev_time);
                time_remaining = (elapsed_time < time_remaining) ? static_cast<unsigned long>(time_remaining - elapsed_time) : 0;
            }
        }

        if (remaining) {
            remaining->tv_sec = static_cast<time_t>(time_remaining / 1000);
            remaining->tv_nsec = static_cast<long>((time_remaining % 1000) * 1000000);
        }

        return result;
    }
}
