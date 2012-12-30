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

#ifndef UP_MEMORY_HEAP_REGION_INTERNAL_HPP
#define UP_MEMORY_HEAP_REGION_INTERNAL_HPP

#include <up/cassert.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/page.hpp>
#include <up/region.hpp>

namespace up
{
    struct UPHIDDEN heap_region_chunk
    {
        heap_region_chunk* next;
        heap_region* owner;
        char* head;
        char* tail;
    };

    struct UPHIDDEN heap_region_finalizer
    {
        heap_region_finalizer* next;
        union {
            allocator* alloc;
            void (*action)(void*, size_t);
        };
        uintptr_t data;
        size_t size;
    };

    struct UPHIDDEN heap_region_recycle_item
    {
        heap_region_recycle_item* next;
    };

    struct UPHIDDEN heap_region
    {
        heap_region_chunk* active_chunk;
        heap_region_finalizer* finalizers;
        heap_region_recycle_item** recycle_bin;
        size_t alignment;
        size_t large_object_size;
        size_t chunk_size;
        allocator* alloc;
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        heap_region_metrics metrics;
#endif // UP_NO_UNBOUNDED_REGION_METRICS
        heap_region_chunk root_chunk;
    };

    extern UPHIDDEN UPNONNULLALL UPALLOC UPWARNRESULT heap_region_chunk* heap_region_add_chunk(heap_region* r) noexcept;
    extern UPHIDDEN UPNONNULLALL UPALLOC UPWARNRESULT void* heap_region_allocate_tail(heap_region* r, size_t n) noexcept;

    inline UPALWAYSINLINE
    bool heap_region_validate_args(allocator* alloc, size_t chunk_size, size_t large_object_size, size_t alignment, size_t offset) noexcept {
        if ( !alloc
            || (chunk_size < heap_region_min_chunk_size) || ((chunk_size & (chunk_size - 1)) != 0)
            || (large_object_size < heap_region_min_large_object_size) || ((chunk_size / 4) < large_object_size)
            || (alignment < sizeof(heap_region_recycle_item*)) || ((alignment & (alignment - 1)) != 0)
            || ((large_object_size / 2) < alignment) || (alignment <= offset)
        ) {
            return false;
        }
        
        return true;
    }
}

#endif
