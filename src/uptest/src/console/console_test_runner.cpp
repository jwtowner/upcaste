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

#include <up/test/console/console_test_runner.hpp>
#include <up/test/default_filter.hpp>
#include <up/test/fixture_name_filter.hpp>
#include <up/cstring.hpp>
#include <up/cwchar.hpp>

namespace up { namespace test
{
    LIBUPTESTAPI
    console_test_runner::console_test_runner(int argc, tchar_t** argv)
    : argc_(argc),
    argv_(argv) {
        add_listener(&listener_);
    }

    LIBUPTESTAPI
    console_test_runner::console_test_runner(int argc, tchar_t** argv, test_suite& suite)
    : test_runner(suite),
    argc_(argc),
    argv_(argv) {
        add_listener(&listener_);
    }

    LIBUPTESTAPI
    int console_test_runner::run() {
        if (argc_ > 1) {
#if UP_TCHAR == UP_TCHAR_WCHAR
            wchar_t const* src = argv_[1];
            char name[1024];
            mbstate_t state;
            size_t n;

            memset(&state, 0, sizeof(state));
            n = wcsrtombs(name, &src, sizeof(name) - 1, &state);
            name[(n < sizeof(name)) ? n : 0] = '\0';

            fixture_name_filter filter(name);
#else
            fixture_name_filter filter(argv_[1]);
#endif
            return run_tests(filter).failed() ? 1 : 0;
        }
        else {
            default_filter filter;
            return run_tests(filter).failed() ? 1 : 0;
        }
    }
}}
