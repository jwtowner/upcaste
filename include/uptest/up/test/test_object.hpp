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

#ifndef UP_TEST_TEST_OBJECT_HPP
#define UP_TEST_TEST_OBJECT_HPP

#include <up/test/test_result.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI test_object
    {
        friend class test_suite;

    public:
            
        virtual ~test_object();
        UPHIDDEN bool ignore() const noexcept;
        UPHIDDEN char const* name() const noexcept;
        UPHIDDEN test_suite* parent() const noexcept;
        UPHIDDEN test_result const& result() const noexcept;
        virtual unsigned int test_count(test_filter& filter) const = 0;
        virtual test_result const& run(test_listener& listener, test_filter& filter) = 0;
            
    protected:

        explicit test_object(char const* name) noexcept;
        UPHIDDEN void ignore(bool value) noexcept;
            
    private:
            
        char* name_;
        test_suite* parent_;
        bool ignore_;
            
    protected:

        test_result result_;
    };
        
    inline UPHIDDENINLINE
    bool test_object::ignore() const noexcept {
        return ignore_;
    }

    inline UPHIDDENINLINE
    void test_object::ignore(bool value) noexcept {
        ignore_ = value;
    }

    inline UPHIDDENINLINE
    char const* test_object::name() const noexcept {
        return name_;
    }
        
    inline UPHIDDENINLINE
    test_suite* test_object::parent() const noexcept {
        return parent_;
    }
        
    inline UPHIDDENINLINE
    test_result const& test_object::result() const noexcept {
        return result_;
    }
}}

#endif
