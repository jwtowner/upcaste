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
//  the following cs:
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

#ifndef UP_DETAIL_BITWISE_GCC_INL
#define UP_DETAIL_BITWISE_GCC_INL

#ifndef UP_BITWISE_HPP
#   error "Do not include this file directly. Instead include <up/bitwise.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    unsigned int rotl(unsigned int x, int s) noexcept {
        return (x << s) | (x >> ((sizeof(x) * CHAR_BIT) - s));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lrotl(unsigned long x, int s) noexcept {
        return (x << s) | (x >> ((sizeof(x) * CHAR_BIT) - s));
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long long llrotl(unsigned long long x, int s) noexcept {
        return (x << s) | (x >> ((sizeof(x) * CHAR_BIT) - s));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int rotr(unsigned int x, int s) noexcept {
        return (x >> s) | (x << ((sizeof(x) * CHAR_BIT) - s));
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long lrotr(unsigned long x, int s) noexcept {
        return (x >> s) | (x << ((sizeof(x) * CHAR_BIT) - s));
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long long llrotr(unsigned long long x, int s) noexcept {
        return (x >> s) | (x << ((sizeof(x) * CHAR_BIT) - s));
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int lzcount(unsigned int x) noexcept {
        return __builtin_clz(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long llzcount(unsigned long x) noexcept {
        return __builtin_clzl(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long lllzcount(unsigned long long x) noexcept {
        return __builtin_clzll(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int tzcount(unsigned int x) noexcept {
        return __builtin_ctz(x);
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long ltzcount(unsigned long x) noexcept {
        return __builtin_ctzl(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long lltzcount(unsigned long long x) noexcept {
        return __builtin_ctzll(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int popcount(unsigned int x) noexcept {
        return __builtin_popcount(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long lpopcount(unsigned long x) noexcept {
        return __builtin_popcountl(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long llpopcount(unsigned long long x) noexcept {
        return __builtin_popcountll(x);
    }
}

#endif
