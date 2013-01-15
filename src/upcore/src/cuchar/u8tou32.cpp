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
    int u8tou32(char32_t* UPRESTRICT u32, char const* UPRESTRICT s, size_t n) noexcept {
        unsigned char const* u8s = reinterpret_cast<unsigned char const*>(s);
        unsigned char const* u8s_end = u8s + n;
        unsigned char octet;
        size_t codepoint;
        ssize_t i, length;
        
        // read initial octet and determine sequence length
        codepoint = (u8s && (n > 0)) ? *u8s : 0;
        length = detail::u8_sequence_length_table[codepoint];
        if (length <= 1) {
            // ascii fast path
            if (u32) {
                *u32 = static_cast<char32_t>(codepoint);
            }

            return static_cast<int>(length);
        }

        // fully decode and validate unicode code point
        for (i = length - 1, ++u8s; (i > 0) && (u8s < u8s_end); --i, ++u8s) {
            octet = *u8s;
            if (!detail::u8_is_trail(octet)) {
                break;
            }
            codepoint = (codepoint << 6) + octet;
        }

        if (!i) {
            codepoint -= detail::u8_offset_table[length];
            if (detail::u32_from_u8_is_valid(codepoint, length)) {
                if (u32) {
                    *u32 = static_cast<char32_t>(codepoint);
                }
                return static_cast<int>(length);
            }
        }

        // illegal sequence detected
        if (u32) {
            *u32 = detail::u32_replacement_character;
        }

        // return negated length of maximal subsequence
        return static_cast<int>(reinterpret_cast<unsigned char const*>(s) - u8s);
    }
}
