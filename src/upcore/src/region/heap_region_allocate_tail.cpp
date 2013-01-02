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

namespace up
{
    UPHIDDEN UPNONNULLALL UPALLOC UPWARNRESULT
    void* heap_region_allocate_tail(heap_region* r, size_t n) noexcept {
        assert(r && (n > 0));

        size_t const aligned_n = (n + (alignof(max_align_t) - 1)) & ~(alignof(max_align_t) - 1);
        heap_region_chunk* active_chunk = r->active_chunk;

        if (aligned_n > static_cast<size_t>(active_chunk->tail - active_chunk->head)) {
            active_chunk = heap_region_add_chunk(r);
            if (UPUNLIKELY(!active_chunk)) {
                return nullptr;
            }
        }

#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.usage += aligned_n;
        if (r->metrics.usage >= r->metrics.peak_usage) {
            r->metrics.peak_usage = r->metrics.usage;
        }
#endif // UP_NO_UNBOUNDED_REGION_METRICS

        active_chunk->tail -= aligned_n;
        return active_chunk->tail;
    }
}
