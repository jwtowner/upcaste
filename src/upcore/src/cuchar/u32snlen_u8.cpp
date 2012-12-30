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

namespace up
{
    LIBUPCOREAPI size_t u32snlen_u8(char32_t const* u32s, size_t n) noexcept {
        assert(u32s || !n);
        
        char32_t const* u32s_end = u32s + n;
        char32_t codepoint;
        size_t count = 0;

        for ( ; u32s < u32s_end; ++u32s) {
            codepoint = *u32s;
            if (codepoint < 0x80) {
                if (!codepoint) {
                    break;
                }
                ++count;
            }
            else if (codepoint < 0x0800) {
                count += 2;
            }
            else if ((codepoint < 0x10000) || (codepoint >= 0x110000)) {
                count += 3;
            }
            else {
                count += 4;
            }
        }

        return count;
    }
}
