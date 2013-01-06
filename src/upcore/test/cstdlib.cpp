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
#include <up/climits.hpp>
#include <up/cmath.hpp>
#include <up/test.hpp>

namespace cstdlib
{
    UP_TEST_CASE(fast_strtol) {
        char* endptr;

        errno = 0;
        require((16 == up::fast_strtol("16", nullptr, 0)) && (errno == 0));
        require((-27 == up::fast_strtol("-27", nullptr, 0)) && (errno == 0));
        require((38 == up::fast_strtol("+38", nullptr, 10)) && (errno == 0));
        require((19 == up::fast_strtol("  \r \n19", nullptr, 10)) && (errno == 0));
        require((21 == up::fast_strtol(" 10101foo ", &endptr, 2)) && (errno == 0) && !strcmp(endptr, "foo "));
        require((-51 == up::fast_strtol(" -110011foo ", &endptr, 2)) && (errno == 0) && !strcmp(endptr, "foo "));
        require((493 == up::fast_strtol(" 0755foo ", &endptr, 0)) && (errno == 0) && !strcmp(endptr, "foo "));
        require((302 == up::fast_strtol(" 456foo ", &endptr, 8)) && (errno == 0) && !strcmp(endptr, "foo "));
        require((-302 == up::fast_strtol(" -0456foo ", &endptr, 8)) && (errno == 0) && !strcmp(endptr, "foo "));
        require((256 == up::fast_strtol(" 0x100goo ", &endptr, 0)) && (errno == 0) && !strcmp(endptr, "goo "));
        require((512 == up::fast_strtol(" 0x200goo ", &endptr, 16)) && (errno == 0) && !strcmp(endptr, "goo "));
        require((-1024 == up::fast_strtol(" -400goo ", &endptr, 16)) && (errno == 0) && !strcmp(endptr, "goo "));
        require((LONG_MIN == up::fast_strtol("-2147483648", nullptr, 0)) && (errno == 0));
        require((LONG_MAX == up::fast_strtol("+2147483647", nullptr, 0)) && (errno == 0));
        require((LONG_MIN == up::fast_strtol("-2147483648", nullptr, 10)) && (errno == 0));
        require((LONG_MAX == up::fast_strtol("2147483647", nullptr, 10)) && (errno == 0));

        errno = 0;
        require((LONG_MIN == up::fast_strtol("-3947483647", nullptr, 10)) && (errno == ERANGE));

        errno = 0;
        require((LONG_MAX == up::fast_strtol("3947483647", nullptr, 10)) && (errno == ERANGE));

        errno = 0;
        require((0 == up::fast_strtol("+-", &endptr, 10)) && (errno == EINVAL) && !strcmp(endptr, "+-"));

        errno = 0;
        require((0 == up::fast_strtol("++3", &endptr, 10)) && (errno == EINVAL) && !strcmp(endptr, "++3"));
    }

    UP_TEST_CASE(fast_strtod) {
        char* endptr;

        errno = 0;
        require((0.0 == up::fast_strtod("0.0", nullptr)) && (errno == 0));
        require((-0.0 == up::fast_strtod("-0.0", nullptr)) && (errno == 0));
        require((16.0 == up::fast_strtod("\r\n16", nullptr)) && (errno == 0));
        require((+456.0 == up::fast_strtod("\v\t +456", nullptr)) && (errno == 0));
        require((-2791.0 == up::fast_strtod("-2791", nullptr)) && (errno == 0));
        require((0.55 == up::fast_strtod(".55", nullptr)) && (errno == 0));
        require((+0.55 == up::fast_strtod("+.55", nullptr)) && (errno == 0));
        require((-0.55 == up::fast_strtod("-.55", nullptr)) && (errno == 0));
        require((1242.55 == up::fast_strtod("1242.55", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((-2791.0 == up::fast_strtod("-2791.", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((0e12 == up::fast_strtod("e12", nullptr)) && (errno == 0));
        require((+0e12 == up::fast_strtod("+e12", nullptr)) && (errno == 0));
        require((-0e12 == up::fast_strtod("-E12", nullptr)) && (errno == 0));
        require((-44e12 == up::fast_strtod("-44E12", nullptr)) && (errno == 0));
        require((-2791.0e2 == up::fast_strtod("-2791.e2", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((+44.589e12 == up::fast_strtod("+44.589e12", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((-44.589e12 == up::fast_strtod("-44.589e12a", &endptr)) && (errno == 0) && !strcmp(endptr, "a"));
        require((-123456.0 == up::fast_strtod("-123456e", &endptr)) && (errno == 0) && !strcmp(endptr, "e"));
        require((-123456.0 == up::fast_strtod("-123456e-", &endptr)) && (errno == 0) && !strcmp(endptr, "e-"));
        require((-123456.0e-2 == up::fast_strtod("-123456e-2", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((1.0 == up::fast_strtod("+0x1", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((-1.0 == up::fast_strtod("-0x01", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((1223629.0 == up::fast_strtod("0x12ABCD", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((15667883.0 == up::fast_strtod("0XEF12AB", &endptr)) && (errno == 0) && !strcmp(endptr, ""));
        require((5.13405190144e+11 == up::fast_strtod("0xef12abp+15a", &endptr)) && (errno == 0) && !strcmp(endptr, "a"));
        require((478.145843505859375 == up::fast_strtod("0XEF12ABP-15b", &endptr)) && (errno == 0) && !strcmp(endptr, "b"));
        require(up::math::isinf(up::fast_strtod("inf", nullptr)) && (errno == 0));
        require(up::math::isinf(up::fast_strtod(" Inf ", nullptr)) && (errno == 0));
        require(up::math::isinf(up::fast_strtod("-INFa",  &endptr)) && (errno == 0) && !strcmp(endptr, "a"));
        require(up::math::isinf(up::fast_strtod(" +INFb",  &endptr)) && (errno == 0) && !strcmp(endptr, "b"));
        require(up::math::isnan(up::fast_strtod("nan", nullptr)) && (errno == 0));
        require(up::math::isnan(up::fast_strtod(" NaN ", nullptr)) && (errno == 0));
        require(up::math::isnan(up::fast_strtod("-NANa",  &endptr)) && (errno == 0) && !strcmp(endptr, "a"));
        require(up::math::isnan(up::fast_strtod(" +NANb",  &endptr)) && (errno == 0) && !strcmp(endptr, "b"));

        errno = 0;
        require((0.0 == up::fast_strtod(".", &endptr)) && !strcmp(endptr, "."));

        errno = 0;
        require((0.0 == up::fast_strtod("+-", &endptr)) && !strcmp(endptr, "+-"));

        errno = 0;
        require((0.0 == up::fast_strtod("+.", &endptr)) && !strcmp(endptr, "+."));

        errno = 0;
        require((0.0 == up::fast_strtod("++9", &endptr)) && !strcmp(endptr, "++9"));

        errno = 0;
        require((0.0 == up::fast_strtod("+ea", &endptr)) && !strcmp(endptr, "+ea"));

        errno = 0;
        require((0.0 == up::fast_strtod("+e-a", &endptr)) && !strcmp(endptr, "+e-a"));

        errno = 0;
        require((0.0 == up::fast_strtod("+.e33", &endptr)) && !strcmp(endptr, "+.e33"));

        errno = 0;
        require((0.0 == up::fast_strtod("-.e2", &endptr)) && !strcmp(endptr, "-.e2"));
    }
}
