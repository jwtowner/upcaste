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

#include "heap_region_internal.hpp"

namespace up
{
    LIBUPCOREAPI UPWARNRESULT
    heap_region* heap_region_construct(
        void* base,
        allocator* alloc,
        size_t chunk_size,
        size_t large_object_size,
        size_t alignment,
        size_t offset
    )
    noexcept {
        void* ptr = base;
        size_t space = chunk_size;
        heap_region* retval;

        if (!base || !heap_region_validate_args(alloc, chunk_size, large_object_size, alignment, offset)) {
            errno = EINVAL;
            return nullptr;
        }

        retval = static_cast<heap_region*>(align(alignof(heap_region), sizeof(heap_region), &ptr, &space));
        if (!retval) {
            return nullptr;
        }
        else if (retval != base) {
            errno = EINVAL;
            return nullptr;
        }

        align_advance(sizeof(heap_region), &ptr, &space);
        if (!align(alignment, offset, alignment, &ptr, &space)) {
            return nullptr;
        }

        retval->root_chunk.next = nullptr;
        retval->root_chunk.owner = retval;
        retval->root_chunk.head = static_cast<char*>(ptr);
        retval->root_chunk.tail = retval->root_chunk.head + space;
        assert((reinterpret_cast<uintptr_t>(retval->root_chunk.tail) & (alignof(max_align_t) - 1)) == 0);

        retval->active_chunk = &retval->root_chunk;
        retval->finalizers = nullptr;
        retval->recycle_bin = nullptr;
        retval->alignment = alignment;
        retval->large_object_size = large_object_size;
        retval->chunk_size = chunk_size;
        retval->alloc = alloc;
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        retval->metrics.num_chunks = 1;
        retval->metrics.num_small_objects = 0;
        retval->metrics.num_large_objects = 0;
        retval->metrics.num_finalizers = 0;
        retval->metrics.usage = 0;
        retval->metrics.peak_usage = 0;
        retval->metrics.recycle_usage = 0;
        retval->metrics.peak_recycle_usage = 0;
#endif
        return retval;
    }
}
