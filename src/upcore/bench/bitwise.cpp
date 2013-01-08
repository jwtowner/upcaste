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
#include <up/ctime.hpp>
#include <up/random.hpp>
#include <up/test.hpp>

namespace bitwise
{
    up::default_random_engine random_engine(static_cast<uint_least32_t>(up::time(nullptr) & UINT_LEAST32_MAX));
    constexpr size_t count = 100000;

    inline UPALWAYSINLINE UPPURE
    unsigned int oldceil2(unsigned int x) noexcept {
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return ++x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long oldceil2(unsigned long x) noexcept {
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
#if (ULONG_MAX > 0xFFFFFFFF)
        x |= x >> 32;
#endif
        return ++x;
    }

    inline UPALWAYSINLINE UPPURE
    unsigned long long oldceil2(unsigned long long x) noexcept {
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;
        return ++x;
    }

    template <class T, T Max>
    UPNOINLINE void ceil2_bench() {
        up::uniform_int_distribution<T> dist(0, Max);
        for (size_t i = 0; i < count; ++i) {
            T value = up::ceil2(dist(random_engine));
            up::sink_dependency(&value);
        }
    }

    template <class T, T Max>
    UPNOINLINE void oldceil2_bench() {
        up::uniform_int_distribution<T> dist(0, Max);
        for (size_t i = 0; i < count; ++i) {
            T value = oldceil2(dist(random_engine));
            up::sink_dependency(&value);
        }
    }

    UP_TEST_BENCHMARK(allocator_bench) {
        UP_TEST_RUN_BENCHMARK("ceil2_uint", count, 200, &ceil2_bench<unsigned int, UINT_MAX>);
        UP_TEST_RUN_BENCHMARK("ceil2_ulong", count, 200, &ceil2_bench<unsigned long, ULONG_MAX>);
        UP_TEST_RUN_BENCHMARK("ceil2_ullong", count, 200, &ceil2_bench<unsigned long long, ULLONG_MAX>);
        UP_TEST_RUN_BENCHMARK("oldceil2_uint", count, 200, &oldceil2_bench<unsigned int, UINT_MAX>);
        UP_TEST_RUN_BENCHMARK("oldceil2_ulong", count, 200, &oldceil2_bench<unsigned long, ULONG_MAX>);
        UP_TEST_RUN_BENCHMARK("oldceil2_ullong", count, 200, &oldceil2_bench<unsigned long long, ULLONG_MAX>);
    }
}
