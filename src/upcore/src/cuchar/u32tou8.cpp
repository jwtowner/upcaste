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
    LIBUPCOREAPI int u32tou8(char* s, char32_t u32) noexcept {
        unsigned char u8temp[U8_CUR_MAX];
        unsigned char* u8s = s ? reinterpret_cast<unsigned char*>(s) : u8temp;
        
        if (u32 < 0x80) {
            u8s[0] = static_cast<unsigned char>(u32);
            return u32 ? 1 : 0;
        }
        else if (u32 < 0x0800) {
            u8s[0] = static_cast<unsigned char>((u32 >> 6) | 0xC0);
            u8s[1] = static_cast<unsigned char>((u32 & 0x3F) | 0x80);
            return 2;
        }
        else if (u32 < 0x10000) {
            if (::up::detail::u32_is_surrogate(u32)) {
                goto error;
            }
            
            u8s[0] = static_cast<unsigned char>((u32 >> 12) | 0xE0);
            u8s[1] = static_cast<unsigned char>(((u32 >> 6) & 0x3F) | 0x80);
            u8s[2] = static_cast<unsigned char>((u32 & 0x3F) | 0x80);
            return 3;
        }
        else if (u32 < 0x110000) {
            u8s[0] = static_cast<unsigned char>((u32 >> 18) | 0xF0);
            u8s[1] = static_cast<unsigned char>(((u32 >> 12) & 0x3F) | 0x80);
            u8s[2] = static_cast<unsigned char>(((u32 >> 6) & 0x3F) | 0x80);
            u8s[3] = static_cast<unsigned char>((u32 & 0x3F) | 0x80);
            return 4;
        }

    error:
    
        u8s[0] = detail::u8_replacement_character_1;
        u8s[1] = detail::u8_replacement_character_2;
        u8s[2] = detail::u8_replacement_character_3;
        return -1;
    }
}
