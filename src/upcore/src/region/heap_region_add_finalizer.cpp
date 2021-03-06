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

namespace up
{
    LIBUPCOREAPI
    heap_region_finalizer* heap_region_add_finalizer(heap_region* r, void (*action)(void*,size_t), void* data, size_t size) noexcept {
        if (UPUNLIKELY(!r || !action || ((reinterpret_cast<uintptr_t>(data) & 3) != 0))) {
            errno = EINVAL;
            return nullptr;
        }

        heap_region_finalizer* finalizer = static_cast<heap_region_finalizer*>(heap_region_allocate_tail(r, sizeof(heap_region_finalizer)));
        if (UPUNLIKELY(!finalizer)) {
            return nullptr;
        }

        finalizer->next = r->finalizers;
        finalizer->action = action;
        finalizer->data = reinterpret_cast<uintptr_t>(data);
        finalizer->size = size;

        r->finalizers = finalizer;
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.num_finalizers++;
#endif // UP_NO_UNBOUNDED_REGION_METRICS

        return finalizer;
    }

    LIBUPCOREAPI
    heap_region_finalizer* heap_region_add_finalizer(heap_region* r, allocator* data_alloc, void* data, size_t size) noexcept {
        if (UPUNLIKELY(!r || !data_alloc || ((reinterpret_cast<uintptr_t>(data) & 3) != 0))) {
            errno = EINVAL;
            return nullptr;
        }

        heap_region_finalizer* finalizer = static_cast<heap_region_finalizer*>(heap_region_allocate_tail(r, sizeof(heap_region_finalizer)));
        if (UPUNLIKELY(!finalizer)) {
            return nullptr;
        }

        finalizer->next = r->finalizers;
        finalizer->alloc = data_alloc;
        finalizer->data = reinterpret_cast<uintptr_t>(data) | 1;
        finalizer->size = size;

        r->finalizers = finalizer;
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        r->metrics.num_finalizers++;
#endif // UP_NO_UNBOUNDED_REGION_METRICS

        return finalizer;
    }
}
