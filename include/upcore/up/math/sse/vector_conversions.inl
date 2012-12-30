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

#ifndef UP_MATH_SSE_VECTOR_CONVERSIONS_INL
#define UP_MATH_SSE_VECTOR_CONVERSIONS_INL

#ifndef UP_MATH_VECTORS_HPP
#   error "Do not include this file directly! Instead include <up/math/vectors.hpp>"
#endif

#define UP_DETAIL_MATH_CONVERT(name, to, from, ...) \
    template <> struct name##_impl<to, from> { static UPALWAYSINLINE UPPURE to convert(UPCONCATENATE(from, _cval_t) v) noexcept __VA_ARGS__ };

namespace up { namespace math { namespace detail
{   
    UP_DETAIL_MATH_CONVERT(as, int4, int4, { return v; })
    UP_DETAIL_MATH_CONVERT(as, int4, uint4, { return v.m; })
    UP_DETAIL_MATH_CONVERT(as, int4, float4, { return _mm_castps_si128(v); })
    UP_DETAIL_MATH_CONVERT(as, uint4, int4, { uint4 r = { v }; return r; })
    UP_DETAIL_MATH_CONVERT(as, uint4, uint4, { return v; })
    UP_DETAIL_MATH_CONVERT(as, uint4, float4, { uint4 r = { _mm_castps_si128(v) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, float4, int4, { return _mm_castsi128_ps(v); })
    UP_DETAIL_MATH_CONVERT(as, float4, uint4, { return _mm_castsi128_ps(v.m); })
    UP_DETAIL_MATH_CONVERT(as, float4, float4, { return v; })
#ifdef UP_SIMD_AVX
    UP_DETAIL_MATH_CONVERT(as, long4, long4, { return v; })
    UP_DETAIL_MATH_CONVERT(as, long4, ulong4, { long4 r = { v.m }; return r; })
    UP_DETAIL_MATH_CONVERT(as, long4, double4, { long4 r = { _mm256_castpd_si256(v) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, ulong4, long4, { ulong4 r = { v.m }; return r; })
    UP_DETAIL_MATH_CONVERT(as, ulong4, ulong4, { return v; })
    UP_DETAIL_MATH_CONVERT(as, ulong4, double4, { ulong4 r = { _mm256_castpd_si256(v) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, double4, long4, { return _mm256_castsi256_pd(v.m); })
    UP_DETAIL_MATH_CONVERT(as, double4, ulong4, { return _mm256_castsi256_pd(v.m); })
    UP_DETAIL_MATH_CONVERT(as, double4, double4, { return v; })
#else
    UP_DETAIL_MATH_CONVERT(as, long4, long4, { return v; })
    UP_DETAIL_MATH_CONVERT(as, long4, ulong4, { long4 r = { v.a, v.b }; return r; })
    UP_DETAIL_MATH_CONVERT(as, long4, double4, { long4 r = { _mm_castpd_si128(v.a), _mm_castpd_si128(v.b) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, ulong4, long4, { ulong4 r = { v.a, v.b }; return r; })
    UP_DETAIL_MATH_CONVERT(as, ulong4, ulong4, { return v; })
    UP_DETAIL_MATH_CONVERT(as, ulong4, double4, { ulong4 r = { _mm_castpd_si128(v.a), _mm_castpd_si128(v.b) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, double4, long4, { double4 r = { _mm_castsi128_pd(v.a), _mm_castsi128_pd(v.b) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, double4, ulong4, { double4 r = { _mm_castsi128_pd(v.a), _mm_castsi128_pd(v.b) }; return r; })
    UP_DETAIL_MATH_CONVERT(as, double4, double4, { return v; })
#endif

    UP_DETAIL_MATH_CONVERT(convert, int4, int4, { return v; })
    UP_DETAIL_MATH_CONVERT(convert, int4, uint4, { return v.m; })
    UP_DETAIL_MATH_CONVERT(convert, int4, float4, { return _mm_cvttps_epi32(v); })
    UP_DETAIL_MATH_CONVERT(convert, uint4, int4, { uint4 r = { v }; return r; })
    UP_DETAIL_MATH_CONVERT(convert, uint4, uint4, { return v; })
    UP_DETAIL_MATH_CONVERT(convert, uint4, float4, { uint4 r = { _mm_cvttps_epi32(v) }; return r; })
    UP_DETAIL_MATH_CONVERT(convert, float4, int4, { return _mm_cvtepi32_ps(v); })
    UP_DETAIL_MATH_CONVERT(convert, float4, uint4, { return _mm_cvtepi32_ps(v.m); })
    UP_DETAIL_MATH_CONVERT(convert, float4, float4, { return v; })
#ifdef UP_SIMD_AVX
    UP_DETAIL_MATH_CONVERT(convert, int4, long4, {
        float4 a = _mm_castsi128_ps(_mm256_castsi256_si128(v.m));
        float4 b = _mm_castsi128_ps(_mm256_extractf128_si256(v.m, 1));
        return _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0)));
    })
    UP_DETAIL_MATH_CONVERT(convert, int4, ulong4, {
        float4 a = _mm_castsi128_ps(_mm256_castsi256_si128(v.m));
        float4 b = _mm_castsi128_ps(_mm256_extractf128_si256(v.m, 1));
        return _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0)));
    })
    UP_DETAIL_MATH_CONVERT(convert, uint4, long4, {
        float4 a = _mm_castsi128_ps(_mm256_castsi256_si128(v.m));
        float4 b = _mm_castsi128_ps(_mm256_extractf128_si256(v.m, 1));
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0))) };
        return r;
    })
    UP_DETAIL_MATH_CONVERT(convert, uint4, ulong4, {
        float4 a = _mm_castsi128_ps(_mm256_castsi256_si128(v.m));
        float4 b = _mm_castsi128_ps(_mm256_extractf128_si256(v.m, 1));
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0))) };
        return r;
    })
    UP_DETAIL_MATH_CONVERT(convert, int4, double4, { return _mm256_cvttpd_epi32(v); })
    UP_DETAIL_MATH_CONVERT(convert, uint4, double4, { uint4 r = { _mm256_cvttpd_epi32(v) }; return r; })
    UP_DETAIL_MATH_CONVERT(convert, float4, double4, { return _mm256_cvtpd_ps(v); })
    UP_DETAIL_MATH_CONVERT(convert, double4, float4, { return _mm256_cvtps_pd(v); })
#else
    UP_DETAIL_MATH_CONVERT(convert, int4, long4, {
        float4 a = _mm_castsi128_ps(v.a);
        float4 b = _mm_castsi128_ps(v.b);
        return _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0)));
    })
    UP_DETAIL_MATH_CONVERT(convert, int4, ulong4, {
        float4 a = _mm_castsi128_ps(v.a);
        float4 b = _mm_castsi128_ps(v.b);
        return _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0)));
    })
    UP_DETAIL_MATH_CONVERT(convert, uint4, long4, {
        float4 a = _mm_castsi128_ps(v.a);
        float4 b = _mm_castsi128_ps(v.b);
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0))) };
        return r;
    })
    UP_DETAIL_MATH_CONVERT(convert, uint4, ulong4, {
        float4 a = _mm_castsi128_ps(v.a);
        float4 b = _mm_castsi128_ps(v.b);
        uint4 r = { _mm_castps_si128(_mm_shuffle_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0))) };
        return r;
    })
    UP_DETAIL_MATH_CONVERT(convert, int4, double4, { return _mm_unpacklo_epi64(_mm_cvttpd_epi32(v.a), _mm_cvttpd_epi32(v.b)); })
    UP_DETAIL_MATH_CONVERT(convert, uint4, double4, { uint4 r = { _mm_unpacklo_epi64(_mm_cvttpd_epi32(v.a), _mm_cvttpd_epi32(v.b)) }; return r; })
    UP_DETAIL_MATH_CONVERT(convert, float4, double4, { return _mm_movelh_ps(_mm_cvtpd_ps(v.a), _mm_cvtpd_ps(v.b)); })
    UP_DETAIL_MATH_CONVERT(convert, double4, float4, { double4 r = { _mm_cvtps_pd(v), _mm_cvtps_pd(_mm_movehl_ps(v, v)) }; return r; })
#endif

    UP_DETAIL_MATH_CONVERT(convert_rte, int4, float4, { return _mm_cvtps_epi32(v); })
    UP_DETAIL_MATH_CONVERT(convert_rte, uint4, float4, { uint4 r = { _mm_cvtps_epi32(v) }; return r; })
#ifdef UP_SIMD_AVX
    UP_DETAIL_MATH_CONVERT(convert_rte, int4, double4, { return _mm256_cvtpd_epi32(v); })
    UP_DETAIL_MATH_CONVERT(convert_rte, uint4, double4, { uint4 r = { _mm256_cvtpd_epi32(v) }; return r; })
#else
    UP_DETAIL_MATH_CONVERT(convert_rte, int4, double4, { return _mm_unpacklo_epi64(_mm_cvtpd_epi32(v.a), _mm_cvtpd_epi32(v.b)); })
    UP_DETAIL_MATH_CONVERT(convert_rte, uint4, double4, { uint4 r = { _mm_unpacklo_epi64(_mm_cvtpd_epi32(v.a), _mm_cvtpd_epi32(v.b)) }; return r; })
#endif

    UP_DETAIL_MATH_CONVERT(convert_rtz, int4, float4, { return _mm_cvttps_epi32(v); })
    UP_DETAIL_MATH_CONVERT(convert_rtz, uint4, float4, { uint4 r = { _mm_cvttps_epi32(v) }; return r; })
#ifdef UP_SIMD_AVX
    UP_DETAIL_MATH_CONVERT(convert_rtz, int4, double4, { return _mm256_cvttpd_epi32(v); })
    UP_DETAIL_MATH_CONVERT(convert_rtz, uint4, double4, { uint4 r = { _mm256_cvttpd_epi32(v) }; return r; })
#else
    UP_DETAIL_MATH_CONVERT(convert_rtz, int4, double4, { return _mm_unpacklo_epi64(_mm_cvttpd_epi32(v.a), _mm_cvttpd_epi32(v.b)); })
    UP_DETAIL_MATH_CONVERT(convert_rtz, uint4, double4, { uint4 r = { _mm_unpacklo_epi64(_mm_cvttpd_epi32(v.a), _mm_cvttpd_epi32(v.b)) }; return r; })
#endif

#if defined(UP_SIMD_SSE_4_1)
    UP_DETAIL_MATH_CONVERT(convert_rtn, int4, float4, { return _mm_cvttps_epi32(_mm_round_ps(v, 0x09)); })
    UP_DETAIL_MATH_CONVERT(convert_rtn, uint4, float4, { uint4 r = { _mm_cvttps_epi32(_mm_round_ps(v, 0x09)) }; return r; })
#else
    UP_DETAIL_MATH_CONVERT(convert_rtn, int4, float4, { return _mm_cvtps_epi32(_mm_sub_ps(v, uniform<float4>::half_minus_epsilon)); })
    UP_DETAIL_MATH_CONVERT(convert_rtn, uint4, float4, { uint4 r = { _mm_cvtps_epi32(_mm_sub_ps(v, uniform<float4>::half_minus_epsilon)) }; return r; })
#endif

#if defined(UP_SIMD_SSE_4_1)
    UP_DETAIL_MATH_CONVERT(convert_rtp, int4, float4, { return _mm_cvttps_epi32(_mm_round_ps(v, 0x0A)); })
    UP_DETAIL_MATH_CONVERT(convert_rtp, uint4, float4, { uint4 r = { _mm_cvttps_epi32(_mm_round_ps(v, 0x0A)) }; return r; })
#else
    UP_DETAIL_MATH_CONVERT(convert_rtp, int4, float4, { return _mm_cvtps_epi32(_mm_add_ps(v, uniform<float4>::half_minus_epsilon)); })
    UP_DETAIL_MATH_CONVERT(convert_rtp, uint4, float4, { uint4 r = { _mm_cvtps_epi32(_mm_add_ps(v, uniform<float4>::half_minus_epsilon)) }; return r; })
#endif

    UP_DETAIL_MATH_CONVERT(convert_sat, int4, int4, { return v; })
    UP_DETAIL_MATH_CONVERT(convert_sat, int4, float4, { return ::up::math::convert_sat_rtz<int4>(v); })
    //UP_DETAIL_MATH_CONVERT(convert_sat, int4, double4, { return ::up::math::convert_sat_rtz<int4>(v); })
    UP_DETAIL_MATH_CONVERT(convert_sat, uint4, uint4, { return v; })
    UP_DETAIL_MATH_CONVERT(convert_sat, uint4, float4, { return ::up::math::convert_sat_rtz<uint4>(v); })
    //UP_DETAIL_MATH_CONVERT(convert_sat, uint4, double4, { return ::up::math::convert_sat_rtz<uint4>(v); })
    UP_DETAIL_MATH_CONVERT(convert_sat, float4, int4, { return _mm_cvtepi32_ps(v); })
    UP_DETAIL_MATH_CONVERT(convert_sat, float4, uint4, { return _mm_cvtepi32_ps(v.m); })
    UP_DETAIL_MATH_CONVERT(convert_sat, float4, float4, { return v; })
#ifdef UP_SIMD_SSE_4_1
    UP_DETAIL_MATH_CONVERT(convert_sat, int4, uint4, { return _mm_min_epu32(v.m, uniform<int4>::max); })
    UP_DETAIL_MATH_CONVERT(convert_sat, uint4, int4, { uint4 r = { _mm_max_epi32(v, uniform<int4>::zero) }; return r; })
#else
    UP_DETAIL_MATH_CONVERT(convert_sat, int4, uint4, {
        int4 mask = _mm_cmpgt_epi32(uniform<int4>::zero, v.m);
        int4 a = _mm_and_si128(mask, v.m);
        int4 b = _mm_andnot_si128(mask, uniform<int4>::max);
        return _mm_or_si128(a, b);
    })
    UP_DETAIL_MATH_CONVERT(convert_sat, uint4, int4, {
        int4 mask = _mm_cmpgt_epi32(v, uniform<int4>::zero);
        uint4 r = { _mm_and_si128(mask, v) };
        return r;
    })
#endif

    UP_DETAIL_MATH_CONVERT(convert_sat_rte, int4, float4, {
        int4 a = _mm_cvtps_epi32(v);
        int4 m1 = _mm_castps_si128(_mm_cmpge_ps(v, uniform<float4>::signed_max));
        int4 m2 = _mm_castps_si128(_mm_cmple_ps(v, uniform<float4>::signed_min));
        int4 b = _mm_and_si128(m1, uniform<int4>::max);
        a = _mm_andnot_si128(m1, a);
        a = _mm_or_si128(a, b);
        b = _mm_and_si128(m2, uniform<int4>::min);
        a = _mm_andnot_si128(m2, a);
        return _mm_or_si128(a, b);
    })

    UP_DETAIL_MATH_CONVERT(convert_sat_rte, uint4, float4, {
        int4 a = _mm_cvtps_epi32(v);
        int4 m1 = _mm_castps_si128(_mm_cmpge_ps(v, uniform<float4>::unsigned_max));
        int4 m2 = _mm_castps_si128(_mm_cmpge_ps(v, uniform<float4>::signed_max));
        int4 m3 = _mm_castps_si128(_mm_cmple_ps(v, uniform<float4>::zero));
        int4 b = _mm_and_si128(m1, as<int4>(uniform<uint4>::max));
        a = _mm_andnot_si128(m1, a);
        a = _mm_or_si128(a, b);
        float4 u = _mm_sub_ps(v, uniform<float4>::signed_max);
        b = _mm_cvtps_epi32(u);
        b = _mm_add_epi32(b, uniform<int4>::max);
        b = _mm_and_si128(m2, b);
        a = _mm_andnot_si128(m2, a);
        a = _mm_or_si128(a, b);
        uint4 r = { _mm_andnot_si128(m3, a) };
        return r;
    })

    UP_DETAIL_MATH_CONVERT(convert_sat_rtz, int4, float4, {
        int4 a = _mm_cvttps_epi32(v);
        int4 m1 = _mm_castps_si128(_mm_cmpge_ps(v, uniform<float4>::signed_max));
        int4 m2 = _mm_castps_si128(_mm_cmple_ps(v, uniform<float4>::signed_min));
        int4 b = _mm_and_si128(m1, uniform<int4>::max);
        a = _mm_andnot_si128(m1, a);
        a = _mm_or_si128(a, b);
        b = _mm_and_si128(m2, uniform<int4>::min);
        a = _mm_andnot_si128(m2, a);
        return _mm_or_si128(a, b);
    })

    UP_DETAIL_MATH_CONVERT(convert_sat_rtz, uint4, float4, {
        int4 a = _mm_cvttps_epi32(v);
        int4 m1 = _mm_castps_si128(_mm_cmpge_ps(v, uniform<float4>::unsigned_max));
        int4 m2 = _mm_castps_si128(_mm_cmpge_ps(v, uniform<float4>::signed_max));
        int4 m3 = _mm_castps_si128(_mm_cmple_ps(v, uniform<float4>::zero));
        int4 b = _mm_and_si128(m1, as<int4>(uniform<uint4>::max));
        a = _mm_andnot_si128(m1, a);
        a = _mm_or_si128(a, b);
        float4 u = _mm_sub_ps(v, uniform<float4>::signed_max);
        b = _mm_cvtps_epi32(u);
        b = _mm_add_epi32(b, uniform<int4>::max);
        b = _mm_and_si128(m2, b);
        a = _mm_andnot_si128(m2, a);
        a = _mm_or_si128(a, b);
        uint4 r = { _mm_andnot_si128(m3, a) };
        return r;
    })

#if defined(UP_SIMD_SSE_4_1)
    UP_DETAIL_MATH_CONVERT(convert_sat_rtn, int4, float4, { return ::up::math::convert_sat_rtz<int4>(_mm_round_ps(v, 0x09)); })
    UP_DETAIL_MATH_CONVERT(convert_sat_rtn, uint4, float4, { return ::up::math::convert_sat_rtz<uint4>(_mm_round_ps(v, 0x09)); })
#else
    UP_DETAIL_MATH_CONVERT(convert_sat_rtn, int4, float4, { return ::up::math::convert_sat_rtz<int4>(_mm_sub_ps(v, uniform<float4>::half_minus_epsilon)); })
    UP_DETAIL_MATH_CONVERT(convert_sat_rtn, uint4, float4, { return ::up::math::convert_sat_rtz<uint4>(_mm_sub_ps(v, uniform<float4>::half_minus_epsilon)); })
#endif

#if defined(UP_SIMD_SSE_4_1)
    UP_DETAIL_MATH_CONVERT(convert_sat_rtp, int4, float4, { return ::up::math::convert_sat_rtz<int4>(_mm_round_ps(v, 0x0A)); })
    UP_DETAIL_MATH_CONVERT(convert_sat_rtp, uint4, float4, { return ::up::math::convert_sat_rtz<uint4>(_mm_round_ps(v, 0x0A)); })
#else
    UP_DETAIL_MATH_CONVERT(convert_sat_rtp, int4, float4, { return ::up::math::convert_sat_rtz<int4>(_mm_add_ps(v, uniform<float4>::half_minus_epsilon)); })
    UP_DETAIL_MATH_CONVERT(convert_sat_rtp, uint4, float4, { return ::up::math::convert_sat_rtz<uint4>(_mm_add_ps(v, uniform<float4>::half_minus_epsilon)); })
#endif
}}}

#undef UP_DETAIL_MATH_CONVERT

#endif
