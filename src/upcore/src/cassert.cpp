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

#include <up/cassert.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdio.hpp>
#include <up/cthreads.hpp>

namespace up
{
    namespace
    {
        thread_local assert_handler tls_handler = nullptr;

        bool do_report_assertion(char const* filename, long line, char const* condition, char const* message) {
            if (tls_handler) {
                return (*tls_handler)(filename, line, condition, message);
            }

            return default_assert_handler(filename, line, condition, message);
        }
    }

    LIBUPCOREAPI UPNONNULLALL
    bool UPCDECL default_assert_handler(char const* filename, long line, char const* condition, char const* message) {
        fast_fprintf(stderr, "ASSERTION FAILURE [%s(%ld)]:\n\n", filename, line);

        if (condition != 0) {
            fast_fprintf(stderr, "Condition: '%s'\n", condition);
        }
        
        if (message != 0) {
            fast_fprintf(stderr, "Message: %s\n", message);
        }
        
        fputs("\n", stderr);
        return true;
    }

    LIBUPCOREAPI UPWARNRESULT
    assert_handler get_assert_handler() {
        return tls_handler;
    }

    LIBUPCOREAPI
    assert_handler set_assert_handler(assert_handler handler) {
        assert_handler result = tls_handler;
        tls_handler = handler;
        return result;
    }

    LIBUPCOREAPI UPNONNULLALL
    bool report_assertion(char const* filename, long line, char const* condition) {
        return do_report_assertion(filename, line, condition, nullptr);
    }
    
    LIBUPCOREAPI UPNONNULLALL
    bool report_assertion(char const* filename, long line, char const* condition, char const* message, ...) {
        char formatted[1024];
        va_list args;

        if (!message) {
            return do_report_assertion(filename, line, condition, nullptr);
        }
        
        va_start(args, message);
        fast_vsnprintf(formatted, sizeof(formatted), message, args);
        va_end(args);
    
        return do_report_assertion(filename, line, condition, formatted);
    }
}