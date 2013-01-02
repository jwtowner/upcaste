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

namespace up
{
    LIBUPCOREAPI char16_t const* u16srchr(char16_t const* s, char32_t u32) noexcept {
        assert(s);

        char16_t const* result = nullptr;
    
        if ((u32 < 0xD800) || ((0xE000 <= u32) && (u32 < 0x10000))) {
            // ucs-2 fast path
            for ( ; *s; ++s) {
                if (*s == static_cast<char16_t>(u32)) {
                    result = s;
                }
            }
        }
        else if (u32 < 0x110000) {
            // utf-16 normal path
            char16_t c_buffer[2];
            c_buffer[0] = static_cast<char16_t>(0xD800 + ((u32 - 0x10000) >> 10));
            c_buffer[1] = static_cast<char16_t>(0xDC00 + ((u32 - 0x10000) & 0x3FF));
            
            // search for character sequence in 's'
            char16_t const* c_end = c_buffer + 2;
            
            for ( ; *s; ++s) {
                char16_t const* c = c_buffer;
                for (char16_t const* s2 = s; (c < c_end) && !(*s2 - *c); ++s2, ++c) ;
                if (c == c_end) {
                    result = s;
                }
            }
        }
        
        return result;
    }
}
