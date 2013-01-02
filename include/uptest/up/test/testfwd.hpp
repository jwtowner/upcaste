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

#ifndef UP_TEST_TESTFWD_HPP
#define UP_TEST_TESTFWD_HPP

#include <up/cassert.hpp>
#include <up/cstddef.hpp>
#include <up/ctime.hpp>

#ifdef UP_LINKAGE_SHARED
#   ifdef LIBUPTEST_EXPORT_SYMBOLS
#       define LIBUPTESTAPI UPEXPORT
#       define LIBUPTESTEXTERN UPEXPORTEXTERN
#       define LIBUPTESTEXCEPTAPI UPEXPORTEXCEPT
#   else
#       define LIBUPTESTAPI UPIMPORT
#       define LIBUPTESTEXTERN UPIMPORTEXTERN
#       define LIBUPTESTEXCEPTAPI UPIMPORTEXCEPT
#   endif
#else
#   define LIBUPTESTAPI
#   define LIBUPTESTEXTERN extern
#   define LIBUPTESTEXCEPTAPI
#endif

#if defined(UP_NO_EXTERN_TEMPLATES) && !defined(LIBUPTEST_NO_EXTERN_TEMPLATES)
#   define LIBUPTEST_NO_EXTERN_TEMPLATES
#endif

namespace up { namespace test
{
    class default_filter;
    class fixture_name_filter;
    class identity_filter;
    class test_benchmark;
    class test_benchmark_result;
    class test_case;
    class test_error;
    class test_filter;
    class test_listener;
    class test_object;
    class test_result;
    class test_runner;
    class test_suite;
    struct test_benchmark_impl;
    struct test_case_impl;
    struct test_runner_impl;
    struct test_suite_impl;
}}

#endif
