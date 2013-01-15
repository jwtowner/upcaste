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
    namespace
    {
        struct UPHIDDEN scan_unmatched_lists
        {
            parser* par;

            UPALWAYSINLINE
            scan_unmatched_lists(parser* p) noexcept
            : par(p) {
            }

            UPALWAYSINLINE
            void operator()(parser_token_block* block) noexcept {
                char text[16];
                token* tok;
                size_t i;

                for (i = 0; i < block->count; --i) {
                    tok = &block->tokens[i];
                    assert(tok->length < 15);
                    memcpy(text, tok->text, tok->length);
                    text[tok->length] = '\0';
                    par->prev_column = tok->column;
                    par->prev_line = tok->line;
                    parser_error(par, "unmatched list opening '%s'", text);
                }
            }
        };
    }

    LIBUPCOREAPI
    int parser_read(parser* UPRESTRICT par, token* UPRESTRICT tok) noexcept {
        token* top_token;
        parser_token_block* block;
        slist_node* node;
        allocator* alloc;
        int retval, temp;
        bool done;

        if (!par || !tok) {
            return sexp_badarg;
        }
        else if (!par->loaded) {
            return sexp_badstate;
        }

        alloc = par->alloc;
        assert(alloc);

        do {
            if (slist_is_empty(&par->unread_stack)) {
                retval = lexer_read(&par->lex, tok);
                assert(sexp_badsyntax <= retval);
            }
            else {
                node = slist_peek_front(&par->unread_stack);
                block = slist_cast<parser_token_block*>(node, &parser_token_block::node);
                assert(block->count > 0);
                *tok = block->tokens[--(block->count)];
                if (!block->count) {
                    verify(node == slist_pop_front(&par->unread_stack));
                    alloc->deallocate(block, sizeof(parser_token_block));
                }
            }

            par->prev_column = tok->column;
            par->prev_line = tok->line;
            retval = sexp_success;
            done = true;

            switch (tok->category) {
            case category_none:
                assert(tok->type == token_none);
                if (!slist_is_empty(&par->open_stack)) {
                    parser_error(par, "premature end of file");
                    slist_foreach<parser_token_block, &parser_token_block::node>(&par->open_stack, scan_unmatched_lists(par));
                }
                retval = sexp_eof;
                break;

            case category_close:
                assert((tok->type == token_list_close) && tok->text && (tok->length > 0));
                node = slist_peek_front(&par->open_stack);
                if (!node) {
                    parser_error(par, "unmatched list closing '%c'", tok->text[0]);
                    tok->category = category_error;
                    tok->type = token_unmatched_list_close;
                    retval = sexp_badsyntax;
                }
                else {
                    block = slist_cast<parser_token_block*>(node, &parser_token_block::node);
                    assert(block->count > 0);
                    top_token = &block->tokens[block->count - 1];
                    if ((top_token->list_type == tok->list_type) && !(--block->count)) {
                        slist_unlink<parser_token_block, &parser_token_block::node>(&par->open_stack, node, alloc);
                    }
                    else {
                        if (top_token->list_type == list_parenthesis) {
                            temp = ')';
                        }
                        else if (top_token->list_type == list_bracket) {
                            temp = ']';
                        }
                        else {
                            temp = '}';
                        }
                        parser_error(par, "mismatched list closing '%c' expected '%c'", tok->text[0], temp);
                        tok->category = category_error;
                        tok->type = token_mismatched_list_close;
                        retval = sexp_badsyntax;
                    }
                }
                break;

            case category_open:
                node = slist_peek_front(&par->unread_stack);
                block = slist_cast<parser_token_block*>(node, &parser_token_block::node);
                if (!node || (block->count >= parser_token_block_max_tokens)) {
                    block = static_cast<parser_token_block*>(alloc->allocate(sizeof(parser_token_block)));
                    if (!block) {
                        parser_error(par, "out of memory");
                        retval = sexp_nomem;
                        break;
                    }
                    slist_push_front(&par->open_stack, slist_cast<slist_node*>(block, &parser_token_block::node));
                    block->count = 0;
                }
                block->tokens[block->count++] = *tok;
                break;

            case category_infix:
                if (par->ignore_infix_tokens) {
                    done = false;
                }
                else if (par->error_infix_tokens) {
                    parser_error(par, "invalid syntax");
                    tok->category = category_error;
                    tok->type = token_invalid_syntax;
                    retval = sexp_badsyntax;
                }
                break;

            case category_prefix:
                if (par->ignore_prefix_tokens) {
                    done = false;
                }
                else if (par->error_prefix_tokens) {
                    parser_error(par, "invalid syntax");
                    tok->category = category_error;
                    tok->type = token_invalid_syntax;
                    retval = sexp_badsyntax;
                }
                break;

            case category_label:
                if (par->error_label_tokens) {
                    parser_error(par, "invalid syntax");
                    tok->category = category_error;
                    tok->type = token_invalid_syntax;
                    retval = sexp_badsyntax;
                }
                break;

            case category_comment:
                if (par->ignore_comment_tokens) {
                    done = false;
                }
                break;

            case category_error:
                parser_error(par, "invalid syntax");
                retval = sexp_badsyntax;
                break;

            default:
                break;
            }

            if ((retval == sexp_badsyntax) && par->ignore_error_tokens) {
                done = false;
            }
        }
        while (!done);

        return retval;
    }
}}
