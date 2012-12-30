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

#include "printf_internal.hpp"
#include <up/cerrno.hpp>
#include <up/climits.hpp>

namespace up { namespace detail
{
    UPHIDDEN UPNONNULLALL
    ptrdiff_t NAME(INFO* UPRESTRICT info) noexcept {
        size_t length, new_length, remaining;
        CHAR* buffer;

        buffer = info->buffer;
        remaining = info->cursor - buffer;
        length = info->length;
        new_length = remaining + length;
        if (UPUNLIKELY(new_length > INT_MAX)) {
            errno = EOVERFLOW;
            return -1;
        }

        if (info->stream) {
            if (UPUNLIKELY(fwrite(buffer, sizeof(CHAR), remaining, info->stream) != remaining)) {
                return -1;
            }
            info->cursor = buffer;
            info->length = new_length;
        }
        else if (info->cursor >= info->end) {
            info->end = nullptr;
        }

        return new_length;
    }
}}
