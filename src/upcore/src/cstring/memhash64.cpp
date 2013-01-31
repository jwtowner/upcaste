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
    uint_least64_t memhash64(void const* key, size_t n, uint_least64_t seed) noexcept {
        constexpr uint_least64_t r8 = UINT64_C(0x1591aefa5e7e5a17);
        constexpr uint_least64_t r9 = UINT64_C(0x2bb6863566c4e761);
        uint_least64_t rax = n;
        uint_least64_t rcx = seed;
        uint_least64_t rdx;
        u64mul_t product;

        uint_least64_t const* k64 = static_cast<uint_least64_t const*>(key);
        uint_least64_t const* const k64_end = k64 + ((n >> 3) & ~1);

        for ( ; k64 < k64_end; k64 += 2) {
            rax = byteswap((rax ^ k64[0]) * r8);
            rcx = byteswap((rcx ^ k64[1]) * r9);
        }

        if (n & 15) {
            if (n & 8) {
                rax ^= *k64++;
            }

            key = static_cast<void const*>(k64);
            rdx = 0;

            if (n & 4) {
                rdx = static_cast<uint_least32_t const*>(key)[0];
                key = &static_cast<uint_least32_t const*>(key)[1];
            }

            if (n & 2) {
                rdx = (rdx << 16) | static_cast<uint_least16_t const*>(key)[0];
                key = &static_cast<uint_least16_t const*>(key)[1];
            }

            if (n & 1) {
                rdx = (rdx << 8) | static_cast<uint_least8_t const*>(key)[0];
            }

            rcx ^= rdx;
            product = u64mul(rax, r8);
            rcx = (rcx * r9) + product.high;
            rax = rcx ^ product.low;
        }

        product = u64mul(rax, r8);
        rcx = (rcx * r9) + product.high;
        rax = rcx ^ product.low;

        product = u64mul(rax, r8);
        rcx = (rcx * r9) + product.high;
        rax = rcx ^ product.low;

        return rax;
    }
}
