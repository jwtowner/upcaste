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

#include <up/random.hpp>
#include <up/cstring.hpp>

//
// generate_random_seed implements the same algorithm as the
// std::seed_seq class's generate member function as described
// in §26.5.7.1 of the C++11 FDIS, N3290.
//

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    void generate_random_seed(
        uint_least32_t* UPRESTRICT dst,
        size_t dst_length,
        uint_least32_t const* UPRESTRICT src,
        size_t src_length
    )
    noexcept {
        if (!dst_length) {
            return;
        }

        size_t const n = dst_length;
        size_t const s = src_length;
        size_t const m = (s >= n) ? s + 1 : n;

        size_t const t = (n >= 623) ? 11
            : (n >= 68) ? 7
            : (n >= 39) ? 5
            : (n >= 7) ? 3
            : (n - 1) / 2;

        size_t const p = (n - t) / 2;
        size_t const q = p + t;
        
        uint_least32_t const pattern = 0x8b8b8b8b;
        memset_pattern4(dst, &pattern, n);
        
        /* for (size_t k = 0; k <= 0; ++k) */ {
            uint_least32_t r = dst[0] ^ dst[p] ^ dst[n - 1];
            r = 1664525 * (r ^ (r >> 27));
            dst[p] += r;
            r += static_cast<uint_least32_t>(s);
            dst[q] += r;
            dst[0] = r;
        }
        
        for (size_t k = 1; k <= s; ++k) {
            size_t const k_mod_n = k % n;
            size_t const kp_mod_n = (k + p) % n;
            uint_least32_t r = dst[k_mod_n] ^ dst[kp_mod_n] ^ dst[(k - 1) % n];
            r = 1664525 * (r ^ (r >> 27));
            dst[kp_mod_n] += r;
            r += static_cast<uint_least32_t>(k_mod_n) + src[k - 1];
            dst[(k + q) % n] += r;
            dst[k_mod_n] = r;
        }
        
        for (size_t k = s + 1; k < m; ++k) {
            size_t const k_mod_n = k % n;
            size_t const kp_mod_n = (k + p) % n;
            uint_least32_t r = dst[k_mod_n] ^ dst[kp_mod_n] ^ dst[(k - 1) % n];
            r = 1664525 * (r ^ (r >> 27));
            dst[kp_mod_n] += r;
            r += static_cast<uint_least32_t>(k_mod_n);
            dst[(k + q) % n] += r;
            dst[k_mod_n] = r;
        }

        for (size_t k = m; k < (m + n); ++k) {
            size_t const k_mod_n = k % n;
            size_t const kp_mod_n = (k + p) % n;
            uint_least32_t r = dst[k_mod_n] + dst[kp_mod_n] + dst[(k - 1) % n];
            r = 1566083941 * (r ^ (r >> 27));
            dst[kp_mod_n] ^= r;
            r -= static_cast<uint_least32_t>(k_mod_n);
            dst[(k + q) % n] ^= r;
            dst[k_mod_n] = r;
        }
    }
}
