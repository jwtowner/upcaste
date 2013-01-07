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

    inline UPALWAYSINLINE float acosh(float x) noexcept { return ::acoshf(x); }
    inline UPALWAYSINLINE float asinh(float x) noexcept { return ::asinhf(x); }
    inline UPALWAYSINLINE float atanh(float x) noexcept { return ::atanhf(x); }
    inline UPALWAYSINLINE float cbrt(float x) noexcept { return ::cbrtf(x); }
    inline UPALWAYSINLINE float copysign(float x, float y) noexcept { return ::copysignf(x, y); }
    inline UPALWAYSINLINE float erf(float x) noexcept { return ::erff(x); }
    inline UPALWAYSINLINE float erfc(float x) noexcept { return ::erfcf(x); }
    inline UPALWAYSINLINE float exp2(float x) noexcept { return ::exp2f(x); }
    inline UPALWAYSINLINE float expm1(float x) noexcept { return ::expm1f(x); }
    inline UPALWAYSINLINE float fdim(float x, float y) noexcept { return ::fdimf(x, y); }
    inline UPALWAYSINLINE float fma(float x, float y, float z) noexcept { return ::fmaf(x, y, z); }
    inline UPALWAYSINLINE float fmax(float x, float y) noexcept { return ::fmaxf(x, y); }
    inline UPALWAYSINLINE float fmin(float x, float y) noexcept { return ::fminf(x, y); }
    inline UPALWAYSINLINE float hypot(float x, float y) noexcept { return ::hypotf(x, y); }    
    inline UPALWAYSINLINE int ilogb(float x) noexcept { return ::ilogbf(x); }
    inline UPALWAYSINLINE float lgamma(float x) noexcept { return ::lgammaf(x); }
    inline UPALWAYSINLINE float log1p(float x) noexcept { return ::log1pf(x); }
    inline UPALWAYSINLINE float log2(float x) noexcept { return ::log2f(x); }
    inline UPALWAYSINLINE float logb(float x) noexcept { return ::logbf(x); }
    inline UPALWAYSINLINE float nearbyint(float x) noexcept { return ::nearbyintf(x); }
    inline UPALWAYSINLINE float nextafter(float x, float y) noexcept { return ::nextafterf(x, y); }
    inline UPALWAYSINLINE float nexttoward(float x, float y) noexcept { return ::nexttowardf(x, y); }
    inline UPALWAYSINLINE float remainder(float x, float y) noexcept { return ::remainderf(x, y); }
    inline UPALWAYSINLINE float remquo(float x, float y, int* q) noexcept { return ::remquof(x, y, q); }
    inline UPALWAYSINLINE float rint(float x) noexcept { return ::rintf(x); }
    inline UPALWAYSINLINE long lrint(float x) noexcept { return ::lrintf(x); }
    inline UPALWAYSINLINE long long llrint(float x) noexcept { return ::llrintf(x); }
    inline UPALWAYSINLINE float round(float x) noexcept { return ::roundf(x); }
    inline UPALWAYSINLINE long lround(float x) noexcept { return ::lroundf(x); }
    inline UPALWAYSINLINE long long llround(float x) noexcept { return ::llroundf(x); }
    inline UPALWAYSINLINE float scalbln(float x, long y) noexcept { return ::scalblnf(x, y); }    
    inline UPALWAYSINLINE float scalbn(float x, int y) noexcept { return ::scalbnf(x, y); }    
    inline UPALWAYSINLINE float tgamma(float x) noexcept { return ::tgammaf(x); }
    inline UPALWAYSINLINE float trunc(float x) noexcept { return ::truncf(x); }

    inline UPALWAYSINLINE long double acosh(long double x) noexcept { return ::acoshl(x); }
    inline UPALWAYSINLINE long double asinh(long double x) noexcept { return ::asinhl(x); }
    inline UPALWAYSINLINE long double atanh(long double x) noexcept { return ::atanhl(x); }
    inline UPALWAYSINLINE long double cbrt(long double x) noexcept { return ::cbrtl(x); }
    inline UPALWAYSINLINE long double copysign(long double x, long double y) noexcept { return ::copysignl(x, y); }
    inline UPALWAYSINLINE long double erf(long double x) noexcept { return ::erfl(x); }
    inline UPALWAYSINLINE long double erfc(long double x) noexcept { return ::erfcl(x); }
    inline UPALWAYSINLINE long double exp2(long double x) noexcept { return ::exp2l(x); }
    inline UPALWAYSINLINE long double expm1(long double x) noexcept { return ::expm1l(x); }
    inline UPALWAYSINLINE long double fdim(long double x, long double y) noexcept { return ::fdiml(x, y); }
    inline UPALWAYSINLINE long double fma(long double x, long double y, long double z) noexcept { return ::fmal(x, y, z); }
    inline UPALWAYSINLINE long double fmax(long double x, long double y) noexcept { return ::fmaxl(x, y); }
    inline UPALWAYSINLINE long double fmin(long double x, long double y) noexcept { return ::fminl(x, y); }
    inline UPALWAYSINLINE long double hypot(long double x, long double y) noexcept { return ::hypotl(x, y); }    
    inline UPALWAYSINLINE int ilogb(long double x) noexcept { return ::ilogbl(x); }
    inline UPALWAYSINLINE long double lgamma(long double x) noexcept { return ::lgammal(x); }
    inline UPALWAYSINLINE long double log1p(long double x) noexcept { return ::log1pl(x); }
    inline UPALWAYSINLINE long double log2(long double x) noexcept { return ::log2l(x); }
    inline UPALWAYSINLINE long double logb(long double x) noexcept { return ::logbl(x); }
    inline UPALWAYSINLINE long double nearbyint(long double x) noexcept { return ::nearbyintl(x); }
    inline UPALWAYSINLINE long double nextafter(long double x, long double y) noexcept { return ::nextafterl(x, y); }
    inline UPALWAYSINLINE long double nexttoward(long double x, long double y) noexcept { return ::nexttowardl(x, y); }
    inline UPALWAYSINLINE long double remainder(long double x, long double y) noexcept { return ::remainderl(x, y); }
    inline UPALWAYSINLINE long double remquo(long double x, long double y, int* q) noexcept { return ::remquol(x, y, q); }
    inline UPALWAYSINLINE long double rint(long double x) noexcept { return ::rintl(x); }
    inline UPALWAYSINLINE long lrint(long double x) noexcept { return ::lrintl(x); }
    inline UPALWAYSINLINE long long llrint(long double x) noexcept { return ::llrintl(x); }
    inline UPALWAYSINLINE long double round(long double x) noexcept { return ::roundl(x); }
    inline UPALWAYSINLINE long lround(long double x) noexcept { return ::lroundl(x); }
    inline UPALWAYSINLINE long long llround(long double x) noexcept { return ::llroundl(x); }
    inline UPALWAYSINLINE long double scalbln(long double x, long y) noexcept { return ::scalblnl(x, y); }    
    inline UPALWAYSINLINE long double scalbn(long double x, int y) noexcept { return ::scalbnl(x, y); }    
    inline UPALWAYSINLINE long double tgamma(long double x) noexcept { return ::tgammal(x); }
    inline UPALWAYSINLINE long double trunc(long double x) noexcept { return ::truncl(x); }
}}

namespace up { namespace math { namespace detail
{
    template <class T> inline UPALWAYSINLINE int fpclassify_impl(T x) noexcept { return fpclassify(x); }
    template <class T> inline UPALWAYSINLINE bool isfinite_impl(T x) noexcept { return isfinite(x); }
    template <class T> inline UPALWAYSINLINE bool isinf_impl(T x) noexcept { return isinf(x); }
    template <class T> inline UPALWAYSINLINE bool isnan_impl(T x) noexcept { return isnan(x); }
    template <class T> inline UPALWAYSINLINE bool isnormal_impl(T x) noexcept { return isnormal(x); }
    template <class T> inline UPALWAYSINLINE bool isgreater_impl(T x, T y) noexcept { return isgreater(x, y); }
    template <class T> inline UPALWAYSINLINE bool isgreaterequal_impl(T x, T y) noexcept { return isgreaterequal(x, y); }
    template <class T> inline UPALWAYSINLINE bool isless_impl(T x, T y) noexcept { return isless(x, y); }
    template <class T> inline UPALWAYSINLINE bool islessequal_impl(T x, T y) noexcept { return islessequal(x, y); }
    template <class T> inline UPALWAYSINLINE bool islessgreater_impl(T x, T y) noexcept { return islessgreater(x, y); }
    template <class T> inline UPALWAYSINLINE bool isunordered_impl(T x, T y) noexcept { return isunordered(x, y); }
    template <class T> inline UPALWAYSINLINE bool signbit_impl(T x) noexcept { return signbit(x); }
}}}

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
#undef signbit

namespace up { namespace math
{
    inline UPALWAYSINLINE UPNONNULLALL double nan(char const* tagp) noexcept { return ::nan(tagp); }
    inline UPALWAYSINLINE UPNONNULLALL float nanf(char const* tagp) noexcept { return ::nanf(tagp); }
    inline UPALWAYSINLINE UPNONNULLALL long double nanl(char const* tagp) noexcept { return ::nanl(tagp); }

    inline UPALWAYSINLINE int fpclassify(double x) noexcept { return ::up::math::detail::fpclassify_impl(x); }
    inline UPALWAYSINLINE bool isfinite(double x) noexcept { return ::up::math::detail::isfinite_impl(x); }
    inline UPALWAYSINLINE bool isinf(double x) noexcept { return ::up::math::detail::isinf_impl(x); }
    inline UPALWAYSINLINE bool isnan(double x) noexcept { return ::up::math::detail::isnan_impl(x); }
    inline UPALWAYSINLINE bool isnormal(double x) noexcept { return ::up::math::detail::isnormal_impl(x); }
    inline UPALWAYSINLINE bool isgreater(double x, double y) noexcept { return ::up::math::detail::isgreater_impl(x, y); }
    inline UPALWAYSINLINE bool isgreaterequal(double x, double y) noexcept { return ::up::math::detail::isgreaterequal_impl(x, y); }
    inline UPALWAYSINLINE bool isless(double x, double y) noexcept { return ::up::math::detail::isless_impl(x, y); }
    inline UPALWAYSINLINE bool islessequal(double x, double y) noexcept { return ::up::math::detail::islessequal_impl(x, y); }
    inline UPALWAYSINLINE bool islessgreater(double x, double y) noexcept { return ::up::math::detail::islessgreater_impl(x, y); }
    inline UPALWAYSINLINE bool isunordered(double x, double y) noexcept { return ::up::math::detail::isunordered_impl(x, y); }
    inline UPALWAYSINLINE bool signbit(double x) noexcept { return ::up::math::detail::signbit_impl(x); }

    inline UPALWAYSINLINE int fpclassify(float x) noexcept { return ::up::math::detail::fpclassify_impl(x); }
    inline UPALWAYSINLINE bool isfinite(float x) noexcept { return ::up::math::detail::isfinite_impl(x); }
    inline UPALWAYSINLINE bool isinf(float x) noexcept { return ::up::math::detail::isinf_impl(x); }
    inline UPALWAYSINLINE bool isnan(float x) noexcept { return ::up::math::detail::isnan_impl(x); }
    inline UPALWAYSINLINE bool isnormal(float x) noexcept { return ::up::math::detail::isnormal_impl(x); }
    inline UPALWAYSINLINE bool isgreater(float x, float y) noexcept { return ::up::math::detail::isgreater_impl(x, y); }
    inline UPALWAYSINLINE bool isgreaterequal(float x, float y) noexcept { return ::up::math::detail::isgreaterequal_impl(x, y); }
    inline UPALWAYSINLINE bool isless(float x, float y) noexcept { return ::up::math::detail::isless_impl(x, y); }
    inline UPALWAYSINLINE bool islessequal(float x, float y) noexcept { return ::up::math::detail::islessequal_impl(x, y); }
    inline UPALWAYSINLINE bool islessgreater(float x, float y) noexcept { return ::up::math::detail::islessgreater_impl(x, y); }
    inline UPALWAYSINLINE bool isunordered(float x, float y) noexcept { return ::up::math::detail::isunordered_impl(x, y); }
    inline UPALWAYSINLINE bool signbit(float x) noexcept { return ::up::math::detail::signbit_impl(x); }

    inline UPALWAYSINLINE int fpclassify(long double x) noexcept { return ::up::math::detail::fpclassify_impl(x); }
    inline UPALWAYSINLINE bool isfinite(long double x) noexcept { return ::up::math::detail::isfinite_impl(x); }
    inline UPALWAYSINLINE bool isinf(long double x) noexcept { return ::up::math::detail::isinf_impl(x); }
    inline UPALWAYSINLINE bool isnan(long double x) noexcept { return ::up::math::detail::isnan_impl(x); }
    inline UPALWAYSINLINE bool isnormal(long double x) noexcept { return ::up::math::detail::isnormal_impl(x); }
    inline UPALWAYSINLINE bool isgreater(long double x, long double y) noexcept { return ::up::math::detail::isgreater_impl(x, y); }
    inline UPALWAYSINLINE bool isgreaterequal(long double x, long double y) noexcept { return ::up::math::detail::isgreaterequal_impl(x, y); }
    inline UPALWAYSINLINE bool isless(long double x, long double y) noexcept { return ::up::math::detail::isless_impl(x, y); }
    inline UPALWAYSINLINE bool islessequal(long double x, long double y) noexcept { return ::up::math::detail::islessequal_impl(x, y); }
    inline UPALWAYSINLINE bool islessgreater(long double x, long double y) noexcept { return ::up::math::detail::islessgreater_impl(x, y); }
    inline UPALWAYSINLINE bool isunordered(long double x, long double y) noexcept { return ::up::math::detail::isunordered_impl(x, y); }
    inline UPALWAYSINLINE bool signbit(long double x) noexcept { return ::up::math::detail::signbit_impl(x); }
}}

#endif
