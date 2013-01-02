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

#ifndef UP_MATH_SSE_INT4_OPERATIONS_INL
#define UP_MATH_SSE_INT4_OPERATIONS_INL

namespace up { namespace math { namespace detail
{
    template <>
    struct constructors<int4>
    {
        typedef int32_t element_type;
        typedef int32_t const* element_pointer;
        typedef int2 const vector2_type;
        typedef int3 const vector3_type;
        typedef int4 const vector4_type;
        typedef int2 const* vector2_pointer;
        typedef int3 const* vector3_pointer;
        typedef int4 const* vector4_pointer;

        //
        // Initialization Functions
        //

        static UPALWAYSINLINE int4 broadcast(int32_t x) noexcept {
            return _mm_set1_epi32(x);
        }

        static UPALWAYSINLINE int4 broadcast2(int2 const v)  noexcept {
            return _mm_set_epi32(v.y, v.x, v.y, v.x);
        }

        static UPALWAYSINLINE int4 make(int32_t x) noexcept {
            union { int32_t s; float f; } r = { x };
            return _mm_castps_si128(_mm_set_ss(r.f));
        }

        static UPALWAYSINLINE int4 make(int32_t x, int32_t y) noexcept {
            return _mm_set_epi32(0, 0, y, x);
        }

        static UPALWAYSINLINE int4 make(int32_t x, int32_t y, int32_t z) noexcept {
            return _mm_set_epi32(0, z, y, x);
        }

        static UPALWAYSINLINE int4 make(int32_t x, int32_t y, int32_t z, int32_t w) noexcept {
            return _mm_set_epi32(w, z, y, x);
        }

        static UPALWAYSINLINE int4 zero() noexcept {
            return _mm_setzero_si128();
        }

        //
        // Load Functions
        //

        static UPALWAYSINLINE int4 loadbroadcast(int32_t const* p) noexcept {
#if defined(UP_SIMD_AVX)
            return _mm_castps_si128(_mm_broadcast_ss(reinterpret_cast<float const*>(p)));
#else
            return _mm_castps_si128(_mm_load_ps1(reinterpret_cast<float const*>(p)));
#endif
        }

        static UPALWAYSINLINE int4 loadbroadcast(size_t offset, int32_t const* p) noexcept {
#if defined(UP_SIMD_AVX)
            return _mm_castps_si128(_mm_broadcast_ss(reinterpret_cast<float const*>(p + offset)));
#else
            return _mm_castps_si128(_mm_load_ps1(reinterpret_cast<float const*>(p + offset)));
#endif
        }

        static UPALWAYSINLINE int4 loadbroadcast2(int2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->y)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            return _mm_unpacklo_epi64(xy, xy);
        }

        static UPALWAYSINLINE int4 loadbroadcast2(size_t offset, int2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->y)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            return _mm_unpacklo_epi64(xy, xy);
        }

        static UPALWAYSINLINE int4 loadscalar(int32_t const* p) noexcept {
            return _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(p)));
        }

        static UPALWAYSINLINE int4 loadscalar(size_t offset, int32_t const* p) noexcept {
            return _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(p + offset)));
        }

        static UPALWAYSINLINE int4 load2(int2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->y)));
            return _mm_unpacklo_epi32(x, y);
        }

        static UPALWAYSINLINE int4 load2(size_t offset, int2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->y)));
            return _mm_unpacklo_epi32(x, y);
        }

        static UPALWAYSINLINE int4 load3(int3 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->y)));
            int4 z = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->z)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            return _mm_unpacklo_epi64(xy, z);
        }

        static UPALWAYSINLINE int4 load3(size_t offset, int3 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->y)));
            int4 z = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->z)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            return _mm_unpacklo_epi64(xy, z);
        }

        static UPALWAYSINLINE int4 load4(int4 const* p) noexcept {
            return _mm_loadu_si128(p);
        }

        static UPALWAYSINLINE int4 load4(size_t offset, int4 const* p) noexcept {
            return _mm_loadu_si128(p + offset);
        }

        static UPALWAYSINLINE int4 loada2(int2 const* p) noexcept {
            return _mm_loadl_epi64(reinterpret_cast<int4 const*>(p));
        }

        static UPALWAYSINLINE int4 loada2(size_t offset, int2 const* p) noexcept {
            return _mm_loadl_epi64(reinterpret_cast<int4 const*>(p + offset));
        }
 
        static UPALWAYSINLINE int4 loada3(int3 const* p) noexcept {
            return _mm_load_si128(reinterpret_cast<int4 const*>(p)); // ignore extra element loaded
        }

        static UPALWAYSINLINE int4 loada3(size_t offset, int3 const* p) noexcept {
            return _mm_load_si128(reinterpret_cast<int4 const*>(p + offset)); // ignore extra element loaded
        }

        static UPALWAYSINLINE int4 loada4(int4 const* p) noexcept {
            return _mm_load_si128(p);
        }

        static UPALWAYSINLINE int4 loada4(size_t offset, int4 const* p) noexcept {
            return _mm_load_si128(p + offset);
        }
    };
}}}

namespace up { namespace math
{
    //
    // Load Functions
    //

    inline UPALWAYSINLINE int4 load4(int4 const* p) noexcept {
        return _mm_loadu_si128(p);
    }

    inline UPALWAYSINLINE int4 load4(size_t offset, int4 const* p) noexcept {
        return _mm_loadu_si128(p + offset);
    }

    inline UPALWAYSINLINE int4 loada4(int4 const* p) noexcept {
        return _mm_load_si128(p);
    }

    inline UPALWAYSINLINE int4 loada4(size_t offset, int4 const* p) noexcept {
        return _mm_load_si128(p + offset);
    }

    //
    // Store Functions
    //

    inline UPALWAYSINLINE void storescalar(int4 const v, int32_t* p) noexcept {
        _mm_store_ps1(reinterpret_cast<float*>(p), _mm_castsi128_ps(v));
    }

    inline UPALWAYSINLINE void storescalar(int4 const v, size_t offset, int32_t* p) noexcept {
        _mm_store_ps1(reinterpret_cast<float*>(p + offset), _mm_castsi128_ps(v));
    }

    inline UPALWAYSINLINE void store2(int4 const v, int2* p) noexcept {
        float4 w = _mm_castsi128_ps(v);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        _mm_store_ss(reinterpret_cast<float*>(&p->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&p->y), u);
    }

    inline UPALWAYSINLINE void store2(int4 const v, size_t offset, int2* p) noexcept {
        float4 w = _mm_castsi128_ps(v);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->y), u); 
    }

    inline UPALWAYSINLINE void store3(int4 const v, int3* p) noexcept {
        float4 w = _mm_castsi128_ps(v);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        float4 t = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(&p->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&p->y), u);
        _mm_store_ss(reinterpret_cast<float*>(&p->z), t);
    }

    inline UPALWAYSINLINE void store3(int4 const v, size_t offset, int3* p) noexcept {
        float4 w = _mm_castsi128_ps(v);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        float4 t = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->y), u);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->z), t);
    }
 
    inline UPALWAYSINLINE void store4(int4 const v, int4* p) noexcept {
        _mm_storeu_si128(p, v);
    }

    inline UPALWAYSINLINE void store4(int4 const v, size_t offset, int4* p) noexcept {
        _mm_storeu_si128(p + offset, v);
    }
    
    inline UPALWAYSINLINE void storea2(int4 const v, int2* p) noexcept {
        _mm_storel_epi64(reinterpret_cast<int4*>(p), v);
    }

    inline UPALWAYSINLINE void storea2(int4 const v, size_t offset, int2* p) noexcept {
        _mm_storel_epi64(reinterpret_cast<int4*>(p + offset), v);
    }

    inline UPALWAYSINLINE void storea3(int4 const v, int3* p) noexcept {
        int4 t = _mm_shuffle_epi32(v, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_storel_epi64(reinterpret_cast<int4*>(p), v);
        _mm_store_ss(reinterpret_cast<float*>(&p->z), _mm_castsi128_ps(t));
    }

    inline UPALWAYSINLINE void storea3(int4 const v, size_t offset, int3* p) noexcept {
        int4 t = _mm_shuffle_epi32(v, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_storel_epi64(reinterpret_cast<int4*>(p + offset), v);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->z), _mm_castsi128_ps(t));
    }

    inline UPALWAYSINLINE void storea4(int4 const v, int4* p) noexcept {
        _mm_store_si128(p, v);
    }

    inline UPALWAYSINLINE void storea4(int4 const v, size_t offset, int4* p) noexcept {
        _mm_store_si128(p + offset, v);
    }

    inline UPALWAYSINLINE void streama4(int4 const v, int4* p) noexcept {
        _mm_stream_si128(p, v);
    }

    inline UPALWAYSINLINE void streama4(int4 const v, size_t offset, int4* p) noexcept {
        _mm_stream_si128(p + offset, v);
    }

    //
    // Accessor Functions
    //

    inline UPALWAYSINLINE int32_t get(int4 const v, size_t n) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        int32_t r;
        switch (n) {
        case 0: r = _mm_extract_epi32(v, 0); break;
        case 1: r = _mm_extract_epi32(v, 1); break;
        case 2: r = _mm_extract_epi32(v, 2); break;
        case 3: r = _mm_extract_epi32(v, 3); break;
        default: r = 0; break;
        }
        return r;
#else
        int4 r;
        switch (n) {
        case 0: r = v; break;
        case 1: r = _mm_srli_si128(v, 4); break;
        case 2: r = _mm_srli_si128(v, 8); break;
        case 3: r = _mm_srli_si128(v, 12); break;
        default: r = _mm_setzero_si128(); break;
        }
        return _mm_cvtsi128_si32(r);
#endif
    }

    inline UPALWAYSINLINE int32_t getx(int4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v, 0);
#else
        return _mm_cvtsi128_si32(v);
#endif
    }

    inline UPALWAYSINLINE int32_t gety(int4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v, 1);
#else
        int4 r = _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm_cvtsi128_si32(r);
#endif
    }

    inline UPALWAYSINLINE int32_t getz(int4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v, 2);
#else
        int4 r = _mm_shuffle_epi32(v, _MM_SHUFFLE(2, 2, 2, 2));
        return _mm_cvtsi128_si32(r);       
#endif
    }

    inline UPALWAYSINLINE int32_t getw(int4 const v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v, 3);
#else
        int4 r = _mm_shuffle_epi32(v, _MM_SHUFFLE(3, 3, 3, 3));
        return _mm_cvtsi128_si32(r);
#endif
    }

    inline UPALWAYSINLINE int4 put(int4 const v, int32_t value, size_t n) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        int4 r;
        switch (n) {
        case 0: r = _mm_insert_epi32(v, value, 0); break;
        case 1: r = _mm_insert_epi32(v, value, 1); break;
        case 2: r = _mm_insert_epi32(v, value, 2); break;
        case 3: r = _mm_insert_epi32(v, value, 3); break;
        default: r = uniform<int4>::zero;
        }
        return r;
#else
        if (n > 3) {
            return uniform<int4>::zero;
        }
        uniform_constant<int4, int32_t> r;
        r.v = v;
        r.s[n] = value;
        return r.v;
#endif
    }

    inline UPALWAYSINLINE int4 putx(int4 const v, int32_t x) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_epi32(v, x, 0);
#else
        union { int32_t i; float f; } c = { x };
        float4 t = _mm_set_ss(c.f);
        t = _mm_move_ss(_mm_castsi128_ps(v), t);
        return _mm_castps_si128(t);
#endif
    }

    inline UPALWAYSINLINE int4 puty(int4 const v, int32_t y) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_epi32(v, y, 1);
#else
        union { int32_t i; float f; } c = { y };
        float4 t = _mm_set_ss(c.f);
        float4 r = _mm_castsi128_ps(v);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 2, 0, 1));
        r = _mm_move_ss(r, t);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 2, 0, 1));
        return _mm_castps_si128(r);
#endif
    }

    inline UPALWAYSINLINE int4 putz(int4 const v, int32_t z) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_epi32(v, z, 2);
#else
        union { int32_t i; float f; } c = { z };
        float4 t = _mm_set_ss(c.f);
        float4 r = _mm_castsi128_ps(v);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 0, 1, 2));
        r = _mm_move_ss(r, t);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 0, 1, 2));
        return _mm_castps_si128(r);
#endif
    }

    inline UPALWAYSINLINE int4 putw(int4 const v, int32_t w) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_insert_epi32(v, w, 3);
#else
        union { int32_t i; float f; } c = { w };
        float4 t = _mm_set_ss(c.f);
        float4 r = _mm_castsi128_ps(v);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 2, 1, 3));
        r = _mm_move_ss(r, t);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 2, 1, 3));
        return _mm_castps_si128(r);
#endif
    }

    //
    // Permutation Functions
    //

    inline UPALWAYSINLINE int4 packxy(int4 const v1, int4 const v2) noexcept {
        return _mm_unpacklo_epi64(v1, v2);
    }

    inline UPALWAYSINLINE int4 packzw(int4 const v1, int4 const v2) noexcept {
        return _mm_unpackhi_epi64(v1, v2);
    }

    inline UPALWAYSINLINE int4 unpackxy(int4 const v1, int4 const v2) noexcept {
        return _mm_unpacklo_epi32(v1, v2);
    }

    inline UPALWAYSINLINE int4 unpackzw(int4 const v1, int4 const v2) noexcept {
        return _mm_unpackhi_epi32(v1, v2);
    }

    inline UPALWAYSINLINE int4 reverse(int4 const v) noexcept {
        return _mm_shuffle_epi32(v, _MM_SHUFFLE(0, 1, 2, 3));
    }

    inline UPALWAYSINLINE int4 shuffle_xxyy(int4 const v) noexcept {
        return _mm_unpacklo_epi32(v, v);
    }

    inline UPALWAYSINLINE int4 shuffle_zzww(int4 const v) noexcept {
        return _mm_unpackhi_epi32(v, v);
    }

    inline UPALWAYSINLINE int4 shuffle_xyxy(int4 const v) noexcept {
        return _mm_unpacklo_epi64(v, v);
    }

    inline UPALWAYSINLINE int4 shuffle_zwzw(int4 const v) noexcept {
        return _mm_unpackhi_epi64(v, v);
    }

    inline UPALWAYSINLINE int4 splat(int4 const v, size_t n) noexcept {
        int4 r;
        switch (n) {
        case 0: r = _mm_shuffle_epi32(v, _MM_SHUFFLE(0, 0, 0, 0)); break;
        case 1: r = _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 1, 1, 1)); break;
        case 2: r = _mm_shuffle_epi32(v, _MM_SHUFFLE(2, 2, 2, 2)); break;
        case 3: r = _mm_shuffle_epi32(v, _MM_SHUFFLE(3, 3, 3, 3)); break;
        default: r = uniform<int4>::zero; break;
        }
        return r;
    }

    inline UPALWAYSINLINE int4 splatx(int4 const v) noexcept {
        return _mm_shuffle_epi32(v, _MM_SHUFFLE(0, 0, 0, 0));
    }

    inline UPALWAYSINLINE int4 splaty(int4 const v) noexcept {
        return _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 1, 1, 1));
    }

    inline UPALWAYSINLINE int4 splatz(int4 const v) noexcept {
        return _mm_shuffle_epi32(v, _MM_SHUFFLE(2, 2, 2, 2));
    }

    inline UPALWAYSINLINE int4 splatw(int4 const v) noexcept {
        return _mm_shuffle_epi32(v, _MM_SHUFFLE(3, 3, 3, 3));
    }

    //
    // Relational Functions
    //

    inline UPALWAYSINLINE int4 isequal(int4 const v1, int4 const v2) noexcept {
        return _mm_cmpeq_epi32(v1, v2);
    }

    inline UPALWAYSINLINE int4 isnotequal(int4 const v1, int4 const v2) noexcept {
        int4 r = _mm_cmpeq_epi32(v1, v2);
        return _mm_andnot_si128(r, uniform<int4>::mask_xyzw);
    }

    inline UPALWAYSINLINE int4 isgreater(int4 const v1, int4 const v2) noexcept {
        return _mm_cmpgt_epi32(v1, v2);
    }

    inline UPALWAYSINLINE int4 isgreaterequal(int4 const v1, int4 const v2) noexcept {
        int4 r = _mm_cmpgt_epi32(v2, v1);
        return _mm_andnot_si128(r, uniform<int4>::mask_xyzw);
    }

    inline UPALWAYSINLINE int4 isless(int4 const v1, int4 const v2) noexcept {
        return _mm_cmpgt_epi32(v2, v1);
    }

    inline UPALWAYSINLINE int4 islessequal(int4 const v1, int4 const v2) noexcept {
        int4 r = _mm_cmpgt_epi32(v1, v2);
        return _mm_andnot_si128(r, uniform<int4>::mask_xyzw);
    }

    inline UPALWAYSINLINE bool all(int4 const v) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_testc_si128(v, uniform<int4>::min) == 1;
#else
        return _mm_movemask_ps(_mm_castsi128_ps(v)) == 0x0F;
#endif
    }

    inline UPALWAYSINLINE bool all(int4 const v, int const mask) noexcept {
        return (_mm_movemask_ps(_mm_castsi128_ps(v)) & mask) == mask;
    }

    inline UPALWAYSINLINE bool all(int4 const v, int4 const mask) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_testc_si128(v, mask) == 1;
#else
        return _mm_movemask_ps(_mm_castsi128_ps(_mm_andnot_si128(v, mask))) == 0x00;
#endif
    }

    inline UPALWAYSINLINE bool any(int4 const v) noexcept {
        return _mm_movemask_ps(_mm_castsi128_ps(v)) != 0x00;
    }

    inline UPALWAYSINLINE int4 bitselect(int4 const v1, int4 const v2, int4 const v3) noexcept {
        int4 r1 = _mm_and_si128(v2, v3);
        int4 r2 = _mm_andnot_si128(v3, v1);
        return _mm_or_si128(r1, r2);
    }

    inline UPALWAYSINLINE int4 select(int4 const v1, int4 const v2, int4 const v3) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(v1), _mm_castsi128_ps(v2), _mm_castsi128_ps(v3)));
#else
        int4 m = _mm_srai_epi32(v3, 31);
        int4 r1 = _mm_and_si128(v2, m);
        int4 r2 = _mm_andnot_si128(m, v1);
        return _mm_or_si128(r1, r2);
#endif
    }

    inline UPALWAYSINLINE int4 select(int4 const v1, int4 const v2, uint4_cval3_t v3) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(v1), _mm_castsi128_ps(v2), _mm_castsi128_ps(v3.m)));
#else
        int4 m = _mm_srai_epi32(v3.m, 31);
        int4 r1 = _mm_and_si128(v2, m);
        int4 r2 = _mm_andnot_si128(m, v1);
        return _mm_or_si128(r1, r2);
#endif
    }

    //
    // Arithmetic Functions
    //

    inline UPALWAYSINLINE int4 add(int4 const v1, int4 const v2) noexcept {
        return _mm_add_epi32(v1, v2);
    }

    inline UPALWAYSINLINE int4 add_sat(int4 const v1, int4 const v2) noexcept {
        int4 r = _mm_add_epi32(v1, v2);
        int4 sign_and = _mm_and_si128(v1, v2);
        int4 sign_or = _mm_or_si128(v1, v2);
        int4 min_mask = _mm_andnot_si128(r, sign_and);
        int4 max_mask = _mm_andnot_si128(sign_or, r);
#ifdef UP_SIMD_SSE_4_1
        float4 t = _mm_blendv_ps(_mm_castsi128_ps(r), as<float4>(uniform<int4>::min), _mm_castsi128_ps(min_mask));
        return _mm_castps_si128(_mm_blendv_ps(t, as<float4>(uniform<int4>::max), _mm_castsi128_ps(max_mask)));
#else
        min_mask = _mm_srai_epi32(min_mask, 31);
        max_mask = _mm_srai_epi32(max_mask, 31);
        int4 a = _mm_and_si128(uniform<int4>::min, min_mask);
        int4 b = _mm_andnot_si128(min_mask, r);
        r = _mm_or_si128(a, b);
        a = _mm_and_si128(uniform<int4>::max, max_mask);
        b = _mm_andnot_si128(max_mask, r);
        return _mm_or_si128(a, b);
#endif
    }

    inline UPALWAYSINLINE int4 sub(int4 const v1, int4 const v2) noexcept {
        return _mm_sub_epi32(v1, v2);
    }

    inline UPALWAYSINLINE int4 sub_sat(int4 const v1, int4 const v2) noexcept {
        int4 t = _mm_cmpeq_epi32(v2, v2);
        t = _mm_xor_si128(t, v2);
        int4 r = _mm_sub_epi32(v1, v2);
        int4 sign_and = _mm_and_si128(v1, t);
        int4 sign_or = _mm_or_si128(v1, t);
        int4 min_mask = _mm_andnot_si128(r, sign_and);
        int4 max_mask = _mm_andnot_si128(sign_or, r);
#ifdef UP_SIMD_SSE_4_1
        float4 t2 = _mm_blendv_ps(_mm_castsi128_ps(r), as<float4>(uniform<int4>::min), _mm_castsi128_ps(min_mask));
        return _mm_castps_si128(_mm_blendv_ps(t2, as<float4>(uniform<int4>::max), _mm_castsi128_ps(max_mask)));
#else
        min_mask = _mm_srai_epi32(min_mask, 31);
        max_mask = _mm_srai_epi32(max_mask, 31);
        int4 a = _mm_and_si128(uniform<int4>::min, min_mask);
        int4 b = _mm_andnot_si128(min_mask, r);
        r = _mm_or_si128(a, b);
        a = _mm_and_si128(uniform<int4>::max, max_mask);
        b = _mm_andnot_si128(max_mask, r);
        return _mm_or_si128(a, b);
#endif
    }

    inline UPALWAYSINLINE int4 neg(int4 const v) noexcept {
        int4 z = _mm_setzero_si128();
        return _mm_sub_epi32(z, v);
    }

    inline UPALWAYSINLINE int4 neg_sat(int4 const v) noexcept {
        int4 z = _mm_setzero_si128();
        int4 r = _mm_sub_epi32(z, v);
        int4 m = _mm_and_si128(v, r);
        m = _mm_srai_epi32(m, 31);
        int4 s = _mm_andnot_si128(r, m);
#ifdef UP_SIMD_SSE_4_1
        return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(r), _mm_castsi128_ps(s), _mm_castsi128_ps(m)));
#else
        s = _mm_and_si128(s, m);
        m = _mm_andnot_si128(m, r);
        return _mm_or_si128(s, m);
#endif
    }

    inline UPALWAYSINLINE int4 abs(int4 const v) noexcept {
#ifdef UP_SIMD_SSSE_3
        return _mm_abs_epi32(v);
#else
        int4 a = _mm_srai_epi32(v, 31);
        int4 b = _mm_xor_si128(v, a);
        return _mm_sub_epi32(b, a);
#endif
    }

    inline UPALWAYSINLINE int4 abs_sat(int4 const v) noexcept {
#ifdef UP_SIMD_SSSE_3
        int4 r = _mm_abs_epi32(v);
#else
        int4 a = _mm_srai_epi32(v, 31);
        int4 b = _mm_xor_si128(v, a);
        int4 r = _mm_sub_epi32(b, a);
#endif
        int4 m = _mm_srai_epi32(r, 31);
        int4 s = _mm_andnot_si128(r, m);
#ifdef UP_SIMD_SSE_4_1
        return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(r), _mm_castsi128_ps(s), _mm_castsi128_ps(m)));
#else
        s = _mm_and_si128(s, m);
        m = _mm_andnot_si128(m, r);
        return _mm_or_si128(s, m);
#endif
    }

    inline UPALWAYSINLINE int4 nabs(int4 const v) noexcept {
#ifdef UP_SIMD_SSSE_3
        int4 z = _mm_setzero_si128();
        int4 r = _mm_abs_epi32(v);
        return _mm_sub_epi32(z, r);
#else
        int4 a = _mm_srai_epi32(v, 31);
        int4 b = _mm_xor_si128(v, a);
        return _mm_sub_epi32(a, b);
#endif
    }

    inline UPALWAYSINLINE int4 nabs_sat(int4 const v) noexcept {
#ifdef UP_SIMD_SSSE_3
        int4 z = _mm_setzero_si128();
        int4 r = _mm_abs_epi32(v);
        return _mm_sub_epi32(z, r);
#else
        int4 a = _mm_srai_epi32(v, 31);
        int4 b = _mm_xor_si128(v, a);
        return _mm_sub_epi32(a, b);
#endif
    }

    inline UPALWAYSINLINE int4 abs_diff(int4 const v1, int4 const v2) noexcept {
        int4 m = _mm_cmpgt_epi32(v1, v2);
        int4 a = _mm_sub_epi32(v1, v2);
        int4 b = _mm_sub_epi32(v2, v1);
#ifdef UP_SIMD_SSE_4_1
        return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(b), _mm_castsi128_ps(a), _mm_castsi128_ps(m)));
#else
        a = _mm_and_si128(a, m);
        b = _mm_andnot_si128(m, b);
        return _mm_or_si128(a, b);
#endif
    }

    inline UPALWAYSINLINE int4 hadd(int4 const v1, int4 const v2) noexcept {
        int4 a = _mm_and_si128(v1, v2);
        int4 b = _mm_xor_si128(v1, v2);
        b = _mm_srai_epi32(b, 1);
        a = _mm_add_epi32(a, b);
        return a;
    }

    inline UPALWAYSINLINE int4 rhadd(int4 const v1, int4 const v2) noexcept {
        int4 a = _mm_or_si128(v1, v2);
        int4 b = _mm_xor_si128(v1, v2);
        b = _mm_srai_epi32(b, 1);
        a = _mm_sub_epi32(a, b);
        return a;
    }

    inline UPALWAYSINLINE int4 mul(int4 const v1, int4 const v2) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_mullo_epi32(v1, v2);
#else
        int4 a1, a2, b1, b2;

        // get absolute values of multiplicands
#   ifdef UP_SIMD_SSSE_3
        a1 = _mm_abs_epi32(v1);
        a2 = _mm_abs_epi32(v2);
#   else
        b1 = _mm_srai_epi32(v1, 31);
        a1 = _mm_xor_si128(v1, b1);
        a1 = _mm_sub_epi32(a1, b1);
        b2 = _mm_srai_epi32(v2, 31);
        a2 = _mm_xor_si128(v2, b2);
        a2 = _mm_sub_epi32(a2, b2);
#   endif

        // perform unsigned multiplication of absolute values
        b1 = _mm_srli_si128(a1, 4);
        b2 = _mm_srli_si128(a2, 4);
        a1 = _mm_mul_epu32(a1, a2);
        b1 = _mm_mul_epu32(b1, b2);
        a1 = _mm_shuffle_epi32(a1, _MM_SHUFFLE(2, 0, 2, 0));
        b1 = _mm_shuffle_epi32(b1, _MM_SHUFFLE(2, 0, 2, 0));
        a1 = _mm_unpacklo_epi32(a1, b1);

        // transfer sign of multiplicands to unsigned product
#   ifdef UP_SIMD_SSSE_3
        a2 = _mm_xor_si128(v1, v2);
        a2 = _mm_srai_epi32(a2, 31);
        a1 = _mm_abs_epi32(a1);
#   else
        a2 = _mm_xor_si128(v1, v2);
        a2 = _mm_xor_si128(a2, a1);
        a2 = _mm_srai_epi32(a2, 31);
#   endif
        a1 = _mm_add_epi32(a1, a2);
        a1 = _mm_xor_si128(a1, a2);
        return a1;
#endif
    }

    inline UPALWAYSINLINE int4 div(int4 const v1, int4 const v2) noexcept {
        int32_t x1, x2, y1, y2, z1, z2, w1, w2;
#ifdef UP_SIMD_SSE_4_1
        uint64_t xy1, xy2, zw1, zw2;
        xy1 = _mm_extract_epi64(v1, 0);
        xy2 = _mm_extract_epi64(v2, 0);
        x1 = static_cast<int32_t>(xy1 & 0xFFFFFFFF);
        x2 = static_cast<int32_t>(xy2 & 0xFFFFFFFF);
        y1 = static_cast<int32_t>((xy1 >> 32) & 0xFFFFFFFF);
        y2 = static_cast<int32_t>((xy2 >> 32) & 0xFFFFFFFF);
        zw1 = _mm_extract_epi64(v1, 1);
        zw2 = _mm_extract_epi64(v2, 1);
        z1 = static_cast<int32_t>(zw1 & 0xFFFFFFFF);
        z2 = static_cast<int32_t>(zw2 & 0xFFFFFFFF);
        w1 = static_cast<int32_t>((zw1 >> 32) & 0xFFFFFFFF);
        w2 = static_cast<int32_t>((zw2 >> 32) & 0xFFFFFFFF);
#else
        int4 t1, t2;
        x1 = _mm_cvtsi128_si32(v1);
        x2 = _mm_cvtsi128_si32(v2);
        t1 = _mm_shuffle_epi32(v1, _MM_SHUFFLE(1, 1, 1, 1));
        t2 = _mm_shuffle_epi32(v2, _MM_SHUFFLE(1, 1, 1, 1));
        y1 = _mm_cvtsi128_si32(t1);
        y2 = _mm_cvtsi128_si32(t2);
        t1 = _mm_shuffle_epi32(v1, _MM_SHUFFLE(2, 2, 2, 2));
        t2 = _mm_shuffle_epi32(v2, _MM_SHUFFLE(2, 2, 2, 2));
        z1 = _mm_cvtsi128_si32(t1);
        z2 = _mm_cvtsi128_si32(t2);
        t1 = _mm_shuffle_epi32(v1, _MM_SHUFFLE(3, 3, 3, 3));
        t2 = _mm_shuffle_epi32(v2, _MM_SHUFFLE(3, 3, 3, 3));
        w1 = _mm_cvtsi128_si32(t1);
        w2 = _mm_cvtsi128_si32(t2);
#endif
        return _mm_set_epi32(w1 / w2, z1 / z2, y1 / y2, x1 / x2);
    }

    //
    // Bitwise Functions
    //

    inline UPALWAYSINLINE int4 bitwisenot(int4 const v) noexcept {
        int4 t = _mm_cmpeq_epi32(v, v);
        return _mm_xor_si128(t, v);
    }

    inline UPALWAYSINLINE int4 bitwiseand(int4 const v1, int4 const v2) noexcept {
        return _mm_and_si128(v1, v2);
    }

    inline UPALWAYSINLINE int4 bitwiseor(int4 const v1, int4 const v2) noexcept {
        return _mm_or_si128(v1, v2);
    }

    inline UPALWAYSINLINE int4 bitwisexor(int4 const v1, int4 const v2) noexcept {
        return _mm_xor_si128(v1, v2);
    }

    inline UPALWAYSINLINE int4 sar(int4 const v, int32_t const i) noexcept {
        return _mm_srai_epi32(v, i);
    }

    inline UPALWAYSINLINE int4 sar(int4 const v, int4 const i) noexcept {
        int4 a = _mm_and_si128(i, uniform<int4>::mask_x);
        int4 b = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        a = _mm_sra_epi32(v, a);
        b = _mm_sra_epi32(v, b);
        c = _mm_sra_epi32(v, c);
        d = _mm_sra_epi32(v, d);
        a = _mm_unpacklo_epi32(a, b);
        c = _mm_unpackhi_epi32(c, d);
        a = _mm_shuffle_epi32(a, _MM_SHUFFLE(3, 0, 3, 0));
        c = _mm_shuffle_epi32(c, _MM_SHUFFLE(3, 0, 3, 0));
        return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(c), _MM_SHUFFLE(3, 2, 1, 0)));
    }

    inline UPALWAYSINLINE int4 shl(int4 const v, int32_t const i) noexcept {
        return _mm_slli_epi32(v, i);
    }

    inline UPALWAYSINLINE int4 shl(int4 const v, int4 const i) noexcept {
        int4 a = _mm_and_si128(i, uniform<int4>::mask_x);
        int4 b = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        a = _mm_sll_epi32(v, a);
        b = _mm_sll_epi32(v, b);
        c = _mm_sll_epi32(v, c);
        d = _mm_sll_epi32(v, d);
        a = _mm_unpacklo_epi32(a, b);
        c = _mm_unpackhi_epi32(c, d);
        a = _mm_shuffle_epi32(a, _MM_SHUFFLE(3, 0, 3, 0));
        c = _mm_shuffle_epi32(c, _MM_SHUFFLE(3, 0, 3, 0));
        return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(c), _MM_SHUFFLE(3, 2, 1, 0)));
    }

    inline UPALWAYSINLINE int4 shr(int4 const v, int32_t const i) noexcept {
        return _mm_srli_epi32(v, i);
    }

    inline UPALWAYSINLINE int4 shr(int4 const v, int4 const i) noexcept {
        int4 a = _mm_and_si128(i, uniform<int4>::mask_x);
        int4 b = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        a = _mm_srl_epi32(v, a);
        b = _mm_srl_epi32(v, b);
        c = _mm_srl_epi32(v, c);
        d = _mm_srl_epi32(v, d);
        a = _mm_unpacklo_epi32(a, b);
        c = _mm_unpackhi_epi32(c, d);
        a = _mm_shuffle_epi32(a, _MM_SHUFFLE(3, 0, 3, 0));
        c = _mm_shuffle_epi32(c, _MM_SHUFFLE(3, 0, 3, 0));
        return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(c), _MM_SHUFFLE(3, 2, 1, 0)));
    }

    inline UPALWAYSINLINE int4 rotate(int4 const v, int32_t const i) noexcept {
        int4 a = _mm_slli_epi32(v, i);
        int4 b = _mm_srli_epi32(v, 32 - i);
        return _mm_or_si128(a, b);
    }

    inline UPALWAYSINLINE int4 rotate(int4 const v, int4 const i) noexcept {
        int4 a1 = _mm_and_si128(i, uniform<int4>::mask_x);
        int4 b1 = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c1 = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d1 = _mm_and_si128(_mm_shuffle_epi32(i, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        int4 a2 = _mm_sub_epi32(uniform<int4>::rotate_max, a1);
        int4 b2 = _mm_sub_epi32(uniform<int4>::rotate_max, b1);
        int4 c2 = _mm_sub_epi32(uniform<int4>::rotate_max, c1);
        int4 d2 = _mm_sub_epi32(uniform<int4>::rotate_max, d1);
        a1 = _mm_sll_epi32(v, a1);
        b1 = _mm_sll_epi32(v, b1);
        c1 = _mm_sll_epi32(v, c1);
        d1 = _mm_sll_epi32(v, d1);
        a2 = _mm_srl_epi32(v, a2);
        b2 = _mm_srl_epi32(v, b2);
        c2 = _mm_srl_epi32(v, c2);
        d2 = _mm_srl_epi32(v, d2);
        a1 = _mm_or_si128(a1, a2);
        b1 = _mm_or_si128(b1, b2);
        c1 = _mm_or_si128(c1, c2);
        d1 = _mm_or_si128(d1, d2);
        a1 = _mm_unpacklo_epi32(a1, b1);
        c1 = _mm_unpackhi_epi32(c1, d1);
        a1 = _mm_shuffle_epi32(a1, _MM_SHUFFLE(3, 0, 3, 0));
        c1 = _mm_shuffle_epi32(c1, _MM_SHUFFLE(3, 0, 3, 0));
        return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a1), _mm_castsi128_ps(c1), _MM_SHUFFLE(3, 2, 1, 0)));
    }

    //
    // Common Functions
    //

    inline UPALWAYSINLINE int4 clamp(int4 const v, int32_t const min_sval, int32_t const max_sval) noexcept {
        int4 min_val = _mm_set1_epi32(min_sval);
        int4 max_val = _mm_set1_epi32(max_sval);
#ifdef UP_SIMD_SSE_4_1
        int4 r = _mm_max_epi32(v, min_val);
        return _mm_min_epi32(r, max_val);
#else
        int4 m = _mm_cmpgt_epi32(v, min_val);
        int4 r1 = _mm_and_si128(v, m);
        int4 r2 = _mm_andnot_si128(m, min_val);
        int4 r = _mm_or_si128(r1, r2);
        m = _mm_cmpgt_epi32(r, max_val);
        r1 = _mm_and_si128(max_val, m);
        r2 = _mm_andnot_si128(m, r);
        return _mm_or_si128(r1, r2);
#endif
    }

    inline UPALWAYSINLINE int4 clamp(int4 const v, int4 const min_val, int4 const max_val) noexcept {
#ifdef UP_SIMD_SSE_4_1
        int4 r = _mm_max_epi32(v, min_val);
        return _mm_min_epi32(r, max_val);
#else
        int4 m = _mm_cmpgt_epi32(v, min_val);
        int4 r1 = _mm_and_si128(v, m);
        int4 r2 = _mm_andnot_si128(m, min_val);
        int4 r = _mm_or_si128(r1, r2);
        m = _mm_cmpgt_epi32(r, max_val);
        r1 = _mm_and_si128(max_val, m);
        r2 = _mm_andnot_si128(m, r);
        return _mm_or_si128(r1, r2);
#endif
    }

    inline UPALWAYSINLINE int4 max(int4 const v, int32_t const s) noexcept {
        int4 v2 = _mm_set1_epi32(s);
#ifdef UP_SIMD_SSE_4_1
        return _mm_max_epi32(v, v2);
#else
        int4 m = _mm_cmpgt_epi32(v, v2);
        int4 r1 = _mm_and_si128(v, m);
        int4 r2 = _mm_andnot_si128(m, v2);
        return _mm_or_si128(r1, r2);
#endif
    }

    inline UPALWAYSINLINE int4 max(int4 const v1, int4 const v2) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_max_epi32(v1, v2);
#else
        int4 m = _mm_cmpgt_epi32(v1, v2);
        int4 r1 = _mm_and_si128(v1, m);
        int4 r2 = _mm_andnot_si128(m, v2);
        return _mm_or_si128(r1, r2);
#endif
    }

    inline UPALWAYSINLINE int4 min(int4 const v, int32_t const s) noexcept {
        int4 v2 = _mm_set1_epi32(s);
#ifdef UP_SIMD_SSE_4_1
        return _mm_min_epi32(v, v2);
#else
        int4 m = _mm_cmpgt_epi32(v, v2);
        int4 r1 = _mm_and_si128(v2, m);
        int4 r2 = _mm_andnot_si128(m, v);
        return _mm_or_si128(r1, r2);
#endif
    }

    inline UPALWAYSINLINE int4 min(int4 const v1, int4 const v2) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_min_epi32(v1, v2);
#else
        int4 m = _mm_cmpgt_epi32(v1, v2);
        int4 r1 = _mm_and_si128(v2, m);
        int4 r2 = _mm_andnot_si128(m, v1);
        return _mm_or_si128(r1, r2);
#endif
    }
}}

#endif
