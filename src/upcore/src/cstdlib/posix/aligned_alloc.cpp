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

#include <up/cstdlib.hpp>
#include <up/cerrno.hpp>

namespace up
{
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* aligned_alloc(size_t alignment, size_t size) noexcept {
        void* result;
        if (size == 0) {
            size = 1;
        }

#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
        int error = ::posix_memalign(&result, alignment, size);
        if (error != 0) {
            errno = error;
            return nullptr;
        }

        return result;
#else
        result = ::memalign(alignment, size);
        if (!result) {
            errno = ENOMEM;
            return nullptr;
        }

        return result;
#endif
    }
}
