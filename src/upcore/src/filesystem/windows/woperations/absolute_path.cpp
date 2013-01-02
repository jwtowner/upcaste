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
    ssize_t absolute_path(wchar_t* d, size_t dsz, wchar_t const* p, wchar_t const* base) noexcept {
        wchar_t const* q;
        wchar_t* buffer;
        size_t n, m, buffer_length, buffer_max;
        ssize_t retval;

        // validate arguments
        if ((!d && dsz) || (dsz > (SSIZE_MAX + 1)) || !p) {
            errno = EINVAL;
            return -1;
        }

        n = wcslen(p);
        if (n > SSIZE_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        // if the path is already absolute, just normalize it into the destination buffer
        q = p;
        m = n;
        if ((n >= 2) && fast_isalpha(static_cast<int>(q[0])) && (q[1] == L':')) {
            q += 2;
            m -= 2;
        }

        if ((*q == L'\\') || (*q == L'/')) {
            return normalize_path(d, dsz, p, n);
        }

        if (!base) {
            // copy current directory path into temporary buffer
            buffer_max = MAX_PATH + m + 2;
            buffer = static_cast<wchar_t*>(malloca(buffer_max * sizeof(wchar_t)));
            if (!buffer) {
                return -1;
            }

            buffer_length = ::GetCurrentDirectoryW(MAX_PATH + 1, buffer);
            if (!buffer_length) {
                set_errno_with_last_oserror();
                freea(buffer);
                return -1;
            }
            else if (buffer_length > MAX_PATH) {
                errno = EOVERFLOW;
                freea(buffer);
                return -1;
            }
        }
        else {
            // get the absolute path of base and place it into temporary buffer
            buffer_max = MAX_PATH + wcslen(base) + m + 3;
            buffer = static_cast<wchar_t*>(malloca(buffer_max * sizeof(wchar_t)));
            if (!buffer) {
                return -1;
            }

            retval = absolute_path(buffer, buffer_max, base, nullptr);
            if (retval < 0) {
                freea(buffer);
                return -1;
            }

            buffer_length = static_cast<size_t>(retval);
        }

        // join the base path in the buffer with the input path
        assert(buffer_length > 0);
        if ((buffer[buffer_length - 1] != '\\') && (buffer[buffer_length - 1] != '/')) {
            buffer[buffer_length] = '\\';
            ++buffer_length;
        }

        assert((buffer_length + m) < buffer_max);
        wmemcpy(buffer + buffer_length, q, m);
        buffer_length += m;

        // normalize the joined path to get final result
        retval = normalize_path(d, dsz, buffer, buffer_length);
        freea(buffer);
        return retval;
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* absolute_path(wchar_t const* p, wchar_t const* base) noexcept {
        size_t length;
        ssize_t slength;
        wchar_t* retval;
        wchar_t default_buffer[1024];
        
        slength = absolute_path(default_buffer, sizeof(default_buffer) / sizeof(wchar_t), p, base);
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

        verify(slength == absolute_path(retval, length + 1, p, base));
        return retval;
    }
}}
