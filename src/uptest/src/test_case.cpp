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
#include <up/cfenv.hpp>
#include <up/csetjmp.hpp>
#include <up/cstring.hpp>
#include <up/cstdio.hpp>
#include <up/memory.hpp>
#include <up/log.hpp>
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
        thread_local jmp_buf current_jump_buffer;
        thread_local test_error* current_failure;
        thread_local bool current_exceptions_enabled;

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


#if UP_COMPILER == UP_COMPILER_MSVC
        void UPCDECL redirect_purecall_handler() {
            if (::IsDebuggerPresent()) {
                ::DebugBreak();
            }
            log_event(log_level_critical, "Pure virtual call detected, aborting...");
            abort();
        }
#endif
    }

    typedef std::vector<char const*> category_container;

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

    LIBUPTESTAPI
    unsigned int test_case::test_count(test_filter& filter) const {
        return filter.matches(*this) ? 1 : 0;
    }

    LIBUPTESTAPI
    test_result const& test_case::run(test_listener& listener, test_filter& filter) {
#if UP_COMPILER == UP_COMPILER_MSVC
        _purecall_handler old_purecall_handler;
#endif
        assert_handler old_assert_handler;
        fenv_t old_env;

        if (!filter.matches(*this)) {
            listener.test_case_ignored(*this);
            result_.ignore();
            return result_;
        }
        
#if UP_COMPILER == UP_COMPILER_MSVC
        old_purecall_handler = _set_purecall_handler(&redirect_purecall_handler);
#endif
        current_exceptions_enabled = exceptions_enabled_;
        old_assert_handler = set_assert_handler(&redirect_assertion_handler);
        fegetenv(&old_env);

#ifndef UP_NO_EXCEPTIONS
        test_result const& result = exceptions_enabled_ ? run_except(listener) : run_setjmp(listener);
#else
        test_result const& result = run_setjmp(listener);
#endif

        fesetenv(&old_env);
        set_assert_handler(old_assert_handler);
#if UP_COMPILER == UP_COMPILER_MSVC
        _set_purecall_handler(old_purecall_handler);
#endif

        return result;
    }

    test_result const& test_case::run_except(test_listener& listener) {
        timespec start, stop, elapsed;
        unsigned int i;
        bool is_error;

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
                result_.fail(&elapsed);
                listener.test_case_failed(*this, error);
                return result_;
            }
        }
        catch (std::exception const& error) {
            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));

            if (!expects_error_) {
                result_.fail(&elapsed);
                listener.test_case_failed(*this, test_error(file_name(), line_number(), "Unexpected Exception", error.what()));
                return result_;
            }
        }
        catch (...) {
            verify(!clock_gettime(CLOCK_MONOTONIC, &stop));
            verify(!timespec_subtract(&start, &stop, &elapsed));

            if (!expects_error_) {
                result_.fail(&elapsed);
                listener.test_case_failed(*this, test_error(file_name(), line_number(), "Unexpected Exception", "Unknown"));
                return result_;
            }
        }

        return run_finalize(listener, &elapsed);
    }

    test_result const& test_case::run_setjmp(test_listener& listener) {
        timespec start, stop, elapsed;
        unsigned int i;
        bool is_error;

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
                result_.fail(&elapsed);
                listener.test_case_failed(*this, error);
                return result_;
            }
        }

        return run_finalize(listener, &elapsed);
    }

    test_result const& test_case::run_finalize(test_listener& listener, timespec const* elapsed) {
        if (expects_error_) {
            result_.fail(elapsed);
            listener.test_case_failed(
                *this,
                test_error(
                    file_name(),
                    line_number(),
                    "Invalid Error",
                    "Expected an exception or assertion error originating from a different source file"
                )
            );
            return result_;
        }

        if (expects_assertion_) {
            result_.fail(elapsed);
            listener.test_case_failed(
                *this,
                test_error(
                    file_name(),
                    line_number(),
                    "Invalid Assertion",
                    "Expected an assertion originating from the test case"
                )
            );
            return result_;
        }

        result_.pass(elapsed);
        listener.test_case_passed(*this);
        return result_;
    }
}}

#if (UP_COMPILER == UP_COMPILER_MSVC)
#   pragma warning(pop)
#endif
