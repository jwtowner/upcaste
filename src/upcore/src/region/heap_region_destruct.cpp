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
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI
    void heap_region_destruct(heap_region* r) noexcept {
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

        // release all chunks, including the root chunk of the region itself
        allocator* const alloc = r->base_alloc;
        size_t const chunk_size = r->chunk_size;
        heap_region_chunk* chunk = r->root_chunk.next, * next_chunk;

        while (chunk) {
            next_chunk = chunk->next;
            alloc->deallocate(chunk, chunk_size);
            chunk = next_chunk;
        }
    }
}
