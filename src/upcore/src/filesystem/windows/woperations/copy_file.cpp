/*//
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

#ifdef UP_HAS_STDC_WCHAR

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    int copy_file(wchar_t const* from, wchar_t const* to) noexcept {
        return copy_file(from, to, copy_option::no_overwrite);
    }

    LIBUPCOREAPI UPNONNULLALL
    bool copy_file(wchar_t const* from, wchar_t const* to, copy_option option) noexcept {
        char* native_from = transcode(from);
        if (!native_from) {
            return false;
        }

        char* native_to = transcode(to);
        if (!native_to) {
            free(native_from);
            return false;
        }

        bool result = copy_file(native_from, native_to, option);
        free(native_to);
        free(native_from);
        return result;
    }
}}

#endif
*/
