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

#include <up/filesystem/operations.hpp>
#include <up/filesystem/transcode.hpp>
#include "../filesystem_internal.hpp"

#ifdef UP_HAS_STDC_WCHAR

#include <up/filesystem/wchar_operations.hpp>

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    bool equivalent(wchar_t const* p1, wchar_t const* p2, std::error_code& ec) noexcept {
        char* native_p1 = transcode(p1, ec);
        if (!native_p1) {
            return false;
        }

        char* native_p2 = transcode(p2, ec);
        if (!native_p2) {
            free(native_p1);
            return false;
        }

        bool result = equivalent(native_p1, native_p2, ec);
        free(native_p2);
        free(native_p1);
        return result;
    }
}}

#endif
*/
