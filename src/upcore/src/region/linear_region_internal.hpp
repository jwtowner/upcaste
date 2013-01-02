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

#ifndef UP_REGION_LINEAR_REGION_INTERNAL_HPP
#define UP_REGION_LINEAR_REGION_INTERNAL_HPP

#include <up/region.hpp>

namespace up { namespace detail
{
    class UPHIDDEN linear_head_allocator : public allocator
    {
    public:

        UPALWAYSINLINE explicit linear_head_allocator(linear_region* r) noexcept : region_(r) { }
        virtual ~linear_head_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n) noexcept;

    private:

        linear_region* region_;
    };

    class UPHIDDEN linear_tail_allocator : public allocator
    {
    public:

        UPALWAYSINLINE explicit linear_tail_allocator(linear_region* r) noexcept : region_(r) { }
        virtual ~linear_tail_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n) noexcept;

    private:

        linear_region* region_;
    };
}}

namespace up
{
    struct UPHIDDEN linear_region
    {
        UPNONCOPYABLE(linear_region);
    
    public:
    
        char* head;
        char* tail;
        size_t alignment;
        size_t space;
        detail::linear_head_allocator head_alloc;
        detail::linear_tail_allocator tail_alloc;

        UPALWAYSINLINE
        linear_region(void* p, size_t n, size_t a, size_t s) noexcept
        : head(static_cast<char*>(p)),
        tail(static_cast<char*>(p) + n),
        alignment(a),
        space(s),
        head_alloc(this),
        tail_alloc(this) {
        }
    };
}

#endif
