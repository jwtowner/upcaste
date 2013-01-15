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
    size_t u8stou16slen(char const* s) noexcept {
        assert(s);
    
        unsigned char const* u8s = reinterpret_cast<unsigned char const*>(s);
        uint_fast32_t codepoint, octet;
        int_fast32_t i, length;
        size_t count = 0;

        for (;;) {
            // read start of next character
            codepoint = *u8s;
            if (!codepoint) {
                break;
            }
            ++count;
            ++u8s;

            // ascii fast-path
            length = detail::u8_sequence_length_table[codepoint];
            if (length <= 1) {
                continue;
            }

            // fully validate unicode codepoint
            for (i = length - 1; i > 0; ++u8s, --i) {
                octet = *u8s;
                if (!detail::u8_is_trail(octet)) {
                    break;
                }
                codepoint = (codepoint << 6) + octet;
            }

            codepoint -= detail::u8_offset_table[length];
            if ((i > 0) || !detail::u32_from_u8_is_valid(codepoint, length)) {
                length = 0;
                u8s = detail::u8s_recover(u8s);
                if (!u8s) {
                    break;
                }
            }
            
            // 4-byte utf-8 characters always map to a utf-16 surrogate pair
            count += (length & 4) >> 2;
        }
        
        return count;
    }
}
