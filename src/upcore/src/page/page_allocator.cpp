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

#include <up/page.hpp>
#include <up/cassert.hpp>
#include <up/cstdlib.hpp>
#include <up/cerrno.hpp>
#include <up/cthreads.hpp>

namespace up
{
    namespace
    {
        once_flag page_allocator_once = ONCE_FLAG_INIT;
        size_t native_page_size = 0;
        size_t native_page_mask = SIZE_MAX;

        void UPCDECL page_allocator_init() noexcept {
            verify(!page_get_default_size(&native_page_size));
            native_page_mask = native_page_size - 1;
        }
    }

    LIBUPCOREAPI
    size_t page_allocator::page_size() noexcept {
        call_once(&page_allocator_once, &page_allocator_init);
        return native_page_size;
    }

    LIBUPCOREAPI
    page_allocator* page_allocator::instance() noexcept {
        static page_allocator instance_;
        call_once(&page_allocator_once, &page_allocator_init);
        return &instance_;
    }

    LIBUPCOREAPI
    page_allocator::page_allocator() noexcept {
    }
    
    LIBUPCOREAPI
    page_allocator::~page_allocator() noexcept {
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* page_allocator::allocate(size_t n) noexcept {
        if ((n & native_page_mask) == 0) {
            return page_allocate(n, page_access_read_write);
        }
        else {
            return malloc(n);
        }
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* page_allocator::allocate_zero(size_t n, size_t s) noexcept {
        if (s && (n > (SIZE_MAX / s))) {
            errno = EOVERFLOW;
            return nullptr;
        }

        size_t const total = n * s;
        if ((total & native_page_mask) == 0) {
            return page_allocate(total, page_access_read_write);
        }
        else {
            return calloc(n, s);
        }
    }

    LIBUPCOREAPI
    void page_allocator::deallocate(void* p, size_t n) noexcept {
        if ((n & native_page_mask) == 0) {
            page_deallocate(p, n);
        }
        else {
            free(p);
        }
    }
}
