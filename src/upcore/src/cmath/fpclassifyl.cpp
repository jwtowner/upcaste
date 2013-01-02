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

#include <up/prolog.hpp>

#if !defined(UP_HAS_STDC_MATH_CXX11)

#include <up/cmath.hpp>
#include <up/cstdlib.hpp>
#include <up/ieee754.hpp>

namespace up { namespace math
{
    LIBUPCOREAPI UPPURE
    int fpclassify(long double x) noexcept {
#if (LDBL_MANT_DIG == 53) && (LDBL_RADIX == 2)
        ieee754_binary64 bin;
        bin.ld = x;
        if (bin.ieee.exponent == 0) {
            return (bin.ieee.mantissa == 0) ? FP_ZERO : FP_SUBNORMAL;
        }
        if (bin.ieee.exponent == 0x7FF) {
            return (bin.ieee.mantissa == 0) ? FP_INFINITE : FP_NAN;
        }
        return FP_NORMAL;
#elif (DBL_MANT_DIG == 64) && (DBL_RADIX == 2)
        ieee754_binary96 bin;
        bin.ld = x;
        if (bin.ieee.exponent == 0) {
            return ((bin.ieee.mantissa0 == 0) && (bin.ieee.mantissa1 == 0)) ? FP_ZERO : FP_SUBNORMAL;
        }
        if (bin.ieee.exponent == 0x7FFF) {
            return ((bin.ieee.mantissa0 == 0) && (bin.ieee.mantissa1 == 0)) ? FP_INFINITE : FP_NAN;
        }
        return FP_NORMAL;
#elif (DBL_MANT_DIG == 113) && (DBL_RADIX == 2)
        ieee754_binary128 bin;
        bin.ld = x;
        if (bin.ieee.exponent == 0) {
            return ((bin.ieee.mantissa0 == 0) && (bin.ieee.mantissa1 == 0)) ? FP_ZERO : FP_SUBNORMAL;
        }
        if (bin.ieee.exponent == 0x7FFF) {
            return ((bin.ieee.mantissa0 == 0) && (bin.ieee.mantissa1 == 0)) ? FP_INFINITE : FP_NAN;
        }
        return FP_NORMAL;
#else
    error "extended double-precision floating-point format not yet supported!"
#endif
    }
}}

#endif
