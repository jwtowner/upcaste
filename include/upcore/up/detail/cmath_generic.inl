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

#ifndef UP_DETAIL_CMATH_GENERIC_INL
#define UP_DETAIL_CMATH_GENERIC_INL

#ifndef UP_CMATH_HPP
#   error "Do not include this header directly. Instead include <up/cmath.hpp>"
#endif

#ifndef MATH_ERRNO
#   define MATH_ERRNO 1
#endif
#ifndef MATH_ERREXCEPT
#   define MATH_ERREXCEPT 2
#endif
#ifndef math_errhandling
#   define math_errhandling ((int)MATH_ERRNO)
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
#ifndef FP_ILOGB0
#   define FP_ILOGB0 INT_MIN
#endif
#ifndef FP_ILOGBNAN
#   define FP_ILOGBNAN INT_MAX
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
#ifndef INFINITY
#   define INFINITY FLT_INFINITY
#endif
#ifndef NAN
#   define NAN FLT_NAN
#endif

namespace up { namespace math
{
#if FLT_EVAL_METHOD == 0
    typedef float float_t;
    typedef double double_t;
#elif FLT_EVAL_METHOD == 1
    typedef double float_t;
    typedef double double_t;
#elif FLT_EVAL_METHOD == 2
    typedef long double float_t;
    typedef long double double_t;
#endif

    extern LIBUPCOREAPI UPNONNULLALL double nan(char const* tagp) noexcept;
    extern LIBUPCOREAPI UPNONNULLALL float nanf(char const* tagp) noexcept;
    extern LIBUPCOREAPI UPNONNULLALL long double nanl(char const* tagp) noexcept;

    extern LIBUPCOREAPI int fpclassify(double x) noexcept;
    extern LIBUPCOREAPI int fpclassify(float x) noexcept;
    extern LIBUPCOREAPI int fpclassify(long double x) noexcept;

    extern LIBUPCOREAPI bool signbit(double x) noexcept;
    extern LIBUPCOREAPI bool signbit(float x) noexcept;
    extern LIBUPCOREAPI bool signbit(long double x) noexcept;

    inline UPALWAYSINLINE bool isfinite(double x) noexcept { int const r = fpclassify(x); return (r != FP_INFINITE) && (r != FP_NAN); }
    inline UPALWAYSINLINE bool isinf(double x) noexcept { return fpclassify(x) == FP_INFINITE; }
    inline UPALWAYSINLINE bool isnan(double x) noexcept { return fpclassify(x) == FP_NAN; }
    inline UPALWAYSINLINE bool isnormal(double x) noexcept { return fpclassify(x) == FP_NORMAL; }
    inline UPALWAYSINLINE bool isgreater(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x > y) : false; }
    inline UPALWAYSINLINE bool isgreaterequal(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x >= y) : false; }
    inline UPALWAYSINLINE bool isless(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x < y) : false; }
    inline UPALWAYSINLINE bool islessequal(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x <= y) : false; }
    inline UPALWAYSINLINE bool islessgreater(double x, double y) noexcept { return (!isnan(x) && !isnan(y)) ? ((x < y) || (x > y)) : false; }
    inline UPALWAYSINLINE bool isunordered(double x, double y) noexcept { return isnan(x) || isnan(y); }

    inline UPALWAYSINLINE bool isfinite(float x) noexcept { int const r = fpclassify(x); return (r != FP_INFINITE) && (r != FP_NAN); }
    inline UPALWAYSINLINE bool isinf(float x) noexcept { return fpclassify(x) == FP_INFINITE; }
    inline UPALWAYSINLINE bool isnan(float x) noexcept { return fpclassify(x) == FP_NAN; }
    inline UPALWAYSINLINE bool isnormal(float x) noexcept { return fpclassify(x) == FP_NORMAL; }
    inline UPALWAYSINLINE bool isgreater(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x > y) : false; }
    inline UPALWAYSINLINE bool isgreaterequal(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x >= y) : false; }
    inline UPALWAYSINLINE bool isless(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x < y) : false; }
    inline UPALWAYSINLINE bool islessequal(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? (x <= y) : false; }
    inline UPALWAYSINLINE bool islessgreater(float x, float y) noexcept { return (!isnan(x) && !isnan(y)) ? ((x < y) || (x > y)) : false; }
    inline UPALWAYSINLINE bool isunordered(float x, float y) noexcept { return isnan(x) || isnan(y); }

    inline UPALWAYSINLINE bool isfinite(long double x) noexcept { int const r = fpclassify(x); return (r != FP_INFINITE) && (r != FP_NAN); }
    inline UPALWAYSINLINE bool isinf(long double x) noexcept { return fpclassify(x) == FP_INFINITE; }
    inline UPALWAYSINLINE bool isnan(long double x) noexcept { return fpclassify(x) == FP_NAN; }
    inline UPALWAYSINLINE bool isnormal(long double x) noexcept { return fpclassify(x) == FP_NORMAL; }
    inline UPALWAYSINLINE bool isgreater(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x > y) : false; }
    inline UPALWAYSINLINE bool isgreaterequal(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x >= y) : false; }
    inline UPALWAYSINLINE bool isless(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x < y) : false; }
    inline UPALWAYSINLINE bool islessequal(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? (x <= y) : false; }
    inline UPALWAYSINLINE bool islessgreater(long double x, long double y) noexcept { return (!isnan(x) && !isnan(y)) ? ((x < y) || (x > y)) : false; }
    inline UPALWAYSINLINE bool isunordered(long double x, long double y) noexcept { return isnan(x) || isnan(y); }

    extern LIBUPCOREAPI double acosh(double x) noexcept;
    extern LIBUPCOREAPI double asinh(double x) noexcept;
    extern LIBUPCOREAPI double atanh(double x) noexcept;
    extern LIBUPCOREAPI double erf(double x) noexcept;
    extern LIBUPCOREAPI double erfc(double x) noexcept;
    extern LIBUPCOREAPI double lgamma(double x) noexcept;
    extern LIBUPCOREAPI double nearbyint(double x) noexcept;
    extern LIBUPCOREAPI double nextafter(double x, double y) noexcept;
    extern LIBUPCOREAPI double nexttoward(double x, long double y) noexcept;
    extern LIBUPCOREAPI double remainder(double x, double y) noexcept;
    extern LIBUPCOREAPI double remquo(double x, double y, int* q) noexcept;
    extern LIBUPCOREAPI double scalbln(double x, long y) noexcept;
    extern LIBUPCOREAPI double scalbn(double x, int y) noexcept;
    extern LIBUPCOREAPI double rint(double x) noexcept;
    extern LIBUPCOREAPI double tgamma(double x) noexcept;

    extern LIBUPCOREAPI float acosh(float x) noexcept;
    extern LIBUPCOREAPI float asinh(float x) noexcept;
    extern LIBUPCOREAPI float atanh(float x) noexcept;
    extern LIBUPCOREAPI float erf(float x) noexcept;
    extern LIBUPCOREAPI float erfc(float x) noexcept;
    extern LIBUPCOREAPI float lgamma(float x) noexcept;
    extern LIBUPCOREAPI float nearbyint(float x) noexcept;
    extern LIBUPCOREAPI float nextafter(float x, float y) noexcept;
    extern LIBUPCOREAPI float nexttoward(float x, long double y) noexcept;
    extern LIBUPCOREAPI float remainder(float x, float y) noexcept;
    extern LIBUPCOREAPI float remquo(float x, float y, int* q) noexcept;
    extern LIBUPCOREAPI float rint(float x) noexcept;
    extern LIBUPCOREAPI float scalbln(float x, long y) noexcept;
    extern LIBUPCOREAPI float scalbn(float x, int y) noexcept;
    extern LIBUPCOREAPI float tgamma(float x) noexcept;

    extern LIBUPCOREAPI long double acosh(long double x) noexcept;
    extern LIBUPCOREAPI long double asinh(long double x) noexcept;
    extern LIBUPCOREAPI long double atanh(long double x) noexcept;
    extern LIBUPCOREAPI long double erf(long double x) noexcept;
    extern LIBUPCOREAPI long double erfc(long double x) noexcept;
    extern LIBUPCOREAPI long double lgamma(long double x) noexcept;
    extern LIBUPCOREAPI long double nearbyint(long double x) noexcept;
    extern LIBUPCOREAPI long double nextafter(long double x, long double y) noexcept;
    extern LIBUPCOREAPI long double nexttoward(long double x, long double y) noexcept;
    extern LIBUPCOREAPI long double remainder(long double x, long double y) noexcept;
    extern LIBUPCOREAPI long double remquo(long double x, long double y, int* q) noexcept;
    extern LIBUPCOREAPI long double rint(long double x) noexcept;
    extern LIBUPCOREAPI long double scalbln(long double x, long y) noexcept;
    extern LIBUPCOREAPI long double scalbn(long double x, int y) noexcept;
    extern LIBUPCOREAPI long double tgamma(long double x) noexcept;

    inline UPALWAYSINLINE int ilogb(double x) noexcept { return static_cast<int>(log(fabs(x)) / DBL_LN2); }
    inline UPALWAYSINLINE int ilogb(float x) noexcept { return static_cast<int>(log(fabs(x)) / FLT_LN2); }
    inline UPALWAYSINLINE int ilogb(long double x) noexcept { return static_cast<int>(log(fabs(x)) / LDBL_LN2); }

    inline UPALWAYSINLINE double cbrt(double x) noexcept { return pow(x, 1.0 / 3.0); }
    inline UPALWAYSINLINE double copysign(double x, double y) noexcept { return fabs(x) * ((y < 0.0) ? -1.0 : 1.0); }
    inline UPALWAYSINLINE double exp2(double x) noexcept { return pow(2.0, x); }
    inline UPALWAYSINLINE double expm1(double x) noexcept { return exp(x) - 1.0; }
    inline UPALWAYSINLINE double fdim(double x, double y) noexcept { return fabs(x - y); }
    inline UPALWAYSINLINE double fma(double x, double y, double z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE double fmax(double x, double y) noexcept { return (x < y) ? y : x; }
    inline UPALWAYSINLINE double fmin(double x, double y) noexcept { return (x < y) ? x : y; }
    inline UPALWAYSINLINE double hypot(double x, double y) noexcept { return sqrt((x * x) + (y * y)); }
    inline UPALWAYSINLINE long long llrint(double x) noexcept { return static_cast<long long>(x); }
    inline UPALWAYSINLINE long long llround(double x) noexcept { return static_cast<long long>(floor(x + 0.5)); }
    inline UPALWAYSINLINE double log1p(double x) noexcept { return log(x + 1.0); }
    inline UPALWAYSINLINE double log2(double x) noexcept { return log(x) / DBL_LN2; }
    inline UPALWAYSINLINE double logb(double x) noexcept { return log(fabs(x)) / DBL_LN2; }
    inline UPALWAYSINLINE long lrint(double x) noexcept { return static_cast<long>(x); }
    inline UPALWAYSINLINE long lround(double x) noexcept { return static_cast<long>(floor(x + 0.5)); }
    inline UPALWAYSINLINE double round(double x) noexcept { return floor(x + 0.5); }
    inline UPALWAYSINLINE double trunc(double x) noexcept { return copysign(floor(fabs(x)), x); }
    
    inline UPALWAYSINLINE float cbrt(float x) noexcept { return pow(x, 1.0f / 3.0f); }
    inline UPALWAYSINLINE float copysign(float x, float y) noexcept { return fabs(x) * ((y < 0.0f) ? -1.0f : 1.0f); }
    inline UPALWAYSINLINE float exp2(float x) noexcept { return pow(2.0f, x); }
    inline UPALWAYSINLINE float expm1(float x) noexcept { return exp(x) - 1.0f; }
    inline UPALWAYSINLINE float fdim(float x, float y) noexcept { return fabs(x - y); }
    inline UPALWAYSINLINE float fma(float x, float y, float z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE float fmax(float x, float y) noexcept { return (x < y) ? y : x; }
    inline UPALWAYSINLINE float fmin(float x, float y) noexcept { return (x < y) ? x : y; }
    inline UPALWAYSINLINE float hypot(float x, float y) noexcept { return sqrt((x * x) + (y * y)); }
    inline UPALWAYSINLINE long long llrint(float x) noexcept { return static_cast<long long>(x); }
    inline UPALWAYSINLINE long long llround(float x) noexcept { return static_cast<long long>(floor(x + 0.5f)); }
    inline UPALWAYSINLINE float log1p(float x) noexcept { return log(x + 1.0f); }
    inline UPALWAYSINLINE float log2(float x) noexcept { return log(x) / FLT_LN2; }
    inline UPALWAYSINLINE float logb(float x) noexcept { return log(fabs(x)) / FLT_LN2; }
    inline UPALWAYSINLINE long lrint(float x) noexcept { return static_cast<long>(x); }
    inline UPALWAYSINLINE long lround(float x) noexcept { return static_cast<long>(floor(x + 0.5f)); }
    inline UPALWAYSINLINE float round(float x) noexcept { return floor(x + 0.5f); }
    inline UPALWAYSINLINE float trunc(float x) noexcept { return copysign(floor(fabs(x)), x); }

    inline UPALWAYSINLINE long double cbrt(long double x) noexcept { return pow(x, 1.0l / 3.0l); }
    inline UPALWAYSINLINE long double copysign(long double x, long double y) noexcept { return fabs(x) * ((y < 0.0l) ? -1.0l : 1.0l); }
    inline UPALWAYSINLINE long double exp2(long double x) noexcept { return pow(2.0l, x); }
    inline UPALWAYSINLINE long double expm1(long double x) noexcept { return exp(x) - 1.0l; }
    inline UPALWAYSINLINE long double fdim(long double x, long double y) noexcept { return fabs(x - y); }
    inline UPALWAYSINLINE long double fma(long double x, long double y, long double z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE long double fmax(long double x, long double y) noexcept { return (x < y) ? y : x; }
    inline UPALWAYSINLINE long double fmin(long double x, long double y) noexcept { return (x < y) ? x : y; }
    inline UPALWAYSINLINE long double hypot(long double x, long double y) noexcept { return sqrt((x * x) + (y * y)); }
    inline UPALWAYSINLINE long long llrint(long double x) noexcept { return static_cast<long long>(x); }
    inline UPALWAYSINLINE long long llround(long double x) noexcept { return static_cast<long long>(floor(x + 0.5l)); }
    inline UPALWAYSINLINE long double log1p(long double x) noexcept { return log(x + 1.0l); }
    inline UPALWAYSINLINE long double log2(long double x) noexcept { return log(x) / LDBL_LN2; }
    inline UPALWAYSINLINE long double logb(long double x) noexcept { return log(fabs(x)) / LDBL_LN2; }
    inline UPALWAYSINLINE long lrint(long double x) noexcept { return static_cast<long>(x); }
    inline UPALWAYSINLINE long lround(long double x) noexcept { return static_cast<long>(floor(x + 0.5l)); }
    inline UPALWAYSINLINE long double round(long double x) noexcept { return floor(x + 0.5l); }
    inline UPALWAYSINLINE long double trunc(long double x) noexcept { return copysign(floor(fabs(x)), x); }
}}

#endif
