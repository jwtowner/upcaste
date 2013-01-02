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
#if UP_COMPILER == UP_COMPILER_GCC
#   include <up/detail/bitwise_gcc.inl>
#elif UP_COMPILER == UP_COMPILER_MSVC
#   include <up/detail/bitwise_msvc.inl>
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    unsigned int logceil2(unsigned int x) noexcept {
        return (x > 1) ? (UINT_BIT - lzcount(x - 1)) : 0;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long logceil2(unsigned long x) noexcept {
        return (x > 1) ? (ULONG_BIT - lzcount(x - 1)) : 0;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long logceil2(unsigned long long x) noexcept {
        return (x > 1) ? (ULLONG_BIT - lzcount(x - 1)) : 0;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int logfloor2(unsigned int x) noexcept {
        return (UINT_BIT - 1) - lzcount(x | 1);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long logfloor2(unsigned long x) noexcept {
        return (ULONG_BIT - 1) - lzcount(x | 1);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long logfloor2(unsigned long long x) noexcept {
        return (ULLONG_BIT - 1) - lzcount(x | 1);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int ceil2(unsigned int x) noexcept {
        return (x > 1) ? (1u << (UINT_BIT - lzcount(x - 1))) : x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long ceil2(unsigned long x) noexcept {
        return (x > 1) ? (1ul << (ULONG_BIT - lzcount(x - 1))) : x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long ceil2(unsigned long long x) noexcept {
        return (x > 1) ? (1ull << (ULLONG_BIT - lzcount(x - 1))) : x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int floor2(unsigned int x) noexcept {
        return static_cast<unsigned int>(x > 0) << ((UINT_BIT - 1) - lzcount(x | 1));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long floor2(unsigned long x) noexcept {
        return static_cast<unsigned long>(x > 0) << ((ULONG_BIT - 1) - lzcount(x | 1));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long floor2(unsigned long long x) noexcept {
        return static_cast<unsigned long long>(x > 0) << ((ULLONG_BIT - 1) - lzcount(x | 1));
    }

    inline UPALWAYSINLINE UPPURE
    bool addition_overflow(unsigned int x, unsigned int y) noexcept {
        return (logceil2(x) >= UINT_BIT) || (logceil2(y) >= UINT_BIT);
    }

    inline UPALWAYSINLINE UPPURE
    bool addition_overflow(unsigned long x, unsigned long y) noexcept {
        return (logceil2(x) >= ULONG_BIT) || (logceil2(y) >= ULONG_BIT);
    }

    inline UPALWAYSINLINE UPPURE
    bool addition_overflow(unsigned long long x, unsigned long long y) noexcept {
        return (logceil2(x) >= ULLONG_BIT) || (logceil2(y) >= ULLONG_BIT);
    }

    inline UPALWAYSINLINE UPPURE
    bool multiplication_overflow(unsigned int x, unsigned int y) noexcept {
        return (logceil2(x) + logceil2(y)) > UINT_BIT;
    }

    inline UPALWAYSINLINE UPPURE
    bool multiplication_overflow(unsigned long x, unsigned long y) noexcept {
        return (logceil2(x) + logceil2(y)) > ULONG_BIT;
    }

    inline UPALWAYSINLINE UPPURE
    bool multiplication_overflow(unsigned long long x, unsigned long long y) noexcept {
        return (logceil2(x) + logceil2(y)) > ULLONG_BIT;
    }
}

#endif
