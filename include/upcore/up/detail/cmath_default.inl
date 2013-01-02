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

#ifndef UP_DETAIL_CMATH_DEFAULT_INL
#define UP_DETAIL_CMATH_DEFAULT_INL

#ifndef UP_CMATH_HPP
#   error "Do not include this header directly. Instead include <up/cmath.hpp>"
#endif

#ifndef HUGE_VAL
#   define HUGE_VAL DBL_INFINITY
#endif
#ifndef HUGE_VALF
#   define HUGE_VALF FLT_INFINITY
#endif
#ifndef HUGE_VALL
#   define HUGE_VALL LDBL_INFINITY
#endif
#ifndef FP_INFINITE
#   define FP_INFINITE  0
#endif
#ifndef FP_NAN
#   define FP_NAN       1
#endif
#ifndef FP_NORMAL
#   define FP_NORMAL    2
#endif
#ifndef FP_SUBNORMAL
#   define FP_SUBNORMAL 3
#endif
#ifndef FP_ZERO
#   define FP_ZERO      4
#endif

namespace up { namespace math
{
    typedef float float_t;
    typedef double double_t;

    extern LIBUPCOREAPI double acosh(double x) noexcept;
    extern LIBUPCOREAPI double asinh(double x) noexcept;
    extern LIBUPCOREAPI double atanh(double x) noexcept;
    extern LIBUPCOREAPI double erf(double x) noexcept;
    extern LIBUPCOREAPI double erfc(double x) noexcept;
    extern LIBUPCOREAPI double lgamma(double x) noexcept;
    extern LIBUPCOREAPI double tgamma(double x) noexcept;

    extern LIBUPCOREAPI float acosh(float x) noexcept;
    extern LIBUPCOREAPI float asinh(float x) noexcept;
    extern LIBUPCOREAPI float atanh(float x) noexcept;
    extern LIBUPCOREAPI float erf(float x) noexcept;
    extern LIBUPCOREAPI float erfc(float x) noexcept;
    extern LIBUPCOREAPI float lgamma(float x) noexcept;
    extern LIBUPCOREAPI float tgamma(float x) noexcept;

    extern LIBUPCOREAPI long double acosh(long double x) noexcept;
    extern LIBUPCOREAPI long double asinh(long double x) noexcept;
    extern LIBUPCOREAPI long double atanh(long double x) noexcept;
    extern LIBUPCOREAPI long double erf(long double x) noexcept;
    extern LIBUPCOREAPI long double erfc(long double x) noexcept;
    extern LIBUPCOREAPI long double lgamma(long double x) noexcept;
    extern LIBUPCOREAPI long double tgamma(long double x) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL double nan(char const* tagp) noexcept;
    extern LIBUPCOREAPI UPNONNULLALL float nanf(char const* tagp) noexcept;
    extern LIBUPCOREAPI UPNONNULLALL long double nanl(char const* tagp) noexcept;

    extern LIBUPCOREAPI UPPURE int fpclassify(double x) noexcept;
    extern LIBUPCOREAPI UPPURE int fpclassify(float x) noexcept;
    extern LIBUPCOREAPI UPPURE int fpclassify(long double x) noexcept;

    extern LIBUPCOREAPI UPPURE bool signbit(double x) noexcept;
    extern LIBUPCOREAPI UPPURE bool signbit(float x) noexcept;
    extern LIBUPCOREAPI UPPURE bool signbit(long double x) noexcept;

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

    inline UPALWAYSINLINE UPPURE double cbrt(double x) noexcept { return pow(x, 1.0 / 3.0); }
    inline UPALWAYSINLINE UPPURE double copysign(double x, double y) noexcept { return fabs(x) * ((y < 0.0) ? -1.0 : 1.0); }
    inline UPALWAYSINLINE UPPURE double exp2(double x) noexcept { return pow(2.0, x); }
    inline UPALWAYSINLINE UPPURE double expm1(double x) noexcept { return exp(x) - 1.0; }
    inline UPALWAYSINLINE UPPURE double fdim(double x, double y) noexcept { return fabs(x - y); }
    inline UPALWAYSINLINE UPPURE double fma(double x, double y, double z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE UPPURE double fmax(double x, double y) noexcept { return (x < y) ? y : x; }
    inline UPALWAYSINLINE UPPURE double fmin(double x, double y) noexcept { return (x < y) ? x : y; }
    inline UPALWAYSINLINE UPPURE double hypot(double x, double y) noexcept { return sqrt((x * x) + (y * y)); }
    inline UPALWAYSINLINE UPPURE int ilogb(double x) noexcept { return static_cast<int>(log(fabs(x)) / DBL_LN2); }
    inline UPALWAYSINLINE UPPURE long long llrint(double x) noexcept { return static_cast<long long>(x); }
    inline UPALWAYSINLINE UPPURE long long llround(double x) noexcept { return static_cast<long long>(floor(x + 0.5)); }
    inline UPALWAYSINLINE UPPURE double log1p(double x) noexcept { return log(x + 1.0); }
    inline UPALWAYSINLINE UPPURE double log2(double x) noexcept { return log(x) / DBL_LN2; }
    inline UPALWAYSINLINE UPPURE double logb(double x) noexcept { return log(fabs(x)) / DBL_LN2; }
    inline UPALWAYSINLINE UPPURE long lrint(double x) noexcept { return static_cast<long>(x); }
    inline UPALWAYSINLINE UPPURE long lround(double x) noexcept { return static_cast<long>(floor(x + 0.5)); }
    inline UPALWAYSINLINE UPPURE double rint(double x) noexcept { return floor(x); }
    inline UPALWAYSINLINE UPPURE double round(double x) noexcept { return floor(x + 0.5); }
    inline UPALWAYSINLINE UPPURE double trunc(double x) noexcept { return copysign(floor(fabs(x)), x); }
    
    inline UPALWAYSINLINE UPPURE float cbrt(float x) noexcept { return pow(x, 1.0f / 3.0f); }
    inline UPALWAYSINLINE UPPURE float copysign(float x, float y) noexcept { return fabs(x) * ((y < 0.0f) ? -1.0f : 1.0f); }
    inline UPALWAYSINLINE UPPURE float exp2(float x) noexcept { return pow(2.0f, x); }
    inline UPALWAYSINLINE UPPURE float expm1(float x) noexcept { return exp(x) - 1.0f; }
    inline UPALWAYSINLINE UPPURE float fdim(float x, float y) noexcept { return fabs(x - y); }
    inline UPALWAYSINLINE UPPURE float fma(float x, float y, float z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE UPPURE float fmax(float x, float y) noexcept { return (x < y) ? y : x; }
    inline UPALWAYSINLINE UPPURE float fmin(float x, float y) noexcept { return (x < y) ? x : y; }
    inline UPALWAYSINLINE UPPURE float hypot(float x, float y) noexcept { return sqrt((x * x) + (y * y)); }
    inline UPALWAYSINLINE UPPURE int ilogb(float x) noexcept { return static_cast<int>(log(fabs(x)) / FLT_LN2); }
    inline UPALWAYSINLINE UPPURE long long llrint(float x) noexcept { return static_cast<long long>(x); }
    inline UPALWAYSINLINE UPPURE long long llround(float x) noexcept { return static_cast<long long>(floor(x + 0.5f)); }
    inline UPALWAYSINLINE UPPURE float log1p(float x) noexcept { return log(x + 1.0f); }
    inline UPALWAYSINLINE UPPURE float log2(float x) noexcept { return log(x) / FLT_LN2; }
    inline UPALWAYSINLINE UPPURE float logb(float x) noexcept { return log(fabs(x)) / FLT_LN2; }
    inline UPALWAYSINLINE UPPURE long lrint(float x) noexcept { return static_cast<long>(x); }
    inline UPALWAYSINLINE UPPURE long lround(float x) noexcept { return static_cast<long>(floor(x + 0.5f)); }
    inline UPALWAYSINLINE UPPURE float rint(float x) noexcept { return floor(x); }
    inline UPALWAYSINLINE UPPURE float round(float x) noexcept { return floor(x + 0.5f); }
    inline UPALWAYSINLINE UPPURE float trunc(float x) noexcept { return copysign(floor(fabs(x)), x); }

    inline UPALWAYSINLINE UPPURE long double cbrt(long double x) noexcept { return pow(x, 1.0l / 3.0l); }
    inline UPALWAYSINLINE UPPURE long double copysign(long double x, long double y) noexcept { return fabs(x) * ((y < 0.0l) ? -1.0l : 1.0l); }
    inline UPALWAYSINLINE UPPURE long double exp2(long double x) noexcept { return pow(2.0l, x); }
    inline UPALWAYSINLINE UPPURE long double expm1(long double x) noexcept { return exp(x) - 1.0l; }
    inline UPALWAYSINLINE UPPURE long double fdim(long double x, long double y) noexcept { return fabs(x - y); }
    inline UPALWAYSINLINE UPPURE long double fma(long double x, long double y, long double z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE UPPURE long double fmax(long double x, long double y) noexcept { return (x < y) ? y : x; }
    inline UPALWAYSINLINE UPPURE long double fmin(long double x, long double y) noexcept { return (x < y) ? x : y; }
    inline UPALWAYSINLINE UPPURE long double hypot(long double x, long double y) noexcept { return sqrt((x * x) + (y * y)); }
    inline UPALWAYSINLINE UPPURE int ilogb(long double x) noexcept { return static_cast<int>(log(fabs(x)) / LDBL_LN2); }
    inline UPALWAYSINLINE UPPURE long long llrint(long double x) noexcept { return static_cast<long long>(x); }
    inline UPALWAYSINLINE UPPURE long long llround(long double x) noexcept { return static_cast<long long>(floor(x + 0.5l)); }
    inline UPALWAYSINLINE UPPURE long double log1p(long double x) noexcept { return log(x + 1.0l); }
    inline UPALWAYSINLINE UPPURE long double log2(long double x) noexcept { return log(x) / LDBL_LN2; }
    inline UPALWAYSINLINE UPPURE long double logb(long double x) noexcept { return log(fabs(x)) / LDBL_LN2; }
    inline UPALWAYSINLINE UPPURE long lrint(long double x) noexcept { return static_cast<long>(x); }
    inline UPALWAYSINLINE UPPURE long lround(long double x) noexcept { return static_cast<long>(floor(x + 0.5l)); }
    inline UPALWAYSINLINE UPPURE long double rint(long double x) noexcept { return floor(x); }
    inline UPALWAYSINLINE UPPURE long double round(long double x) noexcept { return floor(x + 0.5l); }
    inline UPALWAYSINLINE UPPURE long double trunc(long double x) noexcept { return copysign(floor(fabs(x)), x); }
}}

#endif
