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

#include "printf_internal.hpp"
#include <up/climits.hpp>

namespace up
{
    LIBUPCOREAPI UPPRINTF(2,3)
    int fast_sprintf(char* UPRESTRICT s, char const* UPRESTRICT format, ...) noexcept {
        int retval;
        detail::print_info info;
        info.cursor = s;
        info.end = s + INT_MAX;
        info.buffer = s;
        info.buffer_length = INT_MAX;
        info.length = 0;
        info.stream = nullptr;
        info.lc = nullptr;
        va_start(info.args, format);
        retval = detail::fast_print_format(&info, format);
        va_end(info.args);
        return retval;
    }
}
