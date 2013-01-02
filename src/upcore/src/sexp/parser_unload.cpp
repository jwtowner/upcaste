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
    int parser_unload(parser* par) noexcept {
        if (!par) {
            return sexp_badarg;
        }

        allocator* const alloc = par->alloc;
        assert(alloc);

        verify(sexp_success == lexer_construct(&par->lex, nullptr, 0));

        slist_clear<parser_token, &parser_token::node>(&par->open_stack, [=] (parser_token* p) {
            alloc->deallocate(p, sizeof(parser_token));
        });

        slist_clear<parser_token, &parser_token::node>(&par->unread_stack, [=] (parser_token* p) {
            alloc->deallocate(p, sizeof(parser_token));
        });

        if (par->buffer_allocated) {
            alloc->deallocate(par->buffer, par->buffer_length);
            par->buffer = nullptr;
            par->buffer_length = 0;
            par->buffer_allocated = false;
        }

        if (par->filename) {
            alloc->deallocate(par->filename, par->filename_length + 1);
            par->filename = nullptr;
            par->filename_length = 0;
        }

        par->prev_column = 0;
        par->prev_line = 0;
        par->error_count = 0;
        par->warning_count = 0;
        par->loaded = false;
        return sexp_success;
    }
}}
