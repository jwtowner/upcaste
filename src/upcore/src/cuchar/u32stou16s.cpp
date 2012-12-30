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

#include <up/cuchar.hpp>
#include <up/cassert.hpp>
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI size_t u32stou16s(char16_t* UPRESTRICT u16s, char32_t const* UPRESTRICT u32s, size_t n) noexcept {
        assert((u16s && u32s) || !n);
        
        char16_t* u16s_start = u16s, * u16s_end = u16s + n;
        char32_t codepoint;

        for ( ; u16s < u16s_end; ++u32s, ++u16s) {
            codepoint = *u32s;

            if (codepoint < 0x10000) {
                // encode single UTF-16 code unit
                if (!::up::detail::u32_is_surrogate(codepoint)) {
                    *u16s = static_cast<char16_t>(codepoint);
                    if (!codepoint) {
                        break;
                    }
                    
                    continue;
                }
            }
            else if (codepoint < 0x110000) {
                // encode UTF-16 surrogate pair
                if ((u16s + 1) >= u16s_end) {
                    *u16s = 0;
                    return n;
                }
                
                codepoint -= 0x10000;
                *(u16s++) = static_cast<char16_t>(0xD800 + (codepoint >> 10));
                *u16s = static_cast<char16_t>(0xDC00 + (codepoint & 0x3FF));
                continue;
            }
            
            // invalid code point
            *u16s = ::up::detail::u16_replacement_character;
        }

        return static_cast<size_t>(u16s - u16s_start);
    }
}
