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

#ifndef UP_TEST_TEST_RESULT_HPP
#define UP_TEST_TEST_RESULT_HPP

#include <up/test/testfwd.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_result
    {
    public:
            
        test_result() noexcept;
        UPHIDDEN timespec const& elapsed() const noexcept;
        UPHIDDEN bool ignored() const noexcept;
        UPHIDDEN bool failed() const noexcept;
        UPHIDDEN bool passed() const noexcept;
        UPHIDDEN unsigned int test_count() const noexcept;
        UPHIDDEN unsigned int tests_run() const noexcept;
        UPHIDDEN unsigned int tests_ignored() const noexcept;
        UPHIDDEN unsigned int tests_failed() const noexcept;
        UPHIDDEN unsigned int tests_passed() const noexcept;
        void accumulate(test_result const& results) noexcept;
        void ignore() noexcept;
        void fail(timespec const* elapsed) noexcept;
        void pass(timespec const* elapsed) noexcept;
        void reset() noexcept;

    private:
            
        timespec elapsed_;
        unsigned int test_count_;
        unsigned int tests_ignored_;
        unsigned int tests_failed_;
        unsigned int tests_passed_;
    };

    inline UPHIDDENINLINE
    timespec const& test_result::elapsed() const noexcept {
        return elapsed_;
    }

    inline UPHIDDENINLINE
    bool test_result::ignored() const noexcept {
        return tests_ignored_ > 0;
    }

    inline UPHIDDENINLINE
    bool test_result::failed() const noexcept {
        return tests_failed_ > 0;
    }

    inline UPHIDDENINLINE
    bool test_result::passed() const noexcept {
        return test_count_ == tests_passed_;
    }
                
    inline UPHIDDENINLINE
    unsigned int test_result::test_count() const noexcept {
        return test_count_;
    }
        
    inline UPHIDDENINLINE
    unsigned int test_result::tests_run() const noexcept {
        return test_count_ - tests_ignored_;
    }
        
    inline UPHIDDENINLINE
    unsigned int test_result::tests_ignored() const noexcept {
        return tests_ignored_;
    }
        
    inline UPHIDDENINLINE
    unsigned int test_result::tests_failed() const noexcept {
        return tests_failed_;
    }

    inline UPHIDDENINLINE
    unsigned int test_result::tests_passed() const noexcept {
        return tests_passed_;
    }
}}

#endif
