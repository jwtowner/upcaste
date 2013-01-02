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

#ifndef UP_TEST_TEST_SUITE_HPP
#define UP_TEST_TEST_SUITE_HPP

#include <up/test/test_object.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_suite : public test_object
    {
    public:

        static test_suite& instance();
        explicit test_suite(char const* name);
        virtual ~test_suite();
        test_suite* find_or_create_fixture(char const* file_name);
        void add_test(test_object* test, bool should_delete = true);
        void remove_test(test_object* test);
        virtual unsigned int test_count(test_filter& filter) const;
        virtual test_result const& run(test_listener& listener, test_filter& filter);

    private:
            
        test_suite();
        test_suite_impl* impl_;
    };
}}

#endif
