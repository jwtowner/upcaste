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
#include <up/cstdalign.hpp>

namespace up
{
    LIBUPCOREAPI
    malloc_allocator* malloc_allocator::instance() noexcept {
        static malloc_allocator instance_;
        return &instance_;
    }

    LIBUPCOREAPI
    malloc_allocator::malloc_allocator() noexcept {
    }
    
    LIBUPCOREAPI
    malloc_allocator::~malloc_allocator() noexcept {
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* malloc_allocator::allocate(size_t n) noexcept {
        return malloc(n);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* malloc_allocator::allocate_zero(size_t n, size_t s) noexcept {
        return calloc(n, s);
    }

    LIBUPCOREAPI
    void malloc_allocator::deallocate(void* p, size_t) noexcept {
        free(p);
    }
}
