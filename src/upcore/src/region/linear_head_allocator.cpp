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

#include <up/region.hpp>

namespace up
{
    LIBUPCOREAPI
    linear_head_allocator::linear_head_allocator() noexcept
    : region_(nullptr) {
    }

    LIBUPCOREAPI
    linear_head_allocator::linear_head_allocator(linear_region* r) noexcept
    : region_(r) {
    }
    
    LIBUPCOREAPI
    linear_head_allocator::linear_head_allocator(linear_head_allocator const* a) noexcept
    : region_(a->region_) {
    }

    LIBUPCOREAPI
    linear_head_allocator::linear_head_allocator(linear_tail_allocator const* a) noexcept
    : region_(a->region()) {
    }

    LIBUPCOREAPI
    linear_head_allocator::linear_head_allocator(void* ptr, size_t space, size_t alignment, size_t offset) noexcept
    : region_(linear_region_construct(ptr, space, alignment, offset)) {
    }

    LIBUPCOREAPI
    linear_head_allocator::~linear_head_allocator() noexcept {
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* linear_head_allocator::allocate(size_t n) noexcept {
        return linear_region_head_allocate(region_, n);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* linear_head_allocator::allocate_zero(size_t n, size_t s) noexcept {
        return linear_region_head_allocate_zero(region_, n, s);
    }

    LIBUPCOREAPI
    void linear_head_allocator::deallocate(void*, size_t) noexcept {
    }
}
