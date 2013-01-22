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
#include <up/climits.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int parser_expect_symbol(parser* UPRESTRICT par,  char const* UPRESTRICT str, token* UPRESTRICT tok) noexcept {
        int retval = parser_read(par, tok);
        switch (retval) {
        case sexp_success:
            if (tok->category == category_symbol) {
                retval = token_unescaped_equals(tok, str);
                if (retval == sexp_success) {
                    return retval;
                }
            }
            retval = sexp_nomatch;
            // fallthrough

        case sexp_badsyntax:
            parser_error(
                par,
                parser_message(par, message_expected_symbol),
                str,
                static_cast<int>((tok->length <= INT_MAX) ? tok->length : INT_MAX),
                tok->text
            );
            break;

        case sexp_eof:
            parser_error(
                par,
                parser_message(par, message_expected_symbol_eof),
                str
            );
            break;

        default:
            break;
        }

        return retval;
    }
}}
