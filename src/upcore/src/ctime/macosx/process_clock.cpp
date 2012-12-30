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

#include <up/chrono.hpp>
#include <up/cassert.hpp>
#include <sys/time.h>
#include <unistd.h>

#if UP_BASESYSTEM == UP_BASESYSTEM_DARWIN
#   include <mach/mach.h>
#   include <mach/mach_time.h>
#   include <mach/task_info.h>
#   include <mach/thread_info.h>
#endif

namespace up { namespace chrono
{
    LIBUPCOREAPI
    process_clock::time_point process_clock::now() {
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0) && defined(_POSIX_CPUTIME) && (_POSIX_CPUTIME >= 0)
        struct ::timespec tp;
        int result = ::clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
        assert_throw(!result, throw system_error(errno, ::std::system_category(), "up::chrono::process_clock::now -- system error"));
        return process_clock::time_point(process_clock::duration((tp.tv_sec * 1000000000ll) + tp.tv_nsec));
#elif UP_BASESYSTEM == UP_BASESYSTEM_DARWIN
        task_basic_info_data_t info;
        mach_msg_type_number_t size = TASK_BASIC_INFO_COUNT;
        kern_return_t result = ::task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
        assert(result == KERN_SUCCESS);
        time_value_t total;
        total.seconds = info.user_time.seconds + info.system_time.seconds;
        total.microseconds = info.user_time.microseconds + info.system_time.microseconds;
        return process_clock::time_point(process_clock::duration((total.seconds * 1000000ll) + total.microseconds));            
#else
#   error "Platform not supported!"
#endif 
    }
}}
