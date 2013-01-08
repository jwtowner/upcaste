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

#ifndef UP_CUCHAR_HPP
#define UP_CUCHAR_HPP

#include <up/cstring.hpp>
#ifdef UP_HAS_STDC_WCHAR
#   include <up/cwchar.hpp>
#endif
#ifdef UP_HAS_STDC_UCHAR
#   include <uchar.h>
#endif

namespace up
{
#ifdef UP_HAS_STDC_UCHAR
#ifdef UP_NO_NATIVE_CHAR16_T
    using ::char16_t;
#endif
#ifdef UP_NO_NATIVE_CHAR32_T
    using ::char32_t;
#endif
    using ::mbstate_t;
    using ::c16rtomb;
    using ::c32rtomb;
    using ::mbrtoc16;
    using ::mbrtoc32;
#else
#ifdef UP_NO_NATIVE_CHAR16_T
    typedef uint_least16_t char16_t;
#endif
#ifdef UP_NO_NATIVE_CHAR32_T
    typedef uint_least32_t char32_t;
#endif
    extern LIBUPCOREAPI
    size_t c16rtomb(char* UPRESTRICT s, char16_t c16, mbstate_t* UPRESTRICT ps) noexcept;
    
    extern LIBUPCOREAPI
    size_t c32rtomb(char* UPRESTRICT s, char32_t c32, mbstate_t* UPRESTRICT ps) noexcept;
    
    extern LIBUPCOREAPI
    size_t mbrtoc16(char16_t* UPRESTRICT c16, char const* UPRESTRICT s, size_t n, mbstate_t* UPRESTRICT ps) noexcept;
    
    extern LIBUPCOREAPI
    size_t mbrtoc32(char32_t* UPRESTRICT c32, char const* UPRESTRICT s, size_t n, mbstate_t* UPRESTRICT ps) noexcept;
#endif

    constexpr char32_t ueof = UINT_LEAST32_MAX;
    constexpr size_t u8_cur_max = 4;
    constexpr size_t u16_cur_max = 2;

    //
    // utf-8 character string operations
    //

    extern LIBUPCOREAPI
    char const* u8schk(char const* s) noexcept;
    
    extern LIBUPCOREAPI
    char const* u8snchk(char const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u8len(char const* s) noexcept;
    
    extern LIBUPCOREAPI
    int u8nlen(char const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8stou16slen(char const* s) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8stou32slen(char const* s) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8sntou16slen(char const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8sntou32slen(char const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u8tou32(char32_t* UPRESTRICT u32, char const* UPRESTRICT u8, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8stou16s(char16_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8stou32s(char32_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8sntou16s(char16_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t nu8, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8sntou32s(char32_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t nu8, size_t n) noexcept;

    inline UPALWAYSINLINE
    char* u8memchr(char* s, char u8, size_t n) noexcept {
        return static_cast<char*>(memchr(s, static_cast<uint8_t>(u8), n));
    }

    inline UPALWAYSINLINE
    char const* u8memchr(char const* s, char u8, size_t n) noexcept {
        return static_cast<char const*>(memchr(s, static_cast<uint8_t>(u8), n));
    }

    inline UPALWAYSINLINE
    int u8memcmp(char const* s1, char const* s2, size_t n) noexcept {
        return memcmp(s1, s2, n);
    }

    inline UPALWAYSINLINE
    char* u8memcpy(char* UPRESTRICT s1, char const* UPRESTRICT s2, size_t n) noexcept {
        return static_cast<char*>(memcpy(s1, s2, n));
    }

    inline UPALWAYSINLINE
    char* u8memmove(char* s1, char const* s2, size_t n) noexcept {
        return static_cast<char*>(memmove(s1, s2, n));
    }

    inline UPALWAYSINLINE
    char* u8memrchr(char* s, char u8, size_t n) noexcept {
        return static_cast<char*>(memrchr(s, static_cast<uint8_t>(u8), n));
    }
    
    inline UPALWAYSINLINE
    char const* u8memrchr(char const* s, char u8, size_t n) noexcept {
        return static_cast<char const*>(memrchr(s, static_cast<uint8_t>(u8), n));
    }

    inline UPALWAYSINLINE
    char* u8memset(char* s, char u8, size_t n) noexcept {
        return static_cast<char*>(memset(s, static_cast<uint8_t>(u8), n));
    }
    
    extern LIBUPCOREAPI
    char const* u8schr(char const* s, char32_t u32) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8scspn(char const* s, char const* reject) noexcept;
    
    extern LIBUPCOREAPI
    char const* u8spbrk(char const* s, char const* accept) noexcept;
    
    extern LIBUPCOREAPI
    char const* u8srchr(char const* s, char32_t u32) noexcept;
    
    extern LIBUPCOREAPI
    size_t u8sspn(char const* s, char const* accept) noexcept;
    
    extern LIBUPCOREAPI
    char* u8stok(char* UPRESTRICT s, char const* UPRESTRICT delim, char** UPRESTRICT state) noexcept;
    
    inline UPALWAYSINLINE
    char* u8scat(char* UPRESTRICT s1, char const* UPRESTRICT s2) noexcept {
        return strcat(s1, s2);
    }

    inline UPALWAYSINLINE
    int u8scmp(char const* s1, char const* s2) noexcept {
        return strcmp(s1, s2);
    }
    
    inline UPALWAYSINLINE
    char* u8scpy(char* UPRESTRICT s1, char const* UPRESTRICT s2) noexcept {
        return strcpy(s1, s2);
    }
    
    inline UPALWAYSINLINE
    size_t u8slen(char const* s) noexcept {
        return strlen(s);
    }
    
    inline UPALWAYSINLINE
    char* u8sncat(char* UPRESTRICT s1, char const* UPRESTRICT s2, size_t n) noexcept {
        return strncat(s1, s2, n);
    }
    
    inline UPALWAYSINLINE
    int u8sncmp(char const* s1, char const* s2, size_t n) noexcept {
        return strncmp(s1, s2, n);
    }
    
    inline UPALWAYSINLINE
    char* u8sncpy(char* UPRESTRICT s1, char const* UPRESTRICT s2, size_t n) noexcept {
        return strncpy(s1, s2, n);
    }

    inline UPALWAYSINLINE
    size_t u8snlen(char const* s, size_t n) noexcept {
        return strnlen(s, n);
    }

    inline UPALWAYSINLINE
    char* u8schr(char* s, char32_t c) noexcept {
        return const_cast<char*>(u8schr(const_cast<char const*>(s), c));
    }
    
    inline UPALWAYSINLINE
    char* u8spbrk(char* s, char const* accept) noexcept {
        return const_cast<char*>(u8spbrk(const_cast<char const*>(s), accept));
    }
    
    inline UPALWAYSINLINE
    char* u8srchr(char* s, char32_t c) noexcept {
        return const_cast<char*>(u8srchr(const_cast<char const*>(s), c));
    }

    inline UPALWAYSINLINE
    char* u8sstr(char* s1, char const* s2) noexcept {
        return strstr(s1, s2);
    }

    inline UPALWAYSINLINE
    char const* u8sstr(char const* s1, char const* s2) noexcept {
        return strstr(s1, s2);
    }
    
    inline UPALWAYSINLINE
    uint_least32_t u8shash32(char const* s) noexcept {
        return strhash32(s);
    }
    
    inline UPALWAYSINLINE
    uint_least32_t u8snhash32(char const* s, size_t n) noexcept {
        return strnhash32(s, n);
    }

    inline UPALWAYSINLINE
    uint_least64_t u8shash64(char const* s) noexcept {
        return strhash64(s);
    }
    
    inline UPALWAYSINLINE
    uint_least64_t u8snhash64(char const* s, size_t n) noexcept {
        return strnhash64(s, n);
    }

    inline UPALWAYSINLINE
    size_t u8shash(char const* s) noexcept {
        return strhash(s);
    }
    
    inline UPALWAYSINLINE
    size_t u8snhash(char const* s, size_t n) noexcept {
        return strnhash(s, n);
    }

    //
    // utf-16 character strings    
    //
    
    extern LIBUPCOREAPI
    char16_t const* u16schk(char16_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16snchk(char16_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u16len(char16_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    int u16nlen(char16_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16stou8slen(char16_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16stou32slen(char16_t const* s) noexcept;    
    
    extern LIBUPCOREAPI
    size_t u16sntou8slen(char16_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16sntou32slen(char16_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u16tou32(char32_t* UPRESTRICT u32, char16_t const* UPRESTRICT u16, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16stou8s(char* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16stou32s(char32_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16sntou8s(char* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t nu16, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16sntou32s(char32_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t nu16, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16memchr(char16_t const* s, char16_t c, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u16memcmp(char16_t const* s1, char16_t const* s2, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16memrchr(char16_t const* s, char16_t c, size_t n) noexcept;
    
    inline UPALWAYSINLINE
    char16_t* u16memchr(char16_t* s, char16_t c, size_t n) noexcept {
        return const_cast<char16_t*>(u16memchr(const_cast<char16_t const*>(s), c, n));
    }

    inline UPALWAYSINLINE
    char16_t* u16memcpy(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2, size_t n) noexcept {
        return static_cast<char16_t*>(memcpy(s1, s2, n * sizeof(char16_t)));
    }
    
    inline UPALWAYSINLINE
    char16_t* u16memmove(char16_t* s1, char16_t const* s2, size_t n) noexcept {
        return static_cast<char16_t*>(memmove(s1, s2, n * sizeof(char16_t)));
    }

    inline UPALWAYSINLINE
    char16_t* u16memrchr(char16_t* s, char16_t c, size_t n) noexcept {
        return const_cast<char16_t*>(u16memrchr(const_cast<char16_t const*>(s), c, n));
    }

    inline UPALWAYSINLINE
    char16_t* u16memset(char16_t* s1, char16_t c, size_t n) noexcept {
        return static_cast<char16_t*>(memset_pattern2(s1, &c, n));
    }

    extern LIBUPCOREAPI
    char16_t* u16scat(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16schr(char16_t const* s, char32_t c) noexcept;
    
    extern LIBUPCOREAPI
    int u16scmp(char16_t const* s1, char16_t const* s2) noexcept;
    
    extern LIBUPCOREAPI
    char16_t* u16scpy(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16slen(char16_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16scspn(char16_t const* s, char16_t const* reject) noexcept;
    
    extern LIBUPCOREAPI
    char16_t* u16sncat(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16snlen(char16_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16spbrk(char16_t const* s, char16_t const* accept) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16srchr(char16_t const* s, char32_t c) noexcept;
    
    extern LIBUPCOREAPI
    size_t u16sspn(char16_t const* s, char16_t const* accept) noexcept;
    
    extern LIBUPCOREAPI
    char16_t const* u16sstr(char16_t const* s1, char16_t const* s2) noexcept;
    
    extern LIBUPCOREAPI
    char16_t* u16stok(char16_t* UPRESTRICT s, char16_t const* UPRESTRICT delim, char16_t** UPRESTRICT state) noexcept;
    
    extern LIBUPCOREAPI
    uint_least32_t u16shash32(char16_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    uint_least32_t u16snhash32(char16_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    uint_least64_t u16shash64(char16_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    uint_least64_t u16snhash64(char16_t const* s, size_t n) noexcept;

    inline UPALWAYSINLINE
    char16_t* u16schr(char16_t* s, char32_t c) noexcept {
        return const_cast<char16_t*>(u16schr(const_cast<char16_t const*>(s), c));
    }
    
    inline UPALWAYSINLINE
    int u16sncmp(char16_t const* s1, char16_t const* s2, size_t n) noexcept {
        return u16memcmp(s1, s2, n);
    }
    
    inline UPALWAYSINLINE
    char16_t* u16sncpy(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2, size_t n) noexcept {
        return u16memcpy(s1, s2, n);
    }
    
    inline UPALWAYSINLINE
    char16_t* u16spbrk(char16_t* s, char16_t const* accept) noexcept {
        return const_cast<char16_t*>(u16spbrk(const_cast<char16_t const*>(s), accept));
    }

    inline UPALWAYSINLINE
    char16_t* u16srchr(char16_t* s, char32_t c) noexcept {
        return const_cast<char16_t*>(u16srchr(const_cast<char16_t const*>(s), c));
    }

    inline UPALWAYSINLINE
    char16_t* u16sstr(char16_t* s1, char16_t const* s2) noexcept {
        return const_cast<char16_t*>(u16sstr(const_cast<char16_t const*>(s1), s2));
    }

    inline UPALWAYSINLINE
    size_t u16shash(char16_t const* s) noexcept {
#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
        return u16shash32(s);
#else
        return u16shash64(s);
#endif
    }

    inline UPALWAYSINLINE
    size_t u16snhash(char16_t const* s, size_t n) noexcept {
#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
        return u16snhash32(s, n);
#else
        return u16snhash64(s, n);
#endif
    }

    //
    // utf-32 character strings
    //

    extern LIBUPCOREAPI
    char32_t const* u32schk(char32_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32snchk(char32_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u32len(char32_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    int u32nlen(char32_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32stou8slen(char32_t const* s) noexcept;    
    
    extern LIBUPCOREAPI
    size_t u32stou16slen(char32_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32sntou8slen(char32_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32sntou16slen(char32_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u32tou8(char* u8, char32_t u32) noexcept;
    
    extern LIBUPCOREAPI
    int u32tou16(char16_t* u16, char32_t u32) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32stou8s(char* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32stou16s(char16_t* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32sntou8s(char* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t nu32, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32sntou16s(char16_t* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t nu32, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32memchr(char32_t const* s, char32_t c, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    int u32memcmp(char32_t const* s1, char32_t const* s2, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32memrchr(char32_t const* s, char32_t c, size_t n) noexcept;
    
    inline UPALWAYSINLINE
    char32_t* u32memchr(char32_t* s, char32_t c, size_t n) noexcept {
        return const_cast<char32_t*>(u32memchr(const_cast<char32_t const*>(s), c, n));
    }
    
    inline UPALWAYSINLINE
    char32_t* u32memcpy(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2, size_t n) noexcept {
        return static_cast<char32_t*>(memcpy(s1, s2, n * sizeof(char32_t)));
    }
    
    inline UPALWAYSINLINE
    char32_t* u32memmove(char32_t* s1, char32_t const* s2, size_t n) noexcept {
        return static_cast<char32_t*>(memmove(s1, s2, n * sizeof(char32_t)));
    }

    inline UPALWAYSINLINE
    char32_t* u32memrchr(char32_t* s, char32_t c, size_t n) noexcept {
        return const_cast<char32_t*>(u32memrchr(const_cast<char32_t const*>(s), c, n));
    }
    
    inline UPALWAYSINLINE
    char32_t* u32memset(char32_t* s1, char32_t c, size_t n) noexcept {
        return static_cast<char32_t*>(memset_pattern4(s1, &c, n));
    }
    
    extern LIBUPCOREAPI
    char32_t* u32scat(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32schr(char32_t const* s, char32_t c) noexcept;
    
    extern LIBUPCOREAPI
    int u32scmp(char32_t const* s1, char32_t const* s2) noexcept;
    
    extern LIBUPCOREAPI
    char32_t* u32scpy(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32scspn(char32_t const* s, char32_t const* reject) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32slen(char32_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    char32_t* u32sncat(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32snlen(char32_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32spbrk(char32_t const* s, char32_t const* accept) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32srchr(char32_t const* s, char32_t c) noexcept;
    
    extern LIBUPCOREAPI
    size_t u32sspn(char32_t const* s, char32_t const* accept) noexcept;
    
    extern LIBUPCOREAPI
    char32_t const* u32sstr(char32_t const* s1, char32_t const* s2) noexcept;
    
    extern LIBUPCOREAPI
    char32_t* u32stok(char32_t* UPRESTRICT s, char32_t const* UPRESTRICT delim, char32_t** UPRESTRICT state) noexcept;
    
    extern LIBUPCOREAPI
    uint_least32_t u32shash32(char32_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    uint_least32_t u32snhash32(char32_t const* s, size_t n) noexcept;
    
    extern LIBUPCOREAPI
    uint_least64_t u32shash64(char32_t const* s) noexcept;
    
    extern LIBUPCOREAPI
    uint_least64_t u32snhash64(char32_t const* s, size_t n) noexcept;
    
    inline UPALWAYSINLINE
    char32_t* u32schr(char32_t* s, char32_t c) noexcept {
        return const_cast<char32_t*>(u32schr(const_cast<char32_t const*>(s), c));
    }
    
    inline UPALWAYSINLINE
    int u32sncmp(char32_t const* s1, char32_t const* s2, size_t n) noexcept {
        return u32memcmp(s1, s2, n);
    }

    inline UPALWAYSINLINE
    char32_t* u32sncpy(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2, size_t n) noexcept {
        return u32memcpy(s1, s2, n);
    }

    inline UPALWAYSINLINE
    char32_t* u32spbrk(char32_t* s, char32_t const* accept) noexcept {
        return const_cast<char32_t*>(u32spbrk(const_cast<char32_t const*>(s), accept));
    }
    
    inline UPALWAYSINLINE
    char32_t* u32srchr(char32_t* s, char32_t c) noexcept {
        return const_cast<char32_t*>(u32srchr(const_cast<char32_t const*>(s), c));
    }
    
    inline UPALWAYSINLINE
    char32_t* u32sstr(char32_t* s1, char32_t const* s2) noexcept {
        return const_cast<char32_t*>(u32sstr(const_cast<char32_t const*>(s1), s2));
    }
    
    inline UPALWAYSINLINE
    size_t u32shash(char32_t const* s) noexcept {
#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
        return u32shash32(s);
#else
        return u32shash64(s);
#endif
    }

    inline UPALWAYSINLINE
    size_t u32snhash(char32_t const* s, size_t n) noexcept {
#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
        return u32snhash32(s, n);
#else
        return u32snhash64(s, n);
#endif
    }
}

#ifdef UP_HAS_STDC_WCHAR
#   if WCHAR_MAX <= UINT_LEAST8_MAX
#       include <up/detail/cuchar_wchar_utf8.inl>
#   elif WCHAR_MAX <= UINT_LEAST16_MAX
#       include <up/detail/cuchar_wchar_utf16.inl>
#   elif WCHAR_MAX <= UINT_LEAST32_MAX
#       include <up/detail/cuchar_wchar_utf32.inl>
#   else
#       error "Unsupported or unknown wchar_t text encoding!"
#   endif
#endif

#endif
