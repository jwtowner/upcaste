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
    UPHIDDEN UPNONNULLALL UPALLOC UPWARNRESULT
    heap_region_chunk* heap_region_add_chunk(heap_region* r) noexcept {
        assert(r);

        size_t const alignment = r->alignment;
        size_t const offset = r->offset;
        size_t space = r->chunk_size;
        void* ptr;

        heap_region_chunk* new_chunk = r->active_chunk->next;
        if (!new_chunk) {
            new_chunk = static_cast<heap_region_chunk*>(r->base_alloc->allocate(space));
            if (UPUNLIKELY(!new_chunk)) {
                return nullptr;
            }
        }

        ptr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(new_chunk) + sizeof(heap_region_chunk));
        space -= sizeof(heap_region_chunk);

        verify(align(alignment, offset, alignment, &ptr, &space));
        assert((reinterpret_cast<uintptr_t>(ptr) & (alignment - 1)) == 0);
        assert(((reinterpret_cast<uintptr_t>(ptr) + space) & (alignment - 1)) == 0);

        new_chunk->next = nullptr;
        new_chunk->owner = r;
        new_chunk->head = static_cast<char*>(ptr);
        new_chunk->tail = new_chunk->head + space;
        r->active_chunk->next = new_chunk;
        r->active_chunk = new_chunk;

#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.num_chunks++;
#endif // UP_NO_UNBOUNDED_REGION_METRICS

        return new_chunk;
    }
}
