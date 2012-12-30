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

#ifndef UP_MATH_SSE_FLOAT4X4_OPERATIONS_INL
#define UP_MATH_SSE_FLOAT4X4_OPERATIONS_INL

#ifndef UP_MATH_FLOAT4X4_HPP
#   error "Do not include this file directly! Instead include <up/math/float4x4.hpp>"
#endif

namespace up { namespace math
{
    inline UPALWAYSINLINE float4x4 concat(float4x4 const& UPRESTRICT m1, float4x4 const& UPRESTRICT m2) noexcept {
        float4x4 result;
#if defined(UP_SIMD_SSE_4_1)
        float4 w = m2.v[0];
        float4 x = _mm_dp_ps(w, m1.v[0], 0xF1);
        float4 y = _mm_dp_ps(w, m1.v[1], 0xF2);
        float4 z = _mm_dp_ps(w, m1.v[2], 0xF4);
        w = _mm_dp_ps(w, m1.v[3], 0xF8);
        x = _mm_blend_ps(x, y, 0x0E);
        z = _mm_blend_ps(z, w, 0x0B);
        x = _mm_blend_ps(x, z, 0x0C);
        result.v[0] = y;

        w = m2.v[1];
        x = _mm_dp_ps(w, m1.v[0], 0xF1);
        y = _mm_dp_ps(w, m1.v[1], 0xF2);
        z = _mm_dp_ps(w, m1.v[2], 0xF4);
        w = _mm_dp_ps(w, m1.v[3], 0xF8);
        x = _mm_blend_ps(x, y, 0x0E);
        z = _mm_blend_ps(z, w, 0x0B);
        x = _mm_blend_ps(x, z, 0x0C);
        result.v[1] = x;

        w = m2.v[2];
        x = _mm_dp_ps(w, m1.v[0], 0xF1);
        y = _mm_dp_ps(w, m1.v[1], 0xF2);
        z = _mm_dp_ps(w, m1.v[2], 0xF4);
        w = _mm_dp_ps(w, m1.v[3], 0xF8);
        x = _mm_blend_ps(x, y, 0x0E);
        z = _mm_blend_ps(z, w, 0x0B);
        x = _mm_blend_ps(x, z, 0x0C);
        result.v[2] = x;

        w = m2.v[3];
        x = _mm_dp_ps(w, m1.v[0], 0xF1);
        y = _mm_dp_ps(w, m1.v[1], 0xF2);
        z = _mm_dp_ps(w, m1.v[2], 0xF4);
        w = _mm_dp_ps(w, m1.v[3], 0xF8);
        x = _mm_blend_ps(x, y, 0x0E);
        z = _mm_blend_ps(z, w, 0x0B);
        x = _mm_blend_ps(x, z, 0x0C);
        result.v[3] = x;
#else
        float4 w = m1.v[0];
        float4 x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
        float4 y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        float4 z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
        x = _mm_mul_ps(x, m2.v[0]);
        y = _mm_mul_ps(y, m2.v[1]);
        z = _mm_mul_ps(z, m2.v[2]);
        w = _mm_mul_ps(w, m2.v[3]);
        x = _mm_add_ps(x, z);
        y = _mm_add_ps(y, w);
        x = _mm_add_ps(x, y);
        result.v[0] = x;

        w = m1.v[1];
        x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
        y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
        x = _mm_mul_ps(x, m2.v[0]);
        y = _mm_mul_ps(y, m2.v[1]);
        z = _mm_mul_ps(z, m2.v[2]);
        w = _mm_mul_ps(w, m2.v[3]);
        x = _mm_add_ps(x, z);
        y = _mm_add_ps(y, w);
        x = _mm_add_ps(x, y);
        result.v[1] = x;

        w = m1.v[2];
        x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
        y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
        x = _mm_mul_ps(x, m2.v[0]);
        y = _mm_mul_ps(y, m2.v[1]);
        z = _mm_mul_ps(z, m2.v[2]);
        w = _mm_mul_ps(w, m2.v[3]);
        x = _mm_add_ps(x, z);
        y = _mm_add_ps(y, w);
        x = _mm_add_ps(x, y);
        result.v[2] = x;

        w = m1.v[3];
        x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
        y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
        x = _mm_mul_ps(x, m2.v[0]);
        y = _mm_mul_ps(y, m2.v[1]);
        z = _mm_mul_ps(z, m2.v[2]);
        w = _mm_mul_ps(w, m2.v[3]);
        x = _mm_add_ps(x, z);
        y = _mm_add_ps(y, w);
        x = _mm_add_ps(x, y);
        result.v[3] = x;
#endif
        return result;
    }
}

#endif
