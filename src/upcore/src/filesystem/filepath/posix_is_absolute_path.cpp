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

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE)

#include <up/filesystem.hpp>
#include <up/cerrno.hpp>

namespace up { namespace filesystem { namespace posix
{
    LIBUPCOREAPI
    int is_absolute_path(char const* UPRESTRICT p, bool* UPRESTRICT result) noexcept {
        if (!result) {
            errno = EINVAL;
            return -1;
        }

        if (!p) {
            *result = false;
            errno = EINVAL;
            return -1;
        }

        *result = (*p == '/');
        return 0;
    }

    LIBUPCOREAPI
    int is_absolute_path(char const* UPRESTRICT p, size_t n, bool* UPRESTRICT result) noexcept {
        if (!result) {
            errno = EINVAL;
            return -1;
        }

        if (!p && n) {
            *result = false;
            errno = EINVAL;
            return -1;
        }

        *result = (n && (*p == '/'));
        return 0;
    }
}}}

#endif
