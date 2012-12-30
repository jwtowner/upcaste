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

#ifndef UP_TEST_CONSOLE_TEST_RUNNER_HPP
#define UP_TEST_CONSOLE_TEST_RUNNER_HPP

#include <up/test/console/console_test_listener.hpp>
#include <up/test/test_runner.hpp>
#include <up/tchar.hpp>

namespace up { namespace test
{
    class LIBUPTESTAPI console_test_runner : public test_runner
    {
    public:
            
        console_test_runner(int argc, tchar_t** argv);
        console_test_runner(int argc, tchar_t** argv, test_suite& suite);
        virtual int run();
            
    private:
            
        console_test_listener listener_;
        int argc_;
        tchar_t** argv_;
    };
}}

#endif
