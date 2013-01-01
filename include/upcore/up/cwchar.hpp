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

#ifndef UP_CWCHAR_HPP
#define UP_CWCHAR_HPP

#include <up/cstddef.hpp>
#include <up/cstdint.hpp>

#ifndef UP_HAS_STDC_WCHAR
#   error "Header file unavailable! Target configuration, compiler, or platform does not support wchar_t!"
#endif

#include <cwchar>

namespace up
{
    using ::std::wmemchr;
    using ::std::wmemcmp;
    using ::std::wmemcpy;
    using ::std::wmemmove;
    using ::std::wmemset;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wmemrchr(wchar_t const* s, wchar_t c, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wmemrchr(wchar_t* s, wchar_t c, size_t n) noexcept {
        return const_cast<wchar_t*>(wmemrchr(const_cast<wchar_t const*>(s), c, n));
    }

#ifndef UP_HAS_POSIX_WMEMCCPY
    extern LIBUPCOREAPI UPNONNULL(1,2)
    wchar_t* wmemccpy(wchar_t* UPRESTRICT d, wchar_t const* UPRESTRICT s, wchar_t c, size_t n) noexcept;
#endif

#ifndef UP_HAS_GNU_WMEMPCPY
    inline UPALWAYSINLINE UPNONNULL(1,2)
    wchar_t* wmempcpy(wchar_t* UPRESTRICT d, wchar_t const* UPRESTRICT s, size_t n) noexcept {
        return wmemcpy(d, s, n) + n;
    }
#else
    using ::wmempcpy;
#endif

    using ::std::mbstate_t;
    using ::std::wint_t;
    using ::std::btowc;
    using ::std::fgetwc;
    using ::std::fgetws;
    using ::std::fwide;
    using ::std::fwprintf;
    using ::std::fwscanf;
    using ::std::getwc;
    using ::std::getwchar;
    using ::std::mbrlen;
    using ::std::mbrtowc;
    using ::std::mbsinit;
    using ::std::mbsrtowcs;
    using ::std::putwc;
    using ::std::putwchar;
    using ::std::swprintf;
    using ::std::swscanf;
    using ::std::ungetwc;
    using ::std::vfwprintf;
    using ::std::vswprintf;
    using ::std::vwprintf;
    using ::std::wcrtomb;
    using ::std::wcscat;
    using ::std::wcschr;
    using ::std::wcscmp;
    using ::std::wcscoll;
    using ::std::wcscpy;
    using ::std::wcscspn;
    using ::std::wcslen;
    using ::std::wcsncat;
    using ::std::wcsncmp;
    using ::std::wcsncpy;
    using ::std::wcspbrk;
    using ::std::wcsrchr;
    using ::std::wcsrtombs;
    using ::std::wcsspn;
    using ::std::wcsstr;
    using ::std::wcstod;
    using ::std::wcstol;
    using ::std::wcstoul;
    using ::std::wcsxfrm;
    using ::std::wctob;
    using ::std::wprintf;
    using ::std::wscanf;

#ifndef UP_HAS_STDC_WCSTOF
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    float wcstof(wchar_t const* UPRESTRICT nptr, wchar_t** UPRESTRICT endptr) noexcept;
#else
    using ::std::wcstof;
#endif

#ifndef UP_HAS_STDC_WCSTOLD
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    long double wcstold(wchar_t const* UPRESTRICT nptr, wchar_t** UPRESTRICT endptr) noexcept;
#else
    using ::std::wcstold;
#endif

#ifndef UP_HAS_STDC_WCSTOLL
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    long long wcstoll(wchar_t const* UPRESTRICT nptr, wchar_t** UPRESTRICT endptr, int base) noexcept;
#else
    using ::std::wcstoll;
#endif

#ifndef UP_HAS_STDC_WCSTOULL
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    unsigned long wcstoull(wchar_t const* UPRESTRICT nptr, wchar_t** UPRESTRICT endptr, int base) noexcept;
#else
    using ::std::wcstoull;
#endif

#ifndef UP_HAS_POSIX_GETWDELIM
    extern LIBUPCOREAPI UPNONNULL(4)
    ssize_t getwdelim(wchar_t** UPRESTRICT lineptr, size_t* UPRESTRICT n, wint_t delimiter, FILE* UPRESTRICT stream) noexcept;
#endif

#ifndef UP_HAS_POSIX_GETWLINE
    extern LIBUPCOREAPI UPNONNULL(3)
    ssize_t getwline(wchar_t** UPRESTRICT lineptr, size_t* UPRESTRICT n, FILE* UPRESTRICT stream) noexcept;
#endif

#ifndef UP_HAS_POSIX_WCPCPY
    extern LIBUPCOREAPI UPNONNULL(1,2)
    wchar_t* wcpcpy(wchar_t* UPRESTRICT s1, wchar_t const* UPRESTRICT s2) noexcept;
#endif

#ifndef UP_HAS_POSIX_WCPNCPY
    extern LIBUPCOREAPI UPNONNULL(1,2)
    wchar_t* wcpncpy(wchar_t* UPRESTRICT s1, wchar_t const* UPRESTRICT s2, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_WCSCASECMP
    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int wcscasecmp(wchar_t const *s1, wchar_t const *s2) noexcept;
#endif

#ifndef UP_HAS_POSIX_WCSNCASECMP
    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int wcsncasecmp(wchar_t const *s1, wchar_t const *s2, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_WCSDUP
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* wcsdup(wchar_t const* s) noexcept;
#endif

#ifndef UP_HAS_GNU_WCSNDUP
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* wcsndup(wchar_t const* s, size_t n) noexcept;
#endif

#ifndef UP_HAS_POSIX_WCSNLEN
    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    size_t wcsnlen(wchar_t const* s, size_t n) noexcept;
#endif

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t const* wcsnchr(wchar_t const* s, wchar_t c, size_t n) noexcept {
        return wmemchr(s, c, n);
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsnchr(wchar_t* s, wchar_t c, size_t n) noexcept {
        return wmemchr(s, c, n);
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t const* wcsnrchr(wchar_t const* s, wchar_t c, size_t n) noexcept {
        return wmemrchr(s, c, n);
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsnrchr(wchar_t* s, wchar_t c, size_t n) noexcept {
        return wmemrchr(s, c, n);
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcscchr(wchar_t const* s, wchar_t c) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcscchr(wchar_t* s, wchar_t c) noexcept {
        return const_cast<wchar_t*>(wcscchr(const_cast<wchar_t const*>(s), c));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsncchr(wchar_t const* s, wchar_t c, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsncchr(wchar_t* s, wchar_t c, size_t n) noexcept {
        return const_cast<wchar_t*>(wcsncchr(const_cast<wchar_t const*>(s), c, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsrcchr(wchar_t const* s, wchar_t c) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsrcchr(wchar_t* s, wchar_t c) noexcept {
        return const_cast<wchar_t*>(wcsrcchr(const_cast<wchar_t const*>(s), c));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsnrcchr(wchar_t const* s, wchar_t c, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsnrcchr(wchar_t* s, wchar_t c, size_t n) noexcept {
        return const_cast<wchar_t*>(wcsnrcchr(const_cast<wchar_t const*>(s), c, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcscpbrk(wchar_t const* s, wchar_t const* reject) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcscpbrk(wchar_t* s, wchar_t const* reject) noexcept {
        return const_cast<wchar_t*>(wcscpbrk(const_cast<wchar_t const*>(s), reject));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsnpbrk(wchar_t const* s, wchar_t const* accept, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsnpbrk(wchar_t* s, wchar_t const* accept, size_t n) noexcept {
        return const_cast<wchar_t*>(wcsnpbrk(const_cast<wchar_t const*>(s), accept, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsncpbrk(wchar_t const* s, wchar_t const* reject, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsncpbrk(wchar_t* s, wchar_t const* reject, size_t n) noexcept {
        return const_cast<wchar_t*>(wcsncpbrk(const_cast<wchar_t const*>(s), reject, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsrpbrk(wchar_t const* s, wchar_t const* accept) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsrpbrk(wchar_t* s, wchar_t const* accept) noexcept {
        return const_cast<wchar_t*>(wcsrpbrk(const_cast<wchar_t const*>(s), accept));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsrcpbrk(wchar_t const* s, wchar_t const* reject) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsrcpbrk(wchar_t* s, wchar_t const* reject) noexcept {
        return const_cast<wchar_t*>(wcsrcpbrk(const_cast<wchar_t const*>(s), reject));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsnrpbrk(wchar_t const* s, wchar_t const* accept, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsnrpbrk(wchar_t* s, wchar_t const* accept, size_t n) noexcept {
        return const_cast<wchar_t*>(wcsnrpbrk(const_cast<wchar_t const*>(s), accept, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    wchar_t const* wcsnrcpbrk(wchar_t const* s, wchar_t const* reject, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    wchar_t* wcsnrcpbrk(wchar_t* s, wchar_t const* reject, size_t n) noexcept {
        return const_cast<wchar_t*>(wcsnrcpbrk(const_cast<wchar_t const*>(s), reject, n));
    }

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t wcsnspn(wchar_t const* s, wchar_t const* accept, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t wcsncspn(wchar_t const* s, wchar_t const* reject, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t wcsrspn(wchar_t const* s, wchar_t const* accept) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t wcsrcspn(wchar_t const* s, wchar_t const* reject) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t wcsnrspn(wchar_t const* s, wchar_t const* accept, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    size_t wcsnrcspn(wchar_t const* s, wchar_t const* reject, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least32_t wcshash32(wchar_t const* s) noexcept;
    
    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least32_t wcsnhash32(wchar_t const* s, size_t n) noexcept;

    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least64_t wcshash64(wchar_t const* s) noexcept;
    
    extern LIBUPCOREAPI UPPURE UPNONNULL(1)
    uint_least64_t wcsnhash64(wchar_t const* s, size_t n) noexcept;

    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    size_t wcshash(wchar_t const* s) noexcept {
#if (SIZE_MAX <= UINT32_MAX)
        return wcshash32(s);
#else
        return wcshash64(s);
#endif
    }
    
    inline UPALWAYSINLINE UPPURE UPNONNULL(1)
    size_t wcsnhash(wchar_t const* s, size_t n) noexcept {
#if (SIZE_MAX <= UINT32_MAX)
        return wcsnhash32(s, n);
#else
        return wcsnhash64(s, n);
#endif
    }
}

#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
#   include <up/detail/cwchar_msvc.inl>
#elif UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_POSIX
#   include <up/detail/cwchar_posix.inl>
#else
#   error No wchar extensions compatability layer for target platform.
#endif

#endif
