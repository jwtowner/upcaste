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

namespace up { namespace filesystem
{
    LIBUPCOREAPI
    ssize_t transcode(char* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT s) noexcept {
        return transcode(d, dsz, s, s ? wcslen(s) : 0);
    }

    LIBUPCOREAPI
    ssize_t transcode(char* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT s, size_t n) noexcept {
        int retval = 0, length = 0;

        if ((!d && dsz) || (dsz > INT_MAX) || (!s && n) || (n > INT_MAX)) {
            errno = EINVAL;
            return -1;
        }
        
        if (n) {
            retval = ::WideCharToMultiByte(CP_ACP, 0, s, static_cast<int>(n), NULL, 0, NULL, NULL);
            if (retval <= 0) {
                set_errno_with_last_oserror();
                return -1;
            }
        }

        if (d && (dsz > 0)) {
            if (n) {
                length = ::WideCharToMultiByte(CP_ACP, 0, s, static_cast<int>(n), d, static_cast<int>(dsz), NULL, NULL);
                if (length <= 0) {
                    d[0] = '\0';
                    set_errno_with_last_oserror();
                    return -1;
                }
            }

            d[(length >= static_cast<int>(dsz)) ? (dsz - 1) : length] = '\0';
        }

        return retval;
    }

    LIBUPCOREAPI
    ssize_t transcode(wchar_t* UPRESTRICT d, size_t dsz, char const* UPRESTRICT s) noexcept {
        return transcode(d, dsz, s, s ? strlen(s) : 0);
    }

    LIBUPCOREAPI
    ssize_t transcode(wchar_t* UPRESTRICT d, size_t dsz, char const* UPRESTRICT s, size_t n) noexcept {
        int retval = 0, length = 0;

        if ((!d && dsz) || (dsz > INT_MAX) || (!s && n) || (n > INT_MAX)) {
            errno = EINVAL;
            return -1;
        }
        
        if (n) {
            retval = ::MultiByteToWideChar(CP_ACP, 0, s, static_cast<int>(n), NULL, 0);
            if (retval <= 0) {
                set_errno_with_last_oserror();
                return -1;
            }
        }

        if (d && (dsz > 0)) {
            if (n) {
                length = ::MultiByteToWideChar(CP_ACP, 0, s, static_cast<int>(n), d, static_cast<int>(dsz));
                if (length <= 0) {
                    d[0] = L'\0';
                    set_errno_with_last_oserror();
                    return -1;
                }
            }

            d[(length >= static_cast<int>(dsz)) ? (dsz - 1) : length] = L'\0';
        }

        return retval;
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* transcode(wchar_t const* s) noexcept {
        if (!s) {
            errno = EINVAL;
            return nullptr;
        }

        return transcode(s, wcslen(s));
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* transcode(wchar_t const* s, size_t n) noexcept {
        int length = 0;
        char* retval;

        if ((!s && n) || (n > INT_MAX)) {
            errno = EINVAL;
            return nullptr;
        }
        
        if (n) {
            length = ::WideCharToMultiByte(CP_ACP, 0, s, static_cast<int>(n), NULL, 0, NULL, NULL);
            if (length <= 0) {
                set_errno_with_last_oserror();
                return nullptr;
            }
        }

        retval = static_cast<char*>(malloc(length + 1));
        if (!retval) {
            return nullptr;
        }

        if (n) {
            if (length != ::WideCharToMultiByte(CP_ACP, 0, s, static_cast<int>(n), retval, length, NULL, NULL)) {
                set_errno_with_last_oserror();
                free(retval);
                return nullptr;
            }
        }

        retval[length] = '\0';
        return retval;
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* transcode(char const* s) noexcept {
        if (!s) {
            errno = EINVAL;
            return nullptr;
        }

        return transcode(s, strlen(s));        
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* transcode(char const* s, size_t n) noexcept {
        int length = 0;
        wchar_t* retval;

        if ((!s && n) || (n > INT_MAX)) {
            errno = EINVAL;
            return nullptr;
        }
        
        if (n) {
            length = ::MultiByteToWideChar(CP_ACP, 0, s, static_cast<int>(n), NULL, 0);
            if (length <= 0) {
                set_errno_with_last_oserror();
                return nullptr;
            }
        }

        retval = static_cast<wchar_t*>(malloc((length + 1) * sizeof(wchar_t)));
        if (!retval) {
            return nullptr;
        }

        if (n) {
            if (length != ::MultiByteToWideChar(CP_ACP, 0, s, static_cast<int>(n), retval, length)) {
                set_errno_with_last_oserror();
                free(retval);
                return nullptr;
            }
        }

        retval[length] = L'\0';
        return retval;
    }
}}

#endif
