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

#ifndef UP_TEST_TEST_FAILURE_EXCEPTION_HPP
#define UP_TEST_TEST_FAILURE_EXCEPTION_HPP

#include <up/test/testfwd.hpp>
#include <exception>

namespace up { namespace test
{
    class LIBUPTESTEXCEPTAPI test_error : public ::std::exception
    {
    public:

        LIBUPTESTAPI test_error() noexcept;
        LIBUPTESTAPI test_error(char const* file_name, long line_number) noexcept;
        LIBUPTESTAPI test_error(char const* file_name, long line_number, char const* condition) noexcept;
        LIBUPTESTAPI test_error(char const* file_name, long line_number, char const* condition, char const* message) noexcept;
        LIBUPTESTAPI test_error(test_error const& other) noexcept;
        LIBUPTESTAPI test_error& operator=(test_error const& other) noexcept;
        LIBUPTESTAPI virtual ~test_error() UPEXCEPTNOTHROW;
        LIBUPTESTAPI virtual char const* what() const UPEXCEPTNOTHROW;
        UPHIDDEN char const* file_name() const noexcept;        
        UPHIDDEN long line_number() const noexcept;
        UPHIDDEN char const* condition() const noexcept;
        UPHIDDEN char const* message() const noexcept;

    private:

        char const* file_name_;
        long line_number_;
        char* condition_;
        char* message_;
    };

    inline UPHIDDENINLINE
    char const* test_error::file_name() const noexcept {
        return file_name_;
    }

    inline UPHIDDENINLINE
    long test_error::line_number() const noexcept {
        return line_number_;
    }

    inline UPHIDDENINLINE
    char const* test_error::condition() const noexcept {
        return condition_;
    }

    inline UPHIDDENINLINE
    char const* test_error::message() const noexcept {
        return message_;
    }
}}

#endif
