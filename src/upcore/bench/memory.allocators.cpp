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

#include <up/bitwise.hpp>
#include <up/cstdatomic.hpp>
#include <up/memory.hpp>
#include <up/page.hpp>
#include <up/region.hpp>
#include <up/ctime.hpp>
#include <up/test.hpp>
#include <random>

namespace memory_allocators
{
    std::default_random_engine random_engine(static_cast<up::uint32_t>(up::time(nullptr) & UINT32_MAX));

    template <size_t Size, size_t Count>
    UPNOINLINE void malloc_bench() {
        std::uniform_int_distribution<uint32_t> dist(1, Size);
        void* ptrs[Count];

        for (size_t i = 0; i < Count; ++i) {
            size_t sz = (Size > 1) ? up::ceil2(dist(random_engine)) : 1;
            ptrs[i] = up::malloc(sz);
            require(ptrs[i]);
        }

        for (size_t i = 0; i < Count; ++i) {
            up::free(ptrs[i]);
        }

        up::sink_dependency(&ptrs);
    }

    template <size_t Size, size_t Count>
    UPNOINLINE void malloc_allocator_bench() {
        up::allocator* alloc = up::malloc_allocator::instance();
        std::uniform_int_distribution<uint32_t> dist(1, Size);
        void* ptrs[Count];

        for (size_t i = 0; i < Count; ++i) {
            size_t sz = (Size > 1) ? up::ceil2(dist(random_engine)) : 1;
            ptrs[i] = up::allocate(alloc, sz);
            require(ptrs[i]);
        }

        for (size_t i = 0; i < Count; ++i) {
            up::deallocate(alloc, ptrs[i]);
        }

        up::sink_dependency(&ptrs);
    }

    template <size_t Size, size_t Count>
    UPNOINLINE void linear_region_bench() {
        std::uniform_int_distribution<uint32_t> dist(1, Size);
        void* ptrs[Count];

        size_t page_size = up::linear_region_space(Size * Count, 1);
        void* page = up::page_allocate(page_size, up::page_access_read_write);
        require(page);

        up::linear_region* r = up::linear_region_construct(page, page_size, 1);
        require(r);

        for (size_t i = 0; i < Count; ++i) {
            size_t sz = (Size > 1) ? up::ceil2(dist(random_engine)) : 1;
            ptrs[i] = up::linear_region_head_allocate(r, sz);
            require(ptrs[i]);
        }

        require(!up::page_deallocate(page, page_size));
        up::sink_dependency(&ptrs);
    }
    
    template <size_t Size, size_t Count>
    UPNOINLINE void linear_region_allocator_bench() {
        std::uniform_int_distribution<uint32_t> dist(1, Size);
        void* ptrs[Count];

        size_t page_size = up::linear_region_space(Size * Count, 1);
        void* page = up::page_allocate(page_size, up::page_access_read_write);
        require(page);

        up::linear_region* r = up::linear_region_construct(page, page_size, 1);
        require(r);

        up::allocator* head_alloc = up::linear_region_head_allocator(r);

        for (size_t i = 0; i < Count; ++i) {
            size_t sz = (Size > 1) ? up::ceil2(dist(random_engine)) : 1;
            ptrs[i] = up::allocate(head_alloc, sz);
            require(ptrs[i]);
        }

        require(!up::page_deallocate(page, page_size));
        up::sink_dependency(&ptrs);
    }

    template <size_t Size, size_t Count>
    UPNOINLINE void heap_region_bench() {
        std::uniform_int_distribution<uint32_t> dist(1, Size);
        void* ptrs[Count];

        up::heap_region* r = up::heap_region_create();
        require(r);

        for (size_t i = 0; i < Count; ++i) {
            size_t sz = (Size > 1) ? up::ceil2(dist(random_engine)) : 1;
            ptrs[i] = up::heap_region_allocate(r, sz);
            require(ptrs[i]);
        }

        up::heap_region_destroy(r);
        up::sink_dependency(&ptrs);
    }

    UP_TEST_BENCHMARK(allocator_bench) {
        UP_TEST_RUN_BENCHMARK("malloc", 1, 100, &malloc_bench<1, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 2, 100, &malloc_bench<2, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 4, 100, &malloc_bench<4, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 8, 100, &malloc_bench<8, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 16, 100, &malloc_bench<16, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 32, 100, &malloc_bench<32, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 64, 100, &malloc_bench<64, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 128, 100, &malloc_bench<128, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 256, 100, &malloc_bench<256, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 512, 100, &malloc_bench<512, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc", 1024, 100, &malloc_bench<1024, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 1, 100, &malloc_allocator_bench<1, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 2, 100, &malloc_allocator_bench<2, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 4, 100, &malloc_allocator_bench<4, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 8, 100, &malloc_allocator_bench<8, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 16, 100, &malloc_allocator_bench<16, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 32, 100, &malloc_allocator_bench<32, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 64, 100, &malloc_allocator_bench<64, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 128, 100, &malloc_allocator_bench<128, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 256, 100, &malloc_allocator_bench<256, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 512, 100, &malloc_allocator_bench<512, 20000>);
        UP_TEST_RUN_BENCHMARK("malloc_allocator", 1024, 100, &malloc_allocator_bench<1024, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 1, 100, &linear_region_bench<1, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 2, 100, &linear_region_bench<2, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 4, 100, &linear_region_bench<4, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 8, 100, &linear_region_bench<8, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 16, 100, &linear_region_bench<16, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 32, 100, &linear_region_bench<32, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 64, 100, &linear_region_bench<64, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 128, 100, &linear_region_bench<128, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 256, 100, &linear_region_bench<256, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 512, 100, &linear_region_bench<512, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region", 1024, 100, &linear_region_allocator_bench<1024, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 1, 100, &linear_region_allocator_bench<1, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 2, 100, &linear_region_allocator_bench<2, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 4, 100, &linear_region_allocator_bench<4, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 8, 100, &linear_region_allocator_bench<8, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 16, 100, &linear_region_allocator_bench<16, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 32, 100, &linear_region_allocator_bench<32, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 64, 100, &linear_region_allocator_bench<64, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 128, 100, &linear_region_allocator_bench<128, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 256, 100, &linear_region_allocator_bench<256, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 512, 100, &linear_region_allocator_bench<512, 20000>);
        UP_TEST_RUN_BENCHMARK("linear_region_allocator", 1024, 100, &linear_region_allocator_bench<1024, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 1, 100, &heap_region_bench<1, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 2, 100, &heap_region_bench<2, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 4, 100, &heap_region_bench<4, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 8, 100, &heap_region_bench<8, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 16, 100, &heap_region_bench<16, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 32, 100, &heap_region_bench<32, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 64, 100, &heap_region_bench<64, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 128, 100, &heap_region_bench<128, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 256, 100, &heap_region_bench<256, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 512, 100, &heap_region_bench<512, 20000>);
        UP_TEST_RUN_BENCHMARK("heap_region", 1024, 100, &heap_region_bench<1024, 20000>);
    }
}
