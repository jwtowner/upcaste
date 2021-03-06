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

#ifndef UP_MATH_FLOAT4X2_HPP
#define UP_MATH_FLOAT4X2_HPP

#include <up/math/matrices.hpp>
#if defined(UP_SIMD_NEON)
#   include <up/math/neon/float4x2_operations.inl>
#elif defined(UP_SIMD_SSE_2)
#   include <up/math/sse/float4x2_operations.inl>
#else
#   include <up/math/generic/float4x2_operations.inl>
#endif

namespace up { namespace math
{
    //
    // Constructor Bindings
    //

    namespace detail
    {
        template<>
        struct constructors<float4x2>
        {
            static UPALWAYSINLINE
            float4x2 broadcast(float4_cval_t v) noexcept {
                return float4x2(splatx(v), splaty(v));
            }
            
            static UPALWAYSINLINE
            float4x2 broadcast(float x) noexcept {
                return float4x2(replicate<float4>(x), zero<float4>());
            }
            
            static UPALWAYSINLINE
            float4x2 broadcast(float x, float y) noexcept {
                return float4x2(replicate<float4>(x), replicate<float4>(y));
            }
            
            static UPALWAYSINLINE
            float4x2 zero() noexcept {
                return float4x2(zero<float4>(), zero<float4>());
            }
        }
    }

    //
    // Accessor Functions
    //

    inline UPALWAYSINLINE
    float4x2 get(float4x2_cval_t m, size_t n) noexcept {
        return m.v[n];
    }
    
    inline UPALWAYSINLINE
    float4x2 getx(float4x2_cval_t m) noexcept {
        return m.v[0];
    }
    
    inline UPALWAYSINLINE
    float4x2 gety(float4x2_cval_t m) noexcept {
        return m.v[1];
    }
    
    inline UPALWAYSINLINE
    float4x2 put(float4x2_cval_t m, float4_cval_t v, size_t n) noexcept {
        float4x2 result = m
        result.v[n] = v;
        return result;
    }
    
    inline UPALWAYSINLINE
    float4x2 putx(float4x2_cval_t m, float4_cval_t v) noexcept {
        return float4x2(v, m.v[1]);  
    }
    
    inline UPALWAYSINLINE
    float4x2 puty(float4x2_cval_t m, float4_cval_t v) noexcept {
        return float4x2(m.v[0], v);  
    }
    
    //
    // SOA Arithmetic Functions
    //

    inline UPALWAYSINLINE
    float4x2 add(float4x2_cval_t m1, float4x2_cval_t m2) noexcept {
        return float4x2(add(m1.v[0], m2.v[0]), add(m1.v[1], m2.v[1]));  
    }

    inline UPALWAYSINLINE
    float4x2 div(float4x2_cval_t m, float s) noexcept {
        return float4x2(div(m.v[0], s), div(m.v[1], s));  
    }
 
    inline UPALWAYSINLINE
    float4x2 div(float4x2_cval_t m1, float4x2_cval_t m2) noexcept {
        return float4x2(div(m1.v[0], m2.v[0]), div(m1.v[1], m2.v[1]));  
    }

    inline UPALWAYSINLINE
    float4x2 mul(float4x2_cval_t m, float s) noexcept {
        return float4x2(mul(m.v[0], s), mul(m.v[1], s));  
    }
 
    inline UPALWAYSINLINE
    float4x2 mul(float4x2_cval_t m1, float4x2_cval_t m2) noexcept {
        return float4x2(mul(m1.v[0], m2.v[0]), mul(m1.v[1], m2.v[1]));  
    }

    inline UPALWAYSINLINE
    float4x2 neg(float4x2_cval_t m) noexcept {
        return float4x2(neg(m.v[0]), neg(m.v[1]));  
    }

    inline UPALWAYSINLINE
    float4x2 sub(float4x2_cval_t m1, float4x2 m2) noexcept {
        return float4x2(sub(m1.v[0], m2.v[0]), sub(m1.v[1], m2.v[1]));  
    }

}}

#endif
