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
#include <up/cerrno.hpp>

namespace up
{
    LIBUPCOREAPI UPPRINTF(2,0)
    int fast_vfprintf(FILE* UPRESTRICT stream, char const* UPRESTRICT format, va_list args) noexcept {
        int retval;
        detail::print_info info;
        char buffer[4096];
        if (UPUNLIKELY(!stream)) {
            errno = EINVAL;
            return -1;
        }
        info.cursor = buffer;
        info.end = buffer + sizeof(buffer);
        info.buffer = buffer;
        info.buffer_length = sizeof(buffer);
        info.length = 0;
        info.stream = stream;
        info.lc = nullptr;
        va_copy(info.args, args);
        retval = detail::fast_print_format(&info, format);
        va_end(info.args);
        return retval;
    }
}
