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

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE) 

#include <up/filesystem.hpp>
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cctype.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/cwchar.hpp>

namespace up { namespace filesystem { namespace windows
{
    LIBUPCOREAPI
    ssize_t normalize_path(wchar_t* UPRESTRICT dest, size_t dest_max, wchar_t const* UPRESTRICT p) noexcept {
        return normalize_path(dest, dest_max, p, p ? wcslen(p) : 0);
    }

    LIBUPCOREAPI
    ssize_t normalize_path(wchar_t* UPRESTRICT dest, size_t dest_max, wchar_t const* UPRESTRICT p, size_t n) noexcept {
        wchar_t const* const p_end = p + n;
        size_t parts_depth, parts_size, length, drive_length, i;
        wchar_t const** parts;
        
        // validate arguments
        if ((dest_max > (SSIZE_MAX + 1)) || (!dest && dest_max) || (!p && n)) {
            errno = EINVAL;
            return -1;
        }

        length = 0;

        // check if path begins with a drive designator
        if ((n > 1) && fast_isalpha(p[0]) && (p[1] == L':')) {
            if (dest_max > 0) {
                dest[0] = static_cast<wchar_t>(fast_toupper(p[0]));
            }
            if (dest_max > 1) {
                dest[1] = L':';
            }
            p += 2;
            length += 2;
        }

        // if path has a drive designator, collapse initial directory seperators
        // otherwise, path may be a UNC file path, in which we keep the first two
        for (drive_length = length; (p < p_end) && ((*p == L'\\') || (*p == L'/')); ++p) {
            if (length < dest_max) {
                dest[length] = L'\\';
            }
            ++length;
            if (drive_length) {
                break;
            }
            else if (length > 2) {
                length = 2;
                break;
            }
        }

        // allocate parts stack with upper-bound of n / 2
        parts_depth = 0;
        parts_size = n / 2;
        parts = static_cast<wchar_t const**>(malloca(parts_size * sizeof(wchar_t const*)));
        if (!parts) {
            return -1;
        }

        // tokenize the local path and normalize parts into stack
        while (p < p_end) {
            if ((*p == L'\\') || (*p == L'/')) {
                ++p;
                continue;
            }
            else if ((p[0] == L'.') && ((p[1] == L'\\') || (p[1] == L'/') || (p[1] == L'\0'))) {
                p += 2;
                continue;
            }
            else if ((p[0] == L'.') && (p[1] == L'.') && ((p[2] == L'\\') || (p[2] == L'/') || (p[2] == L'\0'))) {
                if (parts_depth) {
                    --parts_depth;
                    p += 3;
                    continue;
                }
                else if (length - drive_length) {
                    p += 3;
                    continue;
                }
            }

            assert(parts_depth < parts_size);
            parts[parts_depth] = p;
            ++parts_depth;

            for ( ; (p < p_end) && (*p != L'\\') && (*p != L'/'); ++p) ;
        }

        // rejoin the path parts in normalized form
        for (i = 0; i < parts_depth; ++i) {
            for (p = parts[i]; (p < p_end) && (*p != L'\\') && (*p != L'/'); ++length, ++p) {
                if (length < dest_max) {
                    dest[length] = *p;
                }
            }

            if (i != (parts_depth - 1)) {
                if (length < dest_max) {
                    dest[length] = L'\\';
                }
                ++length;
            }
        }

        // free temporary buffers
        freea(parts);

        // check for error conditions
        if (length > SSIZE_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        // check for empty path, normalize to current directory
        if (!length) {
            ++length;
            if (dest_max) {
                dest[0] = L'.';
            }
        }

        // null terminate the destination buffer
        if (length < dest_max) {
            dest[length] = L'\0';
        }
        else if (dest_max) {
            dest[dest_max - 1] = L'\0';
        }

        return static_cast<ssize_t>(length);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* normalize_path(wchar_t const* p) noexcept {
        return normalize_path(p, p ? wcslen(p) : 0);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* normalize_path(wchar_t const* p, size_t n) noexcept {
        wchar_t default_buffer[1024];
        wchar_t* retval;
        size_t length;
        ssize_t slength;

        slength = normalize_path(default_buffer, sizeof(default_buffer) / sizeof(wchar_t), p, n);
        if (slength < 0) {
            return nullptr;
        }

        length = static_cast<size_t>(slength);
        if (length < (sizeof(default_buffer) / sizeof(wchar_t))) {
            return wcsndup(default_buffer, length);
        }

        retval = static_cast<wchar_t*>(malloc((length + 1) * sizeof(wchar_t)));
        if (!retval) {
            return nullptr;
        }

        verify(slength == normalize_path(retval, length + 1, p, n));
        return retval;
    }
}}}

#endif
