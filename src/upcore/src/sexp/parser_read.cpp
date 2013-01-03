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
            void operator()(parser_token* pt) noexcept {
                char text[16];
                assert(pt->tok.length < 15);
                memcpy(text, pt->tok.text, pt->tok.length);
                text[pt->tok.length] = '\0';
                par->prev_column = pt->tok.column;
                par->prev_line = pt->tok.line;
                parser_error(par, "unmatched list opening '%s'", text);
            }
        };
    }

    LIBUPCOREAPI
    int parser_read(parser* UPRESTRICT par, token* UPRESTRICT tok) noexcept {
        allocator* alloc;
        slist_node* node;
        parser_token* token_node;
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
                assert((retval == sexp_success) || (retval == sexp_eof) || (retval == sexp_badsyntax));
            }
            else {
                node = slist_pop_front(&par->unread_stack);
                token_node = slist_cast<parser_token*>(node, &parser_token::node);
                *tok = token_node->tok;
                alloc->deallocate(token_node, sizeof(parser_token));
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
                    slist_foreach<parser_token, &parser_token::node>(&par->open_stack, scan_unmatched_lists(par));
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
                    token_node = slist_cast<parser_token*>(node, &parser_token::node);
                    if (token_node->tok.list_type == tok->list_type) {
                        slist_unlink<parser_token, &parser_token::node>(&par->open_stack, node, alloc);
                    }
                    else {
                        if (token_node->tok.list_type == list_parenthesis) {
                            temp = ')';
                        }
                        else if (token_node->tok.list_type == list_bracket) {
                            temp = ']';
                        }
                        else {
                            temp = '}';
                        }
                        parser_error(par, "mismatched list closing '%c', expected '%c'", tok->text[0], temp);
                        tok->category = category_error;
                        tok->type = token_mismatched_list_close;
                        retval = sexp_badsyntax;
                    }
                }
                break;

            case category_open:
                token_node = static_cast<parser_token*>(alloc->allocate(sizeof(parser_token)));
                if (!token_node) {
                    parser_error(par, "out of memory");
                    retval = sexp_nomem;
                }
                else {
                    slist_push_front(&par->open_stack, slist_cast<slist_node*>(token_node, &parser_token::node));
                    token_node->tok = *tok;
                }
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
