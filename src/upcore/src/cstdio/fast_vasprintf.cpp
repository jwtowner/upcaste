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

#include <up/cassert.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdio.hpp>

namespace up
{
    LIBUPCOREAPI UPPRINTF(2,0) UPWARNRESULT
    int fast_vasprintf(char** UPRESTRICT ptr, char const* UPRESTRICT format, va_list ap) noexcept {
        va_list ap_local;
        int length, length2;
        char* str;

        va_copy(ap_local, ap);
        length = fast_vsnprintf(nullptr, 0, format, ap_local);
        va_end(ap_local);

        if (UPUNLIKELY(length < 0)) {
            *ptr = nullptr;
            return length;
        }

        str = static_cast<char*>(malloc(length + 1));
        if (UPUNLIKELY(!str)) {
            *ptr = nullptr;
            return -1;
        }

        va_copy(ap_local, ap);
        length2 = fast_vsnprintf(str, length, format, ap_local);
        assert(length == length2);
        va_end(ap_local);

        *ptr = str;
        return length;
    }
}
