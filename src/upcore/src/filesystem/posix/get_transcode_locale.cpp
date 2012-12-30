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

#include "filesystem_internal.hpp"

#ifdef UP_HAS_STDC_LOCALE

#include <up/clocale.hpp>
#include <langinfo.h>

namespace up { namespace filesystem { namespace detail
{
    namespace
    {
        void UPCDECL transcode_locale_term() noexcept {
            freelocale(transcode_locale);
            transcode_locale = nullptr;
        }
    }

    UPHIDDEN once_flag transcode_locale_guard = ONCE_FLAG_INIT;
    UPHIDDEN locale_t transcode_locale = nullptr;
    UPHIDDEN bool transcode_locale_is_utf8 = false;

    UPHIDDEN
    void UPCDECL transcode_locale_init() noexcept {
        transcode_locale = newlocale(LC_ALL_MASK, "", nullptr);
        assert(transcode_locale);

        locale_t old_locale = uselocale(transcode_locale);
        assert(old_locale);

        char const* codeset = ::nl_langinfo(CODESET);
        assert(codeset);
            
        if ( (fast_strcasecmp(codeset, "utf-8") == 0)
            || (fast_strcasecmp(codeset, "utf_8") == 0)
            || (fast_strcasecmp(codeset, "utf8") == 0)
        ) {
            transcode_locale_is_utf8 = true;
        }
        else {
            transcode_locale_is_utf8 = false;
        }

        verify(transcode_locale == uselocale(old_locale));
        verify(!atexit(&transcode_locale_term));
    }
}}}

namespace up { namespace filesystem
{
    LIBUPCOREAPI
    locale_t get_transcode_locale() noexcept {
        call_once(&detail::transcode_locale_guard, &detail::transcode_locale_init);
        return detail::transcode_locale;
    }
}}

#endif
