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
#include <up/cstdalign.hpp>
#include <up/cstring.hpp>
#include <up/bitwise.hpp>

namespace up
{
    LIBUPCOREAPI
    aligned_malloc_allocator::aligned_malloc_allocator() noexcept
    : alignment_(alignof(max_align_t)) {
    }
    
    LIBUPCOREAPI
    aligned_malloc_allocator::aligned_malloc_allocator(size_t alignment) noexcept
    : alignment_(ceil2(alignment)) {
    }

    LIBUPCOREAPI
    aligned_malloc_allocator::~aligned_malloc_allocator() noexcept {
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* aligned_malloc_allocator::allocate(size_t n) noexcept {
        return aligned_alloc(alignment_, n);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* aligned_malloc_allocator::allocate_zero(size_t n, size_t s) noexcept {
        if (s && (n > (SIZE_MAX / s))) {
            errno = EOVERFLOW;
            return nullptr;
        }

        size_t const total = n * s;
        void* retval = aligned_alloc(alignment_, total);
        if (!retval) {
            return nullptr;
        }

        return memset(retval, 0, total);
    }

    LIBUPCOREAPI
    void aligned_malloc_allocator::deallocate(void* p, size_t) noexcept {
        aligned_free(p);
    }

    LIBUPCOREAPI
    size_t aligned_malloc_allocator::alignment() const noexcept {
        return alignment_;
    }

    LIBUPCOREAPI
    void aligned_malloc_allocator::alignment(size_t value) noexcept {
        alignment_ = ceil2(value);
    }
}
