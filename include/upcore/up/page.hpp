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

#ifndef UP_PAGE_HPP
#define UP_PAGE_HPP

#include <up/allocator.hpp>

namespace up
{
    constexpr unsigned int page_access_none         = 0x0000;
    constexpr unsigned int page_access_read         = 0x0001;
    constexpr unsigned int page_access_write        = 0x0002;
    constexpr unsigned int page_access_read_write   = 0x0003;
    constexpr unsigned int page_access_execute      = 0x0004;
    constexpr unsigned int page_option_huge_relaxed = 0x0010;
    constexpr unsigned int page_option_huge_strict  = 0x0030;

    struct LIBUPCOREAPI page_metrics
    {
        size_t resident_set_size;
        size_t peak_resident_set_size;
        size_t page_usage;
        size_t peak_page_usage;
    };

    extern LIBUPCOREAPI
    int page_get_metrics(page_metrics* metrics) noexcept;

    extern LIBUPCOREAPI
    int page_get_default_size(size_t* page_size) noexcept;

    extern LIBUPCOREAPI
    ssize_t page_get_sizes(size_t* page_sizes, size_t max_page_sizes) noexcept;

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* page_allocate(size_t n, unsigned int flags) noexcept;

    extern LIBUPCOREAPI
    int page_deallocate(void* p, size_t n) noexcept;

    extern LIBUPCOREAPI
    int page_protect(void* p, size_t n, unsigned int flags) noexcept;

    class LIBUPCOREAPI page_allocator : public allocator
    {
    public:

        static page_allocator* instance() noexcept;
        static size_t page_size() noexcept;
        page_allocator() noexcept;
        virtual ~page_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n = 0) noexcept;
    };
}

#endif
