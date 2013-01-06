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

#include <up/cuchar.hpp>
#include <up/cerrno.hpp>
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI
    int u16tou32(char32_t* UPRESTRICT u32, char16_t const* UPRESTRICT u16s, size_t n) noexcept {
        char32_t codepoint, tail;
        int length;

        codepoint = (n > 0) ? (u16s ? *u16s : detail::utf16_tail_surrogate_min) : 0x0000;
        if (!detail::u16_is_surrogate(codepoint)) {
            length = codepoint ? 1 : 0;
            goto done;
        }

        tail = *(++u16s);
        if ((n > 1) && detail::u16_is_surrogate_pair(codepoint, tail)) {
            codepoint = (codepoint << 10) + tail - detail::utf16_surrogate_offset;
            length = 2;
            goto done;
        }

        codepoint = detail::u32_replacement_character;
        length = -1;

    done:

        if (u32) {
            *u32 = codepoint;
        }

        return length;
    }
}
