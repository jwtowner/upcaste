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

#ifndef UP_MATH_SSE_VECTORS_INL
#define UP_MATH_SSE_VECTORS_INL

#ifndef UP_MATH_VECTORS_HPP
#   error "Do not include this file directly! Instead include <up/math/vectors.hpp>"
#endif

#include <up/math/generic/int2_type.inl>
#include <up/math/generic/int3_type.inl>
#include <up/math/generic/uint2_type.inl>
#include <up/math/generic/uint3_type.inl>
#include <up/math/generic/float2_type.inl>
#include <up/math/generic/float3_type.inl>
#include <up/math/generic/long2_type.inl>
#include <up/math/generic/long3_type.inl>
#include <up/math/generic/ulong2_type.inl>
#include <up/math/generic/ulong3_type.inl>
#include <up/math/generic/double2_type.inl>
#include <up/math/generic/double3_type.inl>
#include <immintrin.h>

namespace up { namespace math
{
    //
    // Vector Types
    //

    typedef __m128 float4; 
    typedef __m128i int4;
    struct LIBUPCOREAPI uint4 { __m128i m; };

#ifdef UP_SIMD_AVX
    typedef __m256d double4;
    struct LIBUPCOREAPI long4 { __m256i m; };
    struct LIBUPCOREAPI ulong4 { __m256i m; };
#else
    struct LIBUPCOREAPI double4 { __m128d a; __m128d b; };
    struct LIBUPCOREAPI long4 { __m128i a; __m128i b; };
    struct LIBUPCOREAPI ulong4 { __m128i a; __m128i b; };
#endif

    //
    // Vector Argument Types
    //

    typedef float4 const float4_cval_t;
    typedef float4_cval_t float4_cval1_t;
    typedef float4_cval_t float4_cval2_t;
    typedef float4_cval_t float4_cval3_t;
#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
    typedef float4 const& float4_cval4_t;
#else
    typedef float4_cval_t float4_cval4_t;
#endif

    typedef int4 const int4_cval_t;
    typedef int4_cval_t int4_cval1_t;
    typedef int4_cval_t int4_cval2_t;
    typedef int4_cval_t int4_cval3_t;
#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
    typedef int4 const& int4_cval4_t;
#else
    typedef int4_cval_t int4_cval4_t;
#endif

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
    typedef uint4 const& uint4_cval_t;
#else
    typedef uint4 const uint4_cval_t;
#endif
    typedef uint4_cval_t uint4_cval1_t;
    typedef uint4_cval_t uint4_cval2_t;
    typedef uint4_cval_t uint4_cval3_t;
    typedef uint4_cval_t uint4_cval4_t;

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
    typedef long4 const& long4_cval_t;
#else
    typedef long4 const long4_cval_t;
#endif
    typedef long4_cval_t long4_cval1_t;
    typedef long4_cval_t long4_cval2_t;
    typedef long4_cval_t long4_cval3_t;
    typedef long4_cval_t long4_cval4_t;

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
    typedef ulong4 const& ulong4_cval_t;
#else
    typedef ulong4 const ulong4_cval_t;
#endif
    typedef ulong4_cval_t ulong4_cval1_t;
    typedef ulong4_cval_t ulong4_cval2_t;
    typedef ulong4_cval_t ulong4_cval3_t;
    typedef ulong4_cval_t ulong4_cval4_t;

#if !defined(UP_SIMD_AVX) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
    typedef double4 const& double4_cval_t;
#else
    typedef double4 const double4_cval_t;
#endif
    typedef double4_cval_t double4_cval1_t;
    typedef double4_cval_t double4_cval2_t;
    typedef double4_cval_t double4_cval3_t;
    typedef double4_cval_t double4_cval4_t;
}}

#endif
