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
    size_t u8stou16s(char16_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t n) noexcept {
        assert((dst && src && *src) || !n);

        unsigned char const* usrc = reinterpret_cast<unsigned char const*>(*src);
        char16_t* dst_start = dst, * dst_end = dst + n;
        uint_fast32_t codepoint, octet;
        int_fast32_t i, length;

        for ( ; dst < dst_end; ++usrc, ++dst) {
            codepoint = *usrc;
            
            // ascii fast-path
            if (codepoint < 0x80) {
                *dst = static_cast<char16_t>(codepoint);
                if (!codepoint) {
                    usrc = nullptr;
                    break;
                }
                continue;
            }
            
            // fully decode and validate utf-8 sequence
            length = detail::u8_sequence_length_table[codepoint];
            if (length > 1) {
                for (i = length - 1; i > 0; --i) {
                    octet = *(++usrc);
                    if (!detail::u8_is_trail(octet)) {
                        break;
                    }
                    codepoint = (codepoint << 6) + octet;
                }
                codepoint -= detail::u8_offset_table[length];
                if (!i && detail::u32_from_u8_is_valid(codepoint, length)) {
                    goto encode;
                }
            }

            usrc = detail::u8s_recover(usrc);
            if (!usrc) {
                dst_end = dst + 1;
            }
            --usrc;
            codepoint = detail::u32_replacement_character;
            
        encode:

            // encode utf-16 sequence
            if (codepoint < 0x10000) {
                *dst = static_cast<char16_t>(codepoint);
            }
            else {
                if ((dst + 1) >= dst_end) {
                    break;
                }
                codepoint -= 0x10000;
                *(dst++) = static_cast<char16_t>(0xD800 + (codepoint >> 10));
                *dst = static_cast<char16_t>(0xDC00 + (codepoint & 0x3FF));
            }
        }
        
        *src = reinterpret_cast<char const*>(usrc);
        return static_cast<size_t>(dst - dst_start);
    }
}
