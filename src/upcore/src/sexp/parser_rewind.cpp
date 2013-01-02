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
    int parser_rewind(parser* par) noexcept {
        if (!par) {
            return sexp_badarg;
        }
        else if (!par->loaded) {
            return sexp_badstate;
        }

        allocator* const alloc = par->alloc;
        assert(alloc);

        slist_clear<parser_token, &parser_token::node>(&par->open_stack, [=] (parser_token* p) {
            alloc->deallocate(p, sizeof(parser_token));
        });

        slist_clear<parser_token, &parser_token::node>(&par->unread_stack, [=] (parser_token* p) {
            alloc->deallocate(p, sizeof(parser_token));
        });

        verify(sexp_success == lexer_rewind(&par->lex));
        par->prev_column = 0;
        par->prev_line = 0;
        par->error_count = 0;
        par->warning_count = 0;
        return sexp_success;
    }
}}
