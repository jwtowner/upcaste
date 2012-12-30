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

#ifndef UP_DETAIL_BITWISE_GCC_INL
#define UP_DETAIL_BITWISE_GCC_INL

#ifndef UP_BITWISE_HPP
#   error "Do not include this file directly! Instead include <up/bitwise.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    unsigned int bitscanforward(unsigned int x) noexcept {
        return __builtin_ctz(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned int bitscanreverse(unsigned int x) noexcept {
        return __builtin_clz(x);
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long bitscanforward(unsigned long x) noexcept {
        return __builtin_ctzl(x);
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long bitscanreverse(unsigned long x) noexcept {
        return __builtin_clzl(x);
    }
    
#ifdef UP_ARCHITECTURE_64BIT
    inline UPALWAYSINLINE UPPURE
    unsigned long long bitscanforward(unsigned long long x) noexcept {
        return __builtin_ctzll(x);
    }
    
    inline UPALWAYSINLINE UPPURE
    unsigned long long bitscanreverse(unsigned long long x) noexcept {
        return __builtin_clzll(x);
    }
#endif
}

#endif
