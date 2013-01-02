﻿//
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

//
//  Do not edit! This file is automatically generated.
//

#ifndef UP_MATH_MATHFWD_HPP
#define UP_MATH_MATHFWD_HPP

#include <up/cmath.hpp>
#include <up/utility.hpp>
#include <limits>

namespace up { namespace math
{
    template <class T> struct as_floating_point;
    template <> struct LIBUPCOREAPI as_floating_point<int8_t> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<int16_t> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<int32_t> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<int64_t> { typedef double type; };
    template <> struct LIBUPCOREAPI as_floating_point<uint8_t> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<uint16_t> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<uint32_t> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<uint64_t> { typedef double type; };
    template <> struct LIBUPCOREAPI as_floating_point<float> { typedef float type; };
    template <> struct LIBUPCOREAPI as_floating_point<double> { typedef double type; };
 
    template <class T> struct as_signed;
    template <> struct LIBUPCOREAPI as_signed<int8_t> { typedef int8_t type; };
    template <> struct LIBUPCOREAPI as_signed<int16_t> { typedef int16_t type; };
    template <> struct LIBUPCOREAPI as_signed<int32_t> { typedef int32_t type; };
    template <> struct LIBUPCOREAPI as_signed<int64_t> { typedef int64_t type; };
    template <> struct LIBUPCOREAPI as_signed<uint8_t> { typedef int8_t type; };
    template <> struct LIBUPCOREAPI as_signed<uint16_t> { typedef int16_t type; };
    template <> struct LIBUPCOREAPI as_signed<uint32_t> { typedef int32_t type; };
    template <> struct LIBUPCOREAPI as_signed<uint64_t> { typedef int64_t type; };
    template <> struct LIBUPCOREAPI as_signed<float> { typedef int32_t type; };
    template <> struct LIBUPCOREAPI as_signed<double> { typedef int64_t type; };

    template <class T> struct as_unsigned;
    template <> struct LIBUPCOREAPI as_unsigned<int8_t> { typedef uint8_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<int16_t> { typedef uint16_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<int32_t> { typedef uint32_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<int64_t> { typedef uint64_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<uint8_t> { typedef uint8_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<uint16_t> { typedef uint16_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<uint32_t> { typedef uint32_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<uint64_t> { typedef uint64_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<float> { typedef uint32_t type; };
    template <> struct LIBUPCOREAPI as_unsigned<double> { typedef uint64_t type; };

    template <class T> struct make_scalar;
    template <> struct LIBUPCOREAPI make_scalar<int8_t> { typedef int8_t type; };
    template <> struct LIBUPCOREAPI make_scalar<int16_t> { typedef int16_t type; };
    template <> struct LIBUPCOREAPI make_scalar<int32_t> { typedef int32_t type; };
    template <> struct LIBUPCOREAPI make_scalar<int64_t> { typedef int64_t type; };
    template <> struct LIBUPCOREAPI make_scalar<uint8_t> { typedef uint8_t type; };
    template <> struct LIBUPCOREAPI make_scalar<uint16_t> { typedef uint16_t type; };
    template <> struct LIBUPCOREAPI make_scalar<uint32_t> { typedef uint32_t type; };
    template <> struct LIBUPCOREAPI make_scalar<uint64_t> { typedef uint64_t type; };
    template <> struct LIBUPCOREAPI make_scalar<float> { typedef float type; };
    template <> struct LIBUPCOREAPI make_scalar<double> { typedef double type; };

    template <class T, size_t N> struct make_vector;
    template <> struct LIBUPCOREAPI make_vector<int8_t, 1> { typedef int8_t type; };
    template <> struct LIBUPCOREAPI make_vector<int16_t, 1> { typedef int16_t type; };
    template <> struct LIBUPCOREAPI make_vector<int32_t, 1> { typedef int32_t type; };
    template <> struct LIBUPCOREAPI make_vector<int64_t, 1> { typedef int64_t type; };
    template <> struct LIBUPCOREAPI make_vector<uint8_t, 1> { typedef uint8_t type; };
    template <> struct LIBUPCOREAPI make_vector<uint16_t, 1> { typedef uint16_t type; };
    template <> struct LIBUPCOREAPI make_vector<uint32_t, 1> { typedef uint32_t type; };
    template <> struct LIBUPCOREAPI make_vector<uint64_t, 1> { typedef uint64_t type; };
    template <> struct LIBUPCOREAPI make_vector<float, 1> { typedef float type; };
    template <> struct LIBUPCOREAPI make_vector<double, 1> { typedef double type; };

    template <class T, size_t C, size_t R> struct make_matrix;
    template <> struct LIBUPCOREAPI make_matrix<int8_t, 1, 1> { typedef int8_t type; };
    template <> struct LIBUPCOREAPI make_matrix<int16_t, 1, 1> { typedef int16_t type; };
    template <> struct LIBUPCOREAPI make_matrix<int32_t, 1, 1> { typedef int32_t type; };
    template <> struct LIBUPCOREAPI make_matrix<int64_t, 1, 1> { typedef int64_t type; };
    template <> struct LIBUPCOREAPI make_matrix<uint8_t, 1, 1> { typedef uint8_t type; };
    template <> struct LIBUPCOREAPI make_matrix<uint16_t, 1, 1> { typedef uint16_t type; };
    template <> struct LIBUPCOREAPI make_matrix<uint32_t, 1, 1> { typedef uint32_t type; };
    template <> struct LIBUPCOREAPI make_matrix<uint64_t, 1, 1> { typedef uint64_t type; };
    template <> struct LIBUPCOREAPI make_matrix<float, 1, 1> { typedef float type; };
    template <> struct LIBUPCOREAPI make_matrix<double, 1, 1> { typedef double type; };

    template <class T> struct UPVISIBLE vector_step : std::integral_constant<size_t, 1> { };
    template <class T> struct UPVISIBLE matrix_column_step : std::integral_constant<size_t, 1> { };
    template <class T> struct UPVISIBLE matrix_row_step : std::integral_constant<size_t, 1> { };
    template <class T> struct UPVISIBLE is_vector : std::false_type { };
    template <class T> struct UPVISIBLE is_matrix : std::false_type { };

    template <class T>
    struct uniform;

    template <class T, class Elem>
    union uniform_constant
    {
        Elem s[matrix_column_step<T>::value * matrix_row_step<T>::value];
        T v;
        UPALWAYSINLINE operator T() const noexcept { return v; }
    };

    template <class Tx, class Elem> struct UPVISIBLE vector_step<uniform_constant<Tx, Elem>> : vector_step<Tx> { };
    template <class Tx, class Elem> struct UPVISIBLE matrix_column_step<uniform_constant<Tx, Elem>> : matrix_column_step<Tx> { };
    template <class Tx, class Elem> struct UPVISIBLE matrix_row_step<uniform_constant<Tx, Elem>> : matrix_row_step<Tx> { };
    template <class Tx, class Elem> struct UPVISIBLE is_vector<uniform_constant<Tx, Elem>> : is_vector<Tx> { };
    template <class Tx, class Elem> struct UPVISIBLE is_matrix<uniform_constant<Tx, Elem>> : is_matrix<Tx> { };
}}

namespace up { namespace math { namespace detail
{
    template <class T>
    struct constructors;

    template
    <
        class To,
        class From,
        bool = sizeof(To) == sizeof(From),
        bool = matrix_column_step<To>::value == matrix_column_step<From>::value,
        bool = matrix_row_step<To>::value == matrix_row_step<From>::value
    >
    struct is_convertible_as_impl : std::false_type { };

    template <class To, class From> struct is_convertible_as_impl<To, From, true, true, true> : std::true_type { };
    template <class Tx, class Ux, class E> struct is_convertible_as_impl<Tx, uniform_constant<Ux, E>, true, true, true> : is_convertible_as_impl<Tx, Ux> { };

    template
    <
        class To,
        class From,
        bool = matrix_column_step<To>::value == matrix_column_step<From>::value,
        bool = matrix_row_step<To>::value == matrix_row_step<From>::value
    >
    struct is_convertible_to_impl : std::false_type { };

    template <class To, class From> struct is_convertible_to_impl<To, From, true, true> : std::true_type { };
    template <class Tx, class Ux, class E> struct is_convertible_to_impl<Tx, uniform_constant<Ux, E>, true, true> : is_convertible_as_impl<Tx, Ux> { };

    template
    <
        class To,
        class From,
        bool = std::is_integral<typename make_scalar<To>::type>::value,
        bool = std::is_floating_point<typename make_scalar<From>::type>::value,
        bool = matrix_column_step<To>::value == matrix_column_step<From>::value,
        bool = matrix_row_step<To>::value == matrix_row_step<From>::value
    >
    struct is_roundable_to_impl : std::false_type { };

    template <class To, class From> struct is_roundable_to_impl<To, From, true, true, true, true> : std::true_type { };
    template <class Tx, class Ux, class E> struct is_roundable_to_impl<Tx, uniform_constant<Ux, E>, true, true, true, true> : is_roundable_to_impl<Tx, Ux> { };

    template <class To, class From>
    struct as_impl
    {
        static UPALWAYSINLINE UPPURE
        To convert(From const& v) noexcept {
            union { From a; To b; } r = { v };
            return r.b;
        }
    };

    template <class T>
    struct as_impl<T, T>
    {
        static UPALWAYSINLINE UPPURE
        T convert(T const& v) noexcept {
            return v;
        }
    };

    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_rte_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_rtz_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_rtn_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_rtp_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_sat_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_sat_rte_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_sat_rtz_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_sat_rtn_impl;
    template <class To, class From, size_t = matrix_column_step<To>::value, size_t = matrix_row_step<To>::value> struct generic_convert_sat_rtp_impl;
    template <class T, class U> struct convert_impl : generic_convert_impl<T, U> { };
    template <class T, class U> struct convert_rte_impl : generic_convert_rte_impl<T, U> { };
    template <class T, class U> struct convert_rtz_impl : generic_convert_rtz_impl<T, U> { };
    template <class T, class U> struct convert_rtn_impl : generic_convert_rtn_impl<T, U> { };
    template <class T, class U> struct convert_rtp_impl : generic_convert_rtp_impl<T, U> { };
    template <class T, class U> struct convert_sat_impl : generic_convert_sat_impl<T, U> { };
    template <class T, class U> struct convert_sat_rte_impl : generic_convert_sat_rte_impl<T, U> { };
    template <class T, class U> struct convert_sat_rtz_impl : generic_convert_sat_rtz_impl<T, U> { };
    template <class T, class U> struct convert_sat_rtn_impl : generic_convert_sat_rtn_impl<T, U> { };
    template <class T, class U> struct convert_sat_rtp_impl : generic_convert_sat_rtp_impl<T, U> { };
}}}

namespace up { namespace math
{
    template <class T, class U> struct UPVISIBLE is_convertible_as : detail::is_convertible_as_impl<typename std::decay<T>::type, typename std::decay<U>::type> { };
    template <class T, class U> struct UPVISIBLE is_convertible_to : detail::is_convertible_to_impl<typename std::decay<T>::type, typename std::decay<U>::type> { };
    template <class T, class U> struct UPVISIBLE is_roundable_to : detail::is_roundable_to_impl<typename std::decay<T>::type, typename std::decay<U>::type> { };

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_convertible_as<To, From>::value, To>::type as(From&& v) noexcept {
        return detail::as_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_convertible_to<To, From>::value, To>::type convert(From&& v) noexcept {
        return detail::convert_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_rte(From&& v) noexcept {
        return detail::convert_rte_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_rtz(From&& v) noexcept {
        return detail::convert_rtz_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_rtn(From&& v) noexcept {
        return detail::convert_rtn_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_rtp(From&& v) noexcept {
        return detail::convert_rtp_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE
    typename std::enable_if<is_convertible_to<To, From>::value, To>::type convert_sat(From&& v) noexcept {
        return detail::convert_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE 
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_sat_rte(From&& v) noexcept {
        return detail::convert_sat_rte_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE 
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_sat_rtz(From&& v) noexcept {
        return detail::convert_sat_rtz_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE 
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_sat_rtn(From&& v) noexcept {
        return detail::convert_sat_rtn_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }

    template <class To, class From>
    inline UPALWAYSINLINE UPPURE 
    typename std::enable_if<is_roundable_to<To, From>::value, To>::type convert_sat_rtp(From&& v) noexcept {
        return detail::convert_sat_rtp_impl<typename std::decay<To>::type, typename std::decay<From>::type>::convert(::up::forward<From>(v));
    }
}}

#include <up/math/generic/scalar_operations.inl>

#endif
