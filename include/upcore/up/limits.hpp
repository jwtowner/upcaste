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

#ifndef UP_LIMITS_HPP
#define UP_LIMITS_HPP

#include <up/cfloat.hpp>
#include <up/climits.hpp>

namespace up
{
    enum float_denorm_style
    {
        denorm_indeterminate = -1,
        denorm_absent = 0,
        denorm_present = 1
    };

    enum float_round_style
    {
        round_indeterminate = -1,
        round_toward_zero = 0,
        round_to_nearest = 1,
        round_toward_infinity = 2,
        round_toward_neg_infinity = 3
    };
}

namespace up { namespace detail
{
    struct UPHIDDEN numeric_limits_base
    {
        static constexpr bool is_specialized = false;
        static constexpr int digits = 0;
        static constexpr int digits10 = 0;
        static constexpr int max_digits10 = 0;
        static constexpr bool is_signed = false;
        static constexpr bool is_integer = false;
        static constexpr bool is_exact = false;
        static constexpr int radix = 0;
        static constexpr int min_exponent = 0;
        static constexpr int min_exponent10 = 0;
        static constexpr int max_exponent = 0;
        static constexpr int max_exponent10 = 0;
        static constexpr bool has_infinity = false;
        static constexpr bool has_quiet_NaN = false;
        static constexpr bool has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm = denorm_absent;
        static constexpr bool has_denorm_loss = false;
        static constexpr bool is_iec559 = false;
        static constexpr bool is_bounded = false;
        static constexpr bool is_modulo = false;
        static constexpr bool traps = false;
        static constexpr bool tinyness_before = false;
        static constexpr float_round_style round_style = round_toward_zero;
    };	

    struct UPHIDDEN integral_limits_base
    {
        static constexpr bool is_specialized = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact = true;
        static constexpr int radix = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = true;
    };

    struct UPHIDDEN floating_point_limits_base
    {
        static constexpr bool is_specialized = true;
        static constexpr bool is_signed = true;
        static constexpr int radix = FLT_RADIX;
        static constexpr bool has_infinity = true;
        static constexpr bool has_quiet_NaN = true;
        static constexpr bool has_signaling_NaN = true;
        static constexpr float_denorm_style has_denorm = denorm_present;
        static constexpr bool has_denorm_loss = true;
        static constexpr bool is_iec559 = true;
        static constexpr bool is_bounded = true;
        static constexpr bool traps = true;
        static constexpr bool tinyness_before = true;
        static constexpr float_round_style round_style = round_to_nearest;
    };
}}

namespace up
{
    template <class T>
    struct UPVISIBLE numeric_limits : detail::numeric_limits_base
    {
        static UPALWAYSINLINE UPCONSTEXPR T min() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T max() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T lowest() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T epsilon() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T round_error() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T infinity() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T quiet_NaN() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T signaling_NaN() noexcept { return T(); }
        static UPALWAYSINLINE UPCONSTEXPR T denorm_min() noexcept { return T(); }
    };

    template <class T> struct UPVISIBLE numeric_limits<T const> : numeric_limits<T> { };
    template <class T> struct UPVISIBLE numeric_limits<T volatile> : numeric_limits<T> { };
    template <class T> struct UPVISIBLE numeric_limits<T const volatile> : numeric_limits<T> { };

    template <>
    struct UPVISIBLE numeric_limits<bool> : detail::integral_limits_base
    {
        static constexpr int is_signed = false;
        static constexpr int is_modulo = false;
        static constexpr int digits = 1;
        static constexpr int digits10 = 0;
        static constexpr int max_digits10 = 0;
        static UPALWAYSINLINE UPCONSTEXPR bool min() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool max() noexcept { return true; }
        static UPALWAYSINLINE UPCONSTEXPR bool lowest() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool epsilon() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool round_error() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool infinity() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool quiet_NaN() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool signaling_NaN() noexcept { return false; }
        static UPALWAYSINLINE UPCONSTEXPR bool denorm_min() noexcept { return false; }
    };

    template <>
    struct UPVISIBLE numeric_limits<float> : detail::floating_point_limits_base
    {
        static constexpr int digits = FLT_MANT_DIG;
        static constexpr int digits10 = FLT_DIG;
        static constexpr int max_digits10 = 2 + FLT_DIG;
        static constexpr int max_exponent = static_cast<int>(FLT_MAX_EXP);
        static constexpr int max_exponent10 = static_cast<int>(FLT_MAX_10_EXP);
        static constexpr int min_exponent = static_cast<int>(FLT_MIN_EXP);
        static constexpr int min_exponent10 = static_cast<int>(FLT_MIN_10_EXP);
        static UPALWAYSINLINE UPCONSTEXPR float min() noexcept { return FLT_MIN; }
        static UPALWAYSINLINE UPCONSTEXPR float max() noexcept { return FLT_MAX; }
        static UPALWAYSINLINE UPCONSTEXPR float lowest() noexcept { return -FLT_MAX; }
        static UPALWAYSINLINE UPCONSTEXPR float epsilon() noexcept { return FLT_EPSILON; }
        static UPALWAYSINLINE UPCONSTEXPR float round_error() noexcept { return 0.5f; }
        static UPALWAYSINLINE UPCONSTEXPR float infinity() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR float quiet_NaN() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR float signaling_NaN() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR float denorm_min() noexcept { return 0; }
    };

    template <>
    struct UPVISIBLE numeric_limits<double> : detail::floating_point_limits_base
    {
        static constexpr int digits = DBL_MANT_DIG;
        static constexpr int digits10 = DBL_DIG;
        static constexpr int max_digits10 = 2 + DBL_DIG;
        static constexpr int max_exponent = static_cast<int>(DBL_MAX_EXP);
        static constexpr int max_exponent10 = static_cast<int>(DBL_MAX_10_EXP);
        static constexpr int min_exponent = static_cast<int>(DBL_MIN_EXP);
        static constexpr int min_exponent10 = static_cast<int>(DBL_MIN_10_EXP);
        static UPALWAYSINLINE UPCONSTEXPR double min() noexcept { return DBL_MIN; }
        static UPALWAYSINLINE UPCONSTEXPR double max() noexcept { return DBL_MAX; }
        static UPALWAYSINLINE UPCONSTEXPR double lowest() noexcept { return -DBL_MAX; }
        static UPALWAYSINLINE UPCONSTEXPR double epsilon() noexcept { return DBL_EPSILON; }
        static UPALWAYSINLINE UPCONSTEXPR double round_error() noexcept { return 0.5; }
        static UPALWAYSINLINE UPCONSTEXPR double infinity() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR double quiet_NaN() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR double signaling_NaN() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR double denorm_min() noexcept { return 0; }
    };

    template <>
    struct UPVISIBLE numeric_limits<long double> : detail::floating_point_limits_base
    {
        static constexpr int digits = LDBL_MANT_DIG;
        static constexpr int digits10 = LDBL_DIG;
        static constexpr int max_digits10 = 2 + LDBL_DIG;
        static constexpr int max_exponent = static_cast<int>(LDBL_MAX_EXP);
        static constexpr int max_exponent10 = static_cast<int>(LDBL_MAX_10_EXP);
        static constexpr int min_exponent = static_cast<int>(LDBL_MIN_EXP);
        static constexpr int min_exponent10 = static_cast<int>(LDBL_MIN_10_EXP);
        static UPALWAYSINLINE UPCONSTEXPR long double min() noexcept { return LDBL_MIN; }
        static UPALWAYSINLINE UPCONSTEXPR long double max() noexcept { return LDBL_MAX; }
        static UPALWAYSINLINE UPCONSTEXPR long double lowest() noexcept { return -LDBL_MAX; }
        static UPALWAYSINLINE UPCONSTEXPR long double epsilon() noexcept { return LDBL_EPSILON; }
        static UPALWAYSINLINE UPCONSTEXPR long double round_error() noexcept { return 0.5; }
        static UPALWAYSINLINE UPCONSTEXPR long double infinity() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR long double quiet_NaN() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR long double signaling_NaN() noexcept { return 0; }
        static UPALWAYSINLINE UPCONSTEXPR long double denorm_min() noexcept { return 0; }
    };

#define UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(Type, MinValue, MaxValue) \
    template <> \
    struct UPVISIBLE numeric_limits<Type> : detail::integral_limits_base \
    { \
        static constexpr int is_signed = MinValue != 0; \
        static constexpr int digits = ((CHAR_BIT * sizeof(Type)) - (is_signed ? 1 : 0)); \
        static constexpr int digits10 = ((CHAR_BIT * sizeof(Type)) - (((is_signed ? 1 : 0) * 301L) / 1000)); \
        static constexpr int max_digits10 = (2 + ((CHAR_BIT * sizeof(wchar_t)) - (((is_signed ? 1 : 0) * 301L) / 1000))); \
        static UPALWAYSINLINE UPCONSTEXPR Type min() noexcept { return MinValue; } \
        static UPALWAYSINLINE UPCONSTEXPR Type max() noexcept { return MaxValue; } \
        static UPALWAYSINLINE UPCONSTEXPR Type lowest() noexcept { return MinValue; } \
        static UPALWAYSINLINE UPCONSTEXPR Type epsilon() noexcept { return 0; } \
        static UPALWAYSINLINE UPCONSTEXPR Type round_error() noexcept { return 0; } \
        static UPALWAYSINLINE UPCONSTEXPR Type infinity() noexcept { return 0; } \
        static UPALWAYSINLINE UPCONSTEXPR Type quiet_NaN() noexcept { return 0; } \
        static UPALWAYSINLINE UPCONSTEXPR Type signaling_NaN() noexcept { return 0; } \
        static UPALWAYSINLINE UPCONSTEXPR Type denorm_min() noexcept { return 0; } \
    };

    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(signed char, SCHAR_MIN, SCHAR_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(short, SHRT_MIN, SHRT_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(int, INT_MIN, INT_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(long, LONG_MIN, LONG_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(long long, LLONG_MIN, LLONG_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(unsigned char, 0, UCHAR_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(unsigned short, 0, USHRT_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(unsigned int, 0, UINT_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(unsigned long, 0, ULONG_MAX)
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(unsigned long long, 0, ULLONG_MAX)    
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(char, CHAR_MIN, CHAR_MAX)
#ifndef UP_NO_NATIVE_WCHAR_T
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(wchar_t, WCHAR_MIN, WCHAR_MAX)
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(char16_t, 0, USHRT_MAX)
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL(char32_t, 0, UINT_MAX)
#endif

#undef UP_DETAIL_DEFINE_NUMERIC_LIMITS_INTEGRAL
}

#endif
