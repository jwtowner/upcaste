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

#ifndef UP_DETAIL_BITWISE_MSVC_INL
#define UP_DETAIL_BITWISE_MSVC_INL

#ifndef UP_BITWISE_HPP
#   error "Do not include this file directly! Instead include <up/bitwise.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    unsigned int tzcount(unsigned int x) noexcept {
        unsigned long result;
        _BitScanForward(&result, static_cast<unsigned long>(x));
        return static_cast<unsigned int>(result);
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long tzcount(unsigned long x) noexcept {
        unsigned long result;
        _BitScanForward(&result, x);
        return result;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long tzcount(unsigned long long x) noexcept {
#ifdef UP_ARCHITECTURE_64BIT
        unsigned long result;
        _BitScanForward64(&result, x);
        return result;
#else
        unsigned long result;
        if (!_BitScanForward(&result, static_cast<unsigned long>(x & ULONG_MAX))) {
            _BitScanForward(&result, static_cast<unsigned long>((x >> 32) & ULONG_MAX));
        }
        return result;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int lzcount(unsigned int x) noexcept {
        unsigned long result;
        _BitScanReverse(&result, static_cast<unsigned long>(x));
        return 31 - static_cast<unsigned int>(result);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lzcount(unsigned long x) noexcept {
        unsigned long result;
        _BitScanReverse(&result, x);
        return 31 - result;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long lzcount(unsigned long long x) noexcept {
        unsigned long result;
#   ifdef UP_ARCHITECTURE_64BIT
        _BitScanReverse64(&result, x);
#   else
        if (!_BitScanReverse(&result, static_cast<unsigned long>((x >> 32 & ULONG_MAX)))) {
            _BitScanReverse(&result, static_cast<unsigned long>(x & ULONG_MAX));
        }
#   endif
        return 63 - result;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int popcount(unsigned int x) noexcept {
#ifdef UP_SIMD_SSE_4_2
        return __popcnt(x);
#else
        x -= (x >> 1) & 0x55555555ul;
        x = (x & 0x33333333ul) + ((x >> 2) & 0x33333333ul);
        x = (x + (x >> 4)) & 0x0F0F0F0Ful;
        return (x * 0x01010101ul) >> 24;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long popcount(unsigned long x) noexcept {
#ifdef UP_SIMD_SSE_4_2
        return static_cast<unsigned long>(__popcnt(static_cast<unsigned int>(x)));
#else
        x -= (x >> 1) & 0x55555555ul;
        x = (x & 0x33333333ul) + ((x >> 2) & 0x33333333ul);
        x = (x + (x >> 4)) & 0x0F0F0F0Ful;
        return (x * 0x01010101ul) >> 24;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long popcount(unsigned long long x) noexcept {
#ifdef UP_SIMD_SSE_4_2
        return __popcnt64(x);
#else
        x -= (x >> 1) & 0x5555555555555555ull;
        x = (x & 0x3333333333333333ull) + ((x >> 2) & 0x3333333333333333ull);
        x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0Full;
        return (x * 0x0101010101010101ull) >> 56;
#endif
    }
}

#endif
