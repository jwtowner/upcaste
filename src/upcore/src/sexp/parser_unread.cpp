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
    int parser_unread(parser* UPRESTRICT par, token const* UPRESTRICT tok) noexcept {
        if (!par || !tok) {
            return sexp_badarg;
        }
        else if (!par->loaded) {
            return sexp_badstate;
        }

        allocator* const alloc = par->alloc;
        assert(alloc);

        parser_token* token_node = static_cast<parser_token*>(alloc->allocate(sizeof(parser_token)));
        if (!token_node) {
            parser_error(par, "out of memory");
            return sexp_nomem;
        }
        
        token_node->tok = *tok;
        slist_push_front(&par->unread_stack, slist_cast<slist_node*>(token_node, &parser_token::node));
        return sexp_success;
    }
}}
