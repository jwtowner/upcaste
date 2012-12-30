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

#ifndef UP_DETAIL_CSTRING_MSVC_INL
#define UP_DETAIL_CSTRING_MSVC_INL

#ifndef UP_CSTRING_HPP
#   error "Do not include this header file directly! Instead include <up/cstring.hpp>"
#endif

namespace up
{
#ifdef UP_HAS_POSIX_MEMCCPY
    inline UPALWAYSINLINE UPNONNULL(1,2)
    void* memccpy(void* UPRESTRICT d, void const* UPRESTRICT s, int c, size_t n) noexcept {
        return ::_memccpy(d, s, c, n);
    }
#endif

#ifdef UP_HAS_POSIX_STRCASECMP
    inline UPALWAYSINLINE UPPURE UPNONNULLALL
    int strcasecmp(char const *s1, char const *s2) noexcept {
        return ::_stricmp(s1, s2);
    }
#endif

#ifdef UP_HAS_POSIX_STRNCASECMP
    inline UPALWAYSINLINE UPPURE UPNONNULLALL
    int strncasecmp(char const *s1, char const *s2, size_t n) noexcept {
        return ::_strnicmp(s1, s2, n);
    }
#endif

#ifdef UP_HAS_POSIX_STRDUP
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    char* strdup(char const* s) noexcept {
        return ::_strdup(s);
    }
#endif

#ifdef UP_HAS_POSIX_STRNLEN
    using ::strnlen;
#endif

#ifdef UP_HAS_POSIX_STRTOK_R
    inline UPALWAYSINLINE UPNONNULL(2,3)
    char* strtok_r(char* UPRESTRICT s, char const* UPRESTRICT delim, char** UPRESTRICT state) noexcept {
        return ::strtok_s(s, delim, state);
    }
#endif

#ifdef UP_HAS_POSIX_LOCALE
    inline UPALWAYSINLINE UPPURE UPNONNULLALL
    int strcasecmp_l(char const* s1, char const* s2, ::_locale_t l) noexcept {
        return ::_stricmp_l(s1, s2, l);
    }
    
    inline UPALWAYSINLINE UPPURE UPNONNULLALL
    int strncasecmp_l(char const* s1, char const* s2, size_t n, ::_locale_t l) noexcept {
        return ::_strnicmp_l(s1, s2, n, l);
    }

    inline UPALWAYSINLINE UPPURE UPNONNULLALL
    int strcoll_l(char const* s1, char const* s2, ::_locale_t l) noexcept {
        return ::_strcoll_l(s1, s2, l);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    size_t strxfrm_l(char* UPRESTRICT s1, char const* UPRESTRICT s2, size_t n, ::_locale_t l) noexcept {
        return ::_strxfrm_l(s1, s2, n, l);
    }
#endif
}

#endif
