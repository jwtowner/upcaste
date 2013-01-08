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

#ifndef UP_TEST_TEST_CASE_HPP
#define UP_TEST_TEST_CASE_HPP

#include <up/test/test_object.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_case : public test_object
    {        
    public:

        virtual ~test_case();
        virtual unsigned int test_count(test_filter& filter) const;
        virtual test_result const& run(test_listener& listener, test_filter& filter);
        char const* file_name() const noexcept;
        long line_number() const noexcept;
        bool expects_assertion() const noexcept;
        bool expects_error() const noexcept;
        unsigned int iterations() const noexcept;

    protected:

        test_case(char const* name, char const* file, long line);
        void add_category(char const* category);
        void exceptions_enabled(bool enabled) noexcept;
        void expects_assertion(bool value) noexcept;
        void expects_error(bool value) noexcept;
        void iterations(unsigned int count) noexcept;
        virtual void do_run() = 0;

    private:

        char const* file_name_;
        long line_number_;
        bool exceptions_enabled_;
        bool expects_assertion_;
        bool expects_error_;
        unsigned int iterations_;
        test_case_impl* impl_;
    };

    inline UPHIDDENINLINE
    char const* test_case::file_name() const noexcept {
        return file_name_;
    }
                
    inline UPHIDDENINLINE
    long test_case::line_number() const noexcept {
        return line_number_;
    }

    inline UPHIDDENINLINE
    void test_case::exceptions_enabled(bool enabled) noexcept {
        exceptions_enabled_ = enabled;
    }

    inline UPHIDDENINLINE
    bool test_case::expects_assertion() const noexcept {
        return expects_assertion_;
    }

    inline UPHIDDENINLINE
    void test_case::expects_assertion(bool value) noexcept {
        expects_assertion_ = value;
    }

    inline UPHIDDENINLINE
    bool test_case::expects_error() const noexcept {
        return expects_error_;
    }

    inline UPHIDDENINLINE
    void test_case::expects_error(bool value) noexcept {
        expects_error_ = value;
    }
        
    inline UPHIDDENINLINE
    unsigned int test_case::iterations() const noexcept {
        return iterations_;
    }
        
    inline UPHIDDENINLINE
    void test_case::iterations(unsigned int count) noexcept {
        iterations_ = (count > 1) ? count : 1;
    }
}}

#endif
