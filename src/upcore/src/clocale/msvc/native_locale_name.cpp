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

#include "clocale_internal.hpp"
#include <up/cstring.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace up { namespace detail
{
    UPHIDDEN
    size_t native_locale_name(char* native_locale, char const* posix_locale, size_t n) noexcept {
        if (!posix_locale) {
            errno = EINVAL;
            return static_cast<size_t>(-1);
        }

        if (!strcmp(posix_locale, "POSIX")) {
            if (native_locale && (n > 2)) {
                strcpy(native_locale, "C");
            }

            return 1;
        }

        size_t const posix_len = strlen(posix_locale);

        if (native_locale) {
            strncpy(native_locale, posix_locale, n - 1);
            native_locale[n - 1] = '\0';
        }

        return posix_len;
    }
}}
