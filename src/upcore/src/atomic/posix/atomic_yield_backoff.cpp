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

#include <up/atomic.hpp>
#include <up/cassert.hpp>
#include <pthread.h>
#include <sched.h>
#include <time.h>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    void atomic_yield_backoff(unsigned int* spin_count) noexcept {
        assert(spin_count);

        unsigned int count = *spin_count;
        timespec ts;
        
        if (count < 16) {
            if (!count) {
                count = 1;
            }
            for (unsigned int c = count; c > 0; --c) {
                atomic_yield();
            }
            count <<= 1;
        }
        else if (count < 18) {
            ::sched_yield();
            ++count;
        }
        else if (count < 22) {
            ts.tv_sec = 0;
            ts.tv_nsec = 0;
            while (::nanosleep(&ts, &ts) == -1) ;
            ++count;
        }
        else if (count < 64) {
            ts.tv_sec = 0;
            ts.tv_nsec = 1000000;
            while (::nanosleep(&ts, &ts) == -1) ;
        }
        else {
            ts.tv_sec = 0;
            ts.tv_nsec = 10000000;
            while (::nanosleep(&ts, &ts) == -1) ;
        }

        *spin_count = count;
    }
}
