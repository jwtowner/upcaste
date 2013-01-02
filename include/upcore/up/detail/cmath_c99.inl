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

#ifndef UP_DETAIL_CMATH_C99_INL
#define UP_DETAIL_CMATH_C99_INL

#ifndef UP_CMATH_HPP
#   error "Do not include this header directly. Instead include <up/cmath.hpp>"
#endif

#undef signbit
#undef fpclassify
#undef isfinite
#undef isinf
#undef isnan
#undef isnormal
#undef isgreater
#undef isgreaterequal
#undef isless
#undef islessequal
#undef islessgreater
#undef isunordered

namespace up { namespace math
{
    using ::float_t;
    using ::double_t;
    using ::acosh;
    using ::asinh;
    using ::atanh;
    using ::cbrt;
    using ::copysign;
    using ::erf;
    using ::erfc;
    using ::exp2;
    using ::expm1;
    using ::fdim;
    using ::fma;
    using ::fmax;
    using ::fmin;
    using ::hypot;
    using ::ilogb;
    using ::lgamma;
    using ::llrint;
    using ::llround;
    using ::log1p;
    using ::log2;
    using ::logb;
    using ::lrint;
    using ::lround;
    using ::nearbyint;
    using ::nextafter;
    using ::nexttoward;
    using ::remainder;
    using ::remquo;
    using ::rint;
    using ::round;
    using ::scalbln;
    using ::scalbn;
    using ::tgamma;
    using ::trunc;

    inline UPALWAYSINLINE UPNONNULLALL double nan(char const* tagp) noexcept { return ::nan(tagp); }
    inline UPALWAYSINLINE UPNONNULLALL float nanf(char const* tagp) noexcept { return ::nanf(tagp); }
    inline UPALWAYSINLINE UPNONNULLALL long double nanl(char const* tagp) noexcept { return ::nanl(tagp); }

    inline UPALWAYSINLINE UPPURE float acosh(float x) noexcept { return ::acoshf(x); }
    inline UPALWAYSINLINE UPPURE float asinh(float x) noexcept { return ::asinhf(x); }
    inline UPALWAYSINLINE UPPURE float atanh(float x) noexcept { return ::atanhf(x); }
    inline UPALWAYSINLINE UPPURE float cbrt(float x) noexcept { return ::cbrtf(x); }
    inline UPALWAYSINLINE UPPURE float copysign(float x, float y) noexcept { return ::copysignf(x, y); }
    inline UPALWAYSINLINE UPPURE float erf(float x) noexcept { return ::erff(x); }
    inline UPALWAYSINLINE UPPURE float erfc(float x) noexcept { return ::erfcf(x); }
    inline UPALWAYSINLINE UPPURE float exp2(float x) noexcept { return ::exp2f(x); }
    inline UPALWAYSINLINE UPPURE float expm1(float x) noexcept { return ::expm1f(x); }
    inline UPALWAYSINLINE UPPURE float fdim(float x, float y) noexcept { return ::fdimf(x, y); }
    inline UPALWAYSINLINE UPPURE float fma(float x, float y, float z) noexcept { return ::fmaf(x, y, z); }
    inline UPALWAYSINLINE UPPURE float fmax(float x, float y) noexcept { return ::fmaxf(x, y); }
    inline UPALWAYSINLINE UPPURE float fmin(float x, float y) noexcept { return ::fminf(x, y); }
    inline UPALWAYSINLINE UPPURE float hypot(float x, float y) noexcept { return ::hypotf(x, y); }    
    inline UPALWAYSINLINE UPPURE int ilogb(float x) noexcept { return ::ilogbf(x); }
    inline UPALWAYSINLINE UPPURE float lgamma(float x) noexcept { return ::lgammaf(x); }
    inline UPALWAYSINLINE UPPURE float log1p(float x) noexcept { return ::log1pf(x); }
    inline UPALWAYSINLINE UPPURE float log2(float x) noexcept { return ::log2f(x); }
    inline UPALWAYSINLINE UPPURE float logb(float x) noexcept { return ::logbf(x); }
    inline UPALWAYSINLINE UPPURE float nearbyint(float x) noexcept { return ::nearbyintf(x); }
    inline UPALWAYSINLINE UPPURE float nextafter(float x, float y) noexcept { return ::nextafterf(x, y); }
    inline UPALWAYSINLINE UPPURE float nexttoward(float x, float y) noexcept { return ::nexttowardf(x, y); }
    inline UPALWAYSINLINE UPPURE float remainder(float x, float y) noexcept { return ::remainderf(x, y); }
    inline UPALWAYSINLINE UPPURE float remquo(float x, float y, int* q) noexcept { return ::remquof(x, y, q); }
    inline UPALWAYSINLINE UPPURE float rint(float x) noexcept { return ::rintf(x); }
    inline UPALWAYSINLINE UPPURE long lrint(float x) noexcept { return ::lrintf(x); }
    inline UPALWAYSINLINE UPPURE long long llrint(float x) noexcept { return ::llrintf(x); }
    inline UPALWAYSINLINE UPPURE float round(float x) noexcept { return ::roundf(x); }
    inline UPALWAYSINLINE UPPURE long lround(float x) noexcept { return ::lroundf(x); }
    inline UPALWAYSINLINE UPPURE long long llround(float x) noexcept { return ::llroundf(x); }
    inline UPALWAYSINLINE UPPURE float scalbln(float x, long y) noexcept { return ::scalblnf(x, y); }    
    inline UPALWAYSINLINE UPPURE float scalbn(float x, int y) noexcept { return ::scalbnf(x, y); }    
    inline UPALWAYSINLINE UPPURE float tgamma(float x) noexcept { return ::tgammaf(x); }
    inline UPALWAYSINLINE UPPURE float trunc(float x) noexcept { return ::truncf(x); }

    inline UPALWAYSINLINE UPPURE long double acosh(long double x) noexcept { return ::acoshl(x); }
    inline UPALWAYSINLINE UPPURE long double asinh(long double x) noexcept { return ::asinhl(x); }
    inline UPALWAYSINLINE UPPURE long double atanh(long double x) noexcept { return ::atanhl(x); }
    inline UPALWAYSINLINE UPPURE long double cbrt(long double x) noexcept { return ::cbrtl(x); }
    inline UPALWAYSINLINE UPPURE long double copysign(long double x, long double y) noexcept { return ::copysignl(x, y); }
    inline UPALWAYSINLINE UPPURE long double erf(long double x) noexcept { return ::erfl(x); }
    inline UPALWAYSINLINE UPPURE long double erfc(long double x) noexcept { return ::erfcl(x); }
    inline UPALWAYSINLINE UPPURE long double exp2(long double x) noexcept { return ::exp2l(x); }
    inline UPALWAYSINLINE UPPURE long double expm1(long double x) noexcept { return ::expm1l(x); }
    inline UPALWAYSINLINE UPPURE long double fdim(long double x, long double y) noexcept { return ::fdiml(x, y); }
    inline UPALWAYSINLINE UPPURE long double fma(long double x, long double y, long double z) noexcept { return ::fmal(x, y, z); }
    inline UPALWAYSINLINE UPPURE long double fmax(long double x, long double y) noexcept { return ::fmaxl(x, y); }
    inline UPALWAYSINLINE UPPURE long double fmin(long double x, long double y) noexcept { return ::fminl(x, y); }
    inline UPALWAYSINLINE UPPURE long double hypot(long double x, long double y) noexcept { return ::hypotl(x, y); }    
    inline UPALWAYSINLINE UPPURE int ilogb(long double x) noexcept { return ::ilogbl(x); }
    inline UPALWAYSINLINE UPPURE long double lgamma(long double x) noexcept { return ::lgammal(x); }
    inline UPALWAYSINLINE UPPURE long double log1p(long double x) noexcept { return ::log1pl(x); }
    inline UPALWAYSINLINE UPPURE long double log2(long double x) noexcept { return ::log2l(x); }
    inline UPALWAYSINLINE UPPURE long double logb(long double x) noexcept { return ::logbl(x); }
    inline UPALWAYSINLINE UPPURE long double nearbyint(long double x) noexcept { return ::nearbyintl(x); }
    inline UPALWAYSINLINE UPPURE long double nextafter(long double x, long double y) noexcept { return ::nextafterl(x, y); }
    inline UPALWAYSINLINE UPPURE long double nexttoward(long double x, long double y) noexcept { return ::nexttowardl(x, y); }
    inline UPALWAYSINLINE UPPURE long double remainder(long double x, long double y) noexcept { return ::remainderl(x, y); }
    inline UPALWAYSINLINE UPPURE long double remquo(long double x, long double y, int* q) noexcept { return ::remquol(x, y, q); }
    inline UPALWAYSINLINE UPPURE long double rint(long double x) noexcept { return ::rintl(x); }
    inline UPALWAYSINLINE UPPURE long lrint(long double x) noexcept { return ::lrintl(x); }
    inline UPALWAYSINLINE UPPURE long long llrint(long double x) noexcept { return ::llrintl(x); }
    inline UPALWAYSINLINE UPPURE long double round(long double x) noexcept { return ::roundl(x); }
    inline UPALWAYSINLINE UPPURE long lround(long double x) noexcept { return ::lroundl(x); }
    inline UPALWAYSINLINE UPPURE long long llround(long double x) noexcept { return ::llroundl(x); }
    inline UPALWAYSINLINE UPPURE long double scalbln(long double x, long y) noexcept { return ::scalblnl(x, y); }    
    inline UPALWAYSINLINE UPPURE long double scalbn(long double x, int y) noexcept { return ::scalbnl(x, y); }    
    inline UPALWAYSINLINE UPPURE long double tgamma(long double x) noexcept { return ::tgammal(x); }
    inline UPALWAYSINLINE UPPURE long double trunc(long double x) noexcept { return ::truncl(x); }

    extern LIBUPCOREAPI UPPURE bool signbit(double x) noexcept;
    extern LIBUPCOREAPI UPPURE int fpclassify(double x) noexcept;
    inline UPALWAYSINLINE UPPURE bool isfinite(double x) noexcept { return fpclassify(x) >= FP_NORMAL; }
    inline UPALWAYSINLINE UPPURE bool isinf(double x) noexcept { return fpclassify(x) == FP_INFINITE; }
    inline UPALWAYSINLINE UPPURE bool isnan(double x) noexcept { return fpclassify(x) == FP_NAN; }
    inline UPALWAYSINLINE UPPURE bool isnormal(double x) noexcept { return fpclassify(x) == FP_NORMAL; }
    inline UPALWAYSINLINE UPPURE bool isgreater(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x > y) : false; }
    inline UPALWAYSINLINE UPPURE bool isgreaterequal(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x >= y) : false; }
    inline UPALWAYSINLINE UPPURE bool isless(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x < y) : false; }
    inline UPALWAYSINLINE UPPURE bool islessequal(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x <= y) : false; }
    inline UPALWAYSINLINE UPPURE bool islessgreater(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? ((x < y) || (x > y)) : false; }
    inline UPALWAYSINLINE UPPURE bool isunordered(double x, double y) noexcept { return isnan(x) || isnan(y); }

    extern LIBUPCOREAPI UPPURE bool signbit(float x) noexcept;
    extern LIBUPCOREAPI UPPURE int fpclassify(float x) noexcept;
    inline UPALWAYSINLINE UPPURE bool isfinite(float x) noexcept { return fpclassify(x) >= FP_NORMAL; }
    inline UPALWAYSINLINE UPPURE bool isinf(float x) noexcept { return fpclassify(x) == FP_INFINITE; }
    inline UPALWAYSINLINE UPPURE bool isnan(float x) noexcept { return fpclassify(x) == FP_NAN; }
    inline UPALWAYSINLINE UPPURE bool isnormal(float x) noexcept { return fpclassify(x) == FP_NORMAL; }
    inline UPALWAYSINLINE UPPURE bool isgreater(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x > y) : false; }
    inline UPALWAYSINLINE UPPURE bool isgreaterequal(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x >= y) : false; }
    inline UPALWAYSINLINE UPPURE bool isless(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x < y) : false; }
    inline UPALWAYSINLINE UPPURE bool islessequal(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x <= y) : false; }
    inline UPALWAYSINLINE UPPURE bool islessgreater(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? ((x < y) || (x > y)) : false; }
    inline UPALWAYSINLINE UPPURE bool isunordered(float x, float y) noexcept { return isnan(x) || isnan(y); }

    extern LIBUPCOREAPI UPPURE bool signbit(long double x) noexcept;
    extern LIBUPCOREAPI UPPURE int fpclassify(long double x) noexcept;
    inline UPALWAYSINLINE UPPURE bool isfinite(long double x) noexcept { return fpclassify(x) >= FP_NORMAL; }
    inline UPALWAYSINLINE UPPURE bool isinf(long double x) noexcept { return fpclassify(x) == FP_INFINITE; }
    inline UPALWAYSINLINE UPPURE bool isnan(long double x) noexcept { return fpclassify(x) == FP_NAN; }
    inline UPALWAYSINLINE UPPURE bool isnormal(long double x) noexcept { return fpclassify(x) == FP_NORMAL; }
    inline UPALWAYSINLINE UPPURE bool isgreater(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x > y) : false; }
    inline UPALWAYSINLINE UPPURE bool isgreaterequal(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x >= y) : false; }
    inline UPALWAYSINLINE UPPURE bool isless(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x < y) : false; }
    inline UPALWAYSINLINE UPPURE bool islessequal(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x <= y) : false; }
    inline UPALWAYSINLINE UPPURE bool islessgreater(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? ((x < y) || (x > y)) : false; }
    inline UPALWAYSINLINE UPPURE bool isunordered(long double x, long double y) noexcept { return isnan(x) || isnan(y); }
}}

#endif
