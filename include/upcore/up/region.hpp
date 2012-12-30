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

#ifndef UP_REGION_HPP
#define UP_REGION_HPP

#include <up/allocator.hpp>
#include <up/cstdalign.hpp>

namespace up
{
    struct linear_region;

    extern LIBUPCOREAPI UPPURE UPWARNRESULT
    size_t linear_region_space(size_t n, size_t alignment = alignof(max_align_t)) noexcept;
    
    extern LIBUPCOREAPI UPWARNRESULT
    linear_region* linear_region_construct(void* base, size_t space, size_t alignment = alignof(max_align_t), size_t offset = 0) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* linear_region_head_allocate(linear_region* r, size_t n) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* linear_region_head_allocate_zero(linear_region* r, size_t n, size_t s) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* linear_region_tail_allocate(linear_region* r, size_t n) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* linear_region_tail_allocate_zero(linear_region* r, size_t n, size_t s) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void* linear_region_head_position(linear_region* r) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void linear_region_head_rewind(linear_region* r) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void linear_region_head_rewind(linear_region* r, void* p) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void* linear_region_tail_position(linear_region* r) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void linear_region_tail_rewind(linear_region* r) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void linear_region_tail_rewind(linear_region* r, void* p) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void linear_region_rewind(linear_region* r) noexcept;

    extern LIBUPCOREAPI
    int linear_region_realign(linear_region* r, size_t alignment, size_t offset = 0) noexcept;

    extern LIBUPCOREAPI
    bool linear_region_validate(linear_region* r) noexcept;

    class LIBUPCOREAPI linear_head_allocator : public allocator
    {
    public:

        linear_head_allocator() noexcept;
        explicit linear_head_allocator(linear_region* r) noexcept;
        explicit linear_head_allocator(linear_head_allocator const* a) noexcept;
        explicit linear_head_allocator(linear_tail_allocator const* a) noexcept;
        linear_head_allocator(void* ptr, size_t space, size_t alignment = alignof(max_align_t), size_t offset = 0) noexcept;
        virtual ~linear_head_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n = 0) noexcept;
        UPHIDDEN linear_region* region() const noexcept;
        UPHIDDEN void region(linear_region* r) noexcept;
        UPHIDDEN void* position() const noexcept;
        UPHIDDEN void position(void* p) noexcept;
        UPHIDDEN void rewind() noexcept;

    private:

        linear_region* region_;
    };

    inline UPHIDDENINLINE
    linear_region* linear_head_allocator::region() const noexcept {
        return region_;
    }

    inline UPHIDDENINLINE
    void linear_head_allocator::region(linear_region* r) noexcept {
        region_ = r;
    }

    inline UPHIDDENINLINE
    void* linear_head_allocator::position() const noexcept {
        return linear_region_head_position(region_);
    }

    inline UPHIDDENINLINE
    void linear_head_allocator::position(void* p) noexcept {
        linear_region_head_rewind(region_, p);
    }

    inline UPHIDDENINLINE
    void linear_head_allocator::rewind() noexcept {
        linear_region_head_rewind(region_);
    }

    class LIBUPCOREAPI linear_tail_allocator : public allocator
    {
    public:

        linear_tail_allocator() noexcept;
        explicit linear_tail_allocator(linear_region* r) noexcept;
        explicit linear_tail_allocator(linear_tail_allocator const* a) noexcept;
        explicit linear_tail_allocator(linear_head_allocator const* a) noexcept;
        linear_tail_allocator(void* ptr, size_t space, size_t alignment = alignof(max_align_t), size_t offset = 0) noexcept;
        virtual ~linear_tail_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n = 0) noexcept;
        UPHIDDEN linear_region* region() const noexcept;
        UPHIDDEN void region(linear_region* r) noexcept;
        UPHIDDEN void* position() const noexcept;
        UPHIDDEN void position(void* p) noexcept;
        UPHIDDEN void rewind() noexcept;

    private:

        linear_region* region_;
    };

    inline UPHIDDENINLINE
    linear_region* linear_tail_allocator::region() const noexcept {
        return region_;
    }

    inline UPHIDDENINLINE
    void linear_tail_allocator::region(linear_region* r) noexcept {
        region_ = r;
    }

    inline UPHIDDENINLINE
    void* linear_tail_allocator::position() const noexcept {
        return linear_region_tail_position(region_);
    }

    inline UPHIDDENINLINE
    void linear_tail_allocator::position(void* p) noexcept {
        linear_region_tail_rewind(region_, p);
    }

    inline UPHIDDENINLINE
    void linear_tail_allocator::rewind() noexcept {
        linear_region_tail_rewind(region_);
    }

    constexpr size_t heap_region_default_chunk_size = 65536;
    constexpr size_t heap_region_default_large_object_size = 8192;
    constexpr size_t heap_region_min_chunk_size = 4096;
    constexpr size_t heap_region_min_large_object_size = 256;

    struct heap_region;
    struct heap_region_finalizer;

    struct LIBUPCOREAPI heap_region_metrics
    {
        size_t num_chunks;
        size_t num_small_objects;
        size_t num_large_objects;
        size_t num_finalizers;
        size_t usage;
        size_t peak_usage;
        size_t recycle_usage;
        size_t peak_recycle_usage;
    };

    extern LIBUPCOREAPI
    int heap_region_get_metrics(heap_region* r, heap_region_metrics* metrics) noexcept;

    extern LIBUPCOREAPI UPWARNRESULT
    heap_region* heap_region_construct(
        void* base,
        allocator* alloc,
        size_t chunk_size,
        size_t large_object_size,
        size_t alignment = alignof(max_align_t),
        size_t offset = 0
    )
    noexcept;

    extern LIBUPCOREAPI
    void heap_region_destruct(heap_region* r) noexcept;

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    heap_region* heap_region_create(
        allocator* alloc,
        size_t chunk_size = heap_region_default_chunk_size,
        size_t large_object_size = heap_region_default_large_object_size,
        size_t alignment = alignof(max_align_t),
        size_t offset = 0
    )
    noexcept;

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    heap_region* heap_region_create(
        size_t chunk_size = heap_region_default_chunk_size,
        size_t large_object_size = heap_region_default_large_object_size,
        size_t alignment = alignof(max_align_t),
        size_t offset = 0
    )
    noexcept;

    extern LIBUPCOREAPI
    void heap_region_destroy(heap_region* r) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void heap_region_rewind(heap_region* r) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void heap_region_shrink(heap_region* r) noexcept;

    extern LIBUPCOREAPI
    bool heap_region_validate(heap_region* r) noexcept;

    extern LIBUPCOREAPI
    heap_region_finalizer* heap_region_add_finalizer(heap_region* r, void (*action)(void*,size_t), void* data, size_t size) noexcept;

    extern LIBUPCOREAPI
    heap_region_finalizer* heap_region_add_finalizer(heap_region* r, allocator* alloc, void* data, size_t size) noexcept;

    extern LIBUPCOREAPI
    int heap_region_suppress_finalizer(heap_region_finalizer* finalizer) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* heap_region_allocate(heap_region* r, size_t n) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL UPALLOC UPWARNRESULT
    void* heap_region_allocate_zero(heap_region* r, size_t n, size_t s) noexcept;

    extern LIBUPCOREAPI UPNONNULL(1)
    int heap_region_deallocate(heap_region* r, void* p, size_t n) noexcept;

    class LIBUPCOREAPI heap_allocator : public allocator
    {
    public:

        heap_allocator() noexcept;
        explicit heap_allocator(heap_region* r) noexcept;
        explicit heap_allocator(heap_allocator* r) noexcept;
        virtual ~heap_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n = 0) noexcept;
        UPHIDDEN heap_region* region() const noexcept;
        UPHIDDEN void region(heap_region* r) noexcept;
        UPHIDDEN void rewind() noexcept;

    private:

        heap_region* region_;
    };

    inline UPHIDDENINLINE
    heap_region* heap_allocator::region() const noexcept {
        return region_;
    }

    inline UPHIDDENINLINE
    void heap_allocator::region(heap_region* r) noexcept {
        region_ = r;
    }

    inline UPHIDDENINLINE
    void heap_allocator::rewind() noexcept {
        heap_region_rewind(region_);
    }
}

#endif