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

#include "filesystem_internal.hpp"

#ifdef UP_HAS_STDC_WCHAR

namespace up { namespace filesystem { namespace
{
    inline UPALWAYSINLINE UPNONNULL(2)
    size_t transcode_to_utf8(char* UPRESTRICT d, wchar_t const* UPRESTRICT s, size_t n) noexcept {
        if (!d) {
            return wcslen_u8(s);
        }
        return wcstou8s(d, s, n);
    }

    inline UPALWAYSINLINE UPNONNULL(2)
    size_t transcode_from_utf8(wchar_t* UPRESTRICT d, char const* UPRESTRICT s, size_t n) noexcept {
        if (!d) {
            return u8slen_wc(s);
        }
        return u8stowcs(d, s, n);
    }

    inline UPALWAYSINLINE UPNONNULLALL UPALLOC UPWARNRESULT
    char* transcode_to_utf8(wchar_t const* s) noexcept {
        size_t maxlength = wcslen_u8(s);
        char* result = static_cast<char*>(malloc(maxlength + 1));
        if (!result) {
            return nullptr;
        }
        size_t length = wcstou8s(result, s, maxlength + 1);
        assert(length == maxlength);
        return result;            
    }

    inline UPALWAYSINLINE UPNONNULLALL UPALLOC UPWARNRESULT
    wchar_t* transcode_from_utf8(char const* s) noexcept {
        size_t maxlength = u8slen_wc(s);
        wchar_t* result = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (maxlength + 1)));
        if (!result) {
            return nullptr;
        }
        size_t length = u8stowcs(result, s, maxlength + 1);
        assert(length == maxlength);
        return result;
    }
}}}

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULL(2)
    size_t transcode(char* UPRESTRICT d, wchar_t const* UPRESTRICT s, size_t n) noexcept {
        if (!s) {
            errno = EINVAL;
            return size_t(-1);
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        return transcode_to_utf8(d, s, n, ec);
#else
        locale_t old_locale;
        mbstate_t state;
        wchar_t const* cursor;
        size_t length;

        // get current transcode locale info and handle utf-8 fastpath
        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            return transcode_to_utf8(d, s, n);
        }

        old_locale = uselocale(detail::transcode_locale);
        assert(old_locale);

        // convert to multi-byte character string
        cursor = s;
        memset(&state, 0, sizeof(mbstate_t));
        length = wcsrtombs(d, &cursor, n, &state);
        uselocale(old_locale);
        return length;
#endif
    }
    
    LIBUPCOREAPI UPNONNULL(2)
    size_t transcode(wchar_t* UPRESTRICT d, char const* UPRESTRICT s, size_t n) noexcept {
        if (!s) {
            errno = EINVAL;
            return size_t(-1);
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        return transcode_from_utf8(d, s, n, ec);
#else
        locale_t old_locale;
        mbstate_t state;
        char const* cursor;
        size_t length;

        // get current transcode locale info and handle utf-8 fastpath
        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            return transcode_from_utf8(d, s, n);
        }

        old_locale = uselocale(detail::transcode_locale);
        assert(old_locale);

        // convert to wide-character string
        cursor = s;
        memset(&state, 0, sizeof(mbstate_t));
        length = mbsrtowcs(d, &cursor, n, &state);
        uselocale(old_locale);
        return length;
#endif
    }
    
    LIBUPCOREAPI UPALLOC UPWARNRESULT UPNONNULL(1)
    char* transcode(wchar_t const* s) noexcept {
        if (!s) {
            errno = EINVAL;
            return nullptr;
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        return transcode_to_utf8(s);
#else
        size_t length, maxlength;
        char* result = nullptr;
        locale_t old_locale;
        mbstate_t state;
        wchar_t const* cursor;

        // get current transcode locale info and handle utf-8 fastpath
        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            return transcode_to_utf8(s);
        }

        old_locale = uselocale(detail::transcode_locale);
        assert(old_locale);

        // determine size of multi-byte character string
        cursor = s;
        memset(&state, 0, sizeof(mbstate_t));
        maxlength = wcsrtombs(nullptr, &cursor, 0, &state);
        if (maxlength == size_t(-1)) {
            uselocale(old_locale);
            return nullptr;
        }

        // allocate storage for multi-byte character string
        assert(!cursor);
        result = static_cast<char*>(malloc(maxlength + 1));
        if (!result) {
            uselocale(old_locale);
            return nullptr;
        }

        // convert to multi-byte character encoding
        cursor = s;
        memset(&state, 0, sizeof(mbstate_t));
        length = wcsrtombs(result, &cursor, maxlength + 1, &state);
        assert(!cursor && (length == maxlength));

        uselocale(old_locale);
        return result;
#endif
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT UPNONNULL(1)
    wchar_t* transcode(char const* s) noexcept {
        if (!s) {
            errno = EINVAL;
            return nullptr;
        }

#if defined(UP_HAS_UTF8_FILESYSTEM) || !defined(UP_HAS_STDC_LOCALE)
        return transcode_from_utf8(s);
#else
        size_t length, maxlength;
        wchar_t* result = nullptr;
        locale_t old_locale;
        mbstate_t state;
        char const* cursor;

        // get current transcode locale info and handle utf-8 fastpath
        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        if (detail::transcode_locale_is_utf8) {
            return transcode_from_utf8(s);
        }

        old_locale = uselocale(detail::transcode_locale);
        assert(old_locale);

        // determine size of wide-character string
        cursor = s;
        memset(&state, 0, sizeof(mbstate_t));
        maxlength = mbsrtowcs(nullptr, &cursor, 0, &state);
        if (maxlength == size_t(-1)) {
            uselocale(old_locale);
            return nullptr;
        }

        // allocate storage for wide-character string
        assert(!cursor);
        result = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (maxlength + 1)));
        if (!result) {
            uselocale(old_locale);
            return nullptr;
        }

        // convert to wide-character encoding
        cursor = s;
        memset(&state, 0, sizeof(mbstate_t));
        length = mbsrtowcs(result, &cursor, maxlength + 1, &state);
        assert(!cursor && (length == maxlength));
        
        uselocale(old_locale);
        return result;
#endif
    }
}}

#endif
