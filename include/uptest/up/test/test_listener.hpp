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

#ifndef UP_TEST_TEST_LISTENER_HPP
#define UP_TEST_TEST_LISTENER_HPP

#include <up/test/testfwd.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_listener
    {
    protected:
            
        test_listener();
            
    public:
            
        virtual ~test_listener();
        virtual void started(test_suite const& suite, unsigned int total_tests, unsigned int total_tests_to_run);
        virtual void stopped(test_suite const& suite, test_result const& results);
        virtual void enter_test_suite(test_suite const& suite);
        virtual void exit_test_suite(test_suite const& suite);
        virtual void test_case_ignored(test_case const& t);
        virtual void test_case_failed(test_case const& t, test_error const& failure);
        virtual void test_case_passed(test_case const& t);
        virtual void benchmark_results(test_case const& t, test_benchmark_result const* first, test_benchmark_result const* last);
    };
}}

#endif
