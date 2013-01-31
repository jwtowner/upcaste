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

#ifndef UP_DETAIL_TCHAR_WCHAR_INL
#define UP_DETAIL_TCHAR_WCHAR_INL

#ifndef UP_TCHAR_HPP
#   error "Do not include this header file directly! Instead include <up/tchar.hpp>"
#endif

#ifdef UP_HAS_POSIX_LOCALE
#   include <up/clocale.hpp>
#endif

#include <up/cwchar.hpp>

namespace up
{
    typedef wchar_t tchar_t;
    typedef wint_t tint_t;

    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT float tcstof(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr) noexcept { return wcstof(nptr, endptr); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT double tcstod(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr) noexcept { return wcstod(nptr, endptr); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT long double tcstold(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr) noexcept { return wcstold(nptr, endptr); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT long tcstol(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return wcstol(nptr, endptr, base); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT long long tcstoll(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return wcstoll(nptr, endptr, base); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT unsigned long tcstoul(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return wcstoul(nptr, endptr, base); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT unsigned long long tcstoull(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return wcstoull(nptr, endptr, base); }

    inline UPALWAYSINLINE UPNONNULL(1) tint_t fgettc(FILE* stream) noexcept { return fgetwc(stream); }
    inline UPALWAYSINLINE UPNONNULL(1,3) tchar_t* fgetts(tchar_t* UPRESTRICT s, int n, FILE* UPRESTRICT stream) { return fgetws(s, n, stream); }
    inline UPALWAYSINLINE UPNONNULL(1) tint_t gettc(FILE* stream) noexcept { return getwc(stream); }
    inline UPALWAYSINLINE tint_t gettchar() noexcept { return getwchar(); }
    inline UPALWAYSINLINE UPNONNULL(4) ssize_t gettdelim(tchar_t** UPRESTRICT lineptr, size_t* UPRESTRICT n, tint_t delimiter, FILE* UPRESTRICT stream) noexcept { return getwdelim(lineptr, n, delimiter, stream); }
    inline UPALWAYSINLINE UPNONNULL(3) ssize_t gettline(tchar_t** UPRESTRICT lineptr, size_t* UPRESTRICT n, FILE* UPRESTRICT stream) noexcept { return getwline(lineptr, n, stream); }
    inline UPALWAYSINLINE UPNONNULL(2) tint_t puttc(tchar_t c, FILE* stream) noexcept { return putwc(c, stream); }
    inline UPALWAYSINLINE tint_t puttchar(tchar_t c) noexcept { return putwchar(c); }
    inline UPALWAYSINLINE UPNONNULL(2) tint_t ungettc(tchar_t c, FILE* stream) noexcept { return ungetwc(c, stream); }
    inline UPALWAYSINLINE UPNONNULL(1,2) int vftprintf(FILE* UPRESTRICT stream, tchar_t const* UPRESTRICT format, va_list arg) noexcept { return vfwprintf(stream, format, arg); };
    inline UPALWAYSINLINE UPNONNULL(1,3) int vstprintf(tchar_t* UPRESTRICT s, size_t n, tchar_t const* UPRESTRICT format, va_list arg) noexcept { return vswprintf(s, n, format, arg); };
    inline UPALWAYSINLINE UPNONNULL(1) int vtprintf(tchar_t const* UPRESTRICT format, va_list arg) noexcept { return vwprintf(format, arg); };

    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcscat(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2) noexcept { return wcscat(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcschr(tchar_t const* s, tint_t c) noexcept { return wcschr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcschr(tchar_t* s, tint_t c) noexcept { return wcschr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscmp(tchar_t const* s1, tchar_t const* s2) noexcept { return wcscmp(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscoll(tchar_t const* s1, tchar_t const* s2) noexcept { return wcscoll(s1, s2); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcscpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2) noexcept { return wcscpy(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcscspn(tchar_t const* s1, tchar_t const* s2) noexcept { return wcscspn(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) size_t tcslen(tchar_t const* s) noexcept { return wcslen(s); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcsncat(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return wcsncat(s1, s2, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcsncmp(tchar_t const* s1, tchar_t const* s2, size_t n) noexcept { return wcsncmp(s1, s2, n); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcsncpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return wcsncpy(s1, s2, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t const* tcspbrk(tchar_t const* s1, tchar_t const* s2) noexcept { return wcspbrk(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t* tcspbrk(tchar_t* s1, tchar_t* s2) noexcept { return wcspbrk(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrchr(tchar_t const* s, tint_t c) noexcept { return wcsrchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrchr(tchar_t* s, tint_t c) noexcept { return wcsrchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsspn(tchar_t const* s1, tchar_t const* s2) noexcept { return wcsspn(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t const* tcsstr(tchar_t const* s1, tchar_t const* s2) noexcept { return wcsstr(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t* tcsstr(tchar_t* s1, tchar_t const* s2) noexcept { return wcsstr(s1, s2); }
    inline UPALWAYSINLINE UPNONNULL(2,3) tchar_t* tcstok(tchar_t* UPRESTRICT s, tchar_t const* UPRESTRICT delim, tchar_t** UPRESTRICT state) noexcept { return wcstok(s, delim, state); }
    inline UPALWAYSINLINE UPNONNULL(1,2) size_t tcsxfrm(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return wcsxfrm(s1, s2, n); }

    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcpcpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2) noexcept { return wcpcpy(s1, s2); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcpncpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return wcpncpy(s1, s2, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscasecmp(tchar_t const* s1, tchar_t const* s2) noexcept { return wcscasecmp(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcsncasecmp(tchar_t const* s1, tchar_t const* s2, size_t n) noexcept { return wcsncasecmp(s1, s2, n); }
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT tchar_t* tcsdup(tchar_t const* s) noexcept { return wcsdup(s); }
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT tchar_t* tcsndup(tchar_t const* s, size_t n) noexcept { return wcsndup(s, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) size_t tcsnlen(tchar_t const* s, size_t n) noexcept { return wcsnlen(s, n); }

#ifdef UP_HAS_POSIX_LOCALE
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscasecmp_l(tchar_t const* s1, tchar_t const* s2, locale_t l) noexcept { return wcscasecmp_l(s1, s2, l); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcsncasecmp_l(tchar_t const* s1, tchar_t const* s2, size_t n, locale_t l) noexcept { return wcsncasecmp_l(s1, s2, n, l); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscoll_l(tchar_t const* s1, tchar_t const* s2, locale_t l) noexcept { return wcscoll_l(s1, s2, l); }
    inline UPALWAYSINLINE UPNONNULL(1,2) size_t tcsxfrm_l(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n, locale_t l) noexcept { return wcsxfrm_l(s1, s2, n, l); }
#endif

    inline UPALWAYSINLINE UPPURE UPNONNULL(1) uint_least64_t tcshash(tchar_t const* s, uint_least64_t seed = 0) noexcept { return wcshash(s, seed); }   
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) uint_least64_t tcsnhash(tchar_t const* s, size_t n, uint_least64_t seed = 0) noexcept { return wcsnhash(s, n, seed); }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcscchr(tchar_t const* s, tint_t c) noexcept { return wcscchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcscchr(tchar_t* s, tint_t c) noexcept { return wcscchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsncchr(tchar_t const* s, tint_t c, size_t n) noexcept { return wcsncchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsncchr(tchar_t* s, tint_t c, size_t n) noexcept { return wcsncchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnchr(tchar_t const* s, tint_t c, size_t n) noexcept { return wcsnchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnchr(tchar_t* s, tint_t c, size_t n) noexcept { return wcsnchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrcchr(tchar_t const* s, tint_t c, size_t n) noexcept { return wcsnrcchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrcchr(tchar_t* s, tint_t c, size_t n) noexcept { return wcsnrcchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrchr(tchar_t const* s, tint_t c, size_t n) noexcept { return wcsnrchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrchr(tchar_t* s, tint_t c, size_t n) noexcept {  return wcsnrchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrcchr(tchar_t const* s, tint_t c) noexcept { return wcsrcchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrcchr(tchar_t* s, tint_t c) noexcept { return wcsrcchr(s, c); }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcscpbrk(tchar_t const* s, tchar_t const* reject) noexcept { return wcscpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcscpbrk(tchar_t* s, tchar_t const* reject) noexcept { return wcscpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsncpbrk(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return wcsncpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsncpbrk(tchar_t* s, tchar_t const* reject, size_t n) noexcept { return wcsncpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnpbrk(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return wcsnpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnpbrk(tchar_t* s, tchar_t const* accept, size_t n) noexcept { return wcsnpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrcpbrk(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return wcsnrcpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrcpbrk(tchar_t* s, tchar_t const* reject, size_t n) noexcept { return wcsnrcpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrpbrk(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return wcsnrpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrpbrk(tchar_t* s, tchar_t const* accept, size_t n) noexcept { return wcsnrpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrcpbrk(tchar_t const* s, tchar_t const* reject) noexcept { return wcsrcpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrcpbrk(tchar_t* s, tchar_t const* reject) noexcept { return wcsrcpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrpbrk(tchar_t const* s, tchar_t const* accept) noexcept { return wcsrpbrk(s, accept); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrpbrk(tchar_t* s, tchar_t const* accept) noexcept { return wcsrpbrk(s, accept); }
    
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsnspn(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return wcsnspn(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsncspn(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return wcsncspn(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsrspn(tchar_t const* s, tchar_t const* accept) noexcept { return wcsrspn(s, accept); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsrcspn(tchar_t const* s, tchar_t const* reject) noexcept { return wcsrcspn(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsnrspn(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return wcsnrspn(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsnrcspn(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return wcsnrcspn(s, reject, n); }
}

#endif
