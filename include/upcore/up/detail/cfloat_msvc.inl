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

#ifndef UP_DETAIL_CFLOAT_MSVC_INL
#define UP_DETAIL_CFLOAT_MSVC_INL

#ifndef UP_CFLOAT_HPP
#   error "Do not include this header directly. Instead include <up/cfloat.hpp>"
#endif

#include <up/ieee754.hpp>

namespace up { namespace detail
{
    extern LIBUPCOREAPI ieee754_binary32 const flt_inf_value;
    extern LIBUPCOREAPI ieee754_binary32 const flt_nan_value;
    extern LIBUPCOREAPI ieee754_binary64 const dbl_inf_value;
    extern LIBUPCOREAPI ieee754_binary64 const dbl_nan_value;
}}

#define FLT_INFINITY    ((float)::up::detail::flt_inf_value.f)
#define FLT_NAN         ((float)::up::detail::flt_nan_value.f)
#define DBL_INFINITY    ((double)::up::detail::dbl_inf_value.d)
#define DBL_NAN         ((double)::up::detail::dbl_nan_value.d)
#define LDBL_INFINITY   ((long double)::up::detail::dbl_inf_value.ld)
#define LDBL_NAN        ((long double)::up::detail::dbl_nan_value.ld)

#ifndef DECIMAL_DIG
#   define DECIMAL_DIG 17
#endif

#ifndef FLT_DECIMAL_DIG
#   define FLT_DECIMAL_DIG 9
#endif
#ifndef DBL_DECIMAL_DIG
#   define DBL_DECIMAL_DIG 17
#endif
#ifndef LDBL_DECIMAL_DIG
#   define LDBL_DECIMAL_DIG 17
#endif

#ifndef FLT_HAS_SUBNORM
#   define FLT_HAS_SUBNORM 1
#endif
#ifndef DBL_HAS_SUBNORM
#   define DBL_HAS_SUBNORM 1
#endif
#ifndef LDBL_HAS_SUBNORM
#   define LDBL_HAS_SUBNORM 1
#endif

#ifndef FLT_TRUE_MIN
#   define FLT_TRUE_MIN 1.40129846E-45f
#endif
#ifndef DBL_TRUE_MIN
#   define DBL_TRUE_MIN 4.9406564584124654E-324
#endif
#ifndef LDBL_TRUE_MIN
#   define LDBL_TRUE_MIN 4.9406564584124654E-324l
#endif

#endif
