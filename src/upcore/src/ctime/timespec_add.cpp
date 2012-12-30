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
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    int timespec_add(timespec const* ts1, timespec const* ts2, timespec* result) noexcept {
        assert(ts1 && ts2 && result);

        timespec r;
        long sec;

        r.tv_sec  = ts1->tv_sec + ts2->tv_sec;
        r.tv_nsec = ts1->tv_nsec + ts2->tv_nsec;

        if (r.tv_nsec < 0) {
            sec = (r.tv_nsec / 1000000000) - 1;
            r.tv_sec += sec;
            r.tv_nsec -= sec * 1000000000;
        }
        else if (r.tv_nsec >= 1000000000) {
            sec = r.tv_nsec / 1000000000;
            r.tv_sec += sec;
            r.tv_nsec -= sec * 1000000000;
        }

        *result = r;
        return r.tv_sec < 0;
    }
}
