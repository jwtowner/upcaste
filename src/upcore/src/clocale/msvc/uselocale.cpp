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

namespace up
{
    LIBUPCOREAPI UPWARNRESULT
    locale_t uselocale(locale_t locobj) noexcept {
        int i, j;
        char* undo_buffer[LC_MAX], *result;
        locale_t new_locale, local_locale;

        memset(undo_buffer, 0, sizeof(undo_buffer));
        call_once(&detail::global_locale_once, &detail::global_locale_init);
        verify(-1 != ::_configthreadlocale(_ENABLE_PER_THREAD_LOCALE));

        local_locale = static_cast<locale_t>(tss_get(detail::local_locale_key));
        if (!local_locale) {
            local_locale =  LC_GLOBAL_LOCALE;
        }

        if (locobj == LC_GLOBAL_LOCALE) {
            verify(thrd_success == mtx_lock(&detail::global_locale_guard));
            new_locale = detail::global_locale;
        }
        else {
            new_locale = locobj;
        }

        if (new_locale != nullptr) {
            // backup current locale
            for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
                result = ::setlocale(i, nullptr);
                undo_buffer[i - 1] = result ? strdup(result) : nullptr;
            }

            // apply the new locale
            for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
                if (new_locale->locinfo->lc_category[i].locale) {
                    result = ::setlocale(i, new_locale->locinfo->lc_category[i].locale);
                }
                else {
                    result = ::setlocale(i, "C");
                }

                if (!result) {
                    // revert to current locale
                    for (j = LC_MIN + 1; j < i; ++j) {
                        ::setlocale(j, undo_buffer[j - 1]);
                    }

                    local_locale = nullptr;
                    errno = EINVAL;
                    goto cleanup;
                }
            }

            // save locale for use as return value in future calls to uselocale
            verify(thrd_success == tss_set(detail::local_locale_key, (locobj != LC_GLOBAL_LOCALE) ? new_locale : nullptr));
        }

cleanup:

        if (locobj == LC_GLOBAL_LOCALE) {
            verify(-1 != ::_configthreadlocale(_DISABLE_PER_THREAD_LOCALE));
            verify(thrd_success == mtx_unlock(&detail::global_locale_guard));
        }

        for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
            free(undo_buffer[i - 1]);
        }

        return local_locale;
    }
}
