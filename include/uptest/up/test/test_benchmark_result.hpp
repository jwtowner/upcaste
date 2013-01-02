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

#ifndef UP_TEST_TEST_BENCHMARK_RESULT_HPP
#define UP_TEST_TEST_BENCHMARK_RESULT_HPP

#include <up/test/testfwd.hpp>
#include <up/cstring.hpp>
#include <up/cstdlib.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_benchmark_result
    {
        friend class test_benchmark;

    public:

        UPHIDDEN test_benchmark_result(char const* category, unsigned int complexity, unsigned int iterations) noexcept;
        UPHIDDEN test_benchmark_result(test_benchmark_result const& x) noexcept;
        UPHIDDEN test_benchmark_result& operator=(test_benchmark_result const& x) noexcept;
        UPHIDDEN test_benchmark_result(test_benchmark_result&& x) noexcept;
        UPHIDDEN test_benchmark_result& operator=(test_benchmark_result&& x) noexcept;
        UPHIDDEN ~test_benchmark_result();
        UPHIDDEN char const* category() const noexcept;
        UPHIDDEN unsigned int complexity() const noexcept;
        UPHIDDEN unsigned int iterations() const noexcept;
        UPHIDDEN double average_time() const noexcept;
        UPHIDDEN double total_time() const noexcept;

    private:

        char* category_;
        unsigned int complexity_;
        unsigned int iterations_;
        double average_time_;
        double total_time_;
    };

    inline UPHIDDENINLINE
    test_benchmark_result::test_benchmark_result(char const* category, unsigned int complexity, unsigned int iterations) noexcept
    : category_(category ? strdup(category) : nullptr),
    complexity_(complexity),
    iterations_(iterations),
    average_time_(0.0),
    total_time_(0.0) {
    }

    inline UPHIDDENINLINE
    test_benchmark_result::test_benchmark_result(test_benchmark_result const& x) noexcept
    : category_(x.category_ ? strdup(x.category_) : nullptr),
    complexity_(x.complexity_),
    iterations_(x.iterations_),
    average_time_(x.average_time_),
    total_time_(x.total_time_) {
    }
    
    inline UPHIDDENINLINE
    test_benchmark_result& test_benchmark_result::operator=(test_benchmark_result const& x) noexcept {
        if (this != &x) {
            free(category_);
            category_ = x.category_ ? strdup(x.category_) : nullptr;
            complexity_ = x.complexity_;
            iterations_ = x.iterations_;
            average_time_ = x.average_time_;
            total_time_ = x.total_time_;
        }
        return *this;
    }
    
    inline UPHIDDENINLINE
    test_benchmark_result::test_benchmark_result(test_benchmark_result&& x) noexcept
    : category_(x.category_),
    complexity_(x.complexity_),
    iterations_(x.iterations_),
    average_time_(x.average_time_),
    total_time_(x.total_time_) {
        x.category_ = nullptr;
    }
    
    inline UPHIDDENINLINE
    test_benchmark_result& test_benchmark_result::operator=(test_benchmark_result&& x) noexcept {
        if (this != &x) {
            free(category_);
            category_ = x.category_;
            x.category_ = nullptr;
            complexity_ = x.complexity_;
            iterations_ = x.iterations_;
            average_time_ = x.average_time_;
            total_time_ = x.total_time_;
        }
        return *this;
    }
    
    inline UPHIDDENINLINE
    test_benchmark_result::~test_benchmark_result() {
        free(category_);
    }

    inline UPHIDDENINLINE
    char const* test_benchmark_result::category() const noexcept {
        return category_;
    }

    inline UPHIDDENINLINE
    unsigned int test_benchmark_result::complexity() const noexcept {
        return complexity_;
    }

    inline UPHIDDENINLINE
    unsigned int test_benchmark_result::iterations() const noexcept {
        return iterations_;
    }

    inline UPHIDDENINLINE
    double test_benchmark_result::average_time() const noexcept {
        return average_time_;
    }

    inline UPHIDDENINLINE
    double test_benchmark_result::total_time() const noexcept {
        return total_time_;
    }
}}

#endif
