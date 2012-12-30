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

#ifdef UP_HAS_STDC_WCHAR

#include "../filesystem_internal.hpp"

namespace up { namespace filesystem { namespace detail
{
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* observer_operations<wchar_t>::current_directory_path() noexcept {
        wchar_t* retval;
        DWORD length;
        WCHAR buffer[MAX_PATH + 1];
        
        length = ::GetCurrentDirectoryW(MAX_PATH + 1, buffer);
        if (!length) {
            set_errno_with_last_oserror();
            return nullptr;
        }

        if (length <= MAX_PATH) {
            return wcsndup(buffer, length);
        }

        retval = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));
        if (!retval) {
            return nullptr;
        }

        verify((length - 1) == ::GetCurrentDirectoryW(length, buffer));
        return retval;
    }
}}}

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    int current_directory_path(wchar_t const* p) noexcept {
        BOOL result = ::SetCurrentDirectoryW(p);
        if (!result) {
            set_errno_with_last_oserror();
            return -1;
        }

        return 0;
    }
}}

#endif
