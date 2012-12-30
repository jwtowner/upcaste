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

#include <up/sexp.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int token_to_bool(token const* UPRESTRICT tok, bool* UPRESTRICT result) noexcept {
        int_least32_t int_value;
        float real_value;
        int retval;

        if (!tok || !result) {
            if (result) {
                *result = false;
            }
            return sexp_badarg;
        }

        switch (tok->type) {
        case token_boolean:
            *result = tok->bool_value;
            retval = sexp_success;
            break;

        case token_character:
            *result = (tok->char_value != 0) ? true : false;
            retval = sexp_success;
            break;

        case token_integer:
            int_value = 0;
            retval = token_parse_int32(tok, &int_value);
            if (retval >= 0) {
                *result = (int_value != 0) ? true : false;
                retval = sexp_success;
            }
            break;

        case token_real:
            real_value = 0;
            retval = token_parse_float(tok, &real_value);
            if (retval >= 0) {
                *result = (real_value != 0.0f) ? true : false;
                retval = sexp_success;
            }
            break;
                
        // TODO: rational & complex

        default:
            *result = false;
            retval = sexp_badarg;
            break;
        }

        return retval;
    }
}}
