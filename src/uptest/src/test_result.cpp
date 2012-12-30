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

#include <up/test/test_result.hpp>

namespace up { namespace test
{
    LIBUPTESTAPI
    test_result::test_result() noexcept
    : test_count_(0),
    tests_ignored_(0),
    tests_failed_(0),
    tests_passed_(0) {
        elapsed_.tv_sec = elapsed_.tv_nsec = 0;
    }
     
    LIBUPTESTAPI
    void test_result::accumulate(test_result const& result) noexcept {
        timespec_add(&result.elapsed_, &elapsed_, &elapsed_);
        test_count_ += result.test_count_;
        tests_ignored_ += result.tests_ignored_;
        tests_failed_ += result.tests_failed_;
        tests_passed_ += result.tests_passed_;
    }
        
    LIBUPTESTAPI
    void test_result::reset() noexcept {
        elapsed_.tv_sec = elapsed_.tv_nsec = 0;
        test_count_ = 0;
        tests_ignored_ = 0;
        tests_failed_ = 0;
        tests_passed_ = 0;
    }
                
    LIBUPTESTAPI
    void test_result::ignore() noexcept {
        elapsed_.tv_sec = elapsed_.tv_nsec = 0;
        test_count_ = tests_ignored_ = 1;
        tests_passed_ = tests_failed_ = 0;
    }

    LIBUPTESTAPI
    void test_result::fail(timespec const* elapsed) noexcept {
        assert(elapsed);
        elapsed_ = *elapsed;
        test_count_ = tests_failed_ = 1;
        tests_passed_ = tests_ignored_ = 0;
    }

    LIBUPTESTAPI
    void test_result::pass(timespec const* elapsed) noexcept {
        assert(elapsed);
        elapsed_ = *elapsed;
        test_count_ = tests_passed_ = 1;
        tests_failed_ = tests_ignored_ = 0;
    }
}}
