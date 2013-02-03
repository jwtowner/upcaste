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

#ifndef UP_BITWISE_HPP
#define UP_BITWISE_HPP

#include <up/climits.hpp>
#include <up/cstddef.hpp>
#include <up/cstdint.hpp>
#if UP_COMPILER == UP_COMPILER_CLANG
#   include <up/detail/bitwise_gcc.inl>
#elif UP_COMPILER == UP_COMPILER_GCC
#   include <up/detail/bitwise_gcc.inl>
#elif UP_COMPILER == UP_COMPILER_MSVC
#   include <up/detail/bitwise_msvc.inl>
#else
#   error "Compiler not currently supported!"
#endif

namespace up { namespace detail
{
    template <uintmax_t X, size_t R>
    struct static_floorlog2_impl
    {
        static constexpr size_t value = (X & (uintmax_t(1) << R)) ? R : static_floorlog2_impl<X, R - 1>::value;
    };

    template <uintmax_t X>
    struct static_floorlog2_impl<X, 0>
    {
        static constexpr size_t value = 0;
    };

    template <size_t R>
    struct static_floorlog2_impl<0, R>
    {
        static constexpr size_t value = R + 1;
    };
}}

namespace up
{
    template <class UIntType, uintmax_t X>
    struct UPVISIBLE static_floorlog2
    {
        static constexpr size_t value = detail::static_floorlog2_impl<X, ((sizeof(UIntType) * CHAR_BIT) - 1)>::value;
    };

    inline UPALWAYSINLINE UPPURE
    unsigned int ceillog2(unsigned int x) noexcept {
        return (x > 1) ? ((sizeof(x) * CHAR_BIT) - lzcount(x - 1)) : 0;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lceillog2(unsigned long x) noexcept {
        return (x > 1) ? ((sizeof(x) * CHAR_BIT) - llzcount(x - 1)) : 0;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long llceillog2(unsigned long long x) noexcept {
        return (x > 1) ? ((sizeof(x) * CHAR_BIT) - lllzcount(x - 1)) : 0;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int floorlog2(unsigned int x) noexcept {
        return ((sizeof(x) * CHAR_BIT) - 1) - lzcount(x | 1);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lfloorlog2(unsigned long x) noexcept {
        return ((sizeof(x) * CHAR_BIT) - 1) - llzcount(x | 1);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long llfloorlog2(unsigned long long x) noexcept {
        return ((sizeof(x) * CHAR_BIT) - 1) - lllzcount(x | 1);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int ceil2(unsigned int x) noexcept {
        return (x > 1) ? (1u << ((sizeof(x) * CHAR_BIT) - lzcount(x - 1))) : x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lceil2(unsigned long x) noexcept {
        return (x > 1) ? (1ul << ((sizeof(x) * CHAR_BIT) - llzcount(x - 1))) : x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long llceil2(unsigned long long x) noexcept {
        return (x > 1) ? (1ull << ((sizeof(x) * CHAR_BIT) - lllzcount(x - 1))) : x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int floor2(unsigned int x) noexcept {
        return static_cast<unsigned int>(x > 0) << (((sizeof(x) * CHAR_BIT) - 1) - lzcount(x | 1));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lfloor2(unsigned long x) noexcept {
        return static_cast<unsigned long>(x > 0) << (((sizeof(x) * CHAR_BIT) - 1) - llzcount(x | 1));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long llfloor2(unsigned long long x) noexcept {
        return static_cast<unsigned long long>(x > 0) << (((sizeof(x) * CHAR_BIT) - 1) - lllzcount(x | 1));
    }

    inline UPALWAYSINLINE UPPURE
    bool addition_overflow(unsigned int x, unsigned int y) noexcept {
        return (ceillog2(x) >= (sizeof(x) * CHAR_BIT)) || (ceillog2(y) >= (sizeof(x) * CHAR_BIT));
    }

    inline UPALWAYSINLINE UPPURE
    bool addition_overflow(unsigned long x, unsigned long y) noexcept {
        return (lceillog2(x) >= (sizeof(x) * CHAR_BIT)) || (lceillog2(y) >= (sizeof(x) * CHAR_BIT));
    }

    inline UPALWAYSINLINE UPPURE
    bool addition_overflow(unsigned long long x, unsigned long long y) noexcept {
        return (llceillog2(x) >= (sizeof(x) * CHAR_BIT)) || (llceillog2(y) >= (sizeof(x) * CHAR_BIT));
    }

    inline UPALWAYSINLINE UPPURE
    bool multiplication_overflow(unsigned int x, unsigned int y) noexcept {
        return (ceillog2(x) + ceillog2(y)) > (sizeof(x) * CHAR_BIT);
    }

    inline UPALWAYSINLINE UPPURE
    bool multiplication_overflow(unsigned long x, unsigned long y) noexcept {
        return (lceillog2(x) + lceillog2(y)) > (sizeof(x) * CHAR_BIT);
    }

    inline UPALWAYSINLINE UPPURE
    bool multiplication_overflow(unsigned long long x, unsigned long long y) noexcept {
        return (llceillog2(x) + llceillog2(y)) > (sizeof(x) * CHAR_BIT);
    }

    inline UPALWAYSINLINE UPPURE unsigned long rotl(unsigned long x, int y) noexcept { return lrotl(x, y); }
    inline UPALWAYSINLINE UPPURE unsigned long rotr(unsigned long x, int y) noexcept { return lrotr(x, y); }
    inline UPALWAYSINLINE UPPURE unsigned long lzcount(unsigned long x) noexcept { return llzcount(x); }
    inline UPALWAYSINLINE UPPURE unsigned long tzcount(unsigned long x) noexcept { return ltzcount(x); }
    inline UPALWAYSINLINE UPPURE unsigned long popcount(unsigned long x) noexcept { return lpopcount(x); }
    inline UPALWAYSINLINE UPPURE unsigned long ceil2(unsigned long x) noexcept { return lceil2(x); }
    inline UPALWAYSINLINE UPPURE unsigned long floor2(unsigned long x) noexcept { return lfloor2(x); }
    inline UPALWAYSINLINE UPPURE unsigned long ceillog2(unsigned long x) noexcept { return lceillog2(x); }
    inline UPALWAYSINLINE UPPURE unsigned long floorlog2(unsigned long x) noexcept { return lfloorlog2(x); }

    inline UPALWAYSINLINE UPPURE unsigned long long rotl(unsigned long long x, int y) noexcept { return llrotl(x, y); }
    inline UPALWAYSINLINE UPPURE unsigned long long rotr(unsigned long long x, int y) noexcept { return llrotr(x, y); }
    inline UPALWAYSINLINE UPPURE unsigned long long lzcount(unsigned long long x) noexcept { return lllzcount(x); }
    inline UPALWAYSINLINE UPPURE unsigned long long tzcount(unsigned long long x) noexcept { return lltzcount(x); }
    inline UPALWAYSINLINE UPPURE unsigned long long popcount(unsigned long long x) noexcept { return llpopcount(x); }
    inline UPALWAYSINLINE UPPURE unsigned long long ceil2(unsigned long long x) noexcept { return llceil2(x); }
    inline UPALWAYSINLINE UPPURE unsigned long long floor2(unsigned long long x) noexcept { return llfloor2(x); }
    inline UPALWAYSINLINE UPPURE unsigned long long ceillog2(unsigned long long x) noexcept { return llceillog2(x); }
    inline UPALWAYSINLINE UPPURE unsigned long long floorlog2(unsigned long long x) noexcept { return llfloorlog2(x); }

    inline UPALWAYSINLINE UPPURE uint_least64_t u64rotl(uint_least64_t x, int y) noexcept { return rotl(x, y); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64rotr(uint_least64_t x, int y) noexcept { return rotr(x, y); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64lzcount(uint_least64_t x) noexcept { return lzcount(x); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64tzcount(uint_least64_t x) noexcept { return tzcount(x); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64popcount(uint_least64_t x) noexcept { return popcount(x); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64ceil2(uint_least64_t x) noexcept { return ceil2(x); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64floor2(uint_least64_t x) noexcept { return floor2(x); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64ceillog2(uint_least64_t x) noexcept { return ceillog2(x); }
    inline UPALWAYSINLINE UPPURE uint_least64_t u64floorlog2(uint_least64_t x) noexcept { return floorlog2(x); }

    inline UPALWAYSINLINE UPPURE uint_least32_t u32rotl(uint_least32_t x, int y) noexcept { return rotl(x, y); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32rotr(uint_least32_t x, int y) noexcept { return rotr(x, y); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32lzcount(uint_least32_t x) noexcept { return lzcount(x); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32tzcount(uint_least32_t x) noexcept { return tzcount(x); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32popcount(uint_least32_t x) noexcept { return popcount(x); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32ceil2(uint_least32_t x) noexcept { return ceil2(x); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32floor2(uint_least32_t x) noexcept { return floor2(x); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32ceillog2(uint_least32_t x) noexcept { return ceillog2(x); }
    inline UPALWAYSINLINE UPPURE uint_least32_t u32floorlog2(uint_least32_t x) noexcept { return floorlog2(x); }
}

#endif
