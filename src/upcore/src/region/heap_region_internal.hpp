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

#ifndef UP_MEMORY_HEAP_REGION_INTERNAL_HPP
#define UP_MEMORY_HEAP_REGION_INTERNAL_HPP

#include <up/region.hpp>

namespace up { namespace detail
{
    class UPHIDDEN heap_allocator : public allocator
    {
    public:

        UPALWAYSINLINE explicit heap_allocator(heap_region* r) noexcept : region_(r) { }
        virtual ~heap_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n) noexcept;

    private:

        heap_region* region_;
    };
}}

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
        UPNONCOPYABLE(heap_region);

    public:

        heap_region_chunk* active_chunk;
        heap_region_finalizer* finalizers;
        heap_region_recycle_item** recycle_bin;
        size_t alignment;
        size_t offset;
        size_t chunk_size;
        size_t large_object_size;
        allocator* base_alloc;
        detail::heap_allocator heap_alloc;
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
        heap_region_metrics metrics;
#endif // UP_NO_UNBOUNDED_REGION_METRICS
        heap_region_chunk root_chunk;

        UPALWAYSINLINE
        heap_region(
            void* p,
            size_t n,
            allocator* ba,
            size_t csz,
            size_t losz,
            size_t aln,
            size_t off
        )
        noexcept
        : active_chunk(&root_chunk),
        finalizers(nullptr),
        recycle_bin(nullptr),
        alignment(aln),
        offset(off),
        chunk_size(csz),
        large_object_size(losz),
        base_alloc(ba),
        heap_alloc(this) {
#ifndef UP_NO_UNBOUNDED_REGION_METRICS
            metrics.num_chunks = 1;
            metrics.num_small_objects = 0;
            metrics.num_large_objects = 0;
            metrics.num_finalizers = 0;
            metrics.usage = 0;
            metrics.peak_usage = 0;
            metrics.recycle_usage = 0;
            metrics.peak_recycle_usage = 0;
#endif
            root_chunk.next = nullptr;
            root_chunk.owner = this;
            root_chunk.head = static_cast<char*>(p);
            root_chunk.tail = static_cast<char*>(p) + n;
        }
    };

    extern UPHIDDEN UPNONNULLALL UPALLOC UPWARNRESULT
    heap_region_chunk* heap_region_add_chunk(heap_region* r) noexcept;
    
    extern UPHIDDEN UPNONNULLALL UPALLOC UPWARNRESULT
    void* heap_region_allocate_tail(heap_region* r, size_t n) noexcept;

    inline UPALWAYSINLINE
    bool heap_region_validate_args(allocator* ba, size_t csz, size_t losz, size_t aln, size_t off) noexcept {
        return ba && (csz >= heap_region_min_chunk_size) && ((csz & (csz - 1)) == 0)
            && (losz >= heap_region_min_large_object_size) && ((csz / 4) >= losz)
            && (aln >= sizeof(heap_region_recycle_item*)) && ((aln & (aln - 1)) == 0)
            && ((losz / 2) >= aln) && (aln > off);
    }
}

#endif
