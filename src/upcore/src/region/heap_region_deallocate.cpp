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
#include <up/cstring.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(1)
    int heap_region_deallocate(heap_region* r, void* p, size_t n) noexcept {
        assert(r);

        if (UPUNLIKELY(!p || !n || (n > r->large_object_size))) {
            return 0;
        }

        size_t const alignment = r->alignment;
        size_t const align_mask = alignment - 1;
        size_t const aligned_n = (n + align_mask) & ~align_mask;
        size_t const bin_index = (aligned_n / alignment) - 1;

        heap_region_recycle_item** recycle_bin = r->recycle_bin;
        if (!recycle_bin) {
            size_t const bin_size = (r->large_object_size / alignment) * sizeof(heap_region_recycle_item*);
            recycle_bin = static_cast<heap_region_recycle_item**>(heap_region_allocate_tail(r, bin_size));
            if (UPUNLIKELY(!recycle_bin)) {
                return -1;
            }
            memset(recycle_bin, 0, bin_size);
            r->recycle_bin = recycle_bin;
        }

        heap_region_recycle_item* new_item = static_cast<heap_region_recycle_item*>(p);

#ifdef UP_DEBUG
        // check for double-free
        for (heap_region_recycle_item* item = r->recycle_bin[bin_index]; item; item = item->next) {
            assert(new_item != item);
        }
#endif

#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.usage -= aligned_n;
        r->metrics.recycle_usage += aligned_n;
        if (r->metrics.recycle_usage >= r->metrics.peak_recycle_usage) {
            r->metrics.peak_recycle_usage = r->metrics.recycle_usage;
        }
#endif // UP_NO_UNBOUNDED_REGION_METRICS

        new_item->next = recycle_bin[bin_index];
        recycle_bin[bin_index] = new_item;
        return 0;
    }
}
