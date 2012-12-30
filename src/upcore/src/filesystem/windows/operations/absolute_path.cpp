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

#include "../filesystem_internal.hpp"

namespace up { namespace filesystem
{
    LIBUPCOREAPI
    ssize_t absolute_path(char* d, size_t dsz, char const* p, char const* base) noexcept {
        wchar_t* native_d = nullptr, * native_p = nullptr, * native_base = nullptr;
        ssize_t retval = -1;
        size_t length;

        if (dsz) {
            native_d = static_cast<wchar_t *>(malloca(dsz * sizeof(wchar_t)));
            if (!native_d) {
                return -1;
            }
        }

        native_p = transcode(p);
        if (!native_p) {
            goto error;
        }

        if (base) {
            native_base = transcode(base);
            if (!native_base) {
                goto error;
            }
        }

        retval = absolute_path(native_d, dsz, native_p, native_base);
        if (!retval) {
            length = static_cast<size_t>(retval);
            retval = transcode(d, dsz, native_d, length);
        }

    error:

        free(native_base);
        free(native_p);
        freea(native_d);
        return retval;
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* absolute_path(char const* p, char const* base) noexcept {
        wchar_t* native_p = nullptr, * native_base = nullptr, * native_result = nullptr;
        char* retval = nullptr;

        native_p = transcode(p);
        if (!native_p) {
            goto error;
        }

        if (base) {
            native_base = transcode(base);
            if (!native_base) {
                goto error;
            }
        }

        native_result = absolute_path(native_p, native_base);
        if (native_result) {
            retval = transcode(native_result);
        }

    error:

        free(native_result);
        free(native_base);
        free(native_p);
        return retval;
    }
}}
