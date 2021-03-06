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
    LIBUPCOREAPI
    char const* u8schr(char const* s, char32_t u32) noexcept {
        assert(s);

        // ascii fast-path
        if (u32 < 0x80) {
            return strchr(s, static_cast<int>(u32));
        }
        
        // unicode normal-path
        char buffer[u8_cur_max];
        int buffer_length = u32tou8(buffer, u32);
        if (buffer_length <= -1) {
            return nullptr;
        }
        
        // search for character sequence in 's'
        char const* buffer_end = buffer + buffer_length;
        for ( ; *s; ++s) {
            char const* b = buffer;
            for (char const* s2 = s; (b < buffer_end) && !(*s2 - *b); ++s2, ++b) ;
            if (b == buffer_end) {
                return s;
            }
        }

        return nullptr;
    }
}
