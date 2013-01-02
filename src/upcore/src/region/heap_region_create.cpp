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

#include "heap_region_internal.hpp"
#include <up/cerrno.hpp>
#include <up/cstdlib.hpp>
#include <up/page.hpp>

namespace up
{
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    heap_region* heap_region_create(size_t chunk_size, size_t large_obj_size, size_t alignment, size_t offset) noexcept {
        allocator* alloc;
        if (chunk_size < page_allocator::page_size()) {
            alloc = malloc_allocator::instance();
        }
        else {
            alloc = page_allocator::instance();
        }

        return heap_region_create(alloc, chunk_size, large_obj_size, alignment, offset);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    heap_region* heap_region_create(
        allocator* alloc,
        size_t chunk_size,
        size_t large_obj_size,
        size_t alignment,
        size_t offset
    )
    noexcept {
        if (UPUNLIKELY(!heap_region_validate_args(alloc, chunk_size, large_obj_size, alignment, offset))) {
            errno = EINVAL;
            return nullptr;
        }

        void* const base = alloc->allocate(chunk_size);
        if (UPUNLIKELY(!base)) {
            return nullptr;
        }

        heap_region* const retval = heap_region_construct(base, alloc, chunk_size, large_obj_size, alignment, offset);
        if (UPUNLIKELY(!retval)) {
            alloc->deallocate(base, chunk_size);
            return nullptr;
        }

        return retval;
    }
}
