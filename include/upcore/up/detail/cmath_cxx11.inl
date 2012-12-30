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

#ifndef UP_DETAIL_CMATH_CXX11_INL
#define UP_DETAIL_CMATH_CXX11_INL

#ifndef UP_CMATH_HPP
#   error "Do not include this header directly. Instead include <up/cmath.hpp>"
#endif

namespace up { namespace math
{
    using ::std::float_t;
    using ::std::double_t;
    using ::std::acosh;
    using ::std::asinh;
    using ::std::atanh;
    using ::std::cbrt;
    using ::std::copysign;
    using ::std::erf;
    using ::std::erfc;
    using ::std::exp2;
    using ::std::expm1;
    using ::std::fdim;
    using ::std::fma;
    using ::std::fmax;
    using ::std::fmin;
    using ::std::fpclassify;
    using ::std::hypot;
    using ::std::isfinite;
    using ::std::isgreater;
    using ::std::isgreaterequal;
    using ::std::isinf;
    using ::std::isless;
    using ::std::islessequal;
    using ::std::islessgreater;
    using ::std::isnan;
    using ::std::isnormal;
    using ::std::isunordered;
    using ::std::ilogb;
    using ::std::lgamma;
    using ::std::llrint;
    using ::std::llround;
    using ::std::log1p;
    using ::std::log2;
    using ::std::logb;
    using ::std::lrint;
    using ::std::lround;
    using ::std::nan;
    using ::std::nanf;
    using ::std::nanl;
    using ::std::nearbyint;
    using ::std::nextafter;
    using ::std::nexttoward;
    using ::std::remainder;
    using ::std::remquo;
    using ::std::rint;
    using ::std::round;
    using ::std::scalbln;
    using ::std::scalbn;
    using ::std::signbit;
    using ::std::tgamma;
    using ::std::trunc;
}}

#endif
