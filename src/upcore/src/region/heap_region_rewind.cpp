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
#include <up/cstdalign.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    void heap_region_rewind(heap_region* r) noexcept {
        assert(r);

        // execute all of the finalizers in LIFO order
        for (heap_region_finalizer* finalizer = r->finalizers; finalizer; finalizer = finalizer->next) {
            if (finalizer->data & 1) {
                finalizer->alloc->deallocate(reinterpret_cast<void*>(finalizer->data & ~1), finalizer->size);
            }
            else {
                (*finalizer->action)(reinterpret_cast<void*>(finalizer->data), finalizer->size);
            }
        }

        // determine aligned boundary of root chunk
        size_t const alignment = r->alignment;
        size_t const offset = r->offset;
        void* ptr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(r) + sizeof(heap_region));
        size_t space = r->chunk_size - sizeof(heap_region);
        verify(align(alignment, offset, alignment, &ptr, &space));
        assert((reinterpret_cast<uintptr_t>(ptr) & (alignment - 1)) == 0);
        assert(((reinterpret_cast<uintptr_t>(ptr) + space) & (alignment - 1)) == 0);

        // reset root chunk boundary pointers
        r->root_chunk.head = static_cast<char*>(ptr);
        r->root_chunk.tail = r->root_chunk.head + space;

        // reset the region
        r->active_chunk = &r->root_chunk;
        r->finalizers = nullptr;
        r->recycle_bin = nullptr;
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.num_chunks = 1;
        r->metrics.num_small_objects = 0;
        r->metrics.num_large_objects = 0;
        r->metrics.num_finalizers = 0;
        r->metrics.usage = 0;
        r->metrics.recycle_usage = 0;
#endif
    }
}
