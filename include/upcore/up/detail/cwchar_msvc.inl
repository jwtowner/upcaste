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

#ifndef UP_DETAIL_CWCHAR_MSVC_INL
#define UP_DETAIL_CWCHAR_MSVC_INL

#ifndef UP_CWCHAR_HPP
#   error "Do not include this header file directly! Instead include <up/cwchar.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPNONNULL(2,3)
    wchar_t* wcstok(wchar_t* s, wchar_t const* delim, wchar_t** state) noexcept {
        return ::wcstok_s(s, delim, state);
    }

#ifdef UP_HAS_POSIX_WCSCASECMP
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2)
    int wcscasecmp(wchar_t const *s1, wchar_t const *s2) noexcept {
        return ::_wcsicmp(s1, s2);
    }
#endif

#ifdef UP_HAS_POSIX_WCSNCASECMP
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2)
    int wcsncasecmp(wchar_t const *s1, wchar_t const *s2, size_t n) noexcept {
        return ::_wcsnicmp(s1, s2, n);
    }
#endif

#ifdef UP_HAS_POSIX_WCSDUP
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    wchar_t* wcsdup(wchar_t const* s) noexcept {
        return ::_wcsdup(s);
    }
#endif

#ifdef UP_HAS_POSIX_WCSNLEN
    using ::wcsnlen;
#endif

#ifdef UP_HAS_POSIX_LOCALE
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2)
    int wcscasecmp_l(wchar_t const* s1, wchar_t const* s2, ::_locale_t l) noexcept {
        return ::_wcsicmp_l(s1, s2, l);
    }
    
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2)
    int wcsncasecmp_l(wchar_t const* s1, wchar_t const* s2, size_t n, ::_locale_t l) noexcept {
        return ::_wcsnicmp_l(s1, s2, n, l);
    }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2)
    int wcscoll_l(wchar_t const* s1, wchar_t const* s2, ::_locale_t l) noexcept {
        return ::_wcscoll_l(s1, s2, l);
    }

    inline UPALWAYSINLINE UPNONNULL(1,2)
    size_t wcsxfrm_l(wchar_t* UPRESTRICT s1, wchar_t const* UPRESTRICT s2, size_t n, ::_locale_t l) noexcept {
        return ::_wcsxfrm_l(s1, s2, n, l);
    }
#endif
}

#endif
