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
#include <up/cassert.hpp>
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI
    size_t u16stou32s(char32_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t n) noexcept {
        assert((dst && src && *src) || !n);
        
        char16_t const* src_cur = *src;
        char32_t* const dst_start = dst;
        char32_t* const dst_end = dst + n;
        char32_t lead, tail;
        
        for ( ; dst < dst_end; ++src_cur, ++dst) {
            lead = *src_cur;
            if (!detail::u16_is_surrogate(lead)) {
                *dst = lead;
                if (!lead) {
                    src_cur = nullptr;
                    break;
                }
                continue;
            }

            tail = *(++src_cur);
            if (detail::u16_is_surrogate_pair(lead, tail)) {
                *dst = (lead << 10) + tail - detail::utf16_surrogate_offset;
                continue;
            }

            *dst = detail::u32_replacement_character;
            --src_cur;
        }
        
        *src = src_cur;
        return static_cast<size_t>(dst - dst_start);
    }
}
