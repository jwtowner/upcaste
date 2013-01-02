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
    LIBUPCOREAPI size_t u8snlen_u32(char const* s, size_t n) noexcept {
        assert(s);
    
        unsigned char const* u8s = reinterpret_cast<unsigned char const*>(s);
        unsigned char const* u8s_end = u8s + n;
        uint_fast32_t codepoint, octet;
        int_fast32_t i, length;
        size_t count = 0;

        while (u8s < u8s_end) {
            codepoint = *(u8s++);
            if (!codepoint) {
                break;
            }

            // ascii fast-path
            ++count;
            length = ::up::detail::u8_sequence_length_table[codepoint];
            if (length <= 1) {
                continue;
            }

            // ensure we won't read beyond the end
            if ((u8s + length - 1) > u8s_end) {
                break;
            }

            // fully validate unicode codepoint
            for (i = length - 1; i > 0; ++u8s, --i) {
                octet = *u8s;
                if (!::up::detail::u8_is_trail(octet)) {
                    break;
                }

                codepoint = (codepoint << 6) + octet;
            }

            codepoint -= ::up::detail::u8_offset_table[length];
            if ((i > 0) || !::up::detail::u32_from_u8_is_valid(codepoint, length)) {
                u8s -= (length - i - 1);
            }
        }
        
        return count;
    }
}
