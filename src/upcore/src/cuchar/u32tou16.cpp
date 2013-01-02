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
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI int u32tou16(char16_t* u16s, char32_t u32) noexcept {
        char16_t u16temp[U16_CUR_MAX];
        if (!u16s) {
            u16s = u16temp;
        }
        
        if (u32 < 0x10000) {
            if (!::up::detail::u32_is_surrogate(u32)) {
                u16s[0] = static_cast<char16_t>(u32);
                return u32 ? 1 : 0;
            }
            
            u16s[0] = static_cast<char16_t>(::up::detail::u32_replacement_character);
            return -1;
        }
        
        if (u32 < 0x110000) {
            u32 -= 0x10000;
            u16s[0] = static_cast<char16_t>(0xD800 + (u32 >> 10));
            u16s[1] = static_cast<char16_t>(0xDC00 + (u32 & 0x3FF));
            return 2;
        }
        
        return -1;
    }
}
