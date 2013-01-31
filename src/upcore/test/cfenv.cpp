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

#include <up/prolog.hpp>

#ifndef UP_HAS_STDC_FENV

#include <up/atomic.hpp>
#include <up/bitwise.hpp>
#include <up/cfenv.hpp>
#include <up/cfloat.hpp>
#include <up/csignal.hpp>
#include <up/cstdlib.hpp>
#include <up/test.hpp>

#if UP_PLATFORM == UP_PLATFORM_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#   define COMPARE_EXCEPT_STATE(excepts, except_state) (((int)(excepts)) == ((int)(except_state)))
#else
#   define COMPARE_EXCEPT_STATE(excepts, except_state) (true)
#endif

namespace cfenv
{
    UP_STDC_FENV_ACCESS(ON)

    namespace
    {
        static volatile up::sig_atomic_t caught_count = 0;

        UPNOINLINE
        void test_exception(int test_excepts, int expected_excepts) noexcept {
            up::fexcept_t except_state;
            int cur_excepts = up::fetestexcept(test_excepts);
            require(0 == up::fegetexceptflag(&except_state, FE_ALL_EXCEPT));
            require(cur_excepts == expected_excepts && COMPARE_EXCEPT_STATE(cur_excepts, except_state));
        }

        UPNOINLINE
        void raise_and_test_exception(int raise_excepts, int expected_caught_count = 0) noexcept {
            // raise exception, and check caught_count
            caught_count = 0;
            require(0 == up::feraiseexcept(raise_excepts));
            up::atomic_thread_fence(up::memory_order_seq_cst);
            require(caught_count == expected_caught_count);

            // test and get exception
            test_exception(FE_ALL_EXCEPT, raise_excepts);

            // clear the exception and retest
            require(0 == up::feclearexcept(raise_excepts));
            require(0 == up::fetestexcept(FE_ALL_EXCEPT));
        }

        UPNOINLINE
        void raise_and_test_masked() noexcept {
            // make sure we're in default state
            require(0 == up::fegetexcept());
            test_exception(FE_ALL_EXCEPT, 0);

            // raise and test each exception type in sequence
            raise_and_test_exception(FE_DIVBYZERO);
            raise_and_test_exception(FE_INEXACT);
            raise_and_test_exception(FE_INVALID);
            raise_and_test_exception(FE_OVERFLOW);
            raise_and_test_exception(FE_UNDERFLOW);

            // test raising combinations
            raise_and_test_exception(FE_INEXACT | FE_OVERFLOW);
            raise_and_test_exception(FE_DIVBYZERO | FE_UNDERFLOW);
            raise_and_test_exception(FE_INVALID | FE_INEXACT | FE_DIVBYZERO);
            raise_and_test_exception(FE_ALL_EXCEPT);

            // make sure we're in default state
            require(0 == up::fegetexcept());
            test_exception(FE_ALL_EXCEPT, 0);
        }

        UPNOINLINE
        void raise_and_test_unmasked(int expected_excepts) noexcept {
            // make sure we're in default clear state
            require(expected_excepts == up::fegetexcept());
            test_exception(FE_ALL_EXCEPT, 0);

            // raise and test each exception type in sequence
            raise_and_test_exception(FE_DIVBYZERO, 1);
            raise_and_test_exception(FE_INEXACT, 1);
            raise_and_test_exception(FE_INVALID, 1);
            raise_and_test_exception(FE_OVERFLOW, 1);
            raise_and_test_exception(FE_UNDERFLOW, 1);
            raise_and_test_exception(FE_DENORMAL, 1);

            // test raising combinations
            raise_and_test_exception(FE_INEXACT | FE_OVERFLOW, 2);
            raise_and_test_exception(FE_DIVBYZERO | FE_UNDERFLOW, 2);
            raise_and_test_exception(FE_INVALID | FE_INEXACT | FE_DIVBYZERO, 3);
            raise_and_test_exception(FE_ALL_EXCEPT, up::ceillog2((unsigned int)FE_ALL_EXCEPT));

            // make sure we're in default clear state
            test_exception(FE_ALL_EXCEPT, 0);
            require(expected_excepts == up::fegetexcept());
        }
    }

#if (UP_PLATFORM == UP_PLATFORM_WINDOWS)

    namespace
    {

        UPNOINLINE
        void UPCDECL fpe_signal_handler(int) noexcept {
            ++caught_count;
            up::atomic_signal_fence(up::memory_order_seq_cst);
        }

        UPNOINLINE
        LONG CALLBACK fpe_vectored_handler(PEXCEPTION_POINTERS ExceptionInfo) noexcept {
            LONG retval;
            switch (ExceptionInfo->ExceptionRecord->ExceptionCode) {
            case EXCEPTION_FLT_DENORMAL_OPERAND:
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            case EXCEPTION_FLT_INEXACT_RESULT:
            case EXCEPTION_FLT_INVALID_OPERATION:
            case EXCEPTION_FLT_OVERFLOW:
            case EXCEPTION_FLT_UNDERFLOW:
            case STATUS_FLOAT_MULTIPLE_TRAPS:
                // inform the standard C signal handler mechanism
                up::raise(SIGFPE);

                // we need to reinstall the signal handler, with the MSVC CRT,
                // as with other platforms, the semantics is to overwrite the
                // handler with the default, causing the next raised signal
                // to abort the program
                up::signal(SIGFPE, &fpe_signal_handler);

                // our implementation of feraiseexcept stores the address of the
                // next instruction in EAX/RAX, allowing us to skip over the 
                // fwait or divss instruction that triggered the exception
#if UP_ARCHITECTURE == UP_ARCHITECTURE_X86
                ExceptionInfo->ContextRecord->Eip = ExceptionInfo->ContextRecord->Eax;
#elif UP_ARCHITECTURE == UP_ARCHITECTURE_X64
                ExceptionInfo->ContextRecord->Rip = ExceptionInfo->ContextRecord->Rax;
#else
#   error "Target CPU architecture not yet supported!"
#endif
                retval = EXCEPTION_CONTINUE_EXECUTION;
                break;
            default:
                retval = EXCEPTION_CONTINUE_SEARCH;
                break;
            }
            return retval;
        }
    }

    UP_TEST_CASE(exceptions_masked) {
        void (UPCDECL * old_handler)(int) = up::signal(SIGFPE, &fpe_signal_handler);
        ::AddVectoredExceptionHandler(ULONG_MAX, &fpe_vectored_handler);
        require(0 == up::fesetenv(FE_DFL_ENV));
        raise_and_test_masked();
        ::RemoveVectoredExceptionHandler(&fpe_vectored_handler);
        up::signal(SIGFPE, old_handler);
    }

    UP_TEST_CASE(exceptions_unmasked) {
        void (UPCDECL * old_handler)(int) = up::signal(SIGFPE, &fpe_signal_handler);
        ::AddVectoredExceptionHandler(ULONG_MAX, &fpe_vectored_handler);

        // enable exceptions
        require(0 == up::fesetenv(FE_DFL_ENV));
        require(0 == up::fegetexcept());
        require(0 == up::feenableexcept(FE_ALL_EXCEPT));

        raise_and_test_unmasked(FE_ALL_EXCEPT);

        // disable exceptions
        require(FE_ALL_EXCEPT == up::fedisableexcept(FE_ALL_EXCEPT));
        require(0 == up::fegetexcept());
        
        ::RemoveVectoredExceptionHandler(&fpe_vectored_handler);
        up::signal(SIGFPE, old_handler);
    }

    UP_TEST_CASE(exceptions_dfl_env) {
        void (UPCDECL * old_handler)(int) = up::signal(SIGFPE, &fpe_signal_handler);
        ::AddVectoredExceptionHandler(ULONG_MAX, &fpe_vectored_handler);
        up::fenv_t old_env;

        // install default environment
        require(0 == up::fegetexcept());
        require(0 == up::fegetenv(&old_env));
        require(0 == up::feenableexcept(FE_ALL_EXCEPT));
        require(FE_ALL_EXCEPT == up::fegetexcept());
        require(0 == up::fesetenv(FE_DFL_ENV));
        require(0 == up::fegetexcept());
        
        raise_and_test_masked();

        // install original environment
        require(0 == up::fesetenv(&old_env));
        require(0 == up::fegetexcept());

        ::RemoveVectoredExceptionHandler(&fpe_vectored_handler);
        up::signal(SIGFPE, old_handler);
    }

    UP_TEST_CASE(exceptions_nomask_env) {
        void (UPCDECL * old_handler)(int) = up::signal(SIGFPE, &fpe_signal_handler);
        ::AddVectoredExceptionHandler(ULONG_MAX, &fpe_vectored_handler);
        up::fenv_t old_env, new_env;

        // install momask environment
        require(0 == up::fegetexcept());
        require(0 == up::fegetenv(&old_env));
        require(0 == up::fesetenv(FE_NOMASK_ENV));
        require(0 == up::fegetenv(&new_env));
        require(0 != up::memcmp(&old_env, &new_env, sizeof(up::fenv_t)));

        raise_and_test_unmasked(FE_ALL_EXCEPT);

        // install original environment
        require(0 == up::fesetenv(&old_env));
        require(0 == up::fegetexcept());

        ::RemoveVectoredExceptionHandler(&fpe_vectored_handler);
        up::signal(SIGFPE, old_handler);
    }

#endif

    UP_STDC_FENV_ACCESS(OFF)
}

#endif
