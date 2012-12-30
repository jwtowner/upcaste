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

#include <up/cstdatomic.hpp>
#include <up/densemap.hpp>
#include <up/sparsemap.hpp>
#include <up/test.hpp>

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(disable:4702)
#endif

#include <unordered_map>
#include <map>

namespace container_maps
{
    template <int Count>
    UPNOINLINE void map_insert_bench() {
        std::map<int, float> map;

        for (int i = 0; i < Count; ++i) {
            map.insert(std::pair<int, float>(i, i * 2.0f));
        }

        up::sink_dependency(&map);
    }

    template <int Count>
    UPNOINLINE void unordered_map_insert_bench() {
        std::unordered_map<int, float> map(Count);

        for (int i = 0; i < Count; ++i) {
            map.insert(std::pair<int, float>(i, i * 2.0f));
        }

        up::sink_dependency(&map);
    }

    template <int Count>
    UPNOINLINE void densemap_insert_bench() {
        up::densemap<int, float, std::hash<int>, std::equal_to<int>> map;
        up::allocator* const alloc = up::malloc_allocator::instance();
        int retval;

        retval = up::densemap_construct(map, alloc, Count);
        require(retval == up::dense_success);

        for (int i = 0; i < Count; ++i) {
            up::densemap_insert(map, i, i * 2.0f);
        }

        up::sink_dependency(&map);

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
    }

    template <int Count>
    UPNOINLINE void sparsemap_insert_bench() {
        up::sparsemap<int, float, std::hash<int>, std::equal_to<int>> map;
        up::allocator* const alloc = up::malloc_allocator::instance();
        int retval;

        retval = up::sparsemap_construct(map, alloc, Count);
        require(retval == up::sparse_success);

        for (int i = 0; i < Count; ++i) {
            up::sparsemap_insert(map, alloc, i, i * 2.0f);
        }

        up::sink_dependency(&map);

        retval = up::sparsemap_destruct(map, alloc, alloc);
        require(retval == up::sparse_success);
    }

    UP_TEST_BENCHMARK(insert_bench) {
        UP_TEST_RUN_BENCHMARK("map", 100000, 100, &map_insert_bench<100000>);
        UP_TEST_RUN_BENCHMARK("unordered_map", 100000, 100, &unordered_map_insert_bench<100000>);
        UP_TEST_RUN_BENCHMARK("densemap", 100000, 100, &densemap_insert_bench<100000>);
        UP_TEST_RUN_BENCHMARK("sparsemap", 100000, 100, &sparsemap_insert_bench<100000>);
    }
}
