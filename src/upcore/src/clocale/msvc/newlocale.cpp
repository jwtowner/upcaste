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

#include "clocale_internal.hpp"
#include <up/cstring.hpp>

namespace up
{
    LIBUPCOREAPI UPWARNRESULT
    locale_t newlocale(int category_mask, char const* locale, locale_t base) noexcept {
        int config, i, j;
        char* undo_buffer[LC_MAX], *result;
        locale_t new_locale = nullptr;
        size_t native_locale_length;
        char native_locale[1024];

        if (!locale || (category_mask <= 0) || (LC_ALL_MASK < category_mask) || (base == LC_GLOBAL_LOCALE)) {
            errno = EINVAL;
            return nullptr;
        }

        native_locale_length = detail::native_locale_name(native_locale, locale, sizeof(native_locale));
        if (native_locale_length >= sizeof(native_locale)) {
            errno = EINVAL;
            return nullptr;
        }

        // Fast path for null 'base' and all categories selected. Note that we can't
        // assume that the new locale will cover all categories, which is why can't
        // take this path even when 'base' is non-null, as the new locale may be
        // dependent on 'base'.

        if (!base && (category_mask == LC_ALL_MASK)) {
            // _create_locale doesn't set errno with an invalid locale name encoding
            errno = EINVAL;
            return ::_create_locale(LC_ALL, native_locale);
        }

        // Slow path. Here we set the base locale as thread's current locale, apply
        // each category of the new locale to the current locale using 'setlocale',
        // and then finally use '_get_current_locale' to construct the new locale_t
        // object. We then revert to the original locale before returning.

        config = ::_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
        assert(config != -1);

        // backup current locale
        for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
            result = ::setlocale(i, nullptr);
            undo_buffer[i - 1] = result ? strdup(result) : nullptr;
        }

        // apply base or default state
        for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
            if (base && base->locinfo->lc_category[i].locale) {
                result = ::setlocale(i, base->locinfo->lc_category[i].locale);
            }
            else {
                result = ::setlocale(i, "C");
            }

            if (!result) {
                goto cleanup;
            }
        }

        // apply new category state
        if (category_mask == LC_ALL_MASK) {
            result = ::setlocale(LC_ALL, native_locale);
            if (!result) {
                errno = EINVAL;
                goto cleanup;
            }
        }
        else {
            for (j = LC_MIN + 1; j <= LC_MAX; ++j) {
                if (category_mask & (1 << (j - 1))) {
                    result = ::setlocale(j, native_locale);
                    if (!result) {
                        errno = EINVAL;
                        goto cleanup;
                    }
                }
            }
        }

        // construct new locale
        new_locale = ::_get_current_locale();

    cleanup:

        for (j = LC_MIN + 1; j < i; ++j) {
            ::setlocale(j, undo_buffer[j - 1]);
        }

        verify(-1 != ::_configthreadlocale(config));

        for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
            free(undo_buffer[i - 1]);
        }

        return new_locale;
    }
}
