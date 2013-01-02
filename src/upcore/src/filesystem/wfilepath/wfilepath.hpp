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

#ifndef UP_FILESYSTEM_WFILEPATH_HPP
#define UP_FILESYSTEM_WFILEPATH_HPP

#include <up/filesystem.hpp>
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdlib.hpp>
#include <up/cwchar.hpp>

namespace up { namespace filesystem { namespace detail
{
    inline UPHIDDENINLINE UPNONNULLALL
    size_t join_path_count_args(wchar_t const* first, va_list args) noexcept {
        wchar_t const* cur = first;
        size_t count = 0;

        while (cur) {
            ++count;
            cur = va_arg(args, wchar_t const*);
        }

        return count;
    }

    inline UPHIDDENINLINE UPNONNULLALL
    size_t join_path_copy_args(wchar_t const** UPRESTRICT dest, size_t dest_max, wchar_t const* UPRESTRICT first, va_list args) noexcept {
        wchar_t const* cur = first;
        size_t count = 0;

        while (cur && (count < dest_max)) {
            dest[count++] = cur;
            cur = va_arg(args, wchar_t const*);
        }

        return count;
    }
}}}

#endif
