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

#ifndef UP_MATH_SSE_FLOAT4_OPERATIONS_INL
#define UP_MATH_SSE_FLOAT4_OPERATIONS_INL

#ifndef UP_MATH_FLOAT4_HPP
#   error "Do not include this file directly! Instead include <up/math/float4.hpp>"
#endif

namespace up { namespace math { namespace detail
{
    template <>
    struct constructors<float4>
    {
        typedef float element_type;
        typedef float const* element_pointer;
        typedef float2 const vector2_type;
        typedef float3 const vector3_type;
        typedef float4 const vector4_type;
        typedef float2 const* vector2_pointer;
        typedef float3 const* vector3_pointer;
        typedef float4 const* vector4_pointer;

        //
        // Initialization Functions
        //

        static UPALWAYSINLINE float4 broadcast(float x) noexcept {
            return _mm_set_ps1(x);
        }

        static UPALWAYSINLINE float4 broadcast2(float2 const v) noexcept {
            return _mm_set_ps(v.y, v.x, v.y, v.x);
        }

        static UPALWAYSINLINE float4 make(float x) noexcept {
            return _mm_set_ss(x);
        }

        static UPALWAYSINLINE float4 make(float x, float y) noexcept {
            return _mm_set_ps(0.0f, 0.0f, y, x);
        }

        static UPALWAYSINLINE float4 make(float x, float y, float z) noexcept {
            return _mm_set_ps(0.0f, z, y, x);
        }

        static UPALWAYSINLINE float4 make(float x, float y, float z, float w) noexcept {
            return _mm_set_ps(w, z, y, x);
        }

        static UPALWAYSINLINE float4 zero() noexcept {
            return _mm_setzero_ps();
        }
            
        //
        // Load Functions
        //

        static UPALWAYSINLINE float4 loadbroadcast(float const* p) noexcept {
#if defined(UP_SIMD_AVX)
            return _mm_broadcast_ss(p);
#else
            return _mm_load_ps1(p);
#endif
        }

        static UPALWAYSINLINE float4 loadbroadcast(size_t offset, float const* p) noexcept {
#if defined(UP_SIMD_AVX)
            return _mm_broadcast_ss(p + offset);
#else
            return _mm_load_ps1(p + offset);
#endif
        }

        static UPALWAYSINLINE float4 loadbroadcast2(float2 const* p) noexcept {
            float4 x = _mm_load_ss(&p->x);
            float4 y = _mm_load_ss(&p->y);
            float4 xy = _mm_unpacklo_ps(x, y);
            return _mm_movelh_ps(xy, xy);
        }

        static UPALWAYSINLINE float4 loadbroadcast2(size_t offset, float2 const* p) noexcept {
            float4 x = _mm_load_ss(&(p + offset)->x);
            float4 y = _mm_load_ss(&(p + offset)->y);
            float4 xy = _mm_unpacklo_ps(x, y);
            return _mm_movelh_ps(xy, xy);
        }

        static UPALWAYSINLINE float4 loadscalar(float const* p) noexcept {
            return _mm_load_ss(p);
        }

        static UPALWAYSINLINE float4 loadscalar(size_t offset, float const* p) noexcept {
            return _mm_load_ss(p + offset);
        }

        static UPALWAYSINLINE float4 load2(float2 const* p) noexcept {
            float4 x = _mm_load_ss(&p->x);
            float4 y = _mm_load_ss(&p->y);
            return _mm_unpacklo_ps(x, y);
        }

        static UPALWAYSINLINE float4 load2(size_t offset, float2 const* p) noexcept {
            float4 x = _mm_load_ss(&(p + offset)->x);
            float4 y = _mm_load_ss(&(p + offset)->y);
            return _mm_unpacklo_ps(x, y);
        }

        static UPALWAYSINLINE float4 load3(float3 const* p) noexcept {
            float4 x = _mm_load_ss(&p->x);
            float4 y = _mm_load_ss(&p->y);
            float4 z = _mm_load_ss(&p->z);
            float4 xy = _mm_unpacklo_ps(x, y);
            return _mm_movelh_ps(xy, z);
        }

        static UPALWAYSINLINE float4 load3(size_t offset, float3 const* p) noexcept {
            float4 x = _mm_load_ss(&(p + offset)->x);
            float4 y = _mm_load_ss(&(p + offset)->y);
            float4 z = _mm_load_ss(&(p + offset)->z);
            float4 xy = _mm_unpacklo_ps(x, y);
            return _mm_movelh_ps(xy, z);
        }

        static UPALWAYSINLINE float4 load4(float4 const* p) noexcept {
            return _mm_loadu_ps(reinterpret_cast<float const*>(p));
        }

        static UPALWAYSINLINE float4 load4(size_t offset, float4 const* p) noexcept {
            return _mm_loadu_ps(reinterpret_cast<float const*>(p + offset));
        }

        static UPALWAYSINLINE float4 loada2(float2 const* p) noexcept {
            return _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<int4 const*>(p)));
        }

        static UPALWAYSINLINE float4 loada2(size_t offset, float2 const* p) noexcept {
            return _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<int4 const*>(p + offset)));
        }

        static UPALWAYSINLINE float4 loada3(float3 const* p) noexcept {
            return _mm_load_ps(reinterpret_cast<float const*>(p)); // ignore extra element loaded
        }

        static UPALWAYSINLINE float4 loada3(size_t offset, float3 const* p) noexcept {
            return _mm_load_ps(reinterpret_cast<float const*>(p + offset)); // ignore extra element loaded
        }

        static UPALWAYSINLINE float4 loada4(float4 const* p) noexcept {
            return _mm_load_ps(reinterpret_cast<float const*>(p));
        }

        static UPALWAYSINLINE float4 loada4(size_t offset, float4 const* p) noexcept {
            return _mm_load_ps(reinterpret_cast<float const*>(p + offset));
        }
    };
}}}

namespace up { namespace math
{
    //
    // Load Functions
    //

    inline UPALWAYSINLINE float4 load4(float4 const* p) noexcept {
        return _mm_loadu_ps(reinterpret_cast<float const*>(p));
    }

    inline UPALWAYSINLINE float4 load4(size_t offset, float4 const* p) noexcept {
        return _mm_loadu_ps(reinterpret_cast<float const*>(p + offset));
    }

    inline UPALWAYSINLINE float4 loada4(float4 const* p) noexcept {
        return _mm_load_ps(reinterpret_cast<float const*>(p));
    }

    inline UPALWAYSINLINE float4 loada4(size_t offset, float4 const* p) noexcept {
        return _mm_load_ps(reinterpret_cast<float const*>(p + offset));
    }

    //
    // Store Functions
    //

    inline UPALWAYSINLINE void storescalar(float4 const v, float* p) noexcept {
        _mm_store_ss(p, v);
    }

    inline UPALWAYSINLINE void storescalar(float4 const v, size_t offset, float* p) noexcept {
        _mm_store_ss(p + offset, v);
    }

    inline UPALWAYSINLINE void store2(float4 const v, float2* p) noexcept {
        float4 u = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
        _mm_store_ss(&p->x, v);
        _mm_store_ss(&p->y, u);
    }

    inline UPALWAYSINLINE void store2(float4 const v, size_t offset, float2* p) noexcept {
        float4 u = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
        _mm_store_ss(&(p + offset)->x, v);
        _mm_store_ss(&(p + offset)->y, u); 
    }

    inline UPALWAYSINLINE void store3(float4 const v, float3* p) noexcept {
        float4 u = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
        float4 t = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(&p->x, v);
        _mm_store_ss(&p->y, u);
        _mm_store_ss(&p->z, t);
    }

    inline UPALWAYSINLINE void store3(float4 const v, size_t offset, float3* p) noexcept {
        float4 u = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
        float4 t = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(&(p + offset)->x, v);
        _mm_store_ss(&(p + offset)->y, u);
        _mm_store_ss(&(p + offset)->z, t);
    }

    inline UPALWAYSINLINE void store4(float4 const v, float4* p) noexcept {
        _mm_storeu_ps(reinterpret_cast<float*>(p), v);
    }

    inline UPALWAYSINLINE void store4(float4 const v, size_t offset, float4* p) noexcept {
        _mm_storeu_ps(reinterpret_cast<float*>(p + offset), v);
    }

    inline UPALWAYSINLINE void storea2(float4 const v, float2* p) noexcept {
        _mm_storel_epi64(reinterpret_cast<int4*>(p), _mm_castps_si128(v));
    }

    inline UPALWAYSINLINE void storea2(float4 const v, size_t offset, float2* p) noexcept {
        _mm_storel_epi64(reinterpret_cast<int4*>(p + offset), _mm_castps_si128(v));
    }

    inline UPALWAYSINLINE void storea3(float4 const v, float3* p) noexcept {
        float4 t = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_storel_epi64(reinterpret_cast<int4*>(p), _mm_castps_si128(v));
        _mm_store_ss(&p->z, t);
    }

    inline UPALWAYSINLINE void storea3(float4 const v, size_t offset, float3* p) noexcept {
        float4 t = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_storel_epi64(reinterpret_cast<int4*>(p + offset), _mm_castps_si128(v));
        _mm_store_ss(&(p + offset)->z, t);
    }

    inline UPALWAYSINLINE void storea4(float4 const v, float4* p) noexcept {
        _mm_store_ps(reinterpret_cast<float*>(p), v);
    }

    inline UPALWAYSINLINE void storea4(float4 const v, size_t offset, float4* p) noexcept {
        _mm_store_ps(reinterpret_cast<float*>(p + offset), v);
    }

    inline UPALWAYSINLINE void streama4(float4 const v, float4* p) noexcept {
        _mm_stream_ps(reinterpret_cast<float*>(p), v);
    }

    inline UPALWAYSINLINE void streama4(float4 const v, size_t offset, float4* p) noexcept {
        _mm_stream_ps(reinterpret_cast<float*>(p + offset), v);
    }

    //
    // Accessor Functions
    //

    inline UPALWAYSINLINE float get(float4 const v, size_t const n) noexcept {
        float4 result;
        switch (n) {
        case 0: result = v; break;
        case 1: result = _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(v), 4)); break;
        case 2: result = _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(v), 8)); break;
        case 3: result = _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(v), 12)); break;
        default: result = uniform<float4>::quiet_nan; break;
        }
        return _mm_cvtss_f32(result);
    }

    inline UPALWAYSINLINE float getx(float4 const v) noexcept {
        return _mm_cvtss_f32(v);
    }

    inline UPALWAYSINLINE float gety(float4 const v) noexcept {
        float4 result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm_cvtss_f32(result);
    }

    inline UPALWAYSINLINE float getz(float4 const v) noexcept {
        float4 result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
        return _mm_cvtss_f32(result);
    }

    inline UPALWAYSINLINE float getw(float4 const v) noexcept {
        float4 result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
        return _mm_cvtss_f32(result);
    }

    inline UPALWAYSINLINE float4 put(float4 const v, float value, size_t const n) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 r = _mm_set_ss(value);
        switch (n) {
        case 0: r = _mm_insert_ps(v, r, _MM_MK_INSERTPS_NDX(0, 0, 0)); break;
        case 1: r = _mm_insert_ps(v, r, _MM_MK_INSERTPS_NDX(0, 1, 0)); break;
        case 2: r = _mm_insert_ps(v, r, _MM_MK_INSERTPS_NDX(0, 2, 0)); break;
        case 3: r = _mm_insert_ps(v, r, _MM_MK_INSERTPS_NDX(0, 3, 0)); break;
        default: r = uniform<float4>::quiet_nan;
        }
        return r;
#else
        if (n > 3) {
            return uniform<float4>::quiet_nan;
        }
        uniform_constant<float4, float> r;
        r.v = v;
        r.s[n] = value;
        return r.v;
#endif
    }

    inline UPALWAYSINLINE float4 putx(float4 const v, float x) noexcept {
        float4 r = _mm_set_ss(x);
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_ps(v, r, _MM_MK_INSERTPS_NDX(0, 0, 0));
#else
        return _mm_move_ss(v, r);
#endif
    }

    inline UPALWAYSINLINE float4 puty(float4 const v, float y) noexcept {
        float4 t = _mm_set_ss(y);
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_ps(v, t, _MM_MK_INSERTPS_NDX(0, 1, 0));
#else
        float4 r = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 2, 0, 1));
        r = _mm_move_ss(r, t);
        return _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 2, 0, 1));
#endif
    }

    inline UPALWAYSINLINE float4 putz(float4 const v, float z) noexcept {
        float4 t = _mm_set_ss(z);
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_ps(v, t, _MM_MK_INSERTPS_NDX(0, 2, 0));
#else
        float4 r = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 1, 2));
        r = _mm_move_ss(r, t);
        return _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 0, 1, 2));
#endif
    }

    inline UPALWAYSINLINE float4 putw(float4 const v, float w) noexcept {
        float4 t = _mm_set_ss(w);
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_ps(v, t, _MM_MK_INSERTPS_NDX(0, 3, 0));
#else
        float4 r = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 2, 1, 3));
        r = _mm_move_ss(r, t);
        return _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 2, 1, 3));
#endif
    }

    //
    // Permutation Functions
    //

    inline UPALWAYSINLINE float4 packxy(float4 const v1, float4 const v2) noexcept {
        return _mm_movelh_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 packzw(float4 const v1, float4 const v2) noexcept {
        return _mm_movehl_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 unpackxy(float4 const v1, float4 const v2) noexcept {
        return _mm_unpacklo_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 unpackzw(float4 const v1, float4 const v2) noexcept {
        return _mm_unpackhi_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 reverse(float4 const v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 2, 3));
    }

    inline UPALWAYSINLINE float4 shuffle_xxyy(float4 const v) noexcept {
        return _mm_unpacklo_ps(v, v);
    }

    inline UPALWAYSINLINE float4 shuffle_zzww(float4 const v) noexcept {
        return _mm_unpackhi_ps(v, v);
    }

    inline UPALWAYSINLINE float4 shuffle_xyxy(float4 const v) noexcept {
        return _mm_movelh_ps(v, v);
    }

    inline UPALWAYSINLINE float4 shuffle_zwzw(float4 const v) noexcept {
        return _mm_movehl_ps(v, v);
    }

    inline UPALWAYSINLINE float4 splat(float4 const v, size_t const n) noexcept {
        float4 result;
        switch (n) {
        case 0: result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)); break;
        case 1: result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)); break;
        case 2: result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)); break;
        case 3: result = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)); break;
        default: result = uniform<float4>::quiet_nan; break;
        }
        return result;
    }

    inline UPALWAYSINLINE float4 splatx(float4 const v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
    }

    inline UPALWAYSINLINE float4 splaty(float4 const v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
    }

    inline UPALWAYSINLINE float4 splatz(float4 const v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
    }

    inline UPALWAYSINLINE float4 splatw(float4 const v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
    }

    //
    // Relational Functions
    //

    inline UPALWAYSINLINE int4 isequal(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmpeq_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 isnotequal(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmpneq_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 isgreater(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmpgt_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 isgreaterequal(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmpge_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 isless(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmplt_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 islessequal(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmple_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 islessgreater(float4 const v1, float4 const v2) noexcept {
        float4 t1 = _mm_cmplt_ps(v1, v2);
        float4 t2 = _mm_cmpgt_ps(v1, v2);
        return _mm_castps_si128(_mm_or_ps(t1, t2));
    }

    inline UPALWAYSINLINE int4 isordered(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmpord_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 isunordered(float4 const v1, float4 const v2) noexcept {
        return _mm_castps_si128(_mm_cmpunord_ps(v1, v2));
    }

    inline UPALWAYSINLINE int4 isinf(float4 const v) noexcept {
        float4 t = _mm_andnot_ps(uniform<float4>::negative_zero, v);
        return _mm_castps_si128(_mm_cmpeq_ps(t, uniform<float4>::infinity));
    }

    inline UPALWAYSINLINE int4 isnan(float4 const v) noexcept {
        return _mm_castps_si128(_mm_cmpunord_ps(v, v));
    }

    inline UPALWAYSINLINE int4 signbit(float4 const v) noexcept {
        int4 t = _mm_and_si128(_mm_castps_si128(v), as<int4>(uniform<float4>::negative_zero));
        return _mm_cmpeq_epi32(t, as<int4>(uniform<float4>::negative_zero));
    }

    inline UPALWAYSINLINE float4 bitselect(float4 const v1, float4 const v2, float4 const v3) noexcept {
        float4 t1 = _mm_and_ps(v2, v3);
        float4 t2 = _mm_andnot_ps(v3, v1);
        return _mm_or_ps(t1, t2);
    }

    inline UPALWAYSINLINE float4 select(float4 const v1, float4 const v2, int4 const v3) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_blendv_ps(v1, v2, _mm_castsi128_ps(v3));
#else
        float4 m = _mm_castsi128_ps(_mm_srai_epi32(v3, 31));
        float4 t1 = _mm_and_ps(v2, m);
        float4 t2 = _mm_andnot_ps(m, v1);
        return _mm_or_ps(t1, t2);
#endif
    }

    inline UPALWAYSINLINE float4 select(float4 const v1, float4 const v2, uint4_cval3_t v3) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_blendv_ps(v1, v2, _mm_castsi128_ps(v3.m));
#else
        float4 m = _mm_castsi128_ps(_mm_srai_epi32(v3.m, 31));
        float4 t1 = _mm_and_ps(v2, m);
        float4 t2 = _mm_andnot_ps(m, v1);
        return _mm_or_ps(t1, t2);
#endif
    }

    //
    // Arithmetic Functions
    //

    inline UPALWAYSINLINE float4 add(float4 const v1, float4 const v2) noexcept {
        return _mm_add_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 sub(float4 const v1, float4 const v2) noexcept {
        return _mm_sub_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 neg(float4 const v) noexcept {
        float4 z = _mm_setzero_ps();
        return _mm_sub_ps(z, v);
    }

    inline UPALWAYSINLINE float4 mul(float4 const v1, float4 const v2) noexcept {
        return _mm_mul_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 div(float4 const v1, float4 const v2) noexcept {
        return _mm_div_ps(v1, v2);
    }

    //
    // Mathematical Functions
    //

    inline UPALWAYSINLINE float4 ceil(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_round_ps(v, 0x0A);
#else
        float4 t = _mm_add_ps(v, uniform<float4>::half_minus_epsilon);
        int4 i = _mm_cvtps_epi32(t);
        return _mm_cvtepi32_ps(i);
#endif
    }

    inline UPALWAYSINLINE float4 floor(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_round_ps(v, 0x09);
#else
        float4 t = _mm_sub_ps(v, uniform<float4>::half_minus_epsilon);
        int4 i = _mm_cvtps_epi32(t);
        return _mm_cvtepi32_ps(i);
#endif
    }

    inline UPALWAYSINLINE float4 rint(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_round_ps(v, 0x0C);
#else
        int4 i = _mm_cvtps_epi32(v);
        return _mm_cvtepi32_ps(i);
#endif
    }

    inline UPALWAYSINLINE float4 round(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_round_ps(v, 0x08);
#else
        int4 i = _mm_cvtps_epi32(v);
        float4 t = _mm_cvtepi32_ps(i);
        i = _mm_and_si128(_mm_castps_si128(v), as<int4>(uniform<float4>::absolute_mask));
        i = _mm_cmpgt_epi32(as<int4>(uniform<float4>::fraction_mask), i);
        float4 u = _mm_castsi128_ps(i);
        t = _mm_and_ps(t, u);
        u = _mm_andnot_ps(u, v);
        return _mm_or_ps(t, u);
#endif
    }

    inline UPALWAYSINLINE float4 trunc(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_round_ps(v, 0x0B);
#else
        int4 i = _mm_cvttps_epi32(v);
        float4 t = _mm_cvtepi32_ps(i);
        i = _mm_and_si128(_mm_castps_si128(v), as<int4>(uniform<float4>::absolute_mask));
        i = _mm_cmpgt_epi32(as<int4>(uniform<float4>::fraction_mask), i);
        float4 u = _mm_castsi128_ps(i);
        t = _mm_and_ps(t, u);
        u = _mm_andnot_ps(u, v);
        return _mm_or_ps(t, u);
#endif
    }

    inline UPALWAYSINLINE float4 fma(float4 const v1, float4 const v2, float4 const v3) noexcept {
#if defined(UP_SIMD_AVX_2)
        return _mm_fmadd_ps(v1, v2, v3);
#else
        float4 a = _mm_mul_ps(v1, v2);
        return _mm_add_ps(a, v3);
#endif
    }

    inline UPALWAYSINLINE float4 fms(float4 const v1, float4 const v2, float4 const v3) noexcept {
#if defined(UP_SIMD_AVX_2)
        return _mm_fmsub_ps(v1, v2, v3);
#else
        float4 a = _mm_mul_ps(v1, v2);
        return _mm_sub_ps(a, v3);
#endif
    }

    inline UPALWAYSINLINE float4 fnma(float4 const v1, float4 const v2, float4 const v3) noexcept {
#if defined(UP_SIMD_AVX_2)
        return _mm_fnmadd_ps(v1, v2, v3);
#else
        float4 a = _mm_mul_ps(v1, v2);
        return _mm_sub_ps(v3, a);
#endif
    }

    inline UPALWAYSINLINE float4 fnms(float4 const v1, float4 const v2, float4 const v3) noexcept {
#if defined(UP_SIMD_AVX_2)
        return _mm_fnmsub_ps(v1, v2, v3);
#else
        float4 a = _mm_setzero_ps();
        float4 b = _mm_mul_ps(v1, v2);
        a = _mm_sub_ps(a, v3);
        return _mm_sub_ps(a, b);
#endif
    }

    inline UPALWAYSINLINE float4 mad(float4 const v1, float4 const v2, float4 const v3) noexcept {
#if defined(UP_SIMD_AVX_2)
        return _mm_fmadd_ps(v1, v2, v3);
#else
        float4 a = _mm_mul_ps(v1, v2);
        return _mm_add_ps(a, v3);
#endif
    }

    inline UPALWAYSINLINE float4 fabs(float4 const v) noexcept {
        return _mm_andnot_ps(uniform<float4>::negative_zero, v);
    }

    inline UPALWAYSINLINE float4 fdim(float4 const v1, float4 const v2) noexcept {
        float4 t = _mm_sub_ps(v1, v2);
        return _mm_andnot_ps(uniform<float4>::negative_zero, t);
    }

    inline UPALWAYSINLINE float4 fmax(float4 const v, float const s) noexcept {
        float4 t = _mm_set_ps1(s);
        return _mm_max_ps(t, v);
    }

    inline UPALWAYSINLINE float4 fmax(float4 const v1, float4 const v2) noexcept {
        return _mm_max_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 fmin(float4 const v, float const s) noexcept {
        float4 t = _mm_set_ps1(s);
        return _mm_min_ps(t, v);
    }

    inline UPALWAYSINLINE float4 fmin(float4 const v1, float4 const v2) noexcept {
        return _mm_min_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 recip(float4 const v) noexcept {
        float4 a, b, e;
        e = _mm_rcp_ps(v);
        a = _mm_mul_ps(e, e);
        b = _mm_add_ps(e, e);
#ifdef UP_SIMD_AVX2
        e = _mm_fnmadd_ps(a, v, b);
#else
        a = _mm_mul_ps(a, v);
        e = _mm_sub_ps(b, a);
#endif
        a = _mm_mul_ps(e, e);
        b = _mm_add_ps(e, e);
#ifdef UP_SIMD_AVX2
        a = _mm_fnmadd_ps(a, v, b);
#else
        a = _mm_mul_ps(a, v);
        a = _mm_sub_ps(b, a);
#endif
        return a;
    }

    inline UPALWAYSINLINE float4 rsqrt(float4 const v) noexcept {
        float4 a, b, e;
        e = _mm_rsqrt_ps(v);
        a = _mm_mul_ps(e, e);
        b = _mm_add_ps(e, e);
        a = _mm_mul_ps(a, e);
        b = _mm_add_ps(b, e);
#ifdef UP_SIMD_AVX2
        a = _mm_fnmadd_ps(a, v, b);
#else
        a = _mm_mul_ps(a, v);
        a = _mm_sub_ps(b, a);
#endif
        e = _mm_mul_ps(a, uniform<float4>::half);
        a = _mm_mul_ps(e, e);
        b = _mm_add_ps(e, e);
        a = _mm_mul_ps(a, e);
        b = _mm_add_ps(b, e);
#ifdef UP_SIMD_AVX2
        a = _mm_fnmadd_ps(a, v, b);
#else
        a = _mm_mul_ps(a, v);
        a = _mm_sub_ps(b, a);
#endif
        a = _mm_mul_ps(a, uniform<float4>::half);
        return a;
    }

    inline UPALWAYSINLINE float4 sqrt(float4 const v) noexcept {
        return _mm_sqrt_ps(v);
    }

    inline UPALWAYSINLINE float4 half_recip(float4 const v) noexcept {
        float4 a, b, e;
        e = _mm_rcp_ps(v);
        a = _mm_mul_ps(e, e);
        b = _mm_add_ps(e, e);
#ifdef UP_SIMD_AVX2
        a = _mm_fnmadd_ps(a, v, b);
#else
        a = _mm_mul_ps(a, v);
        a = _mm_sub_ps(b, a);
#endif
        return a;
    }

    inline UPALWAYSINLINE float4 half_rsqrt(float4 const v) noexcept {
        float4 a, b, e;
        e = _mm_rsqrt_ps(v);
        a = _mm_mul_ps(e, e);
        b = _mm_add_ps(e, e);
        a = _mm_mul_ps(a, e);
        b = _mm_add_ps(b, e);
#ifdef UP_SIMD_AVX2
        a = _mm_fnmadd_ps(a, v, b);
#else
        a = _mm_mul_ps(a, v);
        a = _mm_sub_ps(b, a);
#endif
        a = _mm_mul_ps(a, uniform<float4>::half);
        return a;
    }

    inline UPALWAYSINLINE float4 half_sqrt(float4 const v) noexcept {
        return _mm_sqrt_ps(v);
    }

    inline UPALWAYSINLINE float4 native_recip(float4 const v) noexcept {
        return _mm_rcp_ps(v);
    }

    inline UPALWAYSINLINE float4 native_rsqrt(float4 const v) noexcept {
        return _mm_rsqrt_ps(v);
    }

    inline UPALWAYSINLINE float4 native_sqrt(float4 const v) noexcept {
        return _mm_sqrt_ps(v);
    }

    //
    // Common Functions
    //

    inline UPALWAYSINLINE float4 clamp(float4 const v, float4 const min_val, float4 const max_val) noexcept {
        float4 t = _mm_max_ps(v, min_val);
        return _mm_min_ps(t, max_val);
    }

    inline UPALWAYSINLINE float4 clamp(float4 const v, float const min_val, float const max_val) noexcept {
        float4 min_val4 = _mm_set_ps1(min_val);
        float4 max_val4 = _mm_set_ps1(max_val);
        return clamp(v, min_val4, max_val4);
    }

    inline UPALWAYSINLINE float4 max(float4 const v1, float4 const v2) noexcept {
        return _mm_max_ps(v1, v2);
    }

    inline UPALWAYSINLINE float4 max(float4 const v, float const s) noexcept {
        float4 t = _mm_set_ps1(s);
        return _mm_max_ps(t, v);
    }

    inline UPALWAYSINLINE float4 min(float4 const v1, float4 const v2) noexcept {
        return _mm_min_ps(v1, v2);
    }
    
    inline UPALWAYSINLINE float4 min(float4 const v, float const s) noexcept {
        float4 t = _mm_set_ps1(s);
        return _mm_min_ps(t, v);
    }

    inline UPALWAYSINLINE float4 mix(float4 const v1, float4 const v2, float4 const s) noexcept {
        float4 t = _mm_sub_ps(v2, v1);
        return fma(t, s, v1);
    }

    inline UPALWAYSINLINE float4 mix(float4 const v1, float4 const v2, float const s) noexcept {
        float4 s4 = _mm_set_ps1(s);
        return mix(v1, v2, s4);
    }

    //
    // 2D Geometric Functions
    //
    
    inline UPALWAYSINLINE float4 cross2(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(0, 1, 0, 1));
        a = _mm_mul_ps(a, v1);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_sub_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
        return a;
    }

    inline UPALWAYSINLINE float4 dot2(float4 const v1, float4 const v2) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_dp_ps(v1, v2, 0x3F);
#else
        float4 a = _mm_mul_ps(v1, v2);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
        return a;
#endif
    }

    inline UPALWAYSINLINE float4 distance2(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_sub_ps(v1, v2);
#if defined(UP_SIMD_SSE_4_1)
        a = _mm_dp_ps(a, a, 0x3F);
#else
        a = _mm_mul_ps(a, a);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_sqrt_ps(a);
        return a;
    }

    inline UPALWAYSINLINE float4 length2(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x3F);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_sqrt_ps(a);
        return a;
    }

    inline UPALWAYSINLINE float4 rlength2(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x3F);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_sqrt_ps(a);
        a = _mm_div_ps(uniform<float4>::one, a);
        return a;
    }

    inline UPALWAYSINLINE float4 normalize2(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 b, a = _mm_dp_ps(v, v, 0x3F);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        b = _mm_cmpneq_ps(a, uniform<float4>::infinity);
        a = _mm_sqrt_ps(a);
        a = _mm_div_ps(v, a);
        a = _mm_and_ps(a, b);
        return a;
    }

    inline UPALWAYSINLINE float4 reflect2(float4 const incident, float4 const normal) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(incident, normal, 0x3F);
#else
        float4 a = _mm_mul_ps(incident, normal);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_add_ps(a, a);
        a = _mm_mul_ps(a, normal);
        a = _mm_sub_ps(incident, a);
        return a;
    }

    inline UPALWAYSINLINE float4 refract2(float4 const incident, float4 const normal, float4 const index) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 b, a = _mm_dp_ps(incident, normal, 0x3F);
#else
        float4 a = _mm_mul_ps(incident, normal);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        b = _mm_mul_ps(a, a);
        b = _mm_sub_ps(uniform<float4>::one, b);
        b = _mm_mul_ps(b, index);
        b = _mm_mul_ps(b, index);
        b = _mm_sub_ps(uniform<float4>::one, b);
        float4 c = _mm_setzero_ps();
        c = _mm_cmpgt_ps(b, c);
        b = _mm_sqrt_ps(b);
        a = _mm_mul_ps(index, a);
        b = _mm_add_ps(b, a);
        a = _mm_mul_ps(index, incident);
        b = _mm_mul_ps(b, normal);
        a = _mm_sub_ps(a, b);
        a = _mm_and_ps(a, c);
        return a;    
    }

    inline UPALWAYSINLINE float4 refract2(float4 const incident, float4 const normal, float index) noexcept {
        float4 index4 = _mm_set_ps1(index);
        return refract2(incident, normal, index4);
    }

    inline UPALWAYSINLINE float4 fast_distance2(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_sub_ps(v1, v2);
#if defined(UP_SIMD_SSE_4_1)
        a = _mm_dp_ps(a, a, 0x3F);
#else
        a = _mm_mul_ps(a, a);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_sqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_length2(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x3F);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_sqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_rlength2(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x3F);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_rsqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_normalize2(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x3F);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_rsqrt(a);
        a = _mm_mul_ps(v, a);
        return a;
    }

    //
    // 3D Geometric Functions
    //
    
    inline UPALWAYSINLINE float4 cross3(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
        float4 b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));
        float4 c = _mm_mul_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
        a = _mm_mul_ps(a, b);
        c = _mm_sub_ps(c, a);
        c = _mm_and_ps(c, uniform<float4>::mask_xyz);
        return c;
    }

    inline UPALWAYSINLINE float4 dot3(float4 const v1, float4 const v2) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_dp_ps(v1, v2, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v1, v2);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
        return a;
#else
        float4 a = _mm_mul_ps(v1, v2);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
        return a;
#endif
    }

    inline UPALWAYSINLINE float4 distance3(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_sub_ps(v1, v2);
#if defined(UP_SIMD_SSE_4_1)
        a = _mm_dp_ps(a, a, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        a = _mm_mul_ps(a, a);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        a = _mm_mul_ps(a, a);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_sqrt_ps(a);
        return a;
    }

    inline UPALWAYSINLINE float4 length3(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif        
        a = _mm_sqrt_ps(a);
        return a;
    }

    inline UPALWAYSINLINE float4 rlength3(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_sqrt_ps(a);
        a = _mm_div_ps(uniform<float4>::one, a);
        return a;
    }

    inline UPALWAYSINLINE float4 normalize3(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 b, a = _mm_dp_ps(v, v, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 b, a = _mm_mul_ps(v, v);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        b = _mm_cmpneq_ps(a, uniform<float4>::infinity);
        a = _mm_sqrt_ps(a);
        a = _mm_div_ps(v, a);
        a = _mm_and_ps(a, b);
        return a;
    }

    inline UPALWAYSINLINE float4 reflect3(float4 const incident, float4 const normal) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(incident, normal, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(incident, normal);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(incident, normal);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = _mm_add_ps(a, a);
        a = _mm_mul_ps(a, normal);
        a = _mm_sub_ps(incident, a);
        return a;
    }

    inline UPALWAYSINLINE float4 refract3(float4 const incident, float4 const normal, float4 const index) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 b, a = _mm_dp_ps(incident, normal, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 b, a = _mm_mul_ps(incident, normal);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(incident, normal);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        b = _mm_mul_ps(a, a);
        b = _mm_sub_ps(uniform<float4>::one, b);
        b = _mm_mul_ps(b, index);
        b = _mm_mul_ps(b, index);
        b = _mm_sub_ps(uniform<float4>::one, b);
        float4 c = _mm_setzero_ps();
        c = _mm_cmpgt_ps(b, c);
        b = _mm_sqrt_ps(b);
        a = _mm_mul_ps(index, a);
        b = _mm_add_ps(b, a);
        a = _mm_mul_ps(index, incident);
        b = _mm_mul_ps(b, normal);
        a = _mm_sub_ps(a, b);
        a = _mm_and_ps(a, c);
        return a;    
    }

    inline UPALWAYSINLINE float4 refract3(float4 const incident, float4 const normal, float index) noexcept {
        float4 index4 = _mm_set_ps1(index);
        return refract3(incident, normal, index4);
    }

    inline UPALWAYSINLINE float4 fast_distance3(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_sub_ps(v1, v2);
#if defined(UP_SIMD_SSE_4_1)
        a = _mm_dp_ps(a, a, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        a = _mm_mul_ps(a, a);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        a = _mm_mul_ps(a, a);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_sqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_length3(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_sqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_rlength3(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_rsqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_normalize3(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0x7F);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_and_ps(a, uniform<float4>::mask_xyz);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_add_ss(a, b);
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1));
        a = _mm_add_ss(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
#endif
        a = half_rsqrt(a);
        a = _mm_mul_ps(v, a);
        return a;
    }

    //
    // 4D Geometric Functions
    //

    inline UPALWAYSINLINE float4 cross4(float4 const v1, float4 const v2, float4 const v3) noexcept {
        float4 r, a, b, c;
        
        // a = v2.zwyz * v3.wzwy
        r = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 1, 3, 2));
        c = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 3, 2, 3));
        r = _mm_mul_ps(r, c);
        
        // b = v2.wzwy * v3.zwyz
        b = _mm_shuffle_ps(v2, v2,_MM_SHUFFLE(1, 3, 2, 3));
        c = _mm_shuffle_ps(c, c,_MM_SHUFFLE(1, 3, 0, 1));
        b = _mm_mul_ps(b, c);
        
        // r = (a - b) * v1.yxxx
        r = _mm_sub_ps(r, b);
        a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 0, 0, 1));
        r = _mm_mul_ps(r, a);

        // a = v2.ywxz * v3.wxwx
        b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 0, 3, 1));
        c = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 3, 0, 3));
        c = _mm_mul_ps(c, b);
        
        // b = v2.wxwx * v3.ywxz
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 1, 2, 1));
        a = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(2, 0, 3, 1));
        b = _mm_mul_ps(b, a);
        
        // r -= (a - b) * v1.zzyy
        c = _mm_sub_ps(c, b);
        a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(1, 1, 2, 2));
        a = _mm_mul_ps(a, c);
        r = _mm_sub_ps(r, a);

        // a = v2.yzxy * v3.zxyx
        b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(1, 0, 2, 1));
        c = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 1, 0, 2));
        c = _mm_mul_ps(c, b);
        
        // b = v2.zxyx * v3.yzxy
        b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 0, 2, 1));
        a = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 0, 2, 1));
        a = _mm_mul_ps(a, b);
        
        // r += (a - b) * v1.wwwz
        c = _mm_sub_ps(c, a);
        a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(2, 3, 3, 3));
        c = _mm_mul_ps(c, a);
        r = _mm_add_ps(r, c);

        return r;
    }

    inline UPALWAYSINLINE float4 dot4(float4 const v1, float4 const v2) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_dp_ps(v1, v2, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v1, v2);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
        return a;
#else
        float4 a = _mm_mul_ps(v1, v2);
        float4 b = _mm_shuffle_ps(v2, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_add_ps(b, a);
        a = _mm_shuffle_ps(a, b, _MM_SHUFFLE(0, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
        return a;
#endif
    }

    inline UPALWAYSINLINE float4 distance4(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_sub_ps(v1, v2);
#if defined(UP_SIMD_SSE_4_1)
        a = _mm_dp_ps(a, a, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        a = _mm_mul_ps(a, a);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        a = _mm_mul_ps(a, a);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = _mm_sqrt_ps(a);
        return a;
    }

    inline UPALWAYSINLINE float4 length4(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = _mm_sqrt_ps(a);
        return a;
    }

    inline UPALWAYSINLINE float4 rlength4(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = _mm_sqrt_ps(a);
        a = _mm_div_ps(uniform<float4>::one, a);
        return a;
    }

    inline UPALWAYSINLINE float4 normalize4(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 b, a = _mm_dp_ps(v, v, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 b, a = _mm_mul_ps(v, v);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        b = _mm_cmpneq_ps(a, uniform<float4>::infinity);
        a = _mm_sqrt_ps(a);
        a = _mm_div_ps(v, a);
        a = _mm_and_ps(a, b);
        return a;
    }

    inline UPALWAYSINLINE float4 reflect4(float4 const incident, float4 const normal) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(incident, normal, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(incident, normal);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(incident, normal);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = _mm_add_ps(a, a);
        a = _mm_mul_ps(a, normal);
        a = _mm_sub_ps(incident, a);
        return a;
    }

    inline UPALWAYSINLINE float4 refract4(float4 const incident, float4 const normal, float4 const index) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 b, a = _mm_dp_ps(incident, normal, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 b, a = _mm_mul_ps(incident, normal);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(incident, normal);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        b = _mm_mul_ps(a, a);
        b = _mm_sub_ps(uniform<float4>::one, b);
        b = _mm_mul_ps(b, index);
        b = _mm_mul_ps(b, index);
        b = _mm_sub_ps(uniform<float4>::one, b);
        float4 c = _mm_setzero_ps();
        c = _mm_cmpgt_ps(b, c);
        b = _mm_sqrt_ps(b);
        a = _mm_mul_ps(index, a);
        b = _mm_add_ps(b, a);
        a = _mm_mul_ps(index, incident);
        b = _mm_mul_ps(b, normal);
        a = _mm_sub_ps(a, b);
        a = _mm_and_ps(a, c);
        return a;    
    }

    inline UPALWAYSINLINE float4 refract4(float4 const incident, float4 const normal, float index) noexcept {
        float4 index4 = _mm_set_ps1(index);
        return refract4(incident, normal, index4);
    }

    inline UPALWAYSINLINE float4 fast_distance4(float4 const v1, float4 const v2) noexcept {
        float4 a = _mm_sub_ps(v1, v2);
#if defined(UP_SIMD_SSE_4_1)
        a = _mm_dp_ps(a, a, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        a = _mm_mul_ps(a, a);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        a = _mm_mul_ps(a, a);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = half_sqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_length4(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = half_sqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_rlength4(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = half_rsqrt(a);
        return a;
    }

    inline UPALWAYSINLINE float4 fast_normalize4(float4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        float4 a = _mm_dp_ps(v, v, 0xFF);
#elif defined(UP_SIMD_SSE_3)
        float4 a = _mm_mul_ps(v, v);
        a = _mm_hadd_ps(a, a);
        a = _mm_hadd_ps(a, a);
#else
        float4 a = _mm_mul_ps(v, v);
        float4 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 0, 0));
        b = _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 3, 0, 0));
        a = _mm_add_ps(a, b);
        a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
#endif
        a = half_rsqrt(a);
        a = _mm_mul_ps(v, a);
        return a;
    }
}}

#endif
