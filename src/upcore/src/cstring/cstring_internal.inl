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

#ifndef UP_CSTRING_INTERNAL_INL
#define UP_CSTRING_INTERNAL_INL

namespace up { namespace detail
{
    // Constants and functions for efficiently finding a null-terminator in a string
#if defined(UP_ARCHITECTURE_32BIT)
    constexpr uintptr_t null8_test_high_mask = 0x80808080u;
    constexpr uintptr_t null8_test_low_mask = 0x01010101u;
#   if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
    constexpr uintptr_t null8_test_mask = 0x000000FFu;
#   else
    constexpr uintptr_t null8_test_mask = 0xFF000000u;
#   endif
#elif defined(UP_ARCHITECTURE_64BIT)
    constexpr uintptr_t null8_test_high_mask = 0x8080808080808080ull;
    constexpr uintptr_t null8_test_low_mask = 0x0101010101010101ull;
#   if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
    constexpr uintptr_t null8_test_mask = 0x00000000000000FFull;
#   else
    constexpr uintptr_t null8_test_mask = 0xFF00000000000000ull;
#   endif
#else
#   error "Platform architecture not supported."
#endif

    // Tests if a null-terminator is present in the given block
    inline UPALWAYSINLINE UPPURE
    UPCONSTEXPR uintptr_t null8_test(uintptr_t block) noexcept {
        return (block - null8_test_low_mask) & null8_test_high_mask & ~block;
    }

    // Shifts the null-terminator test mask in a endian-neutral manner
    inline UPALWAYSINLINE UPPURE
    UPCONSTEXPR uintptr_t null8_test_shift(uintptr_t mask) noexcept {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
        return mask << 8;
#else
        return mask >> 8;
#endif
    }

    // Determines code unit offset of first null-terminator in the given block
    inline UPALWAYSINLINE UPPURE
    uintptr_t null8_test_locate(uintptr_t block) noexcept {
        uintptr_t mask = null8_test_mask;
        if (!(block & mask)) return 0;
        mask = null8_test_shift(mask);
        if (!(block & mask)) return 1;
        mask = null8_test_shift(mask);
        if (!(block & mask)) return 2;
#if defined(UP_ARCHITECTURE_32BIT)
        return 3;
#elif defined(UP_ARCHITECTURE_64BIT)
        mask = null8_test_shift(mask);
        if (!(block & mask)) return 3;
        mask = null8_test_shift(mask);
        if (!(block & mask)) return 4;
        mask = null8_test_shift(mask);
        if (!(block & mask)) return 5;
        mask = null8_test_shift(mask);
        if (!(block & mask)) return 6;
        return 7;
#else
#   error "Platform architecture not supported!"
#endif
    }
}}

#endif
