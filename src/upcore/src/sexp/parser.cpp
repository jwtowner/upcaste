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
#include <up/log.hpp>

namespace up { namespace sexp
{
    extern LIBUPCOREAPI
    parser_messages const default_parser_messages =
    {
        {
            "none",
            "infix",
            "prefix",
            "comment",
            "label",
            "number",
            "symbol",
            "string",
            "close",
            "open",
            "error"
        },
        {
            "none",
            "dot",
            "quote",
            "quasiquote",
            "unquote",
            "unquote-splicing",
            "syntax",
            "quasisyntax",
            "unsyntax",
            "unsyntax-splicing",
            "datum comment",
            "block comment",
            "line comment",
            "label",
            "label",
            "boolean",
            "character",
            "integer",
            "rational",
            "real",
            "complex",
            "identifier",
            "escaped identifier",
            "string",
            "raw string",
            "list closing",
            "list opening",
            "vector opening",
            "bytevector opening",
            "invalid syntax",
            "invalid character",
            "invalid number",
            "invalid exactness",
            "invalid radix",
            "unclosed block comment",
            "unclosed identifier",
            "unclosed raw string",
            "unclosed string",
            "unmatched list closing",
            "mismatched list closing"
        },
        {
            "parenthesis",
            "bracket",
            "brace"
        },
        {
            "unspecified",
            "exact",
            "inexact"
        },
        {
            "unspecified",
            "half",
            "single",
            "double",
            "extended"
        },
        {
            "expected a %s, but found '%.*s'",
            "expected a %s, but reached end of file",
            "expected a string matching \"%s\", but found '%.*s'",
            "expected a string matching \"%s\", but reached end of file",
            "expected identifier '%s', but found '%.*s'",
            "expected identifier '%s', but reached end of file"
        }
    };

    LIBUPCOREAPI
    int parser_construct(parser* UPRESTRICT par, allocator* UPRESTRICT alloc) noexcept {
        if (!par) {
            return sexp_badarg;
        }

        if (!alloc) {
            alloc = malloc_allocator::instance();
        }
        
        verify(sexp_success == lexer_construct(&par->lex, nullptr, 0));
        slist_init(&par->open_stack);
        slist_init(&par->unread_stack);
        par->prev_column = 0;
        par->prev_line = 0;
        par->messages = &default_parser_messages;
        par->error_handler = &parser_default_error_handler;
        par->warning_handler = &parser_default_warning_handler;
        par->error_count = 0;
        par->warning_count = 0;
        par->alloc = alloc;
        par->filename = nullptr;
        par->filename_length = 0;
        par->buffer = nullptr;
        par->buffer_length = 0;
        par->buffer_allocated = false;
        par->messages_allocated = false;
        par->loaded = false;
        par->error_brace_tokens = false;
        par->error_bracket_tokens = false;
        par->error_infix_tokens = false;
        par->error_label_tokens = false;
        par->error_prefix_tokens = false;
        par->ignore_comment_tokens = true;
        par->ignore_error_tokens = false;
        par->ignore_infix_tokens = false;
        par->ignore_prefix_tokens = false;
        par->unescape_identifiers = true;
        par->unescape_strings = true;
        return sexp_success;
    }

    LIBUPCOREAPI
    int parser_destruct(parser* par) noexcept {
        if (!par) {
            return sexp_badarg;
        }

        allocator* const alloc = par->alloc;
        assert(alloc);

        slist_clear<parser_token_block, &parser_token_block::node>(&par->open_stack, alloc);
        slist_clear<parser_token_block, &parser_token_block::node>(&par->unread_stack, alloc);

        if (par->buffer_allocated) {
            alloc->deallocate(par->buffer, par->buffer_length);
            par->buffer_allocated = false;
        }

        if (par->filename) {
            alloc->deallocate(par->filename, par->filename_length + 1);
            par->filename = nullptr;
        }

        par->loaded = false;

        if (par->messages_allocated) {
            alloc->deallocate(const_cast<parser_messages*>(par->messages), sizeof(parser_messages));
            par->messages_allocated = false;
        }

        return sexp_success;
    }

    LIBUPCOREAPI UPNONNULLALL
    void parser_default_error_handler(
        char const* filename,
        uintmax_t line,
        uintmax_t column,
        char const* message
    )
    noexcept {
        log_eventf(
            "up.sexp",
            log_level_error,
            "%s(%lu,%lu): error - %s\n",
            filename,
            static_cast<unsigned long>(line),
            static_cast<unsigned long>(column),
            message
        );
    }

    LIBUPCOREAPI UPNONNULLALL
    void parser_default_warning_handler(
        char const* filename,
        uintmax_t line,
        uintmax_t column,
        char const* message
    )
    noexcept {
        log_eventf(
            "up.sexp",
            log_level_warning,
            "%s(%lu,%lu): warning - %s\n",
            filename,
            static_cast<unsigned long>(line),
            static_cast<unsigned long>(column),
            message
        );
    }
}}
