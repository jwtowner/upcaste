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

namespace up
{
    LIBUPCOREAPI
    char* setlocale(int category, char const* locale) noexcept {
        char* retval;
        locale_t new_global_locale, old_global_locale;
        size_t native_locale_length;
        char native_locale[1024];

        if ((category < LC_MIN) || (LC_MAX < category)) {
            errno = EINVAL;
            return nullptr;
        }

        if (locale) {
            native_locale_length = detail::native_locale_name(native_locale, locale, sizeof(native_locale));
            if (native_locale_length >= sizeof(native_locale)) {
                errno = EINVAL;
                return nullptr;
            }
        }

        call_once(&detail::global_locale_once, &detail::global_locale_init);
        verify(thrd_success == mtx_lock(&detail::global_locale_guard));
        assert(-1 != ::_configthreadlocale(_DISABLE_PER_THREAD_LOCALE));

        retval = ::setlocale(category, locale ? native_locale : nullptr);
        new_global_locale = ::_get_current_locale();
        if (new_global_locale) {
            old_global_locale = detail::global_locale;
            detail::global_locale = new_global_locale;
            ::_free_locale(old_global_locale);
        }

        verify(thrd_success == mtx_unlock(&detail::global_locale_guard));
        return retval;
    }
}
