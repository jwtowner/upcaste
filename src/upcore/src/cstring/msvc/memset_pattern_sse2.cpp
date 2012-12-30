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

#include <up/cstring.hpp>
#include <up/cassert.hpp>

#if !defined(UP_NO_SIMD) && defined(UP_SIMD_SSE_2)

#include <immintrin.h>

namespace up
{
    LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern2(void* UPRESTRICT dest, void const* UPRESTRICT pattern, size_t count) noexcept {
        assert(pattern && (dest || !count) && ((reinterpret_cast<uintptr_t>(dest) & 0x01) == 0));

        unsigned short* dest16 = static_cast<unsigned short*>(dest);
        unsigned short pattern16 = *static_cast<unsigned short const*>(pattern);

        if (count >= 32) {
            size_t align16_count = (0x10 - (reinterpret_cast<size_t>(dest16) & 0x0F)) >> 1;
            if (count < align16_count) {
                align16_count = count;
            }

            __stosw(dest16, pattern16, align16_count);
            dest16 += align16_count;
            count -= align16_count;

            __m128i* dest128 = reinterpret_cast<__m128i*>(dest16);
            __m128i value_wide = _mm_set1_epi16(pattern16);

            while (count >= 32) {
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                count -= 32;
            }

            dest16 = reinterpret_cast<unsigned short*>(dest128);
        }

        __stosw(dest16, pattern16, count);
        return static_cast<void*>(dest16 + count);
    }

    LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern4(void* UPRESTRICT dest, void const* UPRESTRICT pattern, size_t count) noexcept {
        assert(pattern && (dest || !count) && ((reinterpret_cast<uintptr_t>(dest) & 0x03) == 0));

        unsigned long* dest32 = static_cast<unsigned long*>(dest);
        unsigned long pattern32 = *static_cast<unsigned long const*>(pattern);

        if (count >= 32) {
            size_t align16_count = (0x10 - (reinterpret_cast<size_t>(dest32) & 0x0F)) >> 2;
            if (count < align16_count) {
                align16_count = count;
            }

            __stosd(dest32, pattern32, align16_count);
            dest32 += align16_count;
            count -= align16_count;

            __m128i* dest128 = reinterpret_cast<__m128i*>(dest32);
            __m128i value_wide = _mm_set1_epi32(pattern32);

            while (count >= 16) {
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                count -= 16;
            }

            dest32 = reinterpret_cast<unsigned long*>(dest128);
        }

        __stosd(dest32, pattern32, count);
        return static_cast<void*>(dest32 + count);
    }

    LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern8(void* UPRESTRICT dest, void const* UPRESTRICT pattern, size_t count) noexcept {
        assert(pattern && (dest || !count) && ((reinterpret_cast<uintptr_t>(dest) & 0x07) == 0));

        unsigned long long* dest64 = static_cast<unsigned long long*>(dest);

#ifdef UP_ARCHITECTURE_64BIT
        unsigned long long pattern64 = *static_cast<unsigned long long const*>(pattern);
        
        if (count >= 32) {
            if (((reinterpret_cast<size_t>(dest64) & 0x0F) >> 3) > 0) {
                *dest64++ = pattern64;
                --count;
            }

            __m128i value_wide = _mm_set1_epi64x(pattern64);
            __m128i* dest128 = reinterpret_cast<__m128i*>(dest64);

            while (count >= 8) {
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                _mm_store_si128(dest128++, value_wide);
                count -= 8;
            }
        
            dest64 = reinterpret_cast<unsigned long long*>(dest128);
        }

        __stosq(dest64, pattern64, count);
        return static_cast<void*>(dest64 + count);
#else        
        __m128i value_wide = _mm_loadl_epi64(static_cast<__m128i const*>(pattern));
        value_wide = _mm_unpacklo_epi64(value_wide, value_wide);

        if (reinterpret_cast<uintptr_t>(dest64) & 0x0F) {
            _mm_storel_epi64(reinterpret_cast<__m128i*>(dest64++), value_wide);
            --count;
        }

        __m128i* dest128 = reinterpret_cast<__m128i*>(dest64);

        while (count >= 8) {
            _mm_store_si128(dest128++, value_wide);
            _mm_store_si128(dest128++, value_wide);
            _mm_store_si128(dest128++, value_wide);
            _mm_store_si128(dest128++, value_wide);
            count -= 8;
        }

        while (count >= 2) {
            _mm_store_si128(dest128++, value_wide);
            count -= 2;
        }

        dest64 = reinterpret_cast<unsigned long long*>(dest128);

        if (count > 0) {
            _mm_storel_epi64(reinterpret_cast<__m128i*>(dest64++), value_wide);
        }

        return static_cast<void*>(dest64);
#endif
    }
}

#endif
