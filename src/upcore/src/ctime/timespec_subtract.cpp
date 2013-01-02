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
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    int timespec_subtract(timespec const* ts1, timespec const* ts2, timespec* result) noexcept {
        assert(ts1 && ts2 && result);

        timespec d;
        long sec;

        d.tv_sec  = ts2->tv_sec - ts1->tv_sec;
        d.tv_nsec = ts2->tv_nsec - ts1->tv_nsec;

        if (d.tv_nsec < 0) {
            sec = (d.tv_nsec / 1000000000) - 1;
            d.tv_sec += sec;
            d.tv_nsec -= sec * 1000000000;
        }
        else if (d.tv_nsec >= 1000000000) {
            sec = d.tv_nsec / 1000000000;
            d.tv_sec += sec;
            d.tv_nsec -= sec * 1000000000;
        }

        *result = d;
        return d.tv_sec < 0;
    }
}
