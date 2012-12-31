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
    LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* heap_region_allocate(heap_region* r, size_t n) noexcept {
        assert(r && (n > 0));
        
        if (n > r->large_object_size) {
            allocator* const alloc = r->base_alloc;
            void* const retval = alloc->allocate(n);
            if (UPUNLIKELY(!retval)) {
                return nullptr;
            }

            heap_region_finalizer* finalizer = heap_region_add_finalizer(r, alloc, retval, n);
            if (UPUNLIKELY(!finalizer)) {
                alloc->deallocate(retval, n);
                return nullptr;
            }

#ifndef UP_NO_UNBOUNDED_REGION_METRICS
            r->metrics.num_large_objects++;
            r->metrics.usage += n;
            if (r->metrics.usage >= r->metrics.peak_usage) {
                r->metrics.peak_usage = r->metrics.usage;
            }
#endif // UP_NO_UNBOUNDED_REGION_METRICS

            return retval;
        }

        size_t const alignment = r->alignment;
        size_t const align_mask = alignment - 1;
        size_t const aligned_n = (n + align_mask) & ~align_mask;

        if (r->recycle_bin) {
            size_t const bin_index = (aligned_n / alignment) - 1;
            heap_region_recycle_item* const item = r->recycle_bin[bin_index];
            if (item) {
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
                r->metrics.recycle_usage -= aligned_n;
                r->metrics.usage += aligned_n;
                if (r->metrics.usage >= r->metrics.peak_usage) {
                    r->metrics.peak_usage = r->metrics.usage;
                }
#endif // UP_NO_UNBOUNDED_REGION_METRICS
                r->recycle_bin[bin_index] = item->next;
                return item;
            }
        }

        heap_region_chunk* active_chunk = r->active_chunk;
        if (aligned_n > static_cast<size_t>(active_chunk->tail - active_chunk->head)) {
            active_chunk = heap_region_add_chunk(r);
            if (UPUNLIKELY(!active_chunk)) {
                return nullptr;
            }
        }

#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.num_small_objects++;
        r->metrics.usage += aligned_n;
        if (r->metrics.usage >= r->metrics.peak_usage) {
            r->metrics.peak_usage = r->metrics.usage;
        }
#endif // UP_NO_UNBOUNDED_REGION_METRICS

        void* const retval = active_chunk->head;
        active_chunk->head += aligned_n;
        return retval;
    }
}
