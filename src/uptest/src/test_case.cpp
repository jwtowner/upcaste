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

#include <up/test/test_case.hpp>
#include <up/test/test_error.hpp>
#include <up/test/test_filter.hpp>
#include <up/test/test_listener.hpp>
#include <up/csetjmp.hpp>
#include <up/cstring.hpp>
#include <up/cstdio.hpp>
#include <up/memory.hpp>
#include <algorithm>
#include <vector>

#if (UP_PLATFORM == UP_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif

#if (UP_COMPILER == UP_COMPILER_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4611) // interaction between setjmp and C++ object destruction is non-portable
#endif

namespace up { namespace test
{
    namespace
    {
#ifndef UP_NO_EXCEPTIONS
        thread_local bool current_exceptions_enabled;
#endif
        thread_local jmp_buf current_jump_buffer;
        thread_local test_error* current_failure;
        
        bool UPCDECL redirect_assertion_handler(char const* file, long line, char const* condition) {
#if (UP_PLATFORM == UP_PLATFORM_WINDOWS)
            if (::IsDebuggerPresent()) {
                ::DebugBreak();
            }
#endif

#ifndef UP_NO_EXCEPTIONS
            if (current_exceptions_enabled) {
                throw test_error(file, line, condition);
            }
#endif

            current_failure = malloc_construct<test_error>(file, line, condition);
            longjmp(current_jump_buffer, 1);
        }

        typedef std::vector<char const*> category_container;
    }

    struct UPHIDDEN test_case_impl
    {
        category_container categories;
    };

    LIBUPTESTAPI
    test_case::test_case(char const* name, char const* file, long line)
    : test_object(name),
    file_name_(file),
    line_number_(line),
    expects_assertion_(false),
    expects_error_(false),
    iterations_(1),
    impl_(malloc_construct<test_case_impl>()) {
    }

    LIBUPTESTAPI
    test_case::~test_case() {
        destruct_free(impl_);
    }

    LIBUPTESTAPI
    unsigned int test_case::test_count(test_filter& filter) const {
        return filter.matches(*this) ? 1 : 0;
    }

    LIBUPTESTAPI
    test_result const& test_case::run(test_listener& listener, test_filter& filter) {
        timespec start, stop, elapsed;
        assert_handler old_handler;
        unsigned int i;
        bool is_error;

        if (!filter.matches(*this)) {
            listener.test_case_ignored(*this);
            result_.ignore();
            return result_;
        }
            
        old_handler = set_assert_handler(&redirect_assertion_handler);
            
#ifndef UP_NO_EXCEPTIONS
        current_exceptions_enabled = exceptions_enabled_;
        if (!exceptions_enabled_) {
            goto use_setjmp_longjmp;
        }

        try {
            verify(!clock_gettime(CLOCK_MONOTONIC, &start));
                
            for (i = 0; i < iterations_; ++i) {
                do_run();
            }

            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));
        }
        catch (test_error const& error) {
            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));

            is_error = strcmp(file_name(), error.file_name()) != 0;
            if ((!expects_error_ && is_error) || (!expects_assertion_ && !is_error)) {
                set_assert_handler(old_handler);
                result_.fail(&elapsed);
                listener.test_case_failed(*this, error);
                return result_;
            }
        }
        catch (std::exception const& error) {
            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));

            if (!expects_error_) {
                set_assert_handler(old_handler);
                result_.fail(&elapsed);
                listener.test_case_failed(*this, test_error(file_name(), line_number(), "Unexpected Exception", error.what()));
                return result_;
            }
        }
        catch (...) {
            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));

            if (!expects_error_) {
                set_assert_handler(old_handler);
                result_.fail(&elapsed);
                listener.test_case_failed(*this, test_error(file_name(), line_number(), "Unexpected Exception", "Unknown"));
                return result_;
            }
        }

        goto done;

    use_setjmp_longjmp:

#endif

        if (setjmp(current_jump_buffer) == 0) {
            verify(!clock_gettime(CLOCK_MONOTONIC, &start));
                
            for (i = 0; i < iterations_; ++i) {                
                do_run();
            }

            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));
        }
        else {
            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));

            test_error error(*current_failure);
            destruct_free(current_failure);
            current_failure = nullptr;

            is_error = strcmp(file_name(), error.file_name()) != 0;
            if ((!expects_error_ && is_error) || (!expects_assertion_ && !is_error)) {
                set_assert_handler(old_handler);
                result_.fail(&elapsed);
                listener.test_case_failed(*this, error);
                return result_;
            }
        }

#ifndef UP_NO_EXCEPTIONS
    done:
#endif

        set_assert_handler(old_handler);
                
        if (expects_error_) {
            result_.fail(&elapsed);
            listener.test_case_failed(*this, test_error(file_name(), line_number(), "Invalid Error", "Expected an exception or assertion error originating from a different source file"));
            return result_;
        }

        if (expects_assertion_) {
            result_.fail(&elapsed);
            listener.test_case_failed(*this, test_error(file_name(), line_number(), "Invalid Assertion", "Expected an assertion originating from the test case"));
            return result_;
        }

        result_.pass(&elapsed);
        listener.test_case_passed(*this);
        return result_;
    }

    LIBUPTESTAPI
    void test_case::add_category(char const* category) {
        assert(category);

        category_container::const_iterator itr, end;
        for ( itr  = impl_->categories.begin(), end = impl_->categories.end();
            itr != end;
            ++itr
        ) {
            if ((*itr == category) || !strcmp(*itr, category)) {
                return;
            }
        }

        if (itr != impl_->categories.end()) {
            impl_->categories.push_back(category);
        }
    }
}}

#if (UP_COMPILER == UP_COMPILER_MSVC)
#   pragma warning(pop)
#endif
