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

#include "wfilepath.hpp"

namespace up { namespace filesystem { namespace windows
{
    LIBUPCOREAPI
    ssize_t join_path(wchar_t* UPRESTRICT dest, size_t dest_max, wchar_t const* UPRESTRICT first, ...) noexcept {
        ssize_t retval;
        size_t count;
        wchar_t const** arg_list;
        va_list args;

        va_start(args, first);
        count = detail::join_path_count_args(first, args);
        va_end(args);

        arg_list = static_cast<wchar_t const**>(malloca(count * sizeof(wchar_t const*)));
        if (!arg_list) {
            return size_t(-1);
        }

        va_start(args, first);
        verify(count == detail::join_path_copy_args(arg_list, count, first, args));
        va_end(args);

        retval = join_path(dest, dest_max, arg_list, count);
        freea(arg_list);
        return retval;
    }

    LIBUPCOREAPI
    ssize_t join_path(wchar_t* UPRESTRICT dest, size_t dest_max, wchar_t const* const* UPRESTRICT args, size_t arg_count) noexcept {
        size_t length, part_length, n, i;
        wchar_t const* part, * temp;
        bool add_sep;

        // make sure arguments are valid
        if ((dest_max > (SSIZE_MAX + 1)) || (!dest && dest_max) || (!args && arg_count)) {
            errno = EINVAL;
            return -1;
        }

        length = 0;
        add_sep = true;

        // find first non-empty path part and copy it into destination buffer
        for (i = 0; !length && (i < arg_count); ++i) {
            // skip null or empty paths
            part = args[i];
            part_length = part ? wcslen(part) : 0;
            if (!part_length) {
                continue;
            }

            // remove trailing directory seperators
            temp = wcsnrcpbrk(part, path<wchar_t>::sepset, part_length);
            if (temp) {
                part_length = temp - part + 1;
                assert(part_length > 0);
            }
            else {
                add_sep = false;
            }

            // copy in as much of the path part into destination buffer as there is room
            if (dest_max) {
                n = (part_length < dest_max) ? part_length : dest_max - 1;
                wmemcpy(dest, part, n);
            }

            length += part_length;
        }

        // copy remaining non-empty path parts into destination buffer
        for ( ; i < arg_count; ++i) {
            // skip null or empty path parts
            part = args[i];
            part_length = part ? wcslen(part) : 0;
            if (!part_length) {
                continue;
            }

            // trim leading directory seperators
            temp = wcsncpbrk(part, path<wchar_t>::sepset, part_length);
            if (!temp) {
                continue;
            }

            part_length -= temp - part;
            part = temp;

            // trim trailing directory seperators
            temp = wcsnrcpbrk(part, path<wchar_t>::sepset, part_length);
            if (!temp) {
                continue;
            }

            part_length = temp - part + 1;
            assert(part_length > 0);

            // add a directory seperator in between path parts if necessary
            if (add_sep) {
                n = length++;
                if (length < dest_max) {
                    dest[n] = L'\\';
                }
            }

            // copy in as much of the path part into destination buffer as there is room remaining
            if (length < dest_max) {
                n = ((length + part_length) < dest_max) ? part_length : dest_max - length - 1;
                wmemcpy(dest + length, part, n);
            }

            length += part_length;
            add_sep = true;
        }

        // null terminate the destination buffer
        if (length < dest_max) {
            dest[length] = L'\0';
        }
        else if (dest_max > 0) {
            dest[dest_max - 1] = L'\0';
        }

        if (length > SSIZE_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        return static_cast<ssize_t>(length);
    }
    
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* join_path(wchar_t const* first, ...) noexcept {
        wchar_t* retval;
        size_t count;
        wchar_t const** arg_list;
        va_list args;

        va_start(args, first);
        count = detail::join_path_count_args(first, args);
        va_end(args);

        arg_list = static_cast<wchar_t const**>(malloca(count * sizeof(wchar_t const*)));
        if (!arg_list) {
            return nullptr;
        }

        va_start(args, first);
        verify(count == detail::join_path_copy_args(arg_list, count, first, args));
        va_end(args);

        retval = join_path(arg_list, count);
        freea(arg_list);
        return retval;
    }
    
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* join_path(wchar_t const* const* args, size_t arg_count) noexcept {
        size_t length;
        ssize_t slength;
        wchar_t* retval;
        wchar_t default_buffer[1024];
        
        slength = join_path(default_buffer, sizeof(default_buffer) / sizeof(wchar_t), args, arg_count);
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

        verify(slength == join_path(retval, length + 1, args, arg_count));
        return retval;
    }
}}}

#endif
