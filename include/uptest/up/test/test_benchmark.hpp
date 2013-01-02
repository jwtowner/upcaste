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

#ifndef UP_TEST_TEST_BENCHMARK_HPP
#define UP_TEST_TEST_BENCHMARK_HPP

#include <up/test/test_benchmark_result.hpp>
#include <up/test/test_case.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_benchmark : public test_case
    {
    public:

        virtual ~test_benchmark();
        virtual test_result const& run(test_listener& listener, test_filter& filter);
                
    protected:
                
        test_benchmark(char const* name, char const* file_name, long line_number);
        template <class Function>
        UPVISIBLE void run_benchmark(char const* category, unsigned int complexity, unsigned int iterations, Function function);
        template <class Function, class Fixture>
        UPVISIBLE void run_benchmark(char const* category, unsigned int complexity, unsigned int iterations, Function function, Fixture& fixture);
        bool is_absolute_winner(char const* category) const;
        bool is_average_winner(char const* category) const;
            
    private:

        test_benchmark_result& register_benchmark(char const* category, unsigned int complexity, unsigned int iterations);
        static void finalize_benchmark(test_benchmark_result& result, timespec const* start, timespec const* stop);

    private:

        test_benchmark_impl* impl_;
    };
        
    template <class Function>
    UPVISIBLE void test_benchmark::run_benchmark(
        char const* category,
        unsigned int complexity,
        unsigned int iterations,
        Function function
    ) {
        test_benchmark_result& result = register_benchmark(category, complexity, iterations);
        timespec start, stop;

        UPVERIFY(!clock_gettime(CLOCK_MONOTONIC, &start));

        for (unsigned int i = 0; i < iterations; ++i) {
            function();
        }

        UPVERIFY(!clock_gettime(CLOCK_MONOTONIC, &stop));

        finalize_benchmark(result, &start, &stop);
    }

    template <class Function, class Fixture>
    UPVISIBLE void test_benchmark::run_benchmark(
        char const* category,
        unsigned int complexity,
        unsigned int iterations,
        Function function,
        Fixture& fixture
    ) {
        test_benchmark_result& result = register_benchmark(category, complexity, iterations);
        timespec start, stop, diff, total = { 0, 0 };

        for (unsigned int i = 0; i < iterations; ++i) {
            fixture.setup();
            UPVERIFY(!clock_gettime(CLOCK_MONOTONIC, &start));
            function(fixture);
            UPVERIFY(!clock_gettime(CLOCK_MONOTONIC, &stop));
            timespec_subtract(&start, &stop, &diff);
            timespec_add(&diff, &total, &total);
            fixture.teardown();
        }

        start.tv_sec = start.tv_nsec = 0;
        stop = total;

        finalize_benchmark(result, &start, &stop);
    }
}}

#endif
