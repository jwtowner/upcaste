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

        slist_node* node = slist_peek_front(&par->unread_stack);
        parser_token_block* block = slist_cast<parser_token_block*>(node, &parser_token_block::node);
        if (!node || (block->count >= parser_token_block_max_tokens)) {
            block = static_cast<parser_token_block*>(alloc->allocate(sizeof(parser_token_block)));
            if (!block) {
                parser_error(par, "out of memory");
                return sexp_nomem;
            }
            slist_push_front(&par->unread_stack, slist_cast<slist_node*>(block, &parser_token_block::node));
            block->count = 0;
        }
        
        block->tokens[block->count++] = *tok;
        return sexp_success;
    }
}}
