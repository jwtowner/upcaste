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

namespace up { namespace
{
    inline int64_t convert_filetime_to_usecs(FILETIME const* ft) noexcept {
        return (static_cast<int64_t>(ft->dwHighDateTime) << 32) | ft->dwLowDateTime;
    }

    int monotonic_clock_gettime(timespec* ts) noexcept {
        static uint64_t frequency = 0;
        static uint64_t frequency_numer = 0;
        static uint64_t frequency_denom = 0;
        uint64_t counter;
        int64_t nsecs;
        int result;

        // NOTE: QPC may not be monotonic on multi-core systems running Windows XP. This
        // shouldn't be an issue when only supporting Windows Vista or later.

        if (frequency == 0) {
            result = ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
            if (!result) {
                set_errno_with_last_oserror();
                return -1;
            }
                
            // find the greatest common divisor of frequency and the period (nanoseconds)
            uint64_t divisor, next_divisor;
            divisor = frequency;
            next_divisor = 1000000000ull;
                
            while (next_divisor > 0) {
                uint64_t t = next_divisor;
                next_divisor = divisor % next_divisor;
                divisor = t;
            }

            // store ratio between frequency and the desired period
            frequency_numer = 1000000000ull / divisor;
            frequency_denom = frequency / divisor;
        }

        result = ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
        if (!result) {
            set_errno_with_last_oserror();
            return -1;
        }

        if (frequency_numer == frequency_denom) {
            nsecs = static_cast<int64_t>(counter);
        }
        else if (frequency_denom == 1) {
            nsecs = static_cast<int64_t>(counter * frequency_numer);
        }
        else {
            // decompose elapsed = eta32 * 2^32 + eps32
            uint64_t eta32 = counter >> 32;
            uint64_t eps32 = counter & 0x00000000FFFFFFFFull;

            // form product of decomposed time and numerator
            uint64_t mu64 = frequency_numer * eta32;
            uint64_t lambda64 = frequency_numer * eps32;

            // divide the constituents by denominator
            uint64_t q32 = mu64 / frequency_denom;
            uint64_t r32 = mu64 - (q32 * frequency_denom);
            nsecs = static_cast<int64_t>((q32 << 32) + ((r32 <<32) + lambda64) / frequency_denom);
        }

        if (ts) {
            ts->tv_sec = static_cast<time_t>(nsecs / 1000000000ll);
            ts->tv_nsec = static_cast<long>(nsecs % 1000000000ll);
        }

        return 0;
    }

    int system_clock_gettime(clockid_t clock_id, timespec* ts) noexcept {
        FILETIME creation, exit, kernel, user;
        HANDLE handle;
        int64_t usecs;
        int result;

        switch (clock_id) {
        case CLOCK_REALTIME:
            ::GetSystemTimeAsFileTime(&user);
            usecs = convert_filetime_to_usecs(&user);
            break;

        case CLOCK_PROCESS_CPUTIME_ID:
            handle = ::GetCurrentProcess();
            result = ::GetProcessTimes(handle, &creation, &exit, &kernel, &user);
            if (!result) {
                set_errno_with_last_oserror();
                return -1;
            }
            usecs = convert_filetime_to_usecs(&kernel);
            usecs += convert_filetime_to_usecs(&user);
            break;

        case CLOCK_THREAD_CPUTIME_ID:
            handle = ::GetCurrentThread();
            result = ::GetThreadTimes(handle, &creation, &exit, &kernel, &user);
            if (!result) {
                set_errno_with_last_oserror();
                return -1;
            }
            usecs = convert_filetime_to_usecs(&kernel);
            usecs += convert_filetime_to_usecs(&user);
            break;

        default:
            errno = EINVAL;
            return -1;
        }

        if (ts) {
            ts->tv_sec = static_cast<time_t>(usecs / 10000000ll);
            ts->tv_nsec = static_cast<long>(usecs % 10000000ll) * 100;
        }

        return 0;
    }
}}

namespace up
{
    LIBUPCOREAPI
    int clock_gettime(clockid_t clock_id, timespec* ts) noexcept {
         if (clock_id == CLOCK_MONOTONIC) {
            return monotonic_clock_gettime(ts);
        }
        else {
            return system_clock_gettime(clock_id, ts);
        }
    }
}
