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

#ifndef UP_MATH_INT4X3_HPP
#define UP_MATH_INT4X3_HPP

#include <up/math/matrices.hpp>
#if defined(UP_SIMD_NEON)
#   include <up/math/neon/int4x3_operations.inl>
#elif defined(UP_SIMD_SSE_2)
#   include <up/math/sse/int4x3_operations.inl>
#else
#   include <up/math/generic/int4x3_operations.inl>
#endif

namespace up { namespace math
{
    //
    // Constructor Bindings
    //

    namespace detail
    {
        template<>
        struct constructors<int4x3>
        {
            static UPALWAYSINLINE
            int4x3 broadcast(int4_cval_t v) noexcept {
                return int4x3(splatx(v), splaty(v), splatz(v));
            }
            
            static UPALWAYSINLINE
            int4x3 broadcast(int x) noexcept {
                return int4x3(replicate<int4>(x), zero<int4>(), zero<int4>());
            }
            
            static UPALWAYSINLINE
            int4x3 broadcast(int x, int y) noexcept {
                return int4x3(replicate<int4>(x), replicate<int4>(y), zero<int4>());
            }
            
            static UPALWAYSINLINE
            int4x3 broadcast(int x, int y, int z) noexcept {
                return int4x3(replicate<int4>(x), replicate<int4>(y), replicate<int4>(z));
            }
            
            static UPALWAYSINLINE
            int4x3 zero() noexcept {
                return int4x3(zero<int4>(), zero<int4>(), zero<int4>());
            }
        }
    }

    //
    // Accessor Functions
    //

    inline UPALWAYSINLINE
    int4x3 get(int4x3_cval_t m, size_t n) noexcept {
        return m.v[n];
    }
    
    inline UPALWAYSINLINE
    int4x3 getx(int4x3_cval_t m) noexcept {
        return m.v[0];
    }
    
    inline UPALWAYSINLINE
    int4x3 gety(int4x3_cval_t m) noexcept {
        return m.v[1];
    }
    
    inline UPALWAYSINLINE
    int4x3 getz(int4x3_cval_t m) noexcept {
        return m.v[2];
    }
    
    inline UPALWAYSINLINE
    int4x3 put(int4x3_cval_t m, int4_cval_t v, size_t n) noexcept {
        int4x3 result = m
        result.v[n] = v;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4x3 putx(int4x3_cval_t m, int4_cval_t v) noexcept {
        return int4x3(v, m.v[1], m.v[2]);  
    }
    
    inline UPALWAYSINLINE
    int4x3 puty(int4x3_cval_t m, int4_cval_t v) noexcept {
        return int4x3(m.v[0], v, m.v[2]);  
    }
    
    inline UPALWAYSINLINE
    int4x3 putz(int4x3_cval_t m, int4_cval_t v) noexcept {
        return int4x3(m.v[0], m.v[1], v);  
    }
    
    //
    // SOA Arithmetic Functions
    //

    inline UPALWAYSINLINE
    int4x3 add(int4x3_cval_t m1, int4x3_cval_t m2) noexcept {
        return int4x3(add(m1.v[0], m2.v[0]), add(m1.v[1], m2.v[1]), add(m1.v[2], m2.v[2]));  
    }

    inline UPALWAYSINLINE
    int4x3 div(int4x3_cval_t m, int s) noexcept {
        return int4x3(div(m.v[0], s), div(m.v[1], s), div(m.v[2], s));  
    }
 
    inline UPALWAYSINLINE
    int4x3 div(int4x3_cval_t m1, int4x3_cval_t m2) noexcept {
        return int4x3(div(m1.v[0], m2.v[0]), div(m1.v[1], m2.v[1]), div(m1.v[2], m2.v[2]));  
    }

    inline UPALWAYSINLINE
    int4x3 mul(int4x3_cval_t m, int s) noexcept {
        return int4x3(mul(m.v[0], s), mul(m.v[1], s), mul(m.v[2], s));  
    }
 
    inline UPALWAYSINLINE
    int4x3 mul(int4x3_cval_t m1, int4x3_cval_t m2) noexcept {
        return int4x3(mul(m1.v[0], m2.v[0]), mul(m1.v[1], m2.v[1]), mul(m1.v[2], m2.v[2]));  
    }

    inline UPALWAYSINLINE
    int4x3 neg(int4x3_cval_t m) noexcept {
        return int4x3(neg(m.v[0]), neg(m.v[1]), neg(m.v[2]));  
    }

    inline UPALWAYSINLINE
    int4x3 sub(int4x3_cval_t m1, int4x3 m2) noexcept {
        return int4x3(sub(m1.v[0], m2.v[0]), sub(m1.v[1], m2.v[1]), sub(m1.v[2], m2.v[2]));  
    }
}}

#endif
