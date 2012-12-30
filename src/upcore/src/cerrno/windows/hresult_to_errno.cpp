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

#include <up/cerrno.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace up
{
    LIBUPCOREAPI UPPURE
    int hresult_to_errno(long result) noexcept {
        if (SUCCEEDED(result)) {
            return 0;
        }
        
        if (HRESULT_FACILITY(result) == FACILITY_ITF) {
            return EACCES;
        }

        switch (result) {
        case E_NOTIMPL: return ENOSYS;
        case E_NOINTERFACE: return ENOENT;
        case E_POINTER: return EFAULT;
        case E_ABORT: return ECANCELED;
        case E_FAIL: return EACCES;
        case E_UNEXPECTED: return EACCES;
        case E_ACCESSDENIED: return EACCES;
        case E_HANDLE: return EFAULT;
        case E_OUTOFMEMORY: return ENOMEM;
        case E_INVALIDARG: return EINVAL;
        default: break;
        }

        return oserror_to_errno(static_cast<unsigned long>(HRESULT_CODE(result)));
    }
}
