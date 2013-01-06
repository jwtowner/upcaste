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

#include <up/prolog.hpp>

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE) && defined(UP_HAS_STDC_WCHAR)

#include "filesystem_internal.hpp"
#include <up/climits.hpp>

namespace up { namespace filesystem
{
    LIBUPCOREAPI
    ssize_t transcode(char* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT s) noexcept {
        if ((!d && dsz) || (dsz > INT_MAX) || !s) {
            errno = EINVAL;
            return -1;
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        if (!d) { 
            return static_cast<ssize_t>(wcstou8slen(s));
        }
        wchar_t const* source = s
        size_t length = wcstou8s(d, &source, dsz);
#else
        wchar_t const* source = s;
        size_t length;
        mbstate_t state;
        locale_t old_locale;

        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            // convert to utf-8 character string
            if (!d) { 
                return static_cast<ssize_t>(wcstou8slen(s));
            }
            length = wcstou8s(d, &source, dsz);
        }
        else {
            // convert to multi-byte character string
            old_locale = uselocale(detail::transcode_locale);
            if (!old_locale) {
                return -1;
            }
            memset(&state, 0, sizeof(mbstate_t));
            length = wcsrtombs(d, &source, dsz, &state);
            if (uselocale(old_locale) != detail::transcode_locale) {
                return -1;
            }
            if (!d) {
                return static_cast<ssize_t>(length);
            }
        }
#endif
        
        if (length > INT_MAX) {
            return -1;
        }

        if (source && (dsz > 0)) {
            // make sure result is null-terminated
            if (length > 0) {
                --length;
            }
            d[length] = '\0';
        }

        return static_cast<ssize_t>(length);
    }

    LIBUPCOREAPI
    ssize_t transcode(char* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT s, size_t n) noexcept {
        if ((!d && dsz) || (dsz > INT_MAX) || (!s && n) || (n > INT_MAX)) {
            errno = EINVAL;
            return -1;
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        if (!d) { 
            return static_cast<ssize_t>(wcsntou8slen(s, n));
        }
        wchar_t const* source = s
        size_t length = wcsntou8s(d, &source, n, dsz);
#else
        wchar_t const* source = s;
        size_t length;
        mbstate_t state;
        locale_t old_locale;

        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            // convert to utf-8 character string
            if (!d) { 
                return static_cast<ssize_t>(wcsntou8slen(s, n));
            }
            length = wcsntou8s(d, &source, n, dsz);
        }
        else {
            // convert to multi-byte character string
            old_locale = uselocale(detail::transcode_locale);
            if (!old_locale) {
                return -1;
            }
            memset(&state, 0, sizeof(mbstate_t));
            length = wcsnrtombs(d, &source, n, dsz, &state);
            if (uselocale(old_locale) != detail::transcode_locale) {
                return -1;
            }
            if (!d) {
                return static_cast<ssize_t>(length);
            }
        }
#endif
        
        if (length > INT_MAX) {
            return -1;
        }

        if (source && (dsz > 0)) {
            // make sure result is null-terminated
            if (length > 0) {
                --length;
            }
            d[length] = '\0';
        }

        return static_cast<ssize_t>(length);
    }

    LIBUPCOREAPI
    ssize_t transcode(wchar_t* UPRESTRICT d, size_t dsz, char const* UPRESTRICT s) noexcept {
        if ((!d && dsz) || (dsz > INT_MAX) || !s) {
            errno = EINVAL;
            return -1;
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        if (!d) { 
            return static_cast<ssize_t>(u8stowcslen(s));
        }
        char const* source = s
        size_t length = u8stowcs(d, &source, dsz);
#else
        char const* source = s;
        size_t length;
        mbstate_t state;
        locale_t old_locale;

        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            // convert from utf-8 to wide-character character string
            if (!d) { 
                return static_cast<ssize_t>(u8stowcslen(s));
            }
            length = u8stowcs(d, &source, dsz);
        }
        else {
            // convert to wide-character character string
            old_locale = uselocale(detail::transcode_locale);
            if (!old_locale) {
                return -1;
            }
            memset(&state, 0, sizeof(mbstate_t));
            length = mbsrtowcs(d, &source, dsz, &state);
            if (uselocale(old_locale) != detail::transcode_locale) {
                return -1;
            }
            if (!d) {
                return static_cast<ssize_t>(length);
            }
        }
#endif
        
        if (length > INT_MAX) {
            return -1;
        }

        if (source && (dsz > 0)) {
            // make sure result is null-terminated
            if (length > 0) {
                --length;
            }
            d[length] = L'\0';
        }

        return static_cast<ssize_t>(length);
    }
    
    LIBUPCOREAPI
    ssize_t transcode(wchar_t* UPRESTRICT d, size_t dsz, char const* UPRESTRICT s, size_t n) noexcept {
        if ((!d && dsz) || (dsz > INT_MAX) || (!s && n) || (n > INT_MAX)) {
            errno = EINVAL;
            return -1;
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        if (!d) { 
            return static_cast<ssize_t>(u8sntowcslen(s. n));
        }
        char const* source = s
        size_t length = u8sntowcs(d, &source, n, dsz);
#else
        char const* source = s;
        size_t length;
        mbstate_t state;
        locale_t old_locale;

        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            // convert from utf-8 to wide-character character string
            if (!d) { 
                return static_cast<ssize_t>(u8sntowcslen(s, n));
            }
            length = u8sntowcs(d, &source, n, dsz);
        }
        else {
            // convert to wide-character character string
            old_locale = uselocale(detail::transcode_locale);
            if (!old_locale) {
                return -1;
            }
            memset(&state, 0, sizeof(mbstate_t));
            length = mbsnrtowcs(d, &source, n, dsz, &state);
            if (uselocale(old_locale) != detail::transcode_locale) {
                return -1;
            }
            if (!d) {
                return static_cast<ssize_t>(length);
            }
        }
#endif
        
        if (length > INT_MAX) {
            return -1;
        }

        if (source && (dsz > 0)) {
            // make sure result is null-terminated
            if (length > 0) {
                --length;
            }
            d[length] = L'\0';
        }

        return static_cast<ssize_t>(length);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* transcode(wchar_t const* s) noexcept {
        ssize_t dsz = transcode(nullptr, 0, s);
        if (dsz < 0) {
            return nullptr;
        }

        char* d = static_cast<char*>(malloc(dsz + 1));
        if (!d) {
            return nullptr;
        }

        if (dsz != transcode(d, dsz + 1, s)) {
            free(d);
            return nullptr;
        }

        assert(!d[dsz]);
        return d;
    }
    
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* transcode(wchar_t const* s, size_t n) noexcept {
        ssize_t dsz = transcode(nullptr, 0, s, n);
        if (dsz < 0) {
            return nullptr;
        }

        char* d = static_cast<char*>(malloc(dsz + 1));
        if (!d) {
            return nullptr;
        }

        if (dsz != transcode(d, dsz + 1, s, n)) {
            free(d);
            return nullptr;
        }

        assert(!d[dsz]);
        return d;
    }
    
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* transcode(char const* s) noexcept {
        ssize_t dsz = transcode(nullptr, 0, s);
        if (dsz < 0) {
            return nullptr;
        }

        wchar_t* d = static_cast<wchar_t*>(malloc((dsz + 1) * sizeof(wchar_t)));
        if (!d) {
            return nullptr;
        }

        if (dsz != transcode(d, dsz + 1, s)) {
            free(d);
            return nullptr;
        }

        assert(!d[dsz]);
        return d;
    }
    
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* transcode(char const* s, size_t n) noexcept {
        ssize_t dsz = transcode(nullptr, 0, s, n);
        if (dsz < 0) {
            return nullptr;
        }

        wchar_t* d = static_cast<wchar_t*>(malloc((dsz + 1) * sizeof(wchar_t)));
        if (!d) {
            return nullptr;
        }

        if (dsz != transcode(d, dsz + 1, s, n)) {
            free(d);
            return nullptr;
        }

        assert(!d[dsz]);
        return d;
    }
}}

#endif
