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

#ifndef UP_CLOCALE_MSVC_CLOCALE_INTERNAL_HPP
#define UP_CLOCALE_MSVC_CLOCALE_INTERNAL_HPP

#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/clocale.hpp>
#include <up/cthreads.hpp>

namespace up { namespace detail
{
    extern UPHIDDEN once_flag global_locale_once;
    extern UPHIDDEN mtx_t global_locale_guard;
    extern UPHIDDEN locale_t global_locale;
    extern UPHIDDEN tss_t local_locale_key;

    extern UPHIDDEN
    void UPCDECL global_locale_init() noexcept;

    extern UPHIDDEN
    size_t native_locale_name(char* native_locale, char const* posix_locale, size_t n) noexcept;
}}

#endif
