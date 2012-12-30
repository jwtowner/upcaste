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

#include <up/test/test_error.hpp>
#include <up/cstring.hpp>
#include <up/cstdlib.hpp>
#include <up/utility.hpp>

namespace up { namespace test
{
    LIBUPTESTAPI
    test_error::test_error() noexcept
    : file_name_(nullptr),
    line_number_(0),
    condition_(nullptr),
    message_(nullptr) {
    }

    LIBUPTESTAPI
    test_error::test_error(char const* file_name, long line_number) noexcept
    : file_name_(file_name),
    line_number_(line_number),
    condition_(nullptr),
    message_(nullptr) {
    }

    LIBUPTESTAPI
    test_error::test_error(char const* file_name, long line_number, char const* condition) noexcept
    : file_name_(file_name),
    line_number_(line_number),
    condition_(condition ? strdup(condition) : nullptr),
    message_(nullptr) {
    }

    LIBUPTESTAPI
    test_error::test_error(char const* file_name, long line_number, char const* condition, char const* message) noexcept
    : file_name_(file_name),
    line_number_(line_number),
    condition_(condition ? strdup(condition) : nullptr),
    message_(message ? strdup(message) : nullptr) {
    }

    LIBUPTESTAPI
    test_error::test_error(test_error const& x) noexcept
    :
#ifndef UP_NO_EXCEPTIONS
    exception(x),
#endif
    file_name_(x.file_name_),
    line_number_(x.line_number_),
    condition_(x.condition_ ? strdup(x.condition_) : nullptr),
    message_(x.message_ ? strdup(x.message_) : nullptr) {
    }

    LIBUPTESTAPI
    test_error& test_error::operator=(test_error const& x) noexcept {
        if (this != &x) {
            test_error temp(x);
            swap(file_name_, temp.file_name_);
            swap(line_number_, temp.line_number_);
            swap(condition_, temp.condition_);
            swap(message_, temp.message_);
        }

        return *this;
    }

    LIBUPTESTAPI
    test_error::~test_error() noexcept {
        free(condition_);
        free(message_);
    }

    LIBUPTESTAPI
    char const* test_error::what() const noexcept {
        return "test_error";
    }
}}
