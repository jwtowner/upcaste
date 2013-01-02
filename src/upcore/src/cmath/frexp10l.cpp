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

#include <up/cmath.hpp>
#include <up/ieee754.hpp>

namespace up { namespace math
{
    LIBUPCOREAPI UPNONNULLALL
    long double frexp10(long double x, int* e) noexcept {
        int e2, e10;
        long double s;

        // get base 2 exponent
#if (LDBL_MANT_DIG == 53) && (FLT_RADIX == 2)
        ieee754_binary64 raw;
        raw.ld = x;
        if (!raw.ieee.exponent && !raw.ieee.mantissa) {
            *e = 0;
            return 0.0l;
        }
        e2 = static_cast<int>(raw.ieee.exponent) - ieee754_binary64_bias;
        if (e2 == (ieee754_binary64_bias + 1)) {
            return raw.ieee.mantissa ? LDBL_NAN : x;
        }
#elif (LDBL_MANT_DIG == 64) && (FLT_RADIX == 2)
        ieee754_binary96 raw;
        raw.ld = x;
        if (!raw.ieee.exponent && !raw.ieee.mantissa) {
            *e = 0;
            return 0.0l;
        }
        e2 = static_cast<int>(raw.ieee.exponent) - ieee754_binary96_bias;
        if (e2 == (ieee754_binary96_bias + 1)) {
            return raw.ieee.mantissa ? LDBL_NAN : x;
        }
#elif (LDBL_MANT_DIG == 113) && (FLT_RADIX == 2)
        ieee754_binary128 raw;
        raw.ld = x;
        if (!raw.ieee.exponent && !raw.ieee.mantissa0 && !raw.ieee.mantissa1) {
            *e = 0;
            return 0.0l;
        }
        e2 = static_cast<int>(raw.ieee.exponent) - ieee754_binary128_bias;
        if (e2 == (ieee754_binary128_bias + 1)) {
            return raw.ieee.mantissa ? LDBL_NAN : x;
        }
#else
    error "extended double-precision floating-point format not yet supported!"
#endif

        // determine base 10 exponent
#if INT_MAX >= ((1 << 30) - 1)
        e10 = (e2 * 19728) >> 16;
#else
        e10 = static_cast<int>(e2 * (LDBL_LN2 / LDBL_LN10));
#endif

        // compute significand and fine-tune exponent
        x = raw.ieee.negative ? -x : x;
        s = x * pow10l(-e10);
        
        while (s < 1.0l) {
            --e10;
            s = x * pow10l(-e10);
        }

        while (s >= 10.0l) {
            ++e10;
            s = x * pow10l(-e10);
        }

        *e = e10;
        return raw.ieee.negative ? -s : s;
    }
}}
