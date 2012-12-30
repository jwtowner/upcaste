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

#include <up/cmath.hpp>
#include <up/ieee754.hpp>

namespace up { namespace math
{
    LIBUPCOREAPI UPNONNULLALL
    double frexp10(double x, int* e) noexcept {
        int e2, e10;
        double s;

        // get base 2 exponent
#if (DBL_MANT_DIG == 53) && (DBL_RADIX == 2)
        ieee754_binary64 raw;
        raw.d = x;
        e2 = static_cast<int>(raw.ieee.exponent) - ieee754_binary64_bias;
#else
    error "double-precision floating-point format not yet supported!"
#endif

        // determine base 10 exponent
#if INT_MAX >= ((1 << 30) - 1)
        e10 = (e2 * 19732) >> 16;
#else
        e10 = static_cast<int>(e2 * (DBL_LN2 / DBL_LN10));
#endif

        // compute significand and fine-tune exponent
        s = x / pow10(e10);
        
        while (s < 1.0) {
            --e10;
            s = x / pow10(e10);
        }

        while (s >= 10.0) {
            ++e10;
            s = x / pow10(e10);
        }

        *e = e10;
        return s;
    }
}}
