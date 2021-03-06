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

namespace up { namespace filesystem { namespace detail
{
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* observer_operations<wchar_t>::user_directory_path() noexcept {
#if NTDDI_VERSION >= NTDDI_VISTA
        wchar_t* path;
        HRESULT result = ::SHGetKnownFolderPath(FOLDERID_Profile, KF_FLAG_CREATE, NULL, &path);
        if (FAILED(result)) {
            set_errno_with_hresult(result);
            return nullptr;
        }

        wchar_t* retval = wcsdup(path);
        ::CoTaskMemFree(path);
        return retval;
#else
        wchar_t path[MAX_PATH + 1];
        HRESULT result = ::SHGetFolderPathW(NULL, CSIDL_PROFILE | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, path);
        if (FAILED(result)) {
            set_errno_with_hresult(result);
            return nullptr;
        }

        return wcsdup(path);
#endif
    }
}}}
