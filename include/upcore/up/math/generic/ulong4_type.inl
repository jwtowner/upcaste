﻿//
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

//
//  Do not edit! This file is automatically generated.
//

#ifndef UP_MATH_GENERIC_ULONG4_TYPE_INL
#define UP_MATH_GENERIC_ULONG4_TYPE_INL

#ifndef UP_MATH_VECTORS_HPP
#   error "Do not include this file directly! Instead include <up/math/vectors.hpp>"
#endif

namespace up { namespace math
{
#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(push)
#   pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#endif

    union LIBUPCOREAPI ulong4
    {
        uint64_t s[4];
        struct
        {
            uint64_t s0;
            uint64_t s1;
            uint64_t s2;
            uint64_t s3;
        };
        struct
        {
            uint64_t x;
            uint64_t y;
            uint64_t z;
            uint64_t w;
        };
    };

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(pop)
#endif

    typedef ulong4 const& ulong4_cval_t;
    typedef ulong4_cval_t ulong4_cval1_t;
    typedef ulong4_cval_t ulong4_cval2_t;
    typedef ulong4_cval_t ulong4_cval3_t;
    typedef ulong4_cval_t ulong4_cval4_t;
}}

#endif
