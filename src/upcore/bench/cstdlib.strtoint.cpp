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

#include <up/cstdlib.hpp>
#include <up/cstdatomic.hpp>
#include <up/memory.hpp>
#include <up/test.hpp>
#include <random>

namespace cstdlib_strtoint
{
    std::default_random_engine random_engine(static_cast<uint32_t>(up::time(nullptr) & UINT32_MAX));

    struct integer_data_source
    {
        size_t num_strings;
        char** strings;
        int base;

        integer_data_source(size_t n, int b)
        : num_strings(n), base(b) {
            std::uniform_int_distribution<size_t> length_dist(2, 9);
            std::uniform_int_distribution<int> sign_dist(0, 3);
            std::uniform_int_distribution<int> digit_dist(0, base);
            size_t length;
            char* ptr, * end_ptr;
            int sign, digit;
            
            strings = up::malloc_n<char*>(num_strings);
            assert(strings);

            for (size_t i = 0; i < num_strings; ++i) {
                length = length_dist(random_engine);
                assert(length > 0);

                ptr = strings[i] = static_cast<char*>(up::malloc(length + 1));
                end_ptr = ptr + length;
                assert(ptr);

                sign = sign_dist(random_engine);
                if (sign == 1) {
                    *ptr++ = '+';
                }
                else if (sign == 2) {
                    *ptr++ = '-';
                }

                for ( ; ptr < end_ptr; ++ptr) {
                    digit = digit_dist(random_engine);
                    if (digit < 10) {
                        *ptr = static_cast<char>('0' + digit);
                    }
                    else {
                        *ptr = static_cast<char>('a' + (digit - 10));
                    }
                }

                *ptr = '\0';
            }
        }

        ~integer_data_source() {
            for (size_t i = 0; i < num_strings; ++i) {
                up::free(strings[i]);
                strings[i] = nullptr;
            }

            up::free_n(strings, num_strings);
            strings = nullptr;
        }

        void setup() { }
        void teardown() { }
    };

    #define DEFINE_BENCH_STRTOL(Function, Type) \
    UPNOINLINE void UPCONCATENATE(bench_, Function)(integer_data_source const& ds) { \
        size_t const num_strings = ds.num_strings; \
        char** const strings = ds.strings; \
        int const base = ds.base; \
        Type value; \
        for (size_t i = 0; i < num_strings; ++i) { \
            value = up::Function(strings[i], nullptr, base); \
            up::sink_dependency(value); \
        } \
    }

    DEFINE_BENCH_STRTOL(strtol, long)
    DEFINE_BENCH_STRTOL(strtoul, unsigned long)
    DEFINE_BENCH_STRTOL(strtoll, long long)
    DEFINE_BENCH_STRTOL(strtoull, unsigned long long)
    DEFINE_BENCH_STRTOL(fast_strtol, long)
    DEFINE_BENCH_STRTOL(fast_strtoul, unsigned long)
    DEFINE_BENCH_STRTOL(fast_strtoll, long long)
    DEFINE_BENCH_STRTOL(fast_strtoull, unsigned long long)
    DEFINE_BENCH_STRTOL(fast_strtoi32, int_least32_t)
    DEFINE_BENCH_STRTOL(fast_strtou32, uint_least32_t)
    DEFINE_BENCH_STRTOL(fast_strtoi64, int_least64_t)
    DEFINE_BENCH_STRTOL(fast_strtou64, uint_least64_t)

    #define DEFINE_BENCHMARK(Base, Size, Iterations) \
    UP_TEST_BENCHMARK(UPCONCATENATE(strtol_base, Base)) { \
        integer_data_source ds(Size, Base); \
        UP_TEST_RUN_BENCHMARK("strtol", Size, Iterations, &bench_strtol, ds); \
        UP_TEST_RUN_BENCHMARK("strtoul", Size, Iterations, &bench_strtoul, ds); \
        UP_TEST_RUN_BENCHMARK("strtoll", Size, Iterations, &bench_strtoll, ds); \
        UP_TEST_RUN_BENCHMARK("strtoull", Size, Iterations, &bench_strtoull, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtol", Size, Iterations, &bench_fast_strtol, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtoul", Size, Iterations, &bench_fast_strtoul, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtoll", Size, Iterations, &bench_fast_strtoll, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtoull", Size, Iterations, &bench_fast_strtoull, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtoi32", Size, Iterations, &bench_fast_strtoi32, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtou32", Size, Iterations, &bench_fast_strtou32, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtoi64", Size, Iterations, &bench_fast_strtoi64, ds); \
        UP_TEST_RUN_BENCHMARK("fast_strtou64", Size, Iterations, &bench_fast_strtou64, ds); \
    }

    DEFINE_BENCHMARK(2, 100000, 100)
    DEFINE_BENCHMARK(8, 100000, 100)
    DEFINE_BENCHMARK(10, 100000, 100)
    DEFINE_BENCHMARK(16, 100000, 100)
    DEFINE_BENCHMARK(32, 100000, 100)
}
