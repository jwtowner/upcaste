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

#ifndef UP_CSTRING_HPP
#define UP_CSTRING_HPP

#include <up/cstddef.hpp>
#include <up/cstdint.hpp>
#include <string.h>

namespace up
{
    using ::memchr;
    using ::memcmp;
    using ::memcpy;
    using ::memmove;
    using ::memset;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    void const* memrchr(void const* s, int c, size_t n) noexcept;
    
    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    void* memrchr(void* s, int c, size_t n) noexcept {
        return const_cast<void*>(memrchr(const_cast<void const*>(s), c, n));
    }

#ifndef UP_HAS_POSIX_MEMCCPY
    extern LIBUPCOREAPI UPNONNULL(1,2)
    void* memccpy(void* UPRESTRICT d, void const* UPRESTRICT s, int c, size_t n) noexcept;
#endif

#ifndef UP_HAS_GNU_MEMPCPY
    inline UPALWAYSINLINE UPNONNULL(1,2)
    void* mempcpy(void* UPRESTRICT d, void const* UPRESTRICT s, size_t n) noexcept {
        return static_cast<char*>(memcpy(d, s, n)) + n;
    }
#else
    using ::mempcpy;
#endif

    extern LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern2(void* UPRESTRICT s, void const* UPRESTRICT p, size_t n) noexcept;

#ifndef UP_HAS_BSD_MEMSET_PATTERN
    extern LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern4(void* UPRESTRICT s, void const* UPRESTRICT p, size_t n) noexcept;
    
    extern LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern8(void* UPRESTRICT s, void const* UPRESTRICT p, size_t n) noexcept;
    
    extern LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern16(void* UPRESTRICT s, void const* UPRESTRICT p, size_t n) noexcept;
#else
    using ::memset_pattern4;
    using ::memset_pattern8;
    using ::memset_pattern16;
#endif

    extern LIBUPCOREAPI UPNONNULL(1,2)
    void* memset_pattern32(void* UPRESTRICT s, void const* UPRESTRICT p, size_t n) noexcept;
    
    using ::strcat;
    using ::strchr;
    using ::strcmp;
    using ::strcpy;
    using ::strcspn;
    using ::strerror;
    using ::strlen;
    using ::strncat;
    using ::strncmp;
    using ::strncpy;
    using ::strpbrk;
    using ::strrchr;
    using ::strspn;
    using ::strstr;
    using ::strtok;
    using ::strcoll;
    using ::strxfrm;

#ifndef UP_HAS_POSIX_STPCPY
    extern LIBUPCOREAPI UPNONNULL(1,2)
    char* stpcpy(char* UPRESTRICT s1, char const* UPRESTRICT s2) noexcept;
#endif

#ifndef UP_HAS_POSIX_STPNCPY
    extern LIBUPCOREAPI UPNONNULL(1,2)
    char* stpncpy(char* UPRESTRICT s1, char const* UPRESTRICT s2, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_STRCASECMP
    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int strcasecmp(char const* s1, char const* s2) noexcept;
#endif

#ifndef UP_HAS_POSIX_STRNCASECMP
    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int strncasecmp(char const* s1, char const* s2, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_STRDUP
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* strdup(char const* s) noexcept;
#endif

#ifndef UP_HAS_POSIX_STRNDUP
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* strndup(char const* s, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_STRNLEN
    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    size_t strnlen(char const* s, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_STRTOK_R
    extern LIBUPCOREAPI UPNONNULL(2,3)
    char* strtok_r(char* UPRESTRICT s, char const* UPRESTRICT delim, char** UPRESTRICT state) noexcept;
#endif
}

#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
#   include <up/detail/cstring_msvc.inl>
#elif UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_POSIX
#   include <up/detail/cstring_posix.inl>
#else
#   error "No string extensions compatability layer for target platform."
#endif

namespace up
{
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* strndup(char const* UPRESTRICT s, size_t n, allocator* UPRESTRICT alloc) noexcept;
   
    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char const* strnchr(char const* s, int c, size_t n) noexcept {
        return static_cast<char const*>(memchr(s, c, n));
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strnchr(char* s, int c, size_t n) noexcept {
        return static_cast<char*>(memchr(s, c, n));
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char const* strnrchr(char const* s, int c, size_t n) noexcept {
        return static_cast<char const*>(memrchr(s, c, n));
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strnrchr(char* s, int c, size_t n) noexcept {
        return static_cast<char*>(memrchr(s, c, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strcchr(char const* s, int c) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strcchr(char* s, int c) noexcept {
        return const_cast<char*>(strcchr(const_cast<char const*>(s), c));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strncchr(char const* s, int c, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strncchr(char* s, int c, size_t n) noexcept {
        return const_cast<char*>(strncchr(const_cast<char const*>(s), c, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strrcchr(char const* s, int c) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strrcchr(char* s, int c) noexcept {
        return const_cast<char*>(strrcchr(const_cast<char const*>(s), c));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strnrcchr(char const* s, int c, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strnrcchr(char* s, int c, size_t n) noexcept {
        return const_cast<char*>(strnrcchr(const_cast<char const*>(s), c, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strcpbrk(char const* s, char const* reject) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strcpbrk(char* s, char const* reject) noexcept {
        return const_cast<char*>(strcpbrk(const_cast<char const*>(s), reject));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strnpbrk(char const* s, char const* accept, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strnpbrk(char* s, char const* accept, size_t n) noexcept {
        return const_cast<char*>(strnpbrk(const_cast<char const*>(s), accept, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strncpbrk(char const* s, char const* reject, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strncpbrk(char* s, char const* reject, size_t n) noexcept {
        return const_cast<char*>(strncpbrk(const_cast<char const*>(s), reject, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strrpbrk(char const* s, char const* accept) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strrpbrk(char* s, char const* accept) noexcept {
        return const_cast<char*>(strrpbrk(const_cast<char const*>(s), accept));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strrcpbrk(char const* s, char const* reject) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strrcpbrk(char* s, char const* reject) noexcept {
        return const_cast<char*>(strrcpbrk(const_cast<char const*>(s), reject));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strnrpbrk(char const* s, char const* accept, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strnrpbrk(char* s, char const* accept, size_t n) noexcept {
        return const_cast<char*>(strnrpbrk(const_cast<char const*>(s), accept, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    char const* strnrcpbrk(char const* s, char const* reject, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    char* strnrcpbrk(char* s, char const* reject, size_t n) noexcept {
        return const_cast<char*>(strnrcpbrk(const_cast<char const*>(s), reject, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t strnspn(char const* s, char const* accept, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t strncspn(char const* s, char const* reject, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t strrspn(char const* s, char const* accept) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t strrcspn(char const* s, char const* reject) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t strnrspn(char const* s, char const* accept, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t strnrcspn(char const* s, char const* reject, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least32_t strhash32(char const* s) noexcept;
    
    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least32_t strnhash32(char const* s, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least64_t strhash64(char const* s) noexcept;
    
    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least64_t strnhash64(char const* s, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    size_t strhash(char const* s) noexcept {
#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
        return strhash32(s);
#else
        return strhash64(s);
#endif
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    size_t strnhash(char const* s, size_t n) noexcept {
#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
        return strnhash32(s, n);
#else
        return strnhash64(s, n);
#endif
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int fast_strcasecmp(char const* s1, char const* s2) noexcept;
    
    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int fast_strncasecmp(char const* s1, char const* s2, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    int fast_strcoll(char const *s1, char const *s2) noexcept;

    extern LIBUPCOREAPI UPNONNULL(1,2)
    size_t fast_strxfrm(char* UPRESTRICT s1, char const* UPRESTRICT s2, size_t n) noexcept;
}

#endif
