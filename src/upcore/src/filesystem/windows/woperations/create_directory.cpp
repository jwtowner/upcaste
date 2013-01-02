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

#include "../filesystem_internal.hpp"

namespace up { namespace filesystem
{
    LIBUPCOREAPI
    int create_directory(wchar_t const* p) noexcept {
        BOOL result;
        wchar_t* pext;
        size_t plen;

        if (!p) {
            errno = EINVAL;
            return -1;
        }

        plen = wcslen(p);
        if (plen >= 248) {
            pext = static_cast<wchar_t*>(malloca((plen + 16) * sizeof(wchar_t)));
            if (!pext) {
                return -1;
            }

            wcscpy(pext, L"\\\\?\\");
            wcscpy(pext + 4, p);
            result = ::CreateDirectoryW(pext, NULL);
            freea(pext);
        }
        else {
            result = ::CreateDirectoryW(p, NULL);
        }

        if (!result) {
            set_errno_with_last_oserror();
            return -1;
        }

        return 0;
    }
}}
