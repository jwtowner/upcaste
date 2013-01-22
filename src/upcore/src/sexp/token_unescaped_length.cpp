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

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int token_unescaped_length(token const* UPRESTRICT tok, size_t* UPRESTRICT result) noexcept {
        if (!tok || !result) {
            return sexp_badarg;
        }

        size_t unescaped_length;
        switch (tok->category) {
        case category_string:
            if (tok->type == token_escaped_string) {
                assert(tok->length >= (2 + tok->escape_difference));
                unescaped_length = tok->length - (2 + tok->escape_difference);
            }
            else if (tok->type == token_string) {
                assert(tok->length >= 2);
                unescaped_length = tok->length - 2;
            }
            else {
                assert(tok->type == token_raw_string);
                assert(tok->length >= (tok->raw_info.start_length + tok->raw_info.end_length));
                unescaped_length = tok->length - (tok->raw_info.start_length + tok->raw_info.end_length);
            }
            break;
        
        case category_symbol:
            if (tok->type == token_escaped_identifier) {
                assert(tok->length >= (2 + tok->escape_difference));
                unescaped_length = tok->length - (2 + tok->escape_difference);
                break;
            }
            assert(tok->type == token_identifier);
            // fallthrough

        default:
            unescaped_length = tok->length;
            break;
        }

        *result = unescaped_length;
        return sexp_success;
    }
}}
