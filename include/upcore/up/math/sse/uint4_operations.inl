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

#ifndef UP_MATH_SSE_UINT4_OPERATIONS_INL
#define UP_MATH_SSE_UINT4_OPERATIONS_INL

namespace up { namespace math { namespace detail
{
    template <>
    struct constructors<uint4>
    {
        typedef uint32_t element_type;
        typedef uint32_t const* element_pointer;
        typedef uint2 const vector2_type;
        typedef uint3 const vector3_type;
        typedef uint4 const vector4_type;
        typedef uint2 const* vector2_pointer;
        typedef uint3 const* vector3_pointer;
        typedef uint4 const* vector4_pointer;

        //
        // Initialization Functions
        //

        static UPALWAYSINLINE uint4 broadcast(uint32_t x) noexcept {
            uint4 r = { _mm_set1_epi32(x) }; return r;
        }

        static UPALWAYSINLINE uint4 broadcast2(uint2 const v)  noexcept {
            uint4 r = { _mm_set_epi32(v.y, v.x, v.y, v.x) }; return r;
        }

        static UPALWAYSINLINE uint4 make(uint32_t x) noexcept {
            union { int32_t s; float f; } u = { x };
            uint4 r = { _mm_castps_si128(_mm_set_ss(u.f)) }; return r;
        }

        static UPALWAYSINLINE uint4 make(uint32_t x, uint32_t y) noexcept {
            uint4 r = { _mm_set_epi32(0, 0, y, x) }; return r;
        }

        static UPALWAYSINLINE uint4 make(uint32_t x, uint32_t y, uint32_t z) noexcept {
            uint4 r = { _mm_set_epi32(0, z, y, x) }; return r;
        }

        static UPALWAYSINLINE uint4 make(uint32_t x, uint32_t y, uint32_t z, uint32_t w) noexcept {
            uint4 r = { _mm_set_epi32(w, z, y, x) }; return r;
        }

        static UPALWAYSINLINE uint4 zero() noexcept {
            uint4 r = { _mm_setzero_si128() }; return r;
        }

        //
        // Load Functions
        //

        static UPALWAYSINLINE uint4 loadbroadcast(uint32_t const* p) noexcept {
#ifdef UP_SIMD_AVX
            uint4 r = { _mm_castps_si128(_mm_broadcast_ss(reinterpret_cast<float const*>(p))) }; return r;
#else
            uint4 r = { _mm_castps_si128(_mm_load_ps1(reinterpret_cast<float const*>(p))) }; return r;
#endif
        }

        static UPALWAYSINLINE uint4 loadbroadcast(size_t offset, uint32_t const* p) noexcept {
#ifdef UP_SIMD_AVX
            uint4 r = { _mm_castps_si128(_mm_broadcast_ss(reinterpret_cast<float const*>(p + offset))) }; return r;
#else
            uint4 r = { _mm_castps_si128(_mm_load_ps1(reinterpret_cast<float const*>(p + offset))) }; return r;
#endif
        }

        static UPALWAYSINLINE uint4 loadbroadcast2(uint2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->y)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            uint4 r = { _mm_unpacklo_epi64(xy, xy) };
            return r;
        }

        static UPALWAYSINLINE uint4 loadbroadcast2(size_t offset, uint2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->y)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            uint4 r = { _mm_unpacklo_epi64(xy, xy) };
            return r;
        }

        static UPALWAYSINLINE uint4 loadscalar(uint32_t const* p) noexcept {
            uint4 r = { _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(p))) }; return r;
        }

        static UPALWAYSINLINE uint4 loadscalar(size_t offset, uint32_t const* p) noexcept {
            uint4 r = { _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(p + offset))) }; return r;
        }

        static UPALWAYSINLINE uint4 load2(uint2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->y)));
            uint4 r = { _mm_unpacklo_epi32(x, y) };
            return r;
        }

        static UPALWAYSINLINE uint4 load2(size_t offset, uint2 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->y)));
            uint4 r = { _mm_unpacklo_epi32(x, y) };
            return r;
        }

        static UPALWAYSINLINE uint4 load3(uint3 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->y)));
            int4 z = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&p->z)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            uint4 r = { _mm_unpacklo_epi64(xy, z) };
            return r;
        }

        static UPALWAYSINLINE uint4 load3(size_t offset, uint3 const* p) noexcept {
            int4 x = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->x)));
            int4 y = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->y)));
            int4 z = _mm_castps_si128(_mm_load_ss(reinterpret_cast<float const*>(&(p + offset)->z)));
            int4 xy = _mm_unpacklo_epi32(x, y);
            uint4 r = { _mm_unpacklo_epi64(xy, z) };
            return r;
        }

        static UPALWAYSINLINE uint4 load4(uint4 const* p) noexcept {
            uint4 r = { _mm_loadu_si128(reinterpret_cast<int4 const*>(p)) }; return r;
        }

        static UPALWAYSINLINE uint4 load4(size_t offset, uint4 const* p) noexcept {
            uint4 r = { _mm_loadu_si128(reinterpret_cast<int4 const*>(p + offset)) }; return r;
        }

        static UPALWAYSINLINE uint4 loada2(uint2 const* p) noexcept {
            uint4 r = { _mm_loadl_epi64(reinterpret_cast<int4 const*>(p)) }; return r;
        }

        static UPALWAYSINLINE uint4 loada2(size_t offset, uint2 const* p) noexcept {
            uint4 r = { _mm_loadl_epi64(reinterpret_cast<int4 const*>(p + offset)) }; return r;
        }
 
        static UPALWAYSINLINE uint4 loada3(uint3 const* p) noexcept {
            uint4 r = { _mm_load_si128(reinterpret_cast<int4 const*>(p)) }; return r; // ignore extra element loaded
        }

        static UPALWAYSINLINE uint4 loada3(size_t offset, uint3 const* p) noexcept {
            uint4 r = { _mm_load_si128(reinterpret_cast<int4 const*>(p + offset)) }; return r; // ignore extra element loaded
        }

        static UPALWAYSINLINE uint4 loada4(uint4 const* p) noexcept {
            uint4 r = { _mm_load_si128(reinterpret_cast<int4 const*>(p)) }; return r;
        }

        static UPALWAYSINLINE uint4 loada4(size_t offset, uint4 const* p) noexcept {
            uint4 r = { _mm_load_si128(reinterpret_cast<int4 const*>(p + offset)) }; return r;
        }
    };
}}}

namespace up { namespace math
{
    //
    // Load Functions
    //

    inline UPALWAYSINLINE uint4 load4(uint4 const* p) noexcept {
        uint4 r = { _mm_loadu_si128(reinterpret_cast<int4 const*>(p)) }; return r;
    }

    inline UPALWAYSINLINE uint4 load4(size_t offset, uint4 const* p) noexcept {
        uint4 r = { _mm_loadu_si128(reinterpret_cast<int4 const*>(p + offset)) }; return r;
    }

    inline UPALWAYSINLINE uint4 loada4(uint4 const* p) noexcept {
        uint4 r = { _mm_load_si128(reinterpret_cast<int4 const*>(p)) }; return r;
    }

    inline UPALWAYSINLINE uint4 loada4(size_t offset, uint4 const* p) noexcept {
        uint4 r = { _mm_load_si128(reinterpret_cast<int4 const*>(p + offset)) }; return r;
    }

    //
    // Store Functions
    //

    inline UPALWAYSINLINE void storescalar(uint4_cval_t v, uint32_t* p) noexcept {
        _mm_store_ps1(reinterpret_cast<float*>(p), _mm_castsi128_ps(v.m));
    }

    inline UPALWAYSINLINE void storescalar(uint4_cval_t v, size_t offset, uint32_t* p) noexcept {
        _mm_store_ps1(reinterpret_cast<float*>(p + offset), _mm_castsi128_ps(v.m));
    }

    inline UPALWAYSINLINE void store2(uint4_cval_t v, uint2* p) noexcept {
        float4 w = _mm_castsi128_ps(v.m);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        _mm_store_ss(reinterpret_cast<float*>(&p->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&p->y), u);
    }

    inline UPALWAYSINLINE void store2(uint4_cval_t v, size_t offset, uint2* p) noexcept {
        float4 w = _mm_castsi128_ps(v.m);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->y), u); 
    }

    inline UPALWAYSINLINE void store3(uint4_cval_t v, uint3* p) noexcept {
        float4 w = _mm_castsi128_ps(v.m);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        float4 t = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(&p->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&p->y), u);
        _mm_store_ss(reinterpret_cast<float*>(&p->z), t);
    }

    inline UPALWAYSINLINE void store3(uint4_cval_t v, size_t offset, uint3* p) noexcept {
        float4 w = _mm_castsi128_ps(v.m);
        float4 u = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
        float4 t = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->x), w);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->y), u);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->z), t);
    }
 
    inline UPALWAYSINLINE void store4(uint4_cval_t v, uint4* p) noexcept {
        _mm_storeu_si128(reinterpret_cast<int4*>(p), v.m);
    }

    inline UPALWAYSINLINE void store4(uint4_cval_t v, size_t offset, uint4* p) noexcept {
        _mm_storeu_si128(reinterpret_cast<int4*>(p + offset), v.m);
    }
    
    inline UPALWAYSINLINE void storea2(uint4_cval_t v, uint2* p) noexcept {
        _mm_storel_epi64(reinterpret_cast<int4*>(p), v.m);
    }

    inline UPALWAYSINLINE void storea2(uint4_cval_t v, size_t offset, uint2* p) noexcept {
        _mm_storel_epi64(reinterpret_cast<int4*>(p + offset), v.m);
    }

    inline UPALWAYSINLINE void storea3(uint4_cval_t v, uint3* p) noexcept {
        int4 t = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_storel_epi64(reinterpret_cast<int4*>(p), v.m);
        _mm_store_ss(reinterpret_cast<float*>(&p->z), _mm_castsi128_ps(t));
    }

    inline UPALWAYSINLINE void storea3(uint4_cval_t v, size_t offset, uint3* p) noexcept {
        int4 t = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_storel_epi64(reinterpret_cast<int4*>(p + offset), v.m);
        _mm_store_ss(reinterpret_cast<float*>(&(p + offset)->z), _mm_castsi128_ps(t));
    }

    inline UPALWAYSINLINE void storea4(uint4_cval_t v, uint4* p) noexcept {
        _mm_store_si128(reinterpret_cast<int4*>(p), v.m);
    }

    inline UPALWAYSINLINE void storea4(uint4_cval_t v, size_t offset, int4* p) noexcept {
        _mm_store_si128(reinterpret_cast<int4*>(p + offset), v.m);
    }

    inline UPALWAYSINLINE void streama4(uint4_cval_t v, uint4* p) noexcept {
        _mm_stream_si128(reinterpret_cast<int4*>(p), v.m);
    }

    inline UPALWAYSINLINE void streama4(uint4_cval_t v, size_t offset, uint4* p) noexcept {
        _mm_stream_si128(reinterpret_cast<int4*>(p + offset), v.m);
    }

    //
    // Accessor Functions
    //

    inline UPALWAYSINLINE uint32_t get(uint4_cval_t v, size_t n) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint32_t r;
        switch (n) {
        case 0: r = _mm_extract_epi32(v.m, 0); break;
        case 1: r = _mm_extract_epi32(v.m, 1); break;
        case 2: r = _mm_extract_epi32(v.m, 2); break;
        case 3: r = _mm_extract_epi32(v.m, 3); break;
        default: r = 0; break;
        }
        return r;
#else
        int4 r;
        switch (n) {
        case 0: r = v.m; break;
        case 1: r = _mm_srli_si128(v.m, 4); break;
        case 2: r = _mm_srli_si128(v.m, 8); break;
        case 3: r = _mm_srli_si128(v.m, 12); break;
        default: r = _mm_setzero_si128(); break;
        }
        return _mm_cvtsi128_si32(r);
#endif
    }

    inline UPALWAYSINLINE uint32_t getx(uint4_cval_t v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v.m, 0);
#else
        return _mm_cvtsi128_si32(v.m);
#endif
    }

    inline UPALWAYSINLINE uint32_t gety(uint4_cval_t v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v.m, 1);
#else
        int4 r = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm_cvtsi128_si32(r);
#endif
    }

    inline UPALWAYSINLINE uint32_t getz(uint4_cval_t v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v.m, 2);
#else
        int4 r = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 2, 2, 2));
        return _mm_cvtsi128_si32(r);       
#endif
    }

    inline UPALWAYSINLINE uint32_t getw(uint4_cval_t v) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        return _mm_extract_epi32(v.m, 3);
#else
        int4 r = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(3, 3, 3, 3));
        return _mm_cvtsi128_si32(r);
#endif
    }

    inline UPALWAYSINLINE uint4 put(uint4_cval_t v, uint32_t value, size_t n) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r;
        switch (n) {
        case 0: r.m = _mm_insert_epi32(v.m, value, 0); break;
        case 1: r.m = _mm_insert_epi32(v.m, value, 1); break;
        case 2: r.m = _mm_insert_epi32(v.m, value, 2); break;
        case 3: r.m = _mm_insert_epi32(v.m, value, 3); break;
        default: r = uniform<uint4>::zero;
        }
        return r;
#else
        if (n > 3) {
            return uniform<uint4>::zero;
        }
        uniform_constant<uint4, uint32_t> r;
        r.v = v;
        r.s[n] = value;
        return r.v;
#endif
    }

    inline UPALWAYSINLINE uint4 putx(uint4_cval_t v, uint32_t x) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r = { _mm_insert_epi32(v.m, x, 0) }; return r;
#else
        union { int32_t i; float f; } c = { x };
        float4 t = _mm_set_ss(c.f);
        t = _mm_move_ss(_mm_castsi128_ps(v.m), t);
        uint4 u = { _mm_castps_si128(t) };
        return u;
#endif
    }

    inline UPALWAYSINLINE uint4 puty(uint4_cval_t v, uint32_t y) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r = { _mm_insert_epi32(v.m, y, 1) }; return r;
#else
        union { int32_t i; float f; } c = { y };
        float4 t = _mm_set_ss(c.f);
        float4 r = _mm_castsi128_ps(v.m);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 2, 0, 1));
        r = _mm_move_ss(r, t);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 2, 0, 1));
        uint4 u = { _mm_castps_si128(r) };
        return u;
#endif
    }

    inline UPALWAYSINLINE uint4 putz(uint4_cval_t v, uint32_t z) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r = { _mm_insert_epi32(v.m, z, 2) }; return r;
#else
        union { int32_t i; float f; } c = { z };
        float4 t = _mm_set_ss(c.f);
        float4 r = _mm_castsi128_ps(v.m);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 0, 1, 2));
        r = _mm_move_ss(r, t);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 0, 1, 2));
        uint4 u = { _mm_castps_si128(r) };
        return u;
#endif
    }

    inline UPALWAYSINLINE uint4 putw(uint4_cval_t v, uint32_t w) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r = { _mm_insert_epi32(v.m, w, 3) }; return r;
#else
        union { int32_t i; float f; } c = { w };
        float4 t = _mm_set_ss(c.f);
        float4 r = _mm_castsi128_ps(v.m);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 2, 1, 3));
        r = _mm_move_ss(r, t);
        r = _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 2, 1, 3));
        uint4 u = { _mm_castps_si128(r) };
        return u;
#endif
    }

    //
    // Permutation Functions
    //

    inline UPALWAYSINLINE uint4 packxy(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_unpacklo_epi64(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 packzw(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_unpackhi_epi64(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 unpackxy(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_unpacklo_epi32(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 unpackzw(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_unpackhi_epi32(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 reverse(uint4_cval1_t v) noexcept {
        uint4 r = { _mm_shuffle_epi32(v.m, _MM_SHUFFLE(0, 1, 2, 3)) }; return r;
    }

    inline UPALWAYSINLINE uint4 shuffle_xxyy(uint4_cval_t v) noexcept {
        uint4 r = { _mm_unpacklo_epi32(v.m, v.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 shuffle_zzww(uint4_cval_t v) noexcept {
        uint4 r = { _mm_unpackhi_epi32(v.m, v.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 shuffle_xyxy(uint4_cval_t v) noexcept {
        uint4 r = { _mm_unpacklo_epi64(v.m, v.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 shuffle_zwzw(uint4_cval_t v) noexcept {
        uint4 r = { _mm_unpackhi_epi64(v.m, v.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 splat(uint4_cval_t v, size_t n) noexcept {
        uint4 r;
        switch (n) {
        case 0: r.m = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(0, 0, 0, 0)); break;
        case 1: r.m = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(1, 1, 1, 1)); break;
        case 2: r.m = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 2, 2, 2)); break;
        case 3: r.m = _mm_shuffle_epi32(v.m, _MM_SHUFFLE(3, 3, 3, 3)); break;
        default: r.m = _mm_setzero_si128(); break;
        }
        return r;
    }

    inline UPALWAYSINLINE uint4 splatx(uint4_cval_t v) noexcept {
        uint4 r = { _mm_shuffle_epi32(v.m, _MM_SHUFFLE(0, 0, 0, 0)) }; return r;
    }

    inline UPALWAYSINLINE uint4 splaty(uint4_cval_t v) noexcept {
        uint4 r = { _mm_shuffle_epi32(v.m, _MM_SHUFFLE(1, 1, 1, 1)) }; return r;
    }

    inline UPALWAYSINLINE uint4 splatz(uint4_cval_t v) noexcept {
        uint4 r = { _mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 2, 2, 2)) }; return r;
    }

    inline UPALWAYSINLINE uint4 splatw(uint4_cval_t v) noexcept {
        uint4 r = { _mm_shuffle_epi32(v.m, _MM_SHUFFLE(3, 3, 3, 3)) }; return r;
    }

    //
    // Relational Functions
    //

    inline UPALWAYSINLINE uint4 isequal(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_cmpeq_epi32(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 isnotequal(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 t = _mm_cmpeq_epi32(v1.m, v2.m);
        uint4 r = { _mm_andnot_si128(t, as<int4>(uniform<uint4>::mask_xyzw)) };
        return r;
    }

    inline UPALWAYSINLINE uint4 isgreater(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 m = _mm_cmpgt_epi32(v1.m, v2.m);
        int4 t1 = _mm_srai_epi32(v1.m, 31);
        int4 t2 = _mm_srai_epi32(v2.m, 31);
        int4 s = _mm_xor_si128(t1, t2);
        t2 = _mm_andnot_si128(t2, t1);
        s = _mm_andnot_si128(s, m);
        uint4 r = { _mm_or_si128(t2, s) };
        return r;
    }

    inline UPALWAYSINLINE uint4 isgreaterequal(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = isgreater(v2, v1);
        r.m = _mm_andnot_si128(r.m, as<int4>(uniform<uint4>::mask_xyzw));
        return r;
    }

    inline UPALWAYSINLINE uint4 isless(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        return isgreater(v2, v1);
    }

    inline UPALWAYSINLINE uint4 islessequal(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = isgreater(v1, v2);
        r.m = _mm_andnot_si128(r.m, as<int4>(uniform<uint4>::mask_xyzw));
        return r;
    }

    inline UPALWAYSINLINE bool all(uint4_cval_t v) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_testc_si128(v.m, uniform<int4>::min) == 1;
#else
        return _mm_movemask_ps(_mm_castsi128_ps(v.m)) == 0x0F;
#endif
    }

    inline UPALWAYSINLINE bool all(uint4_cval_t v, int const mask) noexcept {
        return (_mm_movemask_ps(_mm_castsi128_ps(v.m)) & mask) == mask;
    }

    inline UPALWAYSINLINE bool all(uint4_cval1_t v, uint4_cval2_t mask) noexcept {
#ifdef UP_SIMD_SSE_4_1
        return _mm_testc_si128(v.m, mask.m) == 1;
#else
        return _mm_movemask_ps(_mm_castsi128_ps(_mm_andnot_si128(v.m, mask.m))) == 0x00;
#endif
    }

    inline UPALWAYSINLINE bool any(uint4_cval_t v) noexcept {
        return _mm_movemask_ps(_mm_castsi128_ps(v.m)) != 0x00;
    }

    inline UPALWAYSINLINE uint4 bitselect(uint4_cval1_t v1, uint4_cval2_t v2, uint4_cval3_t v3) noexcept {
        int4 r1 = _mm_and_si128(v2.m, v3.m);
        int4 r2 = _mm_andnot_si128(v3.m, v1.m);
        uint4 r = { _mm_or_si128(r1, r2) };
        return r;
    }

    inline UPALWAYSINLINE uint4 select(uint4_cval1_t v1, uint4_cval2_t v2, int4 const v3) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r = { _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(v1.m), _mm_castsi128_ps(v2.m), _mm_castsi128_ps(v3))) };
#else
        int4 m = _mm_srai_epi32(v3, 31);
        int4 r1 = _mm_and_si128(v2.m, m);
        int4 r2 = _mm_andnot_si128(m, v1.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 select(uint4_cval1_t v1, uint4_cval2_t v2, uint4_cval3_t v3) noexcept {
#if defined(UP_SIMD_SSE_4_1)
        uint4 r = { _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(v1.m), _mm_castsi128_ps(v2.m), _mm_castsi128_ps(v3.m))) };
#else
        int4 m = _mm_srai_epi32(v3.m, 31);
        int4 r1 = _mm_and_si128(v2.m, m);
        int4 r2 = _mm_andnot_si128(m, v1.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }

    //
    // Arithmetic Functions
    //

    inline UPALWAYSINLINE uint4 add(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_add_epi32(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 add_sat(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 sum = _mm_add_epi32(v1.m, v2.m);
        int4 mask = _mm_or_si128(v1.m, v2.m);
        mask = _mm_andnot_si128(sum, mask);
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(sum), as<float4>(uniform<uint4>::max), _mm_castsi128_ps(mask))) };
#else
        mask = _mm_srai_epi32(mask, 31);
        int4 a = _mm_and_si128(as<int4>(uniform<uint4>::max), mask);
        int4 b = _mm_andnot_si128(mask, sum);
        uint4 r = { _mm_or_si128(a, b) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 sub(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_sub_epi32(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 sub_sat(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 sum = _mm_sub_epi32(v1.m, v2.m);
        int4 mask = _mm_or_si128(v1.m, v2.m);
        mask = _mm_andnot_si128(sum, mask);
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(sum), as<float4>(uniform<int4>::min), _mm_castsi128_ps(mask))) };
#else
        mask = _mm_srai_epi32(mask, 31);
        int4 a = _mm_and_si128(uniform<int4>::min, mask);
        int4 b = _mm_andnot_si128(mask, sum);
        uint4 r = { _mm_or_si128(a, b) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 abs_diff(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 m = isgreater(v1, v2);
        int4 a = _mm_sub_epi32(v1.m, v2.m);
        int4 b = _mm_sub_epi32(v2.m, v1.m);
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(b), _mm_castsi128_ps(a), _mm_castsi128_ps(m.m))) };
#else
        a = _mm_and_si128(a, m.m);
        b = _mm_andnot_si128(m.m, b);
        uint4 r = { _mm_or_si128(a, b) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 hadd(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 a = _mm_and_si128(v1.m, v2.m);
        int4 b = _mm_xor_si128(v1.m, v2.m);
        b = _mm_srli_epi32(b, 1);
        uint4 r = { _mm_add_epi32(a, b) };
        return r;
    }

    inline UPALWAYSINLINE uint4 rhadd(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 a = _mm_or_si128(v1.m, v2.m);
        int4 b = _mm_xor_si128(v1.m, v2.m);
        b = _mm_srli_epi32(b, 1);
        uint4 r = { _mm_sub_epi32(a, b) };
        return r;
    }

    inline UPALWAYSINLINE uint4 mul(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        int4 u1 = _mm_srli_si128(v1.m, 4);
        int4 u2 = _mm_srli_si128(v2.m, 4);
        int4 lo = _mm_mul_epu32(v1.m, v2.m);
        int4 hi = _mm_mul_epu32(u1, u2);
        lo = _mm_shuffle_epi32(lo, _MM_SHUFFLE(2, 0, 2, 0));
        hi = _mm_shuffle_epi32(hi, _MM_SHUFFLE(2, 0, 2, 0));
        uint4 r = { _mm_unpacklo_epi32(lo, hi) };
        return r;
    }

    inline UPALWAYSINLINE uint4 div(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint32_t x1, x2, y1, y2, z1, z2, w1, w2;
#ifdef UP_SIMD_SSE_4_1
        uint64_t xy1, xy2, zw1, zw2;
        xy1 = _mm_extract_epi64(v1.m, 0);
        xy2 = _mm_extract_epi64(v2.m, 0);
        x1 = static_cast<uint32_t>(xy1 & 0xFFFFFFFF);
        x2 = static_cast<uint32_t>(xy2 & 0xFFFFFFFF);
        y1 = static_cast<uint32_t>((xy1 >> 32) & 0xFFFFFFFF);
        y2 = static_cast<uint32_t>((xy2 >> 32) & 0xFFFFFFFF);
        zw1 = _mm_extract_epi64(v1.m, 1);
        zw2 = _mm_extract_epi64(v2.m, 1);
        z1 = static_cast<uint32_t>(zw1 & 0xFFFFFFFF);
        z2 = static_cast<uint32_t>(zw2 & 0xFFFFFFFF);
        w1 = static_cast<uint32_t>((zw1 >> 32) & 0xFFFFFFFF);
        w2 = static_cast<uint32_t>((zw2 >> 32) & 0xFFFFFFFF);
#else
        int4 t1, t2;
        x1 = _mm_cvtsi128_si32(v1.m);
        x2 = _mm_cvtsi128_si32(v2.m);
        t1 = _mm_shuffle_epi32(v1.m, _MM_SHUFFLE(1, 1, 1, 1));
        t2 = _mm_shuffle_epi32(v2.m, _MM_SHUFFLE(1, 1, 1, 1));
        y1 = _mm_cvtsi128_si32(t1);
        y2 = _mm_cvtsi128_si32(t2);
        t1 = _mm_shuffle_epi32(v1.m, _MM_SHUFFLE(2, 2, 2, 2));
        t2 = _mm_shuffle_epi32(v2.m, _MM_SHUFFLE(2, 2, 2, 2));
        z1 = _mm_cvtsi128_si32(t1);
        z2 = _mm_cvtsi128_si32(t2);
        t1 = _mm_shuffle_epi32(v1.m, _MM_SHUFFLE(3, 3, 3, 3));
        t2 = _mm_shuffle_epi32(v2.m, _MM_SHUFFLE(3, 3, 3, 3));
        w1 = _mm_cvtsi128_si32(t1);
        w2 = _mm_cvtsi128_si32(t2);
#endif
        uint4 r = { _mm_set_epi32(w1 / w2, z1 / z2, y1 / y2, x1 / x2) };
        return r;
    }

    //
    // Bitwise Functions
    //

    inline UPALWAYSINLINE uint4 bitwisenot(uint4_cval_t v) noexcept {
        int4 t = _mm_cmpeq_epi32(v.m, v.m);
        uint4 r = { _mm_xor_si128(t, v.m) };
        return r;
    }

    inline UPALWAYSINLINE uint4 bitwiseand(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_and_si128(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 bitwiseor(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_or_si128(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 bitwisexor(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
        uint4 r = { _mm_xor_si128(v1.m, v2.m) }; return r;
    }

    inline UPALWAYSINLINE uint4 shl(uint4_cval_t v, uint32_t const i) noexcept {
        uint4 r = { _mm_slli_epi32(v.m, i) }; return r;
    }

    inline UPALWAYSINLINE uint4 shl(uint4_cval1_t v, uint4_cval2_t i) noexcept {
        int4 a = _mm_and_si128(i.m, uniform<int4>::mask_x);
        int4 b = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        a = _mm_sll_epi32(v.m, a);
        b = _mm_sll_epi32(v.m, b);
        c = _mm_sll_epi32(v.m, c);
        d = _mm_sll_epi32(v.m, d);
        a = _mm_unpacklo_epi32(a, b);
        c = _mm_unpackhi_epi32(c, d);
        a = _mm_shuffle_epi32(a, _MM_SHUFFLE(3, 0, 3, 0));
        c = _mm_shuffle_epi32(c, _MM_SHUFFLE(3, 0, 3, 0));
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(c), _MM_SHUFFLE(3, 2, 1, 0))) };
        return r;
    }

    inline UPALWAYSINLINE uint4 shr(uint4_cval_t v, uint32_t const i) noexcept {
        uint4 r = { _mm_srli_epi32(v.m, i) }; return r;
    }

    inline UPALWAYSINLINE uint4 shr(uint4_cval1_t v, uint4_cval2_t i) noexcept {
        int4 a = _mm_and_si128(i.m, uniform<int4>::mask_x);
        int4 b = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        a = _mm_srl_epi32(v.m, a);
        b = _mm_srl_epi32(v.m, b);
        c = _mm_srl_epi32(v.m, c);
        d = _mm_srl_epi32(v.m, d);
        a = _mm_unpacklo_epi32(a, b);
        c = _mm_unpackhi_epi32(c, d);
        a = _mm_shuffle_epi32(a, _MM_SHUFFLE(3, 0, 3, 0));
        c = _mm_shuffle_epi32(c, _MM_SHUFFLE(3, 0, 3, 0));
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(c), _MM_SHUFFLE(3, 2, 1, 0))) };
        return r;
    }

    inline UPALWAYSINLINE uint4 rotate(uint4_cval_t v, uint32_t const i) noexcept {
        int4 a = _mm_slli_epi32(v.m, i);
        int4 b = _mm_srli_epi32(v.m, 32 - i);
        uint4 r = { _mm_or_si128(a, b) };
        return r;
    }

    inline UPALWAYSINLINE uint4 rotate(uint4_cval1_t v, uint4_cval2_t i) noexcept {
        int4 a1 = _mm_and_si128(i.m, uniform<int4>::mask_x);
        int4 b1 = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(1, 1, 1, 1)), uniform<int4>::mask_x);
        int4 c1 = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(2, 2, 2, 2)), uniform<int4>::mask_x);
        int4 d1 = _mm_and_si128(_mm_shuffle_epi32(i.m, _MM_SHUFFLE(3, 3, 3, 3)), uniform<int4>::mask_x);
        int4 a2 = _mm_sub_epi32(uniform<int4>::rotate_max, a1);
        int4 b2 = _mm_sub_epi32(uniform<int4>::rotate_max, b1);
        int4 c2 = _mm_sub_epi32(uniform<int4>::rotate_max, c1);
        int4 d2 = _mm_sub_epi32(uniform<int4>::rotate_max, d1);
        a1 = _mm_sll_epi32(v.m, a1);
        b1 = _mm_sll_epi32(v.m, b1);
        c1 = _mm_sll_epi32(v.m, c1);
        d1 = _mm_sll_epi32(v.m, d1);
        a2 = _mm_srl_epi32(v.m, a2);
        b2 = _mm_srl_epi32(v.m, b2);
        c2 = _mm_srl_epi32(v.m, c2);
        d2 = _mm_srl_epi32(v.m, d2);
        a1 = _mm_or_si128(a1, a2);
        b1 = _mm_or_si128(b1, b2);
        c1 = _mm_or_si128(c1, c2);
        d1 = _mm_or_si128(d1, d2);
        a1 = _mm_unpacklo_epi32(a1, b1);
        c1 = _mm_unpackhi_epi32(c1, d1);
        a1 = _mm_shuffle_epi32(a1, _MM_SHUFFLE(3, 0, 3, 0));
        c1 = _mm_shuffle_epi32(c1, _MM_SHUFFLE(3, 0, 3, 0));
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a1), _mm_castsi128_ps(c1), _MM_SHUFFLE(3, 2, 1, 0))) };
        return r;
    }

    //
    // Common Functions
    //

    inline UPALWAYSINLINE uint4 clamp(uint4_cval_t v, uint32_t const min_sval, uint32_t const max_sval) noexcept {
        uint4 min_val = { _mm_set1_epi32(min_sval) };
        uint4 max_val = { _mm_set1_epi32(max_sval) };
#ifdef UP_SIMD_SSE_4_1
        int4 t = _mm_max_epu32(v.m, min_val.m);
        uint4 r = { _mm_min_epu32(t, max_val.m) };
#else
        uint4 m = isgreater(v, min_val);
        int4 r1 = _mm_and_si128(v.m, m.m);
        int4 r2 = _mm_andnot_si128(m.m, min_val.m);
        uint4 t = { _mm_or_si128(r1, r2) };
        m = isgreater(t, max_val);
        r1 = _mm_and_si128(max_val.m, m.m);
        r2 = _mm_andnot_si128(m.m, t.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
     }

    inline UPALWAYSINLINE uint4 clamp(uint4_cval1_t v, uint4_cval2_t min_val, uint4_cval3_t max_val) noexcept {
#ifdef UP_SIMD_SSE_4_1
        int4 t = _mm_max_epu32(v.m, min_val.m);
        uint4 r = { _mm_min_epu32(t, max_val.m) };
#else
        uint4 m = isgreater(v, min_val);
        int4 r1 = _mm_and_si128(v.m, m.m);
        int4 r2 = _mm_andnot_si128(m.m, min_val.m);
        uint4 t = { _mm_or_si128(r1, r2) };
        m = isgreater(t, max_val);
        r1 = _mm_and_si128(max_val.m, m.m);
        r2 = _mm_andnot_si128(m.m, t.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 max(uint4_cval_t v, uint32_t const s) noexcept {
        uint4 v2 = { _mm_set1_epi32(s) };
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_max_epu32(v.m, v2.m) };
#else
        uint4 m = isgreater(v, v2);
        int4 r1 = _mm_and_si128(v.m, m.m);
        int4 r2 = _mm_andnot_si128(m.m, v2.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 max(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_max_epu32(v1.m, v2.m) };
#else
        uint4 m = isgreater(v1, v2);
        int4 r1 = _mm_and_si128(v1.m, m.m);
        int4 r2 = _mm_andnot_si128(m.m, v2.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 min(uint4_cval_t v, uint32_t const s) noexcept {
        uint4 v2 = { _mm_set1_epi32(s) };
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_min_epu32(v.m, v2.m) };
#else
        uint4 m = isgreater(v, v2);
        int4 r1 = _mm_and_si128(v2.m, m.m);
        int4 r2 = _mm_andnot_si128(m.m, v.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }

    inline UPALWAYSINLINE uint4 min(uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
#ifdef UP_SIMD_SSE_4_1
        uint4 r = { _mm_min_epu32(v1.m, v2.m) };
#else
        uint4 m = isgreater(v1, v2);
        int4 r1 = _mm_and_si128(v2.m, m.m);
        int4 r2 = _mm_andnot_si128(m.m, v1.m);
        uint4 r = { _mm_or_si128(r1, r2) };
#endif
        return r;
    }
}}

#endif
