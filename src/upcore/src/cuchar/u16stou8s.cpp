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
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI size_t u16stou8s(char* UPRESTRICT s, char16_t const* UPRESTRICT u16s, size_t n) noexcept {
        assert((u16s && s) || !n);
        
        unsigned char* UPRESTRICT u8s = reinterpret_cast<unsigned char*>(s);
        unsigned char* u8s_end = u8s + n;
        char32_t codepoint, tail;
        
        for ( ; u8s < u8s_end; ++u8s) {
            codepoint = *(u16s++);
            if (codepoint < 0x80) {
                // ascii-fast path
                *u8s = static_cast<unsigned char>(codepoint);
                if (!codepoint) {
                    break;
                }
            }
            else if (codepoint < 0x0800) {
                // two-byte utf-8 character sequence
                if ((u8s + 2) > u8s_end) {
                    goto out_of_storage_space;
                }
                
                *(u8s++) = static_cast<unsigned char>((codepoint >> 6) | 0xC0);
                *u8s = static_cast<unsigned char>((codepoint & 0x3F) | 0x80);
            }
            else {
                // check for utf-16 surrogate pair
                if (::up::detail::u16_is_surrogate(codepoint)) {
                    if (::up::detail::u16_is_surrogate_pair(codepoint, (tail = *u16s))) {
                        // four-byte utf-8 sequence
                        if ((u8s + 4) > u8s_end) {
                            goto out_of_storage_space;
                        }

                        ++u16s;
                        codepoint = (codepoint << 10) + tail - detail::utf16_surrogate_offset;
                        *(u8s++) = static_cast<unsigned char>((codepoint >> 18) | 0xF0);
                        *(u8s++) = static_cast<unsigned char>(((codepoint >> 12) & 0x3F) | 0x80);
                        *(u8s++) = static_cast<unsigned char>(((codepoint >> 6) & 0x3F) | 0x80);
                        *u8s = static_cast<unsigned char>((codepoint & 0x3F) | 0x80);
                        continue;
                    }

                    // invalid surrogate pair sequence
                    codepoint = detail::u32_replacement_character;
                }

                // three-byte utf-8 sequence
                if ((u8s + 3) > u8s_end) {
                    goto out_of_storage_space;
                }

                *(u8s++) = static_cast<unsigned char>((codepoint >> 12) | 0xE0);
                *(u8s++) = static_cast<unsigned char>(((codepoint >> 6) & 0x3F) | 0x80);
                *u8s = static_cast<unsigned char>((codepoint & 0x3F) | 0x80);
            }
        }
        
        return static_cast<size_t>(u8s - reinterpret_cast<unsigned char*>(s));
        
    out_of_storage_space:
    
        // fill remaining space with null-terminators
        for ( ; u8s < u8s_end; ++u8s) {
            *u8s = 0;
        }
        
        return n;
    }
}
