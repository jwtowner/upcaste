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

#ifndef UP_CUCHAR_INTERNAL_INL
#define UP_CUCHAR_INTERNAL_INL

#include <up/type_traits.hpp>

namespace up { namespace detail
{
    //
    // multi-byte to utf-16 & utf-32 conversion
    //

    struct LIBUPCOREAPI ucstate_t
    {
        int count;
        union {
            char c8[u8_cur_max];
            char16_t c16[u16_cur_max];
            char32_t c32;
        };
    };

    static_assert(sizeof(ucstate_t) >= sizeof(mbstate_t), "size of default mbstate_t is not large enough");

    //
    // Constants and functions for efficiently finding a null-terminator in a string
    //

#if defined(UP_ARCHITECTURE_32BIT)
    constexpr uintptr_t null16_test_high_mask = 0x80008000u;
    constexpr uintptr_t null16_test_low_mask = 0x00010001u;
#   if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
    constexpr uintptr_t null16_test_mask = 0x0000FFFFu;
#   else
    constexpr uintptr_t null16_test_mask = 0xFFFF0000u;
#   endif
#elif defined(UP_ARCHITECTURE_64BIT)
    constexpr uintptr_t null16_test_high_mask = 0x8000800080008000ull;
    constexpr uintptr_t null16_test_low_mask = 0x0001000100010001ull;
    constexpr uintptr_t null32_test_high_mask = 0x8000000080000000ull;
    constexpr uintptr_t null32_test_low_mask = 0x0000000100000001ull;
#   if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
    constexpr uintptr_t null16_test_mask = 0x000000000000FFFFull;
    constexpr uintptr_t null32_test_mask = 0x00000000FFFFFFFFull;
#   else
    constexpr uintptr_t null16_test_mask = 0xFFFF000000000000ull;
    constexpr uintptr_t null32_test_mask = 0xFFFFFFFF00000000ull;
#   endif
#else
#   error "Platform architecture not supported."
#endif

    // Tests if a null-terminator is present in the given block
    inline UPALWAYSINLINE
    uintptr_t null16_test(uintptr_t block) noexcept {
        return (block - null16_test_low_mask) & null16_test_high_mask & ~block;
    }
        
    inline UPALWAYSINLINE
    uintptr_t null32_test(uintptr_t block) noexcept {
#ifdef UP_ARCHITECTURE_32BIT
        return !block;
#else
        return (block - null32_test_low_mask) & null32_test_high_mask & ~block;
#endif
    }

    // Shifts the null-terminator test mask in a endian-neutral manner
    inline UPALWAYSINLINE
    uintptr_t null16_test_shift(uintptr_t mask) noexcept {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
        return mask << 16;
#else
        return mask >> 16;
#endif
    }

#ifdef UP_ARCHITECTURE_32BIT
    inline UPALWAYSINLINE
    uintptr_t null32_test_shift(uintptr_t) noexcept {
        return 0;
    }
#else
    inline UPALWAYSINLINE
    uintptr_t null32_test_shift(uintptr_t mask) noexcept {
#   if UP_BYTE_ORDER == UP_LITTLE_ENDIAN        
        return mask << 32;
#   else
        return mask >> 32;
#   endif
    }
#endif

    // Determines code unit offset of first null-terminator in the given block
    inline UPALWAYSINLINE
    uintptr_t null16_test_locate(uintptr_t block) noexcept {
        uintptr_t mask = null16_test_mask;
        if (!(block & mask)) return 0;
#if defined(UP_ARCHITECTURE_32BIT)
        return 1;
#elif defined(UP_ARCHITECTURE_64BIT)
        mask = null16_test_shift(mask);
        if (!(block & mask)) return 1;
        mask = null16_test_shift(mask);
        if (!(block & mask)) return 2;
        return 3;
#else
#   error "Platform architecture not supported!"
#endif
    }

    //
    // Unicode constants
    //

    constexpr char32_t ucs2_max_value = 0x0000FFFFu;
    constexpr char32_t ucs4_max_value = 0x7FFFFFFFu;
    constexpr char32_t utf32_max_value = 0x0010FFFFu;

    constexpr char16_t utf16_lead_surrogate_min = 0xD800;
    constexpr char16_t utf16_lead_surrogate_max = 0xDBFF;
    constexpr char16_t utf16_tail_surrogate_min = 0xDC00;
    constexpr char16_t utf16_tail_surrogate_max = 0xDFFF;
    constexpr char32_t utf16_surrogate_offset = (0x0000D800u << 10) + 0x0000DC00u - 0x00010000u;
        
    constexpr unsigned char u8_replacement_character_1 = 0xEF;
    constexpr unsigned char u8_replacement_character_2 = 0xBF;
    constexpr unsigned char u8_replacement_character_3 = 0xBD;
    constexpr char16_t u16_replacement_character = 0xFFFD;
    constexpr char32_t u32_replacement_character = 0x0000FFFDu;

    extern unsigned char const u8_replacement_sequence[3];
    extern char16_t const u16_replacement_sequence[1];

    extern alignas(UP_MAX_CACHE_LINE_SIZE) int8_t const u8_sequence_length_table[256];
    extern uint_fast32_t const u8_offset_table[8];
    extern uint_fast32_t const u8_min_values[8];

    //
    // Internal utility functions
    //
    
    inline UPALWAYSINLINE UPNONNULLALL
    unsigned char const* u8s_recover(unsigned char const* u8s) noexcept {
        return reinterpret_cast<unsigned char const*>(::up::u8serr(reinterpret_cast<char const*>(u8s)));
    }

    inline UPALWAYSINLINE UPNONNULLALL
    unsigned char const* u8s_recover(unsigned char const* u8s, unsigned char const* u8s_end) noexcept {
        return reinterpret_cast<unsigned char const*>(::up::u8snerr(reinterpret_cast<char const*>(u8s), u8s_end - u8s));
    }

    template <class T>
    inline UPALWAYSINLINE
    bool u8_is_trail(T c) noexcept {
        static_assert(is_unsigned<T>::value, "");
        return (c & 0xC0) == 0x80;
    }

    template <class T>
    inline UPALWAYSINLINE
    bool u16_is_surrogate(T c) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least16_t)), "");
        return (c & 0xF800) == 0xD800;
    }
        
    template <class T>
    inline UPALWAYSINLINE
    bool u16_is_surrogate_pair(T lead, T tail) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least16_t)), "");
        lead &= 0xFC00; tail &= 0xFC00;
        return (lead == 0xD800) && (tail == 0xDC00);
    }

    template <class T>
    inline UPALWAYSINLINE
    bool u16_is_lead_surrogate(T c) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least16_t)), "");
        return (c & 0xFC00) == 0xD800;
    }
        
    template <class T>
    inline UPALWAYSINLINE
    bool u16_is_tail_surrogate(T c) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least16_t)), "");
        return (c & 0xFC00) == 0xDC00;
    }

    template <class T>
    inline UPALWAYSINLINE
    bool u32_is_surrogate(T c) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least32_t)), "");
        return (c & 0xFFFFF800u) == 0x0000D800u;
    }

    template <class T>
    inline UPALWAYSINLINE
    bool u32_is_valid(T c) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least32_t)), "");
        return (c <= utf32_max_value) && !::up::detail::u32_is_surrogate(c);
    }

    template <class T, class U>
    inline UPALWAYSINLINE
    bool u32_from_u8_is_valid(T c, U length) noexcept {
        static_assert(is_unsigned<T>::value && (sizeof(T) >= sizeof(uint_least32_t)) && is_signed<U>::value, "");
        return (::up::detail::u8_min_values[length] <= c) && (c <= utf32_max_value) && !::up::detail::u32_is_surrogate(c);
    }
}}

#endif
