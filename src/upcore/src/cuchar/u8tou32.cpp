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
#include <up/cerrno.hpp>
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI int u8tou32(char32_t* UPRESTRICT u32, char const* UPRESTRICT s, size_t n) noexcept {
        unsigned char const* u8s = reinterpret_cast<unsigned char const*>(s);
        uint_fast32_t codepoint, octet;
        int_fast32_t i, length;
        
        codepoint = (n > 0) ? (u8s ? *u8s : 0xFF) : 0x00;
        length = ::up::detail::u8_sequence_length_table[codepoint];
        if (n < static_cast<size_t>(length)) {
            assert(n < SIZE_MAX);
            goto error;
        }

        // ascii fast-path
        if (length <= 1) {
            goto done;
        }

        // fully decode and validate unicode code point
        for (i = length - 1; i > 0; --i) {
            octet = *(++u8s);
            if (!::up::detail::u8_is_trail(octet)) {
                goto error;
            }
            codepoint = (codepoint << 6) + octet;
        }

        codepoint -= ::up::detail::u8_offset_table[length];
        if (!::up::detail::u32_from_u8_is_valid(codepoint, length)) {
            goto error;
        }

    done:

        if (u32) {
            *u32 = static_cast<char32_t>(codepoint);
        }
        
        return static_cast<int>(length);

    error:

        errno = EILSEQ;

        if (u32) {
            *u32 = ::up::detail::u32_replacement_character;
        }

        return -1;
    }
}
