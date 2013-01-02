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

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE) && defined(UP_HAS_STDC_WCHAR)

#include <up/filesystem.hpp>
#include <up/cerrno.hpp>
#include <up/cwchar.hpp>

namespace up { namespace filesystem { namespace posix
{
    LIBUPCOREAPI 
    int split_path(wchar_t const* UPRESTRICT p, split_result<wchar_t>* UPRESTRICT result) noexcept {
        return split_path(p, p ? wcslen(p) : 0, result);
    }

    LIBUPCOREAPI
    int split_path(wchar_t const* UPRESTRICT p, size_t n, split_result<wchar_t>* UPRESTRICT result) noexcept {
        wchar_t const* sep, * dirend;
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

        // find last directory seperator
        sep = wcsnrchr(p, L'/', n);
        m = sep - p + 1;

        if (m == n) {
            // eat directory separators at end of potential basename
            sep = wcsnrcchr(p, L'/', m);
            if (!sep) {
                // path consists exclusively of separators
                result->dirname = path<wchar_t>::sep;
                result->dirname_length = path<wchar_t>::sep_length;
                result->basename = path<wchar_t>::empty;
                result->basename_length = path<wchar_t>::empty_length;
                return 0;
            }

            // find start of basename
            n = sep - p + 1;
            sep = wcsnrchr(p, L'/', n);
            m = sep - p + 1;
        }

        if (!sep) {
            // handle no directory name
            result->dirname = path<wchar_t>::curdir;
            result->dirname_length = path<wchar_t>::curdir_length;
            result->basename = p;
            result->basename_length = n;
            return 0;
        }

        // find end character of directory name
        dirend = wcsnrcchr(p, L'/', m);
        if (!dirend) {
            dirend = p;
        }

        // store final results
        result->dirname = p;
        result->dirname_length = dirend - p + 1;
        result->basename = sep + 1;
        result->basename_length = n - m;
        return 0;
    }
}}}

#endif
