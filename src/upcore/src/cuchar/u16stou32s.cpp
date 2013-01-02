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
    LIBUPCOREAPI size_t u16stou32s(char32_t* UPRESTRICT u32s, char16_t const* UPRESTRICT u16s, size_t n) noexcept {
        assert((u16s && u32s) || !n);
        
        char32_t const* u32s_start = u32s, * u32s_end = u32s + n;
        char32_t lead, tail;
        
        for ( ; u32s < u32s_end; ++u32s) {
            lead = *(u16s++);
            if (!::up::detail::u16_is_surrogate(lead)) {
                *u32s = lead;
                if (!lead) {
                    break;
                }
            }
            else if (::up::detail::u16_is_surrogate_pair(lead, (tail = *u16s))) {
                *u32s = (lead << 10) + tail - detail::utf16_surrogate_offset;
                ++u16s;
            }
            else {
                *u32s = detail::u32_replacement_character;
            }
        }
        
        return static_cast<size_t>(u32s - u32s_start);
    }
}
