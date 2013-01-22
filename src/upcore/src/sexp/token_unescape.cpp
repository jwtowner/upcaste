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
#include <up/cstring.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int token_unescape(token const* UPRESTRICT tok, char* UPRESTRICT dst, size_t dst_length) noexcept {
        if (!tok || (!dst && dst_length)) {
            return sexp_badarg;
        }

        size_t unescaped_start;
        size_t unescaped_length;

        switch (tok->category) {
        case category_string:
            if (tok->type == token_escaped_string) {
                return sexp_badarg; // TODO
            }
            else if (tok->type == token_string) {
                assert(tok->length >= 2);
                unescaped_start = 1;
                unescaped_length = tok->length - 2;
            }
            else {
                assert(tok->type == token_raw_string);
                assert(tok->length >= (tok->raw_info.start_length + tok->raw_info.end_length));
                unescaped_start = tok->raw_info.start_length;
                unescaped_length = tok->length - (tok->raw_info.start_length + tok->raw_info.end_length);
            }
            break;
        
        case category_symbol:
            if (tok->type == token_escaped_identifier) {
                return sexp_badarg; // TODO
            }
            assert(tok->type == token_identifier);
            // fallthrough

        default:
            unescaped_start = 0;
            unescaped_length = tok->length;
            break;
        }

        // simply copy up to dst_length characters to destination buffer
        if (unescaped_length > dst_length) {
            unescaped_length = dst_length;
        }

        memcpy(dst, tok->text + unescaped_start, unescaped_length);
                
        // only null terminate the destination buffer if it is larger
        // than the token's length
        if (unescaped_length < dst_length) {
            dst[unescaped_length] = '\0';
        }

        return sexp_success;
    }
}}
