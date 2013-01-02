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
#include <up/cstdlib.hpp>

namespace up { namespace detail
{
    UPHIDDEN once_flag global_locale_once = ONCE_FLAG_INIT;
    UPHIDDEN mtx_t global_locale_guard = nullptr;
    UPHIDDEN locale_t global_locale = nullptr;
    UPHIDDEN tss_t local_locale_key = 0;

    namespace
    {
        void UPCDECL global_locale_term() noexcept {
            verify(thrd_success == mtx_lock(&global_locale_guard));
            ::_free_locale(global_locale);
            global_locale = nullptr;
            verify(thrd_success == mtx_unlock(&global_locale_guard));
            tss_delete(local_locale_key);
            mtx_destroy(&global_locale_guard);
        }

        void UPCDECL local_locale_dtor(void* data) noexcept {
            ::_free_locale(static_cast<locale_t>(data));
        }
    }

    UPHIDDEN
    void UPCDECL global_locale_init() noexcept {
        int config = ::_configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
        assert(config != -1);
        global_locale = ::_get_current_locale();
        verify(-1 != ::_configthreadlocale(config));
        verify(thrd_success == mtx_init(&global_locale_guard, mtx_plain));
        verify(thrd_success == tss_create(&local_locale_key, &local_locale_dtor));
        verify(!atexit(&global_locale_term));
    }
}}
