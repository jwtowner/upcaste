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

#ifndef UP_DETAIL_TCHAR_CHAR_INL
#define UP_DETAIL_TCHAR_CHAR_INL

#ifndef UP_TCHAR_HPP
#   error "Do not include this header file directly! Instead include <up/tchar.hpp>"
#endif

#ifdef UP_HAS_POSIX_LOCALE
#   include <up/clocale.hpp>
#endif

#include <up/cstdio.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>

namespace up
{
    typedef char tchar_t;
    typedef int tint_t;

    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT float tcstof(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr) noexcept { return strtof(nptr, endptr); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT double tcstod(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr) noexcept { return strtod(nptr, endptr); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT long double tcstold(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr) noexcept { return strtold(nptr, endptr); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT long tcstol(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return strtol(nptr, endptr, base); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT long long tcstoll(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return strtoll(nptr, endptr, base); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT unsigned long tcstoul(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return strtoul(nptr, endptr, base); }
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT unsigned long long tcstoull(tchar_t const* UPRESTRICT nptr, tchar_t** UPRESTRICT endptr, int base) noexcept { return strtoull(nptr, endptr, base); }

    inline UPALWAYSINLINE UPNONNULL(1) tint_t fgettc(FILE* stream) noexcept { return fgetc(stream); }
    inline UPALWAYSINLINE UPNONNULL(1,3) tchar_t* fgetts(tchar_t* UPRESTRICT s, int n, FILE* UPRESTRICT stream) { return fgets(s, n, stream); }
    inline UPALWAYSINLINE UPNONNULL(1) tint_t gettc(FILE* stream) noexcept { return getc(stream); }
    inline UPALWAYSINLINE tint_t gettchar() noexcept { return getchar(); }
    inline UPALWAYSINLINE UPNONNULL(4) ssize_t gettdelim(tchar_t** UPRESTRICT lineptr, size_t* UPRESTRICT n, tint_t delimiter, FILE* UPRESTRICT stream) noexcept { return getdelim(lineptr, n, delimiter, stream); }
    inline UPALWAYSINLINE UPNONNULL(3) ssize_t gettline(tchar_t** UPRESTRICT lineptr, size_t* UPRESTRICT n, FILE* UPRESTRICT stream) noexcept { return getline(lineptr, n, stream); }
    inline UPALWAYSINLINE UPNONNULL(2) tint_t puttc(tchar_t c, FILE* stream) noexcept { return putc(c, stream); }
    inline UPALWAYSINLINE tint_t puttchar(tchar_t c) noexcept { return putchar(c); }
    inline UPALWAYSINLINE UPNONNULL(2) tint_t ungettc(tchar_t c, FILE* stream) noexcept { return ungetc(c, stream); }
    inline UPALWAYSINLINE UPNONNULL(1,2) int vftprintf(FILE* UPRESTRICT stream, tchar_t const* UPRESTRICT format, va_list arg) noexcept { return vfprintf(stream, format, arg); };
    inline UPALWAYSINLINE UPNONNULL(1,3) int vstprintf(tchar_t* UPRESTRICT s, size_t n, tchar_t const* UPRESTRICT format, va_list arg) noexcept { return vsnprintf(s, n, format, arg); };
    inline UPALWAYSINLINE UPNONNULL(1) int vtprintf(tchar_t const* UPRESTRICT format, va_list arg) noexcept { return vprintf(format, arg); };

    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcscat(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2) noexcept { return strcat(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcschr(tchar_t const* s, tint_t c) noexcept { return strchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcschr(tchar_t* s, tint_t c) noexcept { return strchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscmp(tchar_t const* s1, tchar_t const* s2) noexcept { return strcmp(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscoll(tchar_t const* s1, tchar_t const* s2) noexcept { return strcoll(s1, s2); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcscpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2) noexcept { return strcpy(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcscspn(tchar_t const* s1, tchar_t const* s2) noexcept { return strcspn(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) size_t tcslen(tchar_t const* s) noexcept { return strlen(s); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcsncat(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return strncat(s1, s2, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcsncmp(tchar_t const* s1, tchar_t const* s2, size_t n) noexcept { return strncmp(s1, s2, n); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcsncpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return strncpy(s1, s2, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t const* tcspbrk(tchar_t const* s1, tchar_t const* s2) noexcept { return strpbrk(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t* tcspbrk(tchar_t* s1, tchar_t* s2) noexcept { return strpbrk(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrchr(tchar_t const* s, tint_t c) noexcept { return strrchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrchr(tchar_t* s, tint_t c) noexcept { return strrchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsspn(tchar_t const* s1, tchar_t const* s2) noexcept { return strspn(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t const* tcsstr(tchar_t const* s1, tchar_t const* s2) noexcept { return strstr(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) tchar_t* tcsstr(tchar_t* s1, tchar_t const* s2) noexcept { return strstr(s1, s2); }
    inline UPALWAYSINLINE UPNONNULL(2,3) tchar_t* tcstok(tchar_t* UPRESTRICT s, tchar_t const* UPRESTRICT delim, tchar_t** UPRESTRICT state) noexcept { return strtok_r(s, delim, state); }
    inline UPALWAYSINLINE UPNONNULL(1,2) size_t tcsxfrm(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return strxfrm(s1, s2, n); }

    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcpcpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2) noexcept { return stpcpy(s1, s2); }
    inline UPALWAYSINLINE UPNONNULL(1,2) tchar_t* tcpncpy(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n) noexcept { return stpncpy(s1, s2, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscasecmp(tchar_t const* s1, tchar_t const* s2) noexcept { return strcasecmp(s1, s2); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcsncasecmp(tchar_t const* s1, tchar_t const* s2, size_t n) noexcept { return strncasecmp(s1, s2, n); }
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT tchar_t* tcsdup(tchar_t const* s) noexcept { return strdup(s); }
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT tchar_t* tcsndup(tchar_t const* s, size_t n) noexcept { return strndup(s, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) size_t tcsnlen(tchar_t const* s, size_t n) noexcept { return strnlen(s, n); }

#ifdef UP_HAS_POSIX_LOCALE
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscasecmp_l(tchar_t const* s1, tchar_t const* s2, locale_t l) noexcept { return strcasecmp_l(s1, s2, l); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcsncasecmp_l(tchar_t const* s1, tchar_t const* s2, size_t n, locale_t l) noexcept { return strncasecmp_l(s1, s2, n, l); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) int tcscoll_l(tchar_t const* s1, tchar_t const* s2, locale_t l) noexcept { return strcoll_l(s1, s2, l); }
    inline UPALWAYSINLINE UPNONNULL(1,2) size_t tcsxfrm_l(tchar_t* UPRESTRICT s1, tchar_t const* UPRESTRICT s2, size_t n, locale_t l) noexcept { return strxfrm_l(s1, s2, n, l); }
#endif

    inline UPALWAYSINLINE UPPURE UPNONNULL(1) uint_least64_t tcshash(tchar_t const* s, uint_least64_t seed = 0) noexcept { return strhash(s, seed); }   
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) uint_least64_t tcsnhash(tchar_t const* s, size_t n, uint_least64_t seed = 0) noexcept { return strnhash(s, n, seed); }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcscchr(tchar_t const* s, tint_t c) noexcept { return strcchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcscchr(tchar_t* s, tint_t c) noexcept { return strcchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsncchr(tchar_t const* s, tint_t c, size_t n) noexcept { return strncchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsncchr(tchar_t* s, tint_t c, size_t n) noexcept { return strncchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnchr(tchar_t const* s, tint_t c, size_t n) noexcept { return strnchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnchr(tchar_t* s, tint_t c, size_t n) noexcept { return strnchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrcchr(tchar_t const* s, tint_t c, size_t n) noexcept { return strnrcchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrcchr(tchar_t* s, tint_t c, size_t n) noexcept { return strnrcchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrchr(tchar_t const* s, tint_t c, size_t n) noexcept { return strnrchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrchr(tchar_t* s, tint_t c, size_t n) noexcept {  return strnrchr(s, c, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrcchr(tchar_t const* s, tint_t c) noexcept { return strrcchr(s, c); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrcchr(tchar_t* s, tint_t c) noexcept { return strrcchr(s, c); }

    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcscpbrk(tchar_t const* s, tchar_t const* reject) noexcept { return strcpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcscpbrk(tchar_t* s, tchar_t const* reject) noexcept { return strcpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsncpbrk(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return strncpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsncpbrk(tchar_t* s, tchar_t const* reject, size_t n) noexcept { return strncpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnpbrk(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return strnpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnpbrk(tchar_t* s, tchar_t const* accept, size_t n) noexcept { return strnpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrcpbrk(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return strnrcpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrcpbrk(tchar_t* s, tchar_t const* reject, size_t n) noexcept { return strnrcpbrk(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsnrpbrk(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return strnrpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsnrpbrk(tchar_t* s, tchar_t const* accept, size_t n) noexcept { return strnrpbrk(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrcpbrk(tchar_t const* s, tchar_t const* reject) noexcept { return strrcpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrcpbrk(tchar_t* s, tchar_t const* reject) noexcept { return strrcpbrk(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t const* tcsrpbrk(tchar_t const* s, tchar_t const* accept) noexcept { return strrpbrk(s, accept); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1) tchar_t* tcsrpbrk(tchar_t* s, tchar_t const* accept) noexcept { return strrpbrk(s, accept); }
    
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsnspn(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return strnspn(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsncspn(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return strncspn(s, reject, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsrspn(tchar_t const* s, tchar_t const* accept) noexcept { return strrspn(s, accept); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsrcspn(tchar_t const* s, tchar_t const* reject) noexcept { return strrcspn(s, reject); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsnrspn(tchar_t const* s, tchar_t const* accept, size_t n) noexcept { return strnrspn(s, accept, n); }
    inline UPALWAYSINLINE UPPURE UPNONNULL(1,2) size_t tcsnrcspn(tchar_t const* s, tchar_t const* reject, size_t n) noexcept { return strnrcspn(s, reject, n); }
}

#endif
