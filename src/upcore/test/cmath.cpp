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

#include <up/cstdlib.hpp>
#include <up/cmath.hpp>
#include <up/test.hpp>

namespace cmath
{
    UP_TEST_CASE(frexp10) {
        double significand;
        int exponent;

        significand = up::math::frexp10(44.0e+15, &exponent);
        require(up::math::isnormal(significand) && (exponent == 16));
        require_approx(significand, 4.4, 0.0000000001);

        significand = up::math::frexp10(-293.5e-8, &exponent);
        require(up::math::isnormal(significand) && (exponent == -6));
        require_approx(significand, -2.935, 0.0000000001);

        significand = up::math::frexp10(0.0, &exponent);
        require(!up::math::isnan(significand) && (significand == 0.0) && (exponent == 0));

        significand = up::math::frexp10(DBL_INFINITY, &exponent);
        require(up::math::isinf(significand) && (significand == DBL_INFINITY));

        significand = up::math::frexp10(-DBL_INFINITY, &exponent);
        require(up::math::isinf(significand) && (significand == -DBL_INFINITY));

        significand = up::math::frexp10(DBL_NAN, &exponent);
        require(up::math::isnan(significand));
    }

    UP_TEST_CASE(frexp10f) {
        float significand;
        int exponent;

        significand = up::math::frexp10(44.0e+15f, &exponent);
        require(up::math::isnormal(significand) && (exponent == 16));
        require_approx(significand, 4.4f, 0.000001f);

        significand = up::math::frexp10(-293.5e-8f, &exponent);
        require(up::math::isnormal(significand) && (exponent == -6));
        require_approx(significand, -2.935f, 0.000001f);

        significand = up::math::frexp10(0.0f, &exponent);
        require(!up::math::isnan(significand) && (significand == 0.0f) && (exponent == 0));

        significand = up::math::frexp10(FLT_INFINITY, &exponent);
        require(up::math::isinf(significand) && (significand == FLT_INFINITY));

        significand = up::math::frexp10(-FLT_INFINITY, &exponent);
        require(up::math::isinf(significand) && (significand == -FLT_INFINITY));

        significand = up::math::frexp10(FLT_NAN, &exponent);
        require(up::math::isnan(significand));
    }

    UP_TEST_CASE(frexp10l) {
        long double significand;
        int exponent;

        significand = up::math::frexp10(44.0e+15l, &exponent);
        require(up::math::isnormal(significand) && (exponent == 16));
        require_approx(significand, 4.4l, 0.0000000001l);

        significand = up::math::frexp10(-293.5e-8l, &exponent);
        require(up::math::isnormal(significand) && (exponent == -6));
        require_approx(significand, -2.935l, 0.0000000001l);

        significand = up::math::frexp10(0.0l, &exponent);
        require(!up::math::isnan(significand) && (significand == 0.0l) && (exponent == 0));

        significand = up::math::frexp10(LDBL_INFINITY, &exponent);
        require(up::math::isinf(significand) && (significand == LDBL_INFINITY));

        significand = up::math::frexp10(-LDBL_INFINITY, &exponent);
        require(up::math::isinf(significand) && (significand == -LDBL_INFINITY));

        significand = up::math::frexp10(LDBL_NAN, &exponent);
        require(up::math::isnan(significand));
    }
}
