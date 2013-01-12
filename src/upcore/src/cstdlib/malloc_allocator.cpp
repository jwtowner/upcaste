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

#include <up/atomic.hpp>
#include <up/cstdlib.hpp>
#include <up/cstdalign.hpp>
#include <new>

namespace up
{
    // Use double-checked locking pattern to initialize the
    // allocator instance from static storage. Note that we
    // never want the destructor for the default instance
    // of malloc_allocator to be called, as other subsystems
    // may keep a pointer to it and access it directly, even
    // from within atexit handlers.

    namespace
    {
        alignas(UP_MAX_CACHE_LINE_SIZE) char instance_storage[sizeof(malloc_allocator)];
        atomic<malloc_allocator*> instance_ptr(0);
        atomic_flag lock_flag = ATOMIC_FLAG_INIT;
    }

    LIBUPCOREAPI
    malloc_allocator* malloc_allocator::instance() noexcept {
        malloc_allocator* alloc = instance_ptr.load(memory_order_consume);
        if (alloc) {
            return alloc;
        }

        atomic_flag_spin_lock_explicit(&lock_flag, memory_order_acquire);
        alloc = instance_ptr.load(memory_order_consume);
        if (!alloc) {
            alloc = ::new(&instance_storage) malloc_allocator;
            instance_ptr.store(alloc, memory_order_release);
        }
        atomic_flag_clear_explicit(&lock_flag, memory_order_release);
        return alloc;
    }

    LIBUPCOREAPI
    malloc_allocator::malloc_allocator() noexcept {
    }
    
    LIBUPCOREAPI
    malloc_allocator::~malloc_allocator() noexcept {
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* malloc_allocator::allocate(size_t n) noexcept {
        return malloc(n);
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* malloc_allocator::allocate_zero(size_t n, size_t s) noexcept {
        return calloc(n, s);
    }

    LIBUPCOREAPI
    void malloc_allocator::deallocate(void* p, size_t) noexcept {
        free(p);
    }
}
