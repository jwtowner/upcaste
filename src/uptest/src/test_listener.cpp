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

#include <up/test/test_listener.hpp>

namespace up { namespace test
{
    LIBUPTESTAPI
    test_listener::test_listener() {
    }
                
    LIBUPTESTAPI
    test_listener::~test_listener() {
    }
                
    LIBUPTESTAPI
    void test_listener::started(test_suite const&, unsigned int, unsigned int) {
    }
                
    LIBUPTESTAPI
    void test_listener::stopped(test_suite const&, test_result const&) {
    }
                
    LIBUPTESTAPI
    void test_listener::enter_test_suite(test_suite const&) {
    }
                
    LIBUPTESTAPI
    void test_listener::exit_test_suite(test_suite const&) {
    }
                
    LIBUPTESTAPI
    void test_listener::test_case_ignored(test_case const&) {
    }
                
    LIBUPTESTAPI
    void test_listener::test_case_failed(test_case const&, test_error const&) {
    }

    LIBUPTESTAPI
    void test_listener::test_case_passed(test_case const&) {
    }
        
    LIBUPTESTAPI
    void test_listener::benchmark_results(test_case const&, test_benchmark_result const*, test_benchmark_result const*) {
    }
}}
