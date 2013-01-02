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

#ifndef UP_TEST_HPP
#define UP_TEST_HPP

#include <up/test/test_benchmark.hpp>
#include <up/test/test_case.hpp>
#include <up/test/test_suite.hpp>

#define UP_DETAIL_TEST_CASE_NAME(name) UPCONCATENATE(name, UPCONCATENATE(_test_case_, __LINE__))
#define UP_DETAIL_TEST_CASE_INSTANCE_NAME(name) UPCONCATENATE(UP_DETAIL_TEST_CASE_NAME(name), _instance)

#define UP_DETAIL_TEST_CASE(base, name, ...) \
    class UP_DETAIL_TEST_CASE_NAME(name) : public base \
    { \
    public: \
        UP_DETAIL_TEST_CASE_NAME(name)() : base(UPSTRINGIZE(name), __FILE__, __LINE__) { \
            ::up::test::test_suite* fixture = ::up::test::test_suite::instance().find_or_create_fixture(file_name()); \
            fixture->add_test(this, false); \
            { __VA_ARGS__; } \
        } \
        virtual void do_run(); \
    } \
    UP_DETAIL_TEST_CASE_INSTANCE_NAME(name); \
    \
    void UP_DETAIL_TEST_CASE_NAME(name)::do_run()

#define UP_TEST_CASE(name, ...) UP_DETAIL_TEST_CASE(::up::test::test_case, name, __VA_ARGS__)
#define UP_TEST_BENCHMARK(name, ...) UP_DETAIL_TEST_CASE(::up::test::test_benchmark, name, __VA_ARGS__)
#define UP_TEST_RUN_BENCHMARK(category, complexity, iterations, ...) ::up::test::test_benchmark::run_benchmark(category, complexity, iterations, __VA_ARGS__)
#define UP_TEST_CATEGORY(category) ::up::test::test_case::add_category(category)
#define UP_TEST_EXPECTS_ASSERTION() ::up::test::test_case::expects_assertion(true)
#define UP_TEST_EXPECTS_ERROR() ::up::test::test_case::expects_error(true)
#define UP_TEST_IGNORE() ::up::test::test_object::ignore(true)
#define UP_TEST_ITERATIONS(count) ::up::test::test_case::iterations(count)

#endif
