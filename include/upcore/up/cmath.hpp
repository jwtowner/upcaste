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

#ifndef UP_CMATH_HPP
#define UP_CMATH_HPP

#include <up/cfloat.hpp>
#include <up/cstdlib.hpp>
#include <cmath>

namespace up { namespace math
{
    using ::std::abs;
    using ::std::acos;
    using ::std::asin;
    using ::std::atan;
    using ::std::atan2;
    using ::std::ceil;
    using ::std::cos;
    using ::std::cosh;
    using ::std::exp;
    using ::std::fabs;
    using ::std::floor;
    using ::std::fmod;
    using ::std::frexp;
    using ::std::ldexp;
    using ::std::log;
    using ::std::log10;
    using ::std::modf;
    using ::std::pow;
    using ::std::sin;
    using ::std::sinh;
    using ::std::sqrt;
    using ::std::tan;
    using ::std::tanh;
}}

#if defined(UP_HAS_STDC_MATH_CXX11)
#   include <up/detail/cmath_cxx11.inl>
#elif defined(UP_HAS_STDC_MATH_C99)
#   include <up/detail/cmath_c99.inl>
#else
#   include <up/detail/cmath_generic.inl>
#endif

namespace up { namespace math
{
    extern LIBUPCOREAPI UPNONNULLALL double frexp10(double x, int* exp) noexcept;
    extern LIBUPCOREAPI UPNONNULLALL float frexp10(float x, int* exp) noexcept;
    extern LIBUPCOREAPI UPNONNULLALL long double frexp10(long double x, int* exp) noexcept;

    extern LIBUPCOREAPI UPPURE double pow10(int n) noexcept;
    extern LIBUPCOREAPI UPPURE float pow10f(int n) noexcept;
    extern LIBUPCOREAPI UPPURE long double pow10l(int n) noexcept;

    inline UPALWAYSINLINE double acospi(double x) noexcept { return acos(x) * DBL_1_PI; }
    inline UPALWAYSINLINE double asinpi(double x) noexcept { return asin(x) * DBL_1_PI; }
    inline UPALWAYSINLINE double atanpi(double x) noexcept { return atan(x) * DBL_1_PI; }
    inline UPALWAYSINLINE double atan2pi(double x, double y) noexcept { return atan2(x, y) * DBL_1_PI; }
    inline UPALWAYSINLINE double clamp(double x, double xmin, double xmax) noexcept { return fmax(fmin(x, xmax), xmin); }
    inline UPALWAYSINLINE double cospi(double x) noexcept { return cos(x * DBL_PI); }
    inline UPALWAYSINLINE double degrees(double x) noexcept { return x * DBL_DEGREES; }
    inline UPALWAYSINLINE double distance(double x, double y) noexcept { return fabs(y - x); }
    inline UPALWAYSINLINE double dot(double x, double y) noexcept { return x * y; }
    inline UPALWAYSINLINE double exp10(double x) noexcept { return pow(10.0, x); }
    inline UPALWAYSINLINE double fmod2pi(double x) noexcept { return fmod(x, DBL_2PI); }
    inline UPALWAYSINLINE double fms(double x, double y, double z) noexcept { return fma(x, y, -z); }
    inline UPALWAYSINLINE double fnma(double x, double y, double z) noexcept { return fma(-x, y, z); }
    inline UPALWAYSINLINE double fnms(double x, double y, double z) noexcept { return fma(-x, y, -z); }
    inline UPALWAYSINLINE double length(double x) noexcept { return fabs(x); }
    inline UPALWAYSINLINE double mad(double x, double y, double z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE double max(double x, double y) noexcept { return fmax(x, y); }
    inline UPALWAYSINLINE double min(double x, double y) noexcept { return fmin(x, y); }
    inline UPALWAYSINLINE double mix(double x, double y, double a) noexcept { return x + ((y - x) * a); }
    inline UPALWAYSINLINE double normalize(double) noexcept { return 1.0; }
    inline UPALWAYSINLINE double radians(double x) noexcept { return x * DBL_RADIANS; }
    inline UPALWAYSINLINE double repeat(double edge, double x) noexcept { return mad((floor(x) - x), edge, edge); }
    inline UPALWAYSINLINE double repeat2pi(double x) noexcept { return repeat(DBL_2PI, x); }
    inline UPALWAYSINLINE double rcp(double x) noexcept { return 1.0 / x; }
    inline UPALWAYSINLINE double rsqrt(double x) noexcept { return 1.0 / sqrt(x); }
    inline UPALWAYSINLINE double sinpi(double x) noexcept { return sin(x * DBL_PI); }
    inline UPALWAYSINLINE double sincos(double x, double* c) noexcept { *c = cos(x); return sin(x); }
    inline UPALWAYSINLINE double sign(double x) noexcept { return (x < 0.0) ? -1.0 : 1.0; }
    inline UPALWAYSINLINE double step(double edge, double x) noexcept { return (x < edge) ? 0.0 : 1.0; }
    inline UPALWAYSINLINE double tanpi(double x) noexcept { return tan(x * DBL_PI); }
    inline UPALWAYSINLINE double wrap(double x, double y) noexcept { return x - y * floor(x / y); }
    inline UPALWAYSINLINE double wrappi(double x) noexcept { return x - DBL_2PI * floor((x + DBL_PI) / DBL_2PI); }
    inline UPALWAYSINLINE double wrap2pi(double x) noexcept { return wrap(x, DBL_2PI); }

    inline UPALWAYSINLINE
    double maxmag(double x, double y) noexcept {
        double const ax = fabs(x);
        double const ay = fabs(y);
        return (ax <= ay) ? ((ay <= ax) ? fmax(x, y) : y) : x;
    }

    inline UPALWAYSINLINE
    double minmag(double x, double y) noexcept {
        double const ax = fabs(x);
        double const ay = fabs(y);
        return (ax >= ay) ? ((ay >= ax) ? fmin(x, y) : y) : x;
    }

    inline UPALWAYSINLINE
    double barycentric(double x, double y, double z, double d1, double d2) noexcept {
        return x + ((y - x) * d1) + ((z - x) * d2);
    }

    inline UPALWAYSINLINE
    double catmullrom(double x, double y, double z, double w, double d) noexcept {
        double const d2 = d * d;
        double const d3 = d2 * d;
        double const a = (2.0 * y) + ((z - x) * d);
        double const b = ((2.0 * x) - (5.0 * y) + (4.0 * z) - w) * d2;
        double const c = (-x + (3.0 * y) - (3.0 * z) + w) * d3;
        return (a + b + c) / 2.0;
    }

    inline UPALWAYSINLINE
    double hermite(double x, double y, double d) noexcept {
        double const t = d * d * (3.0 - (2.0 * d));
        return (t * y) - (t * x) + x;
    }

    inline UPALWAYSINLINE
    double smoothstep(double edge0, double edge1, double d) noexcept {
        double const t = clamp((d - edge0) / (edge1 - edge0), 0.0, 1.0);
        return t * t * (3.0 - (2.0 * t));
    }
    
    inline UPALWAYSINLINE float acospi(float x) noexcept { return acos(x) * FLT_1_PI; }
    inline UPALWAYSINLINE float asinpi(float x) noexcept { return asin(x) * FLT_1_PI; }
    inline UPALWAYSINLINE float atanpi(float x) noexcept { return atan(x) * FLT_1_PI; }
    inline UPALWAYSINLINE float atan2pi(float x, float y) noexcept { return atan2(x, y) * FLT_1_PI; }
    inline UPALWAYSINLINE float clamp(float x, float xmin, float xmax) noexcept { return fmax(fmin(x, xmax), xmin); }
    inline UPALWAYSINLINE float cospi(float x) noexcept { return cos(x * FLT_PI); }
    inline UPALWAYSINLINE float degrees(float x) noexcept { return x * FLT_DEGREES; }
    inline UPALWAYSINLINE float distance(float x, float y) noexcept { return fabs(y - x); }
    inline UPALWAYSINLINE float dot(float x, float y) noexcept { return x * y; }
    inline UPALWAYSINLINE float exp10(float x) noexcept { return pow(10.0f, x); }
    inline UPALWAYSINLINE float fmod2pi(float x) noexcept { return fmod(x, FLT_2PI); }
    inline UPALWAYSINLINE float fms(float x, float y, float z) noexcept { return fma(x, y, -z); }
    inline UPALWAYSINLINE float fnma(float x, float y, float z) noexcept { return fma(-x, y, z); }
    inline UPALWAYSINLINE float fnms(float x, float y, float z) noexcept { return fma(-x, y, -z); }
    inline UPALWAYSINLINE float length(float x) noexcept { return fabs(x); }
    inline UPALWAYSINLINE float mad(float x, float y, float z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE float max(float x, float y) noexcept { return fmax(x, y); }
    inline UPALWAYSINLINE float min(float x, float y) noexcept { return fmin(x, y); }
    inline UPALWAYSINLINE float mix(float x, float y, float a) noexcept { return x + ((y - x) * a); }
    inline UPALWAYSINLINE float normalize(float) noexcept { return 1.0f; }
    inline UPALWAYSINLINE float radians(float x) noexcept { return x * FLT_RADIANS; }
    inline UPALWAYSINLINE float repeat(float edge, float x) noexcept { return mad((floor(x) - x), edge, edge); }
    inline UPALWAYSINLINE float repeat2pi(float x) noexcept { return repeat(FLT_2PI, x); }
    inline UPALWAYSINLINE float rcp(float x) noexcept { return 1.0f / x; }
    inline UPALWAYSINLINE float rsqrt(float x) noexcept { return 1.0f / sqrt(x); }
    inline UPALWAYSINLINE float sinpi(float x) noexcept { return sin(x * FLT_PI); }
    inline UPALWAYSINLINE float sincos(float x, float* c) noexcept { *c = cos(x); return sin(x); }
    inline UPALWAYSINLINE float sign(float x) noexcept { return (x < 0.0f) ? -1.0f : 1.0f; }
    inline UPALWAYSINLINE float step(float edge, float x) noexcept { return (x < edge) ? 0.0f : 1.0f; }
    inline UPALWAYSINLINE float tanpi(float x) noexcept { return tan(x * FLT_PI); }
    inline UPALWAYSINLINE float wrap(float x, float y) noexcept { return x - y * floor(x / y); }
    inline UPALWAYSINLINE float wrappi(float x) noexcept { return x - FLT_2PI * floor((x + FLT_PI) / FLT_2PI); }
    inline UPALWAYSINLINE float wrap2pi(float x) noexcept { return wrap(x, FLT_2PI); }

    inline UPALWAYSINLINE
    float maxmag(float x, float y) noexcept {
        float const ax = fabs(x);
        float const ay = fabs(y);
        return (ax <= ay) ? ((ay <= ax) ? fmax(x, y) : y) : x;
    }

    inline UPALWAYSINLINE
    float minmag(float x, float y) noexcept {
        float const ax = fabs(x);
        float const ay = fabs(y);
        return (ax >= ay) ? ((ay >= ax) ? fmin(x, y) : y) : x;
    }

    inline UPALWAYSINLINE
    float barycentric(float x, float y, float z, float d1, float d2) noexcept {
        return x + ((y - x) * d1) + ((z - x) * d2);
    }

    inline UPALWAYSINLINE
    float catmullrom(float x, float y, float z, float w, float d) noexcept {
        float const d2 = d * d;
        float const d3 = d2 * d;
        float const a = (2.0f * y) + ((z - x) * d);
        float const b = ((2.0f * x) - (5.0f * y) + (4.0f * z) - w) * d2;
        float const c = (-x + (3.0f * y) - (3.0f * z) + w) * d3;
        return (a + b + c) / 2.0f;
    }

    inline UPALWAYSINLINE
    float hermite(float x, float y, float d) noexcept {
        float const t = d * d * (3.0f - (2.0f * d));
        return (t * y) - (t * x) + x;
    }

    inline UPALWAYSINLINE
    float smoothstep(float edge0, float edge1, float d) noexcept {
        float const t = clamp((d - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - (2.0f * t));
    }

    inline UPALWAYSINLINE long double acospi(long double x) noexcept { return acos(x) * LDBL_1_PI; }
    inline UPALWAYSINLINE long double asinpi(long double x) noexcept { return asin(x) * LDBL_1_PI; }
    inline UPALWAYSINLINE long double atanpi(long double x) noexcept { return atan(x) * LDBL_1_PI; }
    inline UPALWAYSINLINE long double atan2pi(long double x, long double y) noexcept { return atan2(x, y) * LDBL_1_PI; }
    inline UPALWAYSINLINE long double clamp(long double x, long double xmin, long double xmax) noexcept { return fmax(fmin(x, xmax), xmin); }
    inline UPALWAYSINLINE long double cospi(long double x) noexcept { return cos(x * LDBL_PI); }
    inline UPALWAYSINLINE long double degrees(long double x) noexcept { return x * LDBL_DEGREES; }
    inline UPALWAYSINLINE long double distance(long double x, long double y) noexcept { return fabs(y - x); }
    inline UPALWAYSINLINE long double dot(long double x, long double y) noexcept { return x * y; }
    inline UPALWAYSINLINE long double exp10(long double x) noexcept { return pow(10.0l, x); }
    inline UPALWAYSINLINE long double fmod2pi(long double x) noexcept { return fmod(x, LDBL_2PI); }
    inline UPALWAYSINLINE long double fms(long double x, long double y, long double z) noexcept { return fma(x, y, -z); }
    inline UPALWAYSINLINE long double fnma(long double x, long double y, long double z) noexcept { return fma(-x, y, z); }
    inline UPALWAYSINLINE long double fnms(long double x, long double y, long double z) noexcept { return fma(-x, y, -z); }
    inline UPALWAYSINLINE long double length(long double x) noexcept { return fabs(x); }
    inline UPALWAYSINLINE long double mad(long double x, long double y, long double z) noexcept { return (x * y) + z; }
    inline UPALWAYSINLINE long double max(long double x, long double y) noexcept { return fmax(x, y); }
    inline UPALWAYSINLINE long double min(long double x, long double y) noexcept { return fmin(x, y); }
    inline UPALWAYSINLINE long double mix(long double x, long double y, long double a) noexcept { return x + ((y - x) * a); }
    inline UPALWAYSINLINE long double normalize(long double) noexcept { return 1.0l; }
    inline UPALWAYSINLINE long double radians(long double x) noexcept { return x * LDBL_RADIANS; }
    inline UPALWAYSINLINE long double repeat(long double edge, long double x) noexcept { return mad((floor(x) - x), edge, edge); }
    inline UPALWAYSINLINE long double repeat2pi(long double x) noexcept { return repeat(LDBL_2PI, x); }
    inline UPALWAYSINLINE long double rcp(long double x) noexcept { return 1.0l / x; }
    inline UPALWAYSINLINE long double rsqrt(long double x) noexcept { return 1.0l / sqrt(x); }
    inline UPALWAYSINLINE long double sinpi(long double x) noexcept { return sin(x * LDBL_PI); }
    inline UPALWAYSINLINE long double sincos(long double x, long double* c) noexcept { *c = cos(x); return sin(x); }
    inline UPALWAYSINLINE long double sign(long double x) noexcept { return (x < 0.0l) ? -1.0l : 1.0l; }
    inline UPALWAYSINLINE long double step(long double edge, long double x) noexcept { return (x < edge) ? 0.0l : 1.0l; }
    inline UPALWAYSINLINE long double tanpi(long double x) noexcept { return tan(x * LDBL_PI); }
    inline UPALWAYSINLINE long double wrap(long double x, long double y) noexcept { return x - y * floor(x / y); }
    inline UPALWAYSINLINE long double wrappi(long double x) noexcept { return x - LDBL_2PI * floor((x + LDBL_PI) / LDBL_2PI); }
    inline UPALWAYSINLINE long double wrap2pi(long double x) noexcept { return wrap(x, LDBL_2PI); }

    inline UPALWAYSINLINE
    long double maxmag(long double x, long double y) noexcept {
        long double const ax = fabs(x);
        long double const ay = fabs(y);
        return (ax <= ay) ? ((ay <= ax) ? fmax(x, y) : y) : x;
    }

    inline UPALWAYSINLINE
    long double minmag(long double x, long double y) noexcept {
        long double const ax = fabs(x);
        long double const ay = fabs(y);
        return (ax >= ay) ? ((ay >= ax) ? fmin(x, y) : y) : x;
    }

    inline UPALWAYSINLINE
    long double barycentric(long double x, long double y, long double z, long double d1, long double d2) noexcept {
        return x + ((y - x) * d1) + ((z - x) * d2);
    }

    inline UPALWAYSINLINE
    long double catmullrom(long double x, long double y, long double z, long double w, long double d) noexcept {
        long double const d2 = d * d;
        long double const d3 = d2 * d;
        long double const a = (2.0l * y) + ((z - x) * d);
        long double const b = ((2.0l * x) - (5.0l * y) + (4.0l * z) - w) * d2;
        long double const c = (-x + (3.0l * y) - (3.0l * z) + w) * d3;
        return (a + b + c) / 2.0l;
    }

    inline UPALWAYSINLINE
    long double hermite(long double x, long double y, long double d) noexcept {
        long double const t = d * d * (3.0l - (2.0l * d));
        return (t * y) - (t * x) + x;
    }

    inline UPALWAYSINLINE
    long double smoothstep(long double edge0, long double edge1, long double d) noexcept {
        long double const t = clamp((d - edge0) / (edge1 - edge0), 0.0l, 1.0l);
        return t * t * (3.0l - (2.0l * t));
    }
}}

namespace up { namespace math { namespace detail
{
    template <class> struct math_rebind;

    template <>
    struct UPHIDDEN math_rebind<double>
    {
        static UPALWAYSINLINE UPNONNULLALL double nan(char const* tagp) noexcept { return ::up::math::nan(tagp); };
        static UPALWAYSINLINE UPPURE double pow10(int n) noexcept { return ::up::math::pow10(n); };
    };

    template <>
    struct UPHIDDEN math_rebind<float>
    {
        static UPALWAYSINLINE UPNONNULLALL float nan(char const* tagp) noexcept { return ::up::math::nanf(tagp); };
        static UPALWAYSINLINE UPPURE float pow10(int n) noexcept { return ::up::math::pow10f(n); };
    };

    template <>
    struct UPHIDDEN math_rebind<long double>
    {
        static UPALWAYSINLINE UPNONNULLALL long double nan(char const* tagp) noexcept { return ::up::math::nanl(tagp); };
        static UPALWAYSINLINE UPPURE long double pow10(int n) noexcept { return ::up::math::pow10l(n); };
    };
}}}

namespace up { namespace math
{
    template <class T>
    inline UPALWAYSINLINE UPNONNULLALL
    T nan(char const* tagp) noexcept {
        return ::up::math::detail::math_rebind<T>::nan(tagp);
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    T pow10(int n) noexcept {
        return ::up::math::detail::math_rebind<T>::pow10(n);
    }
}}

#endif

