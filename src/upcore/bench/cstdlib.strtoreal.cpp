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

#include <up/cstdio.hpp>
#include <up/cstdlib.hpp>
#include <up/memory.hpp>
#include <up/random.hpp>
#include <up/test.hpp>

namespace cstdlib_strtoreal
{
    struct real_data_source
    {
        size_t num_strings;
        char** strings;
        int base;

        real_data_source(size_t n, int b)
        : num_strings(n), base(b) {
            up::default_random_engine random_engine(static_cast<uint_least32_t>(up::time(nullptr) & UINT_LEAST32_MAX));
            up::uniform_real_distribution<double> value_dist(-FLT_MAX, FLT_MAX);
            char buffer[256];
            double value;
            int length;
            
            strings = up::malloc_n<char*>(num_strings);
            require(strings);

            for (size_t i = 0; i < num_strings; ++i) {
                value = value_dist(random_engine);
                length = up::snprintf(buffer, sizeof(buffer), "%.12e", value);
                strings[i] = up::strndup(buffer, length);
                require(strings[i]);
            }
        }

        ~real_data_source() {
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

    #define DEFINE_BENCH_STRTOREAL(FunctionName, Function, Type) \
    UPNOINLINE void UPCONCATENATE(bench_, FunctionName)(real_data_source const& ds) { \
        size_t const num_strings = ds.num_strings; \
        char** const strings = ds.strings; \
        Type value; \
        for (size_t i = 0; i < num_strings; ++i) { \
            value = Function(strings[i], nullptr); \
            up::sink_dependency(value); \
        } \
    }

    DEFINE_BENCH_STRTOREAL(libc_strtod, ::strtod, double)
    DEFINE_BENCH_STRTOREAL(strtof, up::strtof, float)
    DEFINE_BENCH_STRTOREAL(strtod, up::strtod, double)
    DEFINE_BENCH_STRTOREAL(strtold, up::strtold, long double)
    DEFINE_BENCH_STRTOREAL(fast_strtof, up::fast_strtof, float)
    DEFINE_BENCH_STRTOREAL(fast_strtod, up::fast_strtod, double)
    DEFINE_BENCH_STRTOREAL(fast_strtold, up::fast_strtold, long double)
    
    #define DEFINE_BENCHMARK(Base, Size, Iterations) \
    UP_TEST_BENCHMARK(UPCONCATENATE(strtol_base, Base)) { \
        real_data_source ds(Size, Base); \
        UP_TEST_RUN_BENCHMARK("libc strtod", Size, Iterations, &bench_libc_strtod, ds); \
        UP_TEST_RUN_BENCHMARK("up::strtof", Size, Iterations, &bench_strtof, ds); \
        UP_TEST_RUN_BENCHMARK("up::strtod", Size, Iterations, &bench_strtod, ds); \
        UP_TEST_RUN_BENCHMARK("up::strtold", Size, Iterations, &bench_strtold, ds); \
        UP_TEST_RUN_BENCHMARK("up::fast_strtof", Size, Iterations, &bench_fast_strtof, ds); \
        UP_TEST_RUN_BENCHMARK("up::fast_strtod", Size, Iterations, &bench_fast_strtod, ds); \
        UP_TEST_RUN_BENCHMARK("up::fast_strtold", Size, Iterations, &bench_fast_strtold, ds); \
    }

    DEFINE_BENCHMARK(10, 100000, 100)
    DEFINE_BENCHMARK(16, 100000, 100)
}
