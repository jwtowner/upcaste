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

#include <up/cerrno.hpp>
#include <up/cmath.hpp>

namespace up { namespace math
{
    LIBUPCOREAPI
    REAL NAME(REAL x, LONGREAL y) noexcept {
        REAL significand, adjustment;
        int exponent;

        if (!isfinite(x)) {
            errno = EINVAL;
            return x;
        }
        else if (isnan(y)) {
            errno = EINVAL;
            return static_cast<REAL>(y);
        }
        else if (x == y) {
            return static_cast<REAL>(y);
        }

        if (x < y) {
            if (x >= REAL_MAX) {
                errno = ERANGE;
                return REAL_HUGE_VAL;
            }

            significand = frexp(x, &exponent);
            if (significand == -REAL_HALF) {
                --exponent;
            }

            adjustment = ldexp(REAL_ONE, exponent - REAL_MANT_DIG);
            if (adjustment == REAL_ZERO) {
                adjustment = REAL_MIN;
            }

            return significand + adjustment;
        }
        else {
            if (x <= -REAL_MAX) {
                errno = ERANGE;
                return -REAL_HUGE_VAL;
            }

            significand = frexp(x, &exponent);
            if (significand == REAL_HALF) {
                --exponent;
            }

            adjustment = ldexp(REAL_ONE, exponent - REAL_MANT_DIG);
            if (adjustment == REAL_ZERO) {
                adjustment = REAL_MIN;
            }

            return significand - adjustment;
        }
    }
}}
