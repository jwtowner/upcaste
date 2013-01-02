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

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int NAME(token const* UPRESTRICT tok, ITYPE* UPRESTRICT result) noexcept {
        RTYPE real_value;
        int retval;

        if (!tok || !result) {
            if (result) {
                *result = 0;
            }
            return sexp_badarg;
        }

        switch (tok->type) {
        case token_boolean:
            *result = (ITYPE)tok->bool_value;
            retval = sexp_success;
            break;

        case token_character:
            if (tok->char_value > MAX_VALUE) {
                *result = MAX_VALUE;
                retval = sexp_overflow;
            }
            else if (tok->char_value < MIN_VALUE) {
                *result = MIN_VALUE;
                retval = sexp_overflow;
            }
            else {
                *result = (ITYPE)tok->char_value;
                retval = sexp_success;
            }
            break;

        case token_integer:
            retval = PARSE_INTEGER(tok, result);
            break;

        case token_real:
            real_value = 0;
            retval = PARSE_REAL(tok, &real_value);
            if (real_value > MAX_VALUE) {
                *result = MAX_VALUE;
                retval = sexp_overflow;
            }
            else if (real_value < MIN_VALUE) {
                *result = MIN_VALUE;
                retval = sexp_overflow;
            }
            else {
                *result = (ITYPE)real_value;
            }
            break;
                
        // TODO: rational & complex

        default:
            *result = 0;
            retval = sexp_badarg;
            break;
        }

        return retval;
    }
}}
