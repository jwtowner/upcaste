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
    size_t u32sntou8s(char* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t nu32, size_t n) noexcept {
        assert((dst || !n) && src && (*src || !nu32));
        
        unsigned char* udst = reinterpret_cast<unsigned char*>(dst);
        unsigned char* const udst_end = udst + n;
        char32_t const* src_cur = *src;
        char32_t const* const src_end = src_cur + nu32;
        char32_t codepoint;

        for ( ; (udst < udst_end) && (src_cur < src_end); ++src_cur, ++udst) {
            codepoint = *src_cur;
            
            if (codepoint < 0x80) {
                // ascii-fast path
                *udst = static_cast<unsigned char>(codepoint);
                if (!codepoint) {
                    src_cur = nullptr;
                    break;
                }
                continue;
            }
            
            if (codepoint < 0x0800) {
                // encode two-byte utf-8 sequence
                if ((udst + 2) > udst_end) {
                    break;
                }
                
                *(udst++) = static_cast<unsigned char>((codepoint >> 6) | 0xC0);
                *udst = static_cast<unsigned char>((codepoint & 0x3F) | 0x80);
                continue;
            }

            // check if this is a supplementary code point
            if (codepoint >= 0x10000) {
                if (codepoint < 0x110000) {
                    // encode four-byte utf-8 sequence
                    if ((udst + 4) > udst_end) {
                        break;
                    }

                    *(udst++) = static_cast<unsigned char>((codepoint >> 18) | 0xF0);
                    *(udst++) = static_cast<unsigned char>(((codepoint >> 12) & 0x3F) | 0x80);
                    *(udst++) = static_cast<unsigned char>(((codepoint >> 6) & 0x3F) | 0x80);
                    *udst = static_cast<unsigned char>((codepoint & 0x3F) | 0x80);
                    continue;
                }

                // invalid code point
                codepoint = detail::u32_replacement_character;
            }
            else if (detail::u32_is_surrogate(codepoint)) {
                // invalid surrogate sequence
                codepoint = detail::u32_replacement_character;
            }
                
            // encode three-byte utf-8 sequence
            if ((udst + 3) > udst_end) {
                break;
            }

            *(udst++) = static_cast<unsigned char>((codepoint >> 12) | 0xE0);
            *(udst++) = static_cast<unsigned char>(((codepoint >> 6) & 0x3F) | 0x80);
            *udst = static_cast<unsigned char>((codepoint & 0x3F) | 0x80);
        }

        *src = src_cur;
        return static_cast<size_t>(udst - reinterpret_cast<unsigned char*>(dst));
    }
}
