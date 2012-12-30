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

#include <up/filesystem/operations.hpp>
#include <up/filesystem/transcode.hpp>
#include "../filesystem_internal.hpp"

#ifdef UP_HAS_STDC_WCHAR

#include <up/filesystem/wchar_operations.hpp>

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    bool copy_symlink(wchar_t const* existing_symlink, wchar_t const* new_symlink, std::error_code& ec) noexcept {
        char* native_existing = transcode(existing_symlink, ec);
        if (!native_existing) {
            return false;
        }

        char* native_new = transcode(new_symlink, ec);
        if (!native_new) {
            free(native_existing);
            return false;
        }

        bool result = copy_symlink(native_existing, native_new, ec);
        free(native_new);
        free(native_existing);
        return result;
    }
}}

#endif
