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

#include <up/test/test_runner.hpp>
#include <up/test/test_suite.hpp>
#include <up/test/identity_filter.hpp>
#include <up/memory.hpp>
#include <vector>

namespace up { namespace test
{
    typedef std::vector<test_listener*> listener_container;

    struct UPHIDDEN test_runner_impl
    {
        listener_container listeners;
    };

    LIBUPTESTAPI
    test_runner::test_runner()
    : suite_(test_suite::instance()),
    impl_(malloc_construct<test_runner_impl>()) {
    }
        
    LIBUPTESTAPI
    test_runner::test_runner(test_suite& suite)
    : suite_(suite),
    impl_(malloc_construct<test_runner_impl>()) {
    }

    LIBUPTESTAPI
    test_runner::~test_runner() {
        destruct_free(impl_);
    }
        
    LIBUPTESTAPI
    void test_runner::add_listener(test_listener* listener) {
        assert(listener);
        impl_->listeners.push_back(listener);
    }
        
    LIBUPTESTAPI
    void test_runner::remove_listener(test_listener* listener) {
        assert(listener);
        for (listener_container::iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            if (listener == *itr) {
                impl_->listeners.erase(itr);
                break;
            }
        }
    }
            
    LIBUPTESTAPI
    void test_runner::started(test_suite const& suite, unsigned int total_tests, unsigned int total_tests_to_run) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->started(suite, total_tests, total_tests_to_run);
        }        
    }
        
    LIBUPTESTAPI
    void test_runner::stopped(test_suite const& suite, test_result const& results) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->stopped(suite, results);
        }        
    }
        
    LIBUPTESTAPI
    void test_runner::enter_test_suite(test_suite const& suite) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->enter_test_suite(suite);
        }        
    }
        
    LIBUPTESTAPI
    void test_runner::exit_test_suite(test_suite const& suite) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->exit_test_suite(suite);
        }        
    }
        
    LIBUPTESTAPI
    void test_runner::test_case_ignored(test_case const& t) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->test_case_ignored(t);
        }        
    }
        
    LIBUPTESTAPI
    void test_runner::test_case_failed(test_case const& t, test_error const& error) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->test_case_failed(t, error);
        }        
    }

    LIBUPTESTAPI
    void test_runner::test_case_passed(test_case const& t) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->test_case_passed(t);
        }        
    }

    LIBUPTESTAPI
    void test_runner::benchmark_results(
        test_case const& t,
        test_benchmark_result const* first,
        test_benchmark_result const* last
    ) {
        for (listener_container::const_iterator itr = impl_->listeners.begin(), end = impl_->listeners.end(); itr != end; ++itr) {
            (*itr)->benchmark_results(t, first, last);
        }
    }
        
    LIBUPTESTAPI
    test_result test_runner::run_tests(test_filter& filter) {
        test_result results;
        identity_filter ident_filter;

        unsigned int total_tests = suite_.test_count(ident_filter);
        unsigned int total_tests_to_run = suite_.test_count(filter);
        started(suite_, total_tests, total_tests_to_run);
            
        UPTRY {
            results = suite_.run(*this, filter);
        }
        UPCATCHANY_RETHROW(
            stopped(suite_, results);
        );

        stopped(suite_, results);
        return results;
    }
}}
