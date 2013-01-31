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

#include <up/byteswap.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>

namespace up
{
    LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least32_t memhash32(void const* key, size_t n, uint_least32_t seed) noexcept {
        constexpr uint_least32_t r8 = UINT32_C(0x5e7e5a17);
        constexpr uint_least32_t r9 = UINT32_C(0x66c4e761);
        uint_least32_t rax = static_cast<uint32_t>(n);
        uint_least32_t rcx = seed;
        uint_least32_t rdx;
        u32mul_t product;

        uint_least32_t const* k32 = static_cast<uint_least32_t const*>(key);
        uint_least32_t const* const k32_end = k32 + ((n >> 2) & ~1);

        for ( ; k32 < k32_end; k32 += 2) {
            rax = byteswap((rax ^ k32[0]) * r8);
            rcx = byteswap((rcx ^ k32[1]) * r9);
        }

        if (n & 7) {
            if (n & 4) {
                rax ^= *k32++;
            }
        
            key = static_cast<void const*>(k32);
            rdx = 0;

            if (n & 2) {
                rdx = static_cast<uint_least16_t const*>(key)[0];
                key = &static_cast<uint_least16_t const*>(key)[1];
            }

            if (n & 1) {
                rdx = (rdx << 8) | static_cast<uint_least8_t const*>(key)[0];
            }

            rcx ^= rdx;
            product = u32mul(rax, r8);
            rcx = (rcx * r9) + product.low;
            rax = rcx ^ product.high;
        }

        product = u32mul(rax, r8);
        rcx = (rcx * r9) + product.low;
        rax = rcx ^ product.high;

        product = u32mul(rax, r8);
        rcx = (rcx * r9) + product.low;
        rax = rcx ^ product.high;

        return rax;
    }
}
