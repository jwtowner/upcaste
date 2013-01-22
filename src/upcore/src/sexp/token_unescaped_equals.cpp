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
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int token_unescaped_equals(token const* tok, char const* str) noexcept {
        char* buffer;
        char const* text;
        size_t length;
        int retval;
        
        switch (tok->category) {
        case category_string:
            if (tok->type == token_escaped_string) {
                // escaped string: we need to fetch an unescaped version of the
                // token into a temp buffer that we can then compare
                retval = token_unescaped_length(tok, &length);
                if (retval != sexp_success) {
                    return retval;
                }

                buffer = static_cast<char*>(malloca(length + 1));
                if (!buffer) {
                    return sexp_nomem;
                }

                retval = token_unescape(tok, buffer, length + 1);
                if (retval != sexp_success) {
                    freea(buffer);
                    return retval;
                }

                retval = strncmp(str, buffer, length + 1);
                freea(buffer);
                if (retval != 0) {
                    return sexp_nomatch;
                }

                return sexp_success;
            }
            else if (tok->type == token_string) {
                assert(tok->length >= 2);
                text = tok->text + 1;
                length = tok->length - 2;
            }
            else {
                assert(tok->type == token_raw_string);
                assert(tok->length >= (tok->raw_info.start_length + tok->raw_info.end_length));
                text = tok->text + tok->raw_info.start_length;
                length = tok->length - (tok->raw_info.start_length + tok->raw_info.end_length);
            }
            break;

        case category_symbol:
            if (tok->type == token_escaped_identifier) {
                return sexp_badarg; // TODO
            }
            assert(tok->type == token_identifier);
            // fallthrough

        default:
            text = tok->text;
            length = tok->length;
            break;
        }

        if ((strncmp(str, text, length) != 0) || (str[length] != '\0')) {
            return sexp_nomatch;
        }

        return sexp_success;
    }
}}
