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
#include <up/memory.hpp>
#include <up/test.hpp>
#include <random>

namespace cstdio_printf
{
	const char* double_format[] = {
		"%.16e",
		"%22.16e",
		"%022.16e",
		"%-22.16e",
		"%#+'022.16e",
		"foo|%#+0123.9E|bar",
		"%-123.9e",
		"%123.9e",
		"%+23.9e",
		"%+05.8e",
		"%-05.8e",
		"%05.8e",
		"%+5.8e",
		"%-5.8e",
		"% 5.8e",
		"%5.8e",
		"%+4.9e",
		"%+#010.0e",
		"%#10.1e",
		"%10.5e",
		"% 10.5e",
		"%5.0e",
		"%5.e",
		"%#5.0e",
		"%#5.e",
		"%3.2e",
		"%3.1e",
		"%-1.5e",
		"%1.5e",
		"%01.3e",
		"%1.e",
		"%.1e",
		"%#.0e",
		"%+.0e",
		"% .0e",
		"%.0e",
		"%#.e",
		"%+.e",
		"% .e",
		"%.e",
		"%4e",
		"%e",
		"%E",
		"foo|%#+0123.9F|bar",
		"%-123.9f",
		"%123.9f",
		"%+23.9f",
		"%+#010.0f",
		"%#10.1f",
		"%10.5f",
		"% 10.5f",
		"%+05.8f",
		"%-05.8f",
		"%05.8f",
		"%+5.8f",
		"%-5.8f",
		"% 5.8f",
		"%5.8f",
		"%5.0f",
		"%5.f",
		"%#5.0f",
		"%#5.f",
		"%+4.9f",
		"%3.2f",
		"%3.1f",
		"%-1.5f",
		"%1.5f",
		"%01.3f",
		"%1.f",
		"%.1f",
		"%#.0f",
		"%+.0f",
		"% .0f",
		"%.0f",
		"%#.f",
		"%+.f",
		"% .f",
		"%.f",
		"%4f",
		"%f",
		"%F",
		"foo|%#+0123.9G|bar",
		"%-123.9g",
		"%123.9g",
		"%+23.9g",
		"%+05.8g",
		"%-05.8g",
		"%05.8g",
		"%+5.8g",
		"%-5.8g",
		"% 5.8g",
		"%5.8g",
		"%+4.9g",
		"%+#010.0g",
		"%#10.1g",
		"%10.5g",
		"% 10.5g",
		"%5.0g",
		"%5.g",
		"%#5.0g",
		"%#5.g",
		"%3.2g",
		"%3.1g",
		"%-1.5g",
		"%1.5g",
		"%01.3g",
		"%1.g",
		"%.1g",
		"%#.0g",
		"%+.0g",
		"% .0g",
		"%.0g",
		"%#.g",
		"%+.g",
		"% .g",
		"%.g",
		"%4g",
		"%g",
		"%G",
		nullptr
	};

    const char* long_format[] = {
		"foo|%0123ld|bar",
		"%123.9ld",
		"% 123.9ld",
		"%+123.9ld",
		"%-123.9ld",
		"%0123ld",
		"% 0123ld",
		"%+0123ld",
		"%-0123ld",
		"%10.5ld",
		"% 10.5ld",
		"%+10.5ld",
		"%-10.5ld",
		"%010ld",
		"% 010ld",
		"%+010ld",
		"%-010ld",
		"%4.2ld",
		"% 4.2ld",
		"%+4.2ld",
		"%-4.2ld",
		"%04ld",
		"% 04ld",
		"%+04ld",
		"%-04ld",
		"%5.5ld",
		"%+22.33ld",
		"%01.3ld",
		"%1.5ld",
		"%-1.5ld",
		"%44ld",
		"%4ld",
		"%4.0ld",
		"%4.ld",
		"%.44ld",
		"%.4ld",
		"%.0ld",
		"%.ld",
		"%ld",
		nullptr
	};
    
    const char* ulong_format[] = {
		"foo|%0123lu|bar",
		"%123.9lu",
		"% 123.9lu",
		"%+123.9lu",
		"%-123.9lu",
		"%0123lu",
		"% 0123lu",
		"%+0123lu",
		"%-0123lu",
		"%5.5lu",
		"%+22.33lu",
		"%01.3lu",
		"%1.5lu",
		"%-1.5lu",
		"%44lu",
		"%lu",
		"foo|%#0123lo|bar",
		"%#123.9lo",
		"%# 123.9lo",
		"%#+123.9lo",
		"%#-123.9lo",
		"%#0123lo",
		"%# 0123lo",
		"%#+0123lo",
		"%#-0123lo",
		"%#5.5lo",
		"%#+22.33lo",
		"%#01.3lo",
		"%#1.5lo",
		"%#-1.5lo",
		"%#44lo",
		"%#lo",
		"%123.9lo",
		"% 123.9lo",
		"%+123.9lo",
		"%-123.9lo",
		"%0123lo",
		"% 0123lo",
		"%+0123lo",
		"%-0123lo",
		"%5.5lo",
		"%+22.33lo",
		"%01.3lo",
		"%1.5lo",
		"%-1.5lo",
		"%44lo",
		"%lo",
		"foo|%#0123lX|bar",
		"%#123.9lx",
		"%# 123.9lx",
		"%#+123.9lx",
		"%#-123.9lx",
		"%#0123lx",
		"%# 0123lx",
		"%#+0123lx",
		"%#-0123lx",
		"%#5.5lx",
		"%#+22.33lx",
		"%#01.3lx",
		"%#1.5lx",
		"%#-1.5lx",
		"%#44lx",
		"%#lx",
		"%#lX",
		"%123.9lx",
		"% 123.9lx",
		"%+123.9lx",
		"%-123.9lx",
		"%0123lx",
		"% 0123lx",
		"%+0123lx",
		"%-0123lx",
		"%5.5lx",
		"%+22.33lx",
		"%01.3lx",
		"%1.5lx",
		"%-1.5lx",
		"%44lx",
		"%lx",
		"%lX",
		nullptr
	};

	const char* llong_format[] = {
		"foo|%0123lld|bar",
		"%123.9lld",
		"% 123.9lld",
		"%+123.9lld",
		"%-123.9lld",
		"%0123lld",
		"% 0123lld",
		"%+0123lld",
		"%-0123lld",
		"%5.5lld",
		"%+22.33lld",
		"%01.3lld",
		"%1.5lld",
		"%-1.5lld",
		"%44lld",
		"%lld",
		nullptr
	};
	    
    const char* string_format[] = {
		"foo|%10.10s|bar",
		"%-10.10s",
		"%10.10s",
		"%10.5s",
		"%5.10s",
		"%10.1s",
		"%1.10s",
		"%10.0s",
		"%0.10s",
		"%-42.5s",
		"%2.s",
		"%.10s",
		"%.1s",
		"%.0s",
		"%.s",
		"%4s",
		"%s",
		nullptr
	};

	const char* pointer_format[] = {
		"foo|%p|bar",
		"%42p",
		"%p",
		nullptr
	};

    struct data_source
    {
        size_t count;
        long* long_values;
        unsigned long* ulong_values;
        long long* llong_values;
        double* double_values;

        data_source(size_t n)
        : count(n) {
            std::default_random_engine random_engine(static_cast<uint32_t>(up::time(nullptr) & UINT32_MAX));
            std::uniform_int_distribution<long> long_dist(LONG_MIN, LONG_MAX);
            std::uniform_int_distribution<unsigned long> ulong_dist(0, ULONG_MAX);
            std::uniform_int_distribution<long long> llong_dist(LLONG_MIN, LLONG_MAX);
            std::uniform_real_distribution<double> double_dist(-FLT_MAX, FLT_MAX);

            long_values = up::malloc_n<long>(count);
            ulong_values = up::malloc_n<unsigned long>(count);
            llong_values = up::malloc_n<long long>(count);
            double_values = up::malloc_n<double>(count);
            require(double_values && long_values && ulong_values && llong_values);

            for (size_t i = 0; i < count; ++i) {
                long_values[i] = long_dist(random_engine);
                ulong_values[i] = ulong_dist(random_engine);
                llong_values[i] = llong_dist(random_engine);
                double_values[i] = double_dist(random_engine);
            }
        }

        ~data_source() {
            /*for (size_t i = 0; i < num_strings; ++i) {
                up::free(strings[i]);
                strings[i] = nullptr;
            }

            up::free_n(strings, num_strings);*/
            up::free_n(double_values, count);
            up::free_n(llong_values, count);
            up::free_n(ulong_values, count);
            up::free_n(long_values, count);
        }

        void setup() { }
        void teardown() { }
    };

#define DEFINE_BENCH_VSNPRINTF_LIST(Function, FormatList, ValueList) \
        for (size_t i = 0; FormatList[i] != nullptr; ++i) { \
            char const* const current_format = FormatList[i]; \
            for (size_t j = 0; j < count; ++j) { \
                int length = up::Function(buffer, sizeof(buffer), current_format, ds.ValueList[j]); \
                up::sink_dependency(length); \
            } \
        }

#define DEFINE_BENCH_VSNPRINTF(Function) \
    UPNOINLINE void UPCONCATENATE(bench_, Function)(data_source const& ds) { \
        size_t const count = ds.count; \
        char buffer[1024]; \
        DEFINE_BENCH_VSNPRINTF_LIST(Function, long_format, long_values); \
        DEFINE_BENCH_VSNPRINTF_LIST(Function, ulong_format, ulong_values); \
        DEFINE_BENCH_VSNPRINTF_LIST(Function, llong_format, llong_values); \
        DEFINE_BENCH_VSNPRINTF_LIST(Function, double_format, double_values); \
    }

    DEFINE_BENCH_VSNPRINTF(snprintf)
    DEFINE_BENCH_VSNPRINTF(fast_snprintf)

    UP_TEST_BENCHMARK(printf) {
        constexpr size_t Size = 10000;
        constexpr size_t Iterations = 10;
        data_source ds(Size);
        UP_TEST_RUN_BENCHMARK("snprintf", Size, Iterations, &bench_snprintf, ds);
        UP_TEST_RUN_BENCHMARK("fast_snprintf", Size, Iterations, &bench_fast_snprintf, ds); 
    }
}
