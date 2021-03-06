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

#include <up/sexp.hpp>
#include <up/cassert.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdio.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int parser_error(parser* UPRESTRICT par, char const* UPRESTRICT format, ...) noexcept {
        char const* filename;
        char message[4096];
        va_list args;

        if (!par || !format) {
            return sexp_badarg;
        }
        
        ++par->error_count;

        if (par->error_handler) {
            va_start(args, format);
            verify(fast_vsnprintf(message, sizeof(message), format, args) < static_cast<int>(sizeof(message)));
            va_end(args);

            filename = par->filename;
            if (!filename) {
                filename = "";
            }

            (*par->error_handler)(filename, par->prev_line, par->prev_column, message);
        }

        return sexp_success;
    }
}}
