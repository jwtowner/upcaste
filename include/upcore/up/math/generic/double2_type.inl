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

#ifndef UP_MATH_GENERIC_DOUBLE2_TYPE_INL
#define UP_MATH_GENERIC_DOUBLE2_TYPE_INL

#ifndef UP_MATH_VECTORS_HPP
#   error "Do not include this file directly! Instead include <up/math/vectors.hpp>"
#endif

namespace up { namespace math
{
#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(push)
#   pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#endif

    union LIBUPCOREAPI double2
    {
        double s[2];
        struct
        {
            double s0;
            double s1;
        };
        struct
        {
            double x;
            double y;
        };
    };

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(pop)
#endif

    typedef double2 const& double2_cval_t;
    typedef double2_cval_t double2_cval1_t;
    typedef double2_cval_t double2_cval2_t;
    typedef double2_cval_t double2_cval3_t;
    typedef double2_cval_t double2_cval4_t;
}}

#endif
