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

#include <up/clocale.hpp>
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cstring.hpp>

namespace up
{
    LIBUPCOREAPI UPWARNRESULT
    locale_t duplocale(locale_t locobj) noexcept {
        int config, i, j;
        char* undo_buffer[LC_MAX], *result;
        locale_t new_locale = nullptr;

        if (!locobj) {
            errno = EINVAL;
            return nullptr;
        }

        if (locobj == LC_GLOBAL_LOCALE) {
            return LC_GLOBAL_LOCALE;
        }

        config = ::_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
        assert(config != -1);

        // backup current locale
        for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
            result = ::setlocale(i, nullptr);
            undo_buffer[i - 1] = result ? strdup(result) : nullptr;
        }

        // apply base or default state
        for (i = LC_MIN + 1; i <= LC_MAX; ++i) {
            if (locobj->locinfo->lc_category[i].locale) {
                result = ::setlocale(i, locobj->locinfo->lc_category[i].locale);
            }
            else {
                result = ::setlocale(i, "C");
            }

            if (!result) {
                goto cleanup;
            }
        }

        // construct duplicate locale
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
