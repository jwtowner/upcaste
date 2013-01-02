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

#include <up/test/test_benchmark.hpp>
#include <up/test/test_filter.hpp>
#include <up/test/test_listener.hpp>
#include <up/cfloat.hpp>
#include <up/cstring.hpp>
#include <up/memory.hpp>
#include <vector>

namespace up { namespace test
{
    typedef std::vector<test_benchmark_result> result_container;

    struct UPHIDDEN test_benchmark_impl
    {
        result_container results;
    };

    LIBUPTESTAPI
    test_benchmark::test_benchmark(char const* name, char const* file, long line)
    : test_case(name, file, line),
    impl_(malloc_construct<test_benchmark_impl>()) {
        impl_->results.reserve(8);
    }
        
    LIBUPTESTAPI
    test_benchmark::~test_benchmark() {
        destruct_free(impl_);
    }

    LIBUPTESTAPI
    test_result const& test_benchmark::run(test_listener& listener, test_filter& filter) {
        impl_->results.clear();
        test_result const& results = test_case::run(listener, filter);
        listener.benchmark_results(*this, impl_->results.data(), impl_->results.data() + impl_->results.size());
        return results;
    }

    LIBUPTESTAPI
    bool test_benchmark::is_absolute_winner(char const* category) const {
        assert(category);

        char const* winner_name = nullptr;
        double winner_time = -DBL_MAX;

        for (result_container::const_iterator itr = impl_->results.begin(), end = impl_->results.end(); itr != end; ++itr) {
            if (itr->total_time_ >= winner_time) {
                winner_name = itr->category_;
                winner_time = itr->total_time_;
            }
        }

        return winner_name ? !strcmp(category, winner_name) : false;
    }

    LIBUPTESTAPI
    bool test_benchmark::is_average_winner(char const* category) const {
        assert(category);
            
        char const* winner_name = nullptr;
        double winner_time = -DBL_MAX;
            
        for (result_container::const_iterator itr = impl_->results.begin(), end = impl_->results.end(); itr != end; ++itr) {
            if (itr->average_time_ >= winner_time) {
                winner_name = itr->category_;
                winner_time = itr->average_time_;
            }
        }

        return winner_name ? !strcmp(category, winner_name) : false;
    }

    LIBUPTESTAPI
    test_benchmark_result& test_benchmark::register_benchmark(char const* category, unsigned int complexity, unsigned int iterations) {
        assert(category);
        impl_->results.push_back(test_benchmark_result(category, complexity, iterations));
        return impl_->results.back();
    }
        
    LIBUPTESTAPI
    void test_benchmark::finalize_benchmark(test_benchmark_result& result, timespec const* start, timespec const* stop) {
        result.total_time_ = timespec_diff(start, stop);
        result.average_time_ = result.total_time_ / static_cast<double>(result.iterations_);
    }
}}
