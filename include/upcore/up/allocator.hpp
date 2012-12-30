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

#ifndef UP_ALLOCATOR_HPP
#define UP_ALLOCATOR_HPP

#include <up/cstddef.hpp>

namespace up
{
    class LIBUPCOREAPI allocator
    {
        UPNONCOPYABLE(allocator);

    public:
        
        virtual ~allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept = 0;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept = 0;
        virtual void deallocate(void* p, size_t n = 0) noexcept = 0;
        UPALLOC UPWARNRESULT void* allocate_throw(size_t n);
        UPALLOC UPWARNRESULT void* allocate_zero_throw(size_t n, size_t s);

    protected:

        UPHIDDENINLINE allocator() noexcept { }
    };

    class page_allocator;
    class malloc_allocator;
    class aligned_malloc_allocator;
    class linear_head_allocator;
    class linear_tail_allocator;
    class concurrent_linear_allocator;
    class heap_allocator;
}

#endif