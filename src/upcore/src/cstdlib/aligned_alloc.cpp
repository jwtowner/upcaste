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

#include <up/cstdlib.hpp>

#if !defined(UP_HAS_STDC_ALIGNED_ALLOC) && !defined(UP_HAS_POSIX_MEMALIGN) && (UP_STDC_EXTENSIONS != UP_STDC_EXTENSIONS_MSVC)

namespace up
{
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* aligned_alloc(size_t alignment, size_t size) noexcept {
        if (!alignment || ((alignment & (alignment -1)) != 0)) {
            errno = EINVAL;
            return nullptr;
        }

        uintptr_t addr = reinterpret_cast<uintptr_t>(malloc(size + (alignment - 1) + sizeof(void*)));
        if (!addr) {
            return nullptr;
        }

        uintptr_t aligned_addr = (addr + (alignment - 1) + sizeof(void*)) & ~(alignment - 1);
        void** addr_ptr = reinterpret_cast<void**>(aligned_addr - sizeof(void*));
        *addr_ptr = reinterpret_cast<void*>(addr);
        return reinterpret_cast<void*>(aligned_addr);
    }

    LIBUPCOREAPI
    void aligned_free(void* ptr) noexcept {
        free(*reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(ptr) - sizeof(void*)));
    }
}

#endif
