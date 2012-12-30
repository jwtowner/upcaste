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

#ifndef UP_CUCHAR_HPP
#define UP_CUCHAR_HPP

#include <up/cstring.hpp>
#ifdef UP_HAS_STDC_WCHAR
#   include <up/cwchar.hpp>
#endif
#if defined(UP_HAS_STDC_UCHAR_CXX11)
#   include <cuchar>
#elif defined(UP_HAS_STDC_UCHAR_C99)
#   include <uchar.h>
#else
#   ifndef __STDC_UTF_16__
#       define __STDC_UTF_16__
#   endif
#   ifndef __STDC_UTF_32__
#       define __STDC_UTF_32__
#   endif
#endif

#define U8_CUR_MAX 4
#define U16_CUR_MAX 2
#define UEOF UINT_LEAST32_MAX

namespace up
{
    //
    // char16_t and char32_t types
    //

#ifndef UP_HAS_STDC_UCHAR
#   ifdef UP_NO_NATIVE_CHAR16_T
    typedef unsigned short char16_t;
#   endif
#   ifdef UP_NO_NATIVE_CHAR32_T
    typedef unsigned int char32_t;
#   endif
#else
#   ifdef UP_NO_NATIVE_CHAR16_T
    using ::std::char16_t;
#   endif
#   ifdef UP_NO_NATIVE_CHAR32_T
    using ::std::char32_t;
#   endif
#endif

    //
    // multi-byte to utf-16 & utf-32 conversion
    //

    struct LIBUPCOREAPI ucstate_t
    {
        int count;
        union {
            char c[U8_CUR_MAX];
            char16_t c16[U16_CUR_MAX];
            char32_t c32;
        };
    };

#ifndef UP_HAS_STDC_UCHAR
#   ifndef UP_HAS_STDC_WCHAR
    typedef ucstate_t mbstate_t;
#   endif
    //extern LIBUPCOREAPI size_t c16rtomb(char* UPRESTRICT s, char16_t c16, mbstate_t* UPRESTRICT state) noexcept;
    //extern LIBUPCOREAPI size_t c32rtomb(char* UPRESTRICT s, char32_t c32, mbstate_t* UPRESTRICT state) noexcept;
    //extern LIBUPCOREAPI size_t mbrtoc16(char16_t* UPRESTRICT c16s, char const* UPRESTRICT s, size_t n, mbstate_t* UPRESTRICT state) noexcept;
    //extern LIBUPCOREAPI size_t mbrtoc32(char32_t* UPRESTRICT c32s, char const* UPRESTRICT s, size_t n, mbstate_t* UPRESTRICT state) noexcept;
#else
    using ::std::mbstate_t;
    using ::std::c16rtomb;
    using ::std::c32rtomb;
    using ::std::mbrtoc16;
    using ::std::mbrtoc32;
#endif

    static_assert(sizeof(ucstate_t) >= sizeof(mbstate_t), "size of default mbstate_t is not large enough");

    //
    // utf-8 character string operations
    //

    extern LIBUPCOREAPI char const* u8schk(char const* s) noexcept;
    extern LIBUPCOREAPI char const* u8snchk(char const* s, size_t n) noexcept;
    extern LIBUPCOREAPI int u8len(char const* s) noexcept;
    extern LIBUPCOREAPI int u8nlen(char const* s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u8slen_u16(char const* s) noexcept;
    extern LIBUPCOREAPI size_t u8slen_u32(char const* s) noexcept;
    extern LIBUPCOREAPI size_t u8snlen_u16(char const* s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u8snlen_u32(char const* s, size_t n) noexcept;
    extern LIBUPCOREAPI int u8tou32(char32_t* UPRESTRICT u32, char const* UPRESTRICT u8s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u8stou16s(char16_t* UPRESTRICT u16s, char const* UPRESTRICT u8s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u8stou32s(char32_t* UPRESTRICT u32s, char const* UPRESTRICT u8s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u8silseq(char const* s) noexcept;
    extern LIBUPCOREAPI size_t u8snilseq(char const* s, size_t n) noexcept;

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
    
    extern LIBUPCOREAPI char const* u8schr(char const* s, char32_t u32) noexcept;
    extern LIBUPCOREAPI size_t u8scspn(char const* s, char const* reject) noexcept;
    extern LIBUPCOREAPI char const* u8spbrk(char const* s, char const* accept) noexcept;
    extern LIBUPCOREAPI char const* u8srchr(char const* s, char32_t u32) noexcept;
    extern LIBUPCOREAPI size_t u8sspn(char const* s, char const* accept) noexcept;
    extern LIBUPCOREAPI char* u8stok(char* UPRESTRICT s, char const* UPRESTRICT delim, char** UPRESTRICT state) noexcept;
    
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
    
    extern LIBUPCOREAPI char16_t const* u16schk(char16_t const* s) noexcept;
    extern LIBUPCOREAPI char16_t const* u16snchk(char16_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI int u16len(char16_t const* s) noexcept;
    extern LIBUPCOREAPI int u16nlen(char16_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u16slen_u8(char16_t const* s) noexcept;
    extern LIBUPCOREAPI size_t u16slen_u32(char16_t const* s) noexcept;    
    extern LIBUPCOREAPI size_t u16snlen_u8(char16_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u16snlen_u32(char16_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI int u16tou32(char32_t* UPRESTRICT u32, char16_t const* UPRESTRICT u16s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u16stou8s(char* UPRESTRICT u8s, char16_t const* UPRESTRICT u16s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u16stou32s(char32_t* UPRESTRICT u32s, char16_t const* UPRESTRICT u16s, size_t n) noexcept;

    extern LIBUPCOREAPI char16_t const* u16memchr(char16_t const* s, char16_t c, size_t n) noexcept;
    extern LIBUPCOREAPI int u16memcmp(char16_t const* s1, char16_t const* s2, size_t n) noexcept;
    extern LIBUPCOREAPI char16_t const* u16memrchr(char16_t const* s, char16_t c, size_t n) noexcept;
    
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

    extern LIBUPCOREAPI char16_t* u16scat(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2) noexcept;
    extern LIBUPCOREAPI char16_t const* u16schr(char16_t const* s, char32_t c) noexcept;
    extern LIBUPCOREAPI int u16scmp(char16_t const* s1, char16_t const* s2) noexcept;
    extern LIBUPCOREAPI char16_t* u16scpy(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2) noexcept;
    extern LIBUPCOREAPI size_t u16slen(char16_t const* s) noexcept;
    extern LIBUPCOREAPI size_t u16scspn(char16_t const* s, char16_t const* reject) noexcept;
    extern LIBUPCOREAPI char16_t* u16sncat(char16_t* UPRESTRICT s1, char16_t const* UPRESTRICT s2, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u16snlen(char16_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI char16_t const* u16spbrk(char16_t const* s, char16_t const* accept) noexcept;
    extern LIBUPCOREAPI char16_t const* u16srchr(char16_t const* s, char32_t c) noexcept;
    extern LIBUPCOREAPI size_t u16sspn(char16_t const* s, char16_t const* accept) noexcept;
    extern LIBUPCOREAPI char16_t const* u16sstr(char16_t const* s1, char16_t const* s2) noexcept;
    extern LIBUPCOREAPI char16_t* u16stok(char16_t* UPRESTRICT s, char16_t const* UPRESTRICT delim, char16_t** UPRESTRICT state) noexcept;
    extern LIBUPCOREAPI uint_least32_t u16shash32(char16_t const* s) noexcept;
    extern LIBUPCOREAPI uint_least32_t u16snhash32(char16_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI uint_least64_t u16shash64(char16_t const* s) noexcept;
    extern LIBUPCOREAPI uint_least64_t u16snhash64(char16_t const* s, size_t n) noexcept;

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
#if (SIZE_MAX <= UINT32_MAX)
        return u16shash32(s);
#else
        return u16shash64(s);
#endif
    }

    inline UPALWAYSINLINE
    size_t u16snhash(char16_t const* s, size_t n) noexcept {
#if (SIZE_MAX <= UINT32_MAX)
        return u16snhash32(s, n);
#else
        return u16snhash64(s, n);
#endif
    }

    //
    // utf-32 character strings
    //

    extern LIBUPCOREAPI char32_t const* u32schk(char32_t const* s) noexcept;
    extern LIBUPCOREAPI char32_t const* u32snchk(char32_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI int u32len(char32_t const* s) noexcept;
    extern LIBUPCOREAPI int u32nlen(char32_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u32slen_u8(char32_t const* s) noexcept;    
    extern LIBUPCOREAPI size_t u32slen_u16(char32_t const* s) noexcept;
    extern LIBUPCOREAPI size_t u32snlen_u8(char32_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u32snlen_u16(char32_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI int u32tou8(char* u8s, char32_t u32) noexcept;
    extern LIBUPCOREAPI int u32tou16(char16_t* u16s, char32_t u32) noexcept;
    extern LIBUPCOREAPI size_t u32stou8s(char* UPRESTRICT u8s, char32_t const* UPRESTRICT u32s, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u32stou16s(char16_t* UPRESTRICT u16s, char32_t const* UPRESTRICT u32s, size_t n) noexcept;
    
    inline UPALWAYSINLINE
    int u32rlen(char32_t const* UPRESTRICT s, size_t n, mbstate_t* UPRESTRICT) noexcept {
        return u32nlen(s, n);
    }
    
    inline UPALWAYSINLINE
    int u32rtou8(char* UPRESTRICT u8s, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32tou8(u8s, u32);
    }
    
    inline UPALWAYSINLINE
    int u32rtou16(char16_t* UPRESTRICT u16s, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32tou16(u16s, u32);
    }

    extern LIBUPCOREAPI char32_t const* u32memchr(char32_t const* s, char32_t c, size_t n) noexcept;
    extern LIBUPCOREAPI int u32memcmp(char32_t const* s1, char32_t const* s2, size_t n) noexcept;
    extern LIBUPCOREAPI char32_t const* u32memrchr(char32_t const* s, char32_t c, size_t n) noexcept;
    
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
    
    extern LIBUPCOREAPI char32_t* u32scat(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2) noexcept;
    extern LIBUPCOREAPI char32_t const* u32schr(char32_t const* s, char32_t c) noexcept;
    extern LIBUPCOREAPI int u32scmp(char32_t const* s1, char32_t const* s2) noexcept;
    extern LIBUPCOREAPI char32_t* u32scpy(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2) noexcept;
    extern LIBUPCOREAPI size_t u32scspn(char32_t const* s, char32_t const* reject) noexcept;
    extern LIBUPCOREAPI size_t u32slen(char32_t const* s) noexcept;
    extern LIBUPCOREAPI char32_t* u32sncat(char32_t* UPRESTRICT s1, char32_t const* UPRESTRICT s2, size_t n) noexcept;
    extern LIBUPCOREAPI size_t u32snlen(char32_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI char32_t const* u32spbrk(char32_t const* s, char32_t const* accept) noexcept;
    extern LIBUPCOREAPI char32_t const* u32srchr(char32_t const* s, char32_t c) noexcept;
    extern LIBUPCOREAPI size_t u32sspn(char32_t const* s, char32_t const* accept) noexcept;
    extern LIBUPCOREAPI char32_t const* u32sstr(char32_t const* s1, char32_t const* s2) noexcept;
    extern LIBUPCOREAPI char32_t* u32stok(char32_t* UPRESTRICT s, char32_t const* UPRESTRICT delim, char32_t** UPRESTRICT state) noexcept;
    extern LIBUPCOREAPI uint_least32_t u32shash32(char32_t const* s) noexcept;
    extern LIBUPCOREAPI uint_least32_t u32snhash32(char32_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI uint_least64_t u32shash64(char32_t const* s) noexcept;
    extern LIBUPCOREAPI uint_least64_t u32snhash64(char32_t const* s, size_t n) noexcept;
    
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
#if (SIZE_MAX <= UINT32_MAX)
        return u32shash32(s);
#else
        return u32shash64(s);
#endif
    }

    inline UPALWAYSINLINE
    size_t u32snhash(char32_t const* s, size_t n) noexcept {
#if (SIZE_MAX <= UINT32_MAX)
        return u32snhash32(s, n);
#else
        return u32snhash64(s, n);
#endif
    }

    //
    // wide character / unicode character string conversion
    //

#ifdef UP_HAS_STDC_WCHAR

#if (WCHAR_MAX <= UINT8_MAX)

    inline UPALWAYSINLINE
    wchar_t const* wcsschk(wchar_t const* wcs) noexcept {
        return reinterpret_cast<wchar_t const*>(u8schk(reinterpret_cast<char const*>(wcs)));
    }
    
    inline UPALWAYSINLINE
    wchar_t const* wcsnchk(wchar_t const* wcs, size_t n) noexcept {
        return reinterpret_cast<wchar_t const*>(u8snchk(reinterpret_cast<char const*>(wcs), n));
    }
    
    inline UPALWAYSINLINE
    int wclen(wchar_t const* wcs) noexcept {
        return u8len(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    int wcnlen(wchar_t const* wcs, size_t n) noexcept {
        return u8nlen(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    int wctou32(char32_t* UPRESTRICT u32, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u8tou32(u32, reinterpret_cast<char const*>(wcs), n);
    }

    inline UPALWAYSINLINE
    int u32towc(wchar_t* wcs, char32_t u32) noexcept {
        return u32tou8(reinterpret_cast<char*>(wcs), u32);
    }
    
    inline UPALWAYSINLINE
    int u32rtowc(wchar_t* UPRESTRICT wcs, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32tou8(reinterpret_cast<char*>(wcs), u32);
    }

    inline UPALWAYSINLINE
    size_t wcslen_u8(wchar_t const* wcs) noexcept {
        return u8slen(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u16(wchar_t const* wcs) noexcept {
        return u8slen_u16(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u32(wchar_t const* wcs) noexcept {
        return u8slen_u32(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u8(wchar_t const* wcs, size_t n) noexcept {
        return u8snlen(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u16(wchar_t const* wcs, size_t n) noexcept {
        return u8snlen_u16(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u32(wchar_t const* wcs, size_t n) noexcept {
        return u8snlen_u32(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou8s(char* UPRESTRICT s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        if (n-- > 0) {
            memcpy(s, wcs, n);
            s[n] = 0;
            return n;
        }
        return 0;
    }
    
    inline UPALWAYSINLINE
    size_t wcstou16s(char16_t* UPRESTRICT u16s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u8stou16s(u16s, reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou32s(char32_t* UPRESTRICT u32s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u8stou32s(u32s, reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t u8slen_wc(char const* s) noexcept {
        return u8slen(s);
    }
    
    inline UPALWAYSINLINE
    size_t u8snlen_wc(char const* s, size_t n) noexcept {
        return u8snlen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u8stowcs(wchar_t* UPRESTRICT wcs, char const* UPRESTRICT s, size_t n) noexcept {
        if (n-- > 0) {
            memcpy(wcs, s, n);
            wcs[n] = 0;
            return n;
        }
        return 0;
    }
    
    inline UPALWAYSINLINE
    size_t u16slen_wc(char16_t const* s) noexcept {
        return u16slen_u8(s);
    }
    
    inline UPALWAYSINLINE
    size_t u16snlen_wc(char16_t const* s, size_t n) noexcept {
        return u16snlen_u8(s, n);
    }

    inline UPALWAYSINLINE
    size_t u16stowcs(wchar_t* UPRESTRICT wcs, char16_t const* UPRESTRICT u16s, size_t n) noexcept {
        return u16stou8s(reinterpret_cast<char*>(wcs), u16s, n);
    }

    inline UPALWAYSINLINE
    size_t u32slen_wc(char32_t const* s) noexcept {
        return u32slen_u8(s);
    }

    inline UPALWAYSINLINE
    size_t u32snlen_wc(char32_t const* s, size_t n) noexcept {
        return u32snlen_u8(s, n);
    }

    inline UPALWAYSINLINE
    size_t u32stowcs(wchar_t* UPRESTRICT wcs, char32_t const* UPRESTRICT u32s, size_t n) noexcept {
        return u32stou8s(reinterpret_cast<char*>(wcs), u32s, n);
    }
    
#elif (WCHAR_MAX <= UINT16_MAX)

    inline UPALWAYSINLINE
    wchar_t const* wcsschk(wchar_t const* wcs) noexcept {
        return reinterpret_cast<wchar_t const*>(u16schk(reinterpret_cast<char16_t const*>(wcs)));
    }
    
    inline UPALWAYSINLINE
    wchar_t const* wcsnchk(wchar_t const* wcs, size_t n) noexcept {
        return reinterpret_cast<wchar_t const*>(u16snchk(reinterpret_cast<char16_t const*>(wcs), n));
    }
    
    inline UPALWAYSINLINE
    int wclen(wchar_t const* wcs) noexcept {
        return u16len(reinterpret_cast<char16_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    int wcnlen(wchar_t const* wcs, size_t n) noexcept {
        return u16nlen(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    int u32towc(wchar_t* wcs, char32_t u32) noexcept {
        return u32tou16(reinterpret_cast<char16_t*>(wcs), u32);
    }
    
    inline UPALWAYSINLINE
    int u32rtowc(wchar_t* UPRESTRICT wcs, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32tou16(reinterpret_cast<char16_t*>(wcs), u32);
    }
    
    inline UPALWAYSINLINE
    int wctou32(char32_t* UPRESTRICT u32, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u16tou32(u32, reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u8(wchar_t const* wcs) noexcept {
        return u16slen_u8(reinterpret_cast<char16_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u16(wchar_t const* wcs) noexcept {
        return u16slen(reinterpret_cast<char16_t const*>(wcs));
    }

    inline UPALWAYSINLINE
    size_t wcslen_u32(wchar_t const* wcs) noexcept {
        return u16slen_u32(reinterpret_cast<char16_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u8(wchar_t const* wcs, size_t n) noexcept {
        return u16snlen_u8(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u16(wchar_t const* wcs, size_t n) noexcept {
        return u16snlen(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u32(wchar_t const* wcs, size_t n) noexcept {
        return u16snlen_u32(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou8s(char* UPRESTRICT s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u16stou8s(s, reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE 
    size_t wcstou16s(char16_t* UPRESTRICT u16s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        if (n-- > 0) {
            u16memcpy(u16s, reinterpret_cast<char16_t const*>(wcs), n);
            u16s[n] = 0;
            return n;
        }
        return 0;
    }
    
    inline UPALWAYSINLINE
    size_t wcstou32s(char32_t* UPRESTRICT u32s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u16stou32s(u32s, reinterpret_cast<char16_t const*>(wcs), n);
    }

    inline UPALWAYSINLINE size_t u8slen_wc(char const* s) noexcept { return u8slen_u16(s); }
    inline UPALWAYSINLINE size_t u8snlen_wc(char const* s, size_t n) noexcept  { return u8snlen_u16(s, n); }

    inline UPALWAYSINLINE
    size_t u8stowcs(wchar_t* UPRESTRICT wcs, char const* UPRESTRICT s, size_t n) noexcept {
        return u8stou16s(reinterpret_cast<char16_t*>(wcs), s, n);
    }

    inline UPALWAYSINLINE
    size_t u16slen_wc(char16_t const* s) noexcept {
        return u16slen(s);
    }

    inline UPALWAYSINLINE
    size_t u16snlen_wc(char16_t const* s, size_t n) noexcept {
        return u16snlen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u16stowcs(wchar_t* UPRESTRICT wcs, char16_t const* UPRESTRICT u16s, size_t n) noexcept {
        if (n-- > 0) {
            u16memcpy(reinterpret_cast<char16_t*>(wcs), u16s, n);
            wcs[n] = 0;
            return n;
        }
        return 0;
    }

    inline UPALWAYSINLINE
    size_t u32slen_wc(char32_t const* s) noexcept {
        return u32slen_u16(s);
    }
    
    inline UPALWAYSINLINE
    size_t u32snlen_wc(char32_t const* s, size_t n) noexcept {
        return u32snlen_u16(s, n);
    }

    inline UPALWAYSINLINE
    size_t u32stowcs(wchar_t* UPRESTRICT wcs, char32_t const* UPRESTRICT u32s, size_t n) noexcept {
        return u32stou16s(reinterpret_cast<char16_t*>(wcs), u32s, n);
    }

#elif (WCHAR_MAX <= UINT32_MAX)

    inline UPALWAYSINLINE
    wchar_t const* wcsschk(wchar_t const* wcs) noexcept {
        return reinterpret_cast<wchar_t const*>(u32schk(reinterpret_cast<char32_t const*>(wcs)));
    }
    
    inline UPALWAYSINLINE
    wchar_t const* wcsnchk(wchar_t const* wcs, size_t n) noexcept {
        return reinterpret_cast<wchar_t const*>(u32snchk(reinterpret_cast<char32_t const*>(wcs), n));
    }
    
    inline UPALWAYSINLINE
    int wclen(wchar_t const* wcs) noexcept {
        return u32len(reinterpret_cast<char32_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    int wcnlen(wchar_t const* wcs, size_t n) noexcept {
        return u32nlen(reinterpret_cast<char32_t const*>(wcs), n);
    }

    inline UPALWAYSINLINE
    int u32towc(wchar_t* wcs, char32_t u32) noexcept {
        if (wcs) {
            *wcs = static_cast<wchar_t>(u32);
        }
        return 1;
    }
    
    inline UPALWAYSINLINE
    int u32rtowc(wchar_t* UPRESTRICT wcs, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32towc(wcs, u32);
    }
    
    inline UPALWAYSINLINE
    int wctou32(char32_t* UPRESTRICT u32, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        if (!wcs) {
            return -1;
        }
        if (u32 && (n > 0)) {
            *u32 = static_cast<char32_t>(*wcs);
        }
        return (n > 0) ? 1 : 0;
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u8(wchar_t const* wcs) noexcept {
        return u32slen_u8(reinterpret_cast<char32_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u16(wchar_t const* wcs) noexcept {
        return u32slen_u16(reinterpret_cast<char32_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcslen_u32(wchar_t const* wcs) noexcept {
        return u32slen(reinterpret_cast<char32_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u8(wchar_t const* wcs, size_t n) noexcept {
        return u32snlen_u8(reinterpret_cast<char32_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u16(wchar_t const* wcs, size_t n) noexcept {
        return u32snlen_u16(reinterpret_cast<char32_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsnlen_u32(wchar_t const* wcs, size_t n) noexcept {
        return u32snlen(reinterpret_cast<char32_t const*>(wcs), n);
    }

    inline UPALWAYSINLINE
    size_t wcstou8s(char* UPRESTRICT s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u32stou8s(s, reinterpret_cast<char32_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou16s(char16_t* UPRESTRICT u16s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u32stou16s(u16s, reinterpret_cast<char32_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou32s(char32_t* UPRESTRICT u32s, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        if (n-- > 0) {
            u32memcpy(u32s, reinterpret_cast<char32_t const*>(wcs), n);
            u32s[n] = 0;
            return n;
        }
        return 0;
    }

    inline UPALWAYSINLINE
    size_t u8slen_wc(char const* s) noexcept {
        return u8slen_u32(s);
    }

    inline UPALWAYSINLINE
    size_t u8snlen_wc(char const* s, size_t n) noexcept {
        return u8snlen_u32(s, n);
    }

    inline UPALWAYSINLINE
    size_t u8stowcs(wchar_t* UPRESTRICT wcs, char const* UPRESTRICT s, size_t n) noexcept {
        return u8stou32s(reinterpret_cast<char32_t*>(wcs), s, n);
    }

    inline UPALWAYSINLINE
    size_t u16slen_wc(char16_t const* s) noexcept {
        return u16slen_u32(s);
    }
    
    inline UPALWAYSINLINE
    size_t u16snlen_wc(char16_t const* s, size_t n) noexcept {
        return u16snlen_u32(s, n);
    }

    inline UPALWAYSINLINE
    size_t u16stowcs(wchar_t* UPRESTRICT wcs, char16_t const* UPRESTRICT u16s, size_t n) noexcept {
        return u16stou32s(reinterpret_cast<char32_t*>(wcs), u16s, n);
    }
    
    inline UPALWAYSINLINE
    size_t u32slen_wc(char32_t const* s) noexcept {
        return u32slen(s);
    }

    inline UPALWAYSINLINE
    size_t u32snlen_wc(char32_t const* s, size_t n) noexcept {
        return u32snlen(s, n);
    }
    
    inline UPALWAYSINLINE
    size_t u32stowcs(wchar_t* UPRESTRICT wcs, char32_t const* UPRESTRICT u32s, size_t n) noexcept {
        if (n-- > 0) {
            u32memcpy(reinterpret_cast<char32_t*>(wcs), u32s, n);
            wcs[n] = 0;
            return n;
        }
        return 0;
    }
#else
#   error "Target platform's wchar_t type size not supported!"
#endif
#endif
}

#endif
