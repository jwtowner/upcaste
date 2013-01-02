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
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <new>

namespace up
{
    LIBUPCOREAPI UPWARNRESULT
    heap_region* heap_region_construct(
        void* base,
        allocator* base_alloc,
        size_t chunk_size,
        size_t large_object_size,
        size_t alignment,
        size_t offset
    )
    noexcept {
        size_t space = chunk_size;
        void* ptr = base;
        void* retval;

        if ( UPUNLIKELY(!base
                || !heap_region_validate_args(base_alloc, chunk_size, large_object_size, alignment, offset)
            )
        ) {
            errno = EINVAL;
            return nullptr;
        }

        retval = static_cast<heap_region*>(align(alignof(heap_region), sizeof(heap_region), &ptr, &space));
        if (UPUNLIKELY(!retval)) {
            return nullptr;
        }
        else if (UPUNLIKELY(retval != base)) {
            errno = EINVAL;
            return nullptr;
        }

        align_advance(sizeof(heap_region), &ptr, &space);
        if (!align(alignment, offset, alignment, &ptr, &space)) {
            return nullptr;
        }

        assert((reinterpret_cast<uintptr_t>(ptr) & (alignment - 1)) == 0);
        assert(((reinterpret_cast<uintptr_t>(ptr) + space) & (alignment - 1)) == 0);
        return ::new(retval) heap_region(ptr, space, base_alloc, chunk_size, large_object_size, alignment, offset);
    }
}
