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

#include <up/prolog.hpp>

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE) && defined(UP_HAS_STDC_WCHAR)

#include <up/filesystem.hpp>
#include <up/cctype.hpp>
#include <up/cerrno.hpp>
#include <up/cwchar.hpp>

namespace up { namespace filesystem { namespace windows
{
    LIBUPCOREAPI 
    int split_path(wchar_t const* UPRESTRICT p, split_result<wchar_t>* UPRESTRICT result) noexcept {
        return split_path(p, p ? wcslen(p) : 0, result);
    }

    LIBUPCOREAPI
    int split_path(wchar_t const* UPRESTRICT p, size_t n, split_result<wchar_t>* UPRESTRICT result) noexcept {
        wchar_t const* sep, * dirend, * drive;
        size_t m;

        if (!result || (!p && n)) {
            errno = EINVAL;
            return -1;
        }

        // handle zero-length path
        if (!n) {
            result->dirname = path<wchar_t>::curdir;
            result->dirname_length = path<wchar_t>::curdir_length;
            result->basename = path<wchar_t>::empty;
            result->basename_length = path<wchar_t>::empty_length;
            return 0;
        }

        // skip the drive specifier off of the path
        if ((n > 1) && fast_isalpha(static_cast<int>(*p)) && (*(p + 1) == ':')) {
            drive = p;
            p += 2;
            n -= 2;
        }
        else {
            drive = nullptr;
        }

        // find last directory seperator and start of basename
        sep = wcsnrpbrk(p, path<wchar_t>::sepset, n);
        m = sep - p + 1;

        if (m == n) {
            // directory separators at end of path name, trim them off of right-hand side of basename
            sep = wcsnrcpbrk(p, path<wchar_t>::sepset, m);
            if (!sep) {
                // path consists exclusively of separators
                result->dirname = drive ? drive : path<wchar_t>::sep;
                result->dirname_length = drive ? 3 : path<wchar_t>::sep_length;
                result->basename = path<wchar_t>::empty;
                result->basename_length = path<wchar_t>::empty_length;
                return 0;
            }

            // find start of basename
            n = sep - p + 1;
            sep = wcsnrpbrk(p, path<wchar_t>::sepset, n);
            m = sep - p + 1;
        }

        if (!sep) {
            // handle no directory name
            result->dirname = drive ? drive : path<wchar_t>::curdir;
            result->dirname_length = drive ? 2 : path<wchar_t>::curdir_length;
            result->basename = p;
            result->basename_length = n;
            return 0;
        }

        // find end character of directory name
        dirend = wcsnrcpbrk(p, path<wchar_t>::sepset, m);
        if (!dirend) {
            dirend = p;
        }

        // store final results
        result->dirname = drive ? drive : p;
        result->dirname_length = dirend - result->dirname + 1;
        result->basename = sep + 1;
        result->basename_length = n - m;
        return 0;
    }
}}}

#endif
