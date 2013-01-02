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

#ifndef UP_SEXP_HPP
#define UP_SEXP_HPP

#include <up/allocator.hpp>
#include <up/cstdint.hpp>
#include <up/slist.hpp>

namespace up { namespace sexp
{
    constexpr int sexp_success = 0;
    constexpr int sexp_eof = -1;
    constexpr int sexp_badsyntax = -2;
    constexpr int sexp_badstate = -3;
    constexpr int sexp_badarg = -4;
    constexpr int sexp_nomatch = -5;
    constexpr int sexp_nomem = -6;
    constexpr int sexp_overflow = -7;
    constexpr int sexp_underflow = -8;

    constexpr unsigned int list_parenthesis             = 0;
    constexpr unsigned int list_bracket                 = 1;
    constexpr unsigned int list_brace                   = 2;

    constexpr unsigned int exactness_unspecified        = 0;
    constexpr unsigned int exactness_exact              = 1;
    constexpr unsigned int exactness_inexact            = 2;

    constexpr unsigned int precision_unspecified        = 0;
    constexpr unsigned int precision_single             = 1;
    constexpr unsigned int precision_double             = 2;

    constexpr unsigned int category_none                = 0x0000;
    constexpr unsigned int category_infix               = 0x0001;
    constexpr unsigned int category_prefix              = 0x0002;
    constexpr unsigned int category_comment             = 0x0003;
    constexpr unsigned int category_label               = 0x0004;
    constexpr unsigned int category_number              = 0x0005;
    constexpr unsigned int category_symbol              = 0x0006;
    constexpr unsigned int category_string              = 0x0007;
    constexpr unsigned int category_close               = 0x0008;
    constexpr unsigned int category_open                = 0x0009;
    constexpr unsigned int category_error               = 0x000A;

    constexpr unsigned int token_none                   = 0x0000; // <eof>
    constexpr unsigned int token_dot                    = 0x0001; // .
    constexpr unsigned int token_quote                  = 0x0002; // '
    constexpr unsigned int token_quasiquote             = 0x0003; // `
    constexpr unsigned int token_unquote                = 0x0004; // ,
    constexpr unsigned int token_unquote_splicing       = 0x0005; // ,@
    constexpr unsigned int token_syntax                 = 0x0006; // #'
    constexpr unsigned int token_quasisyntax            = 0x0007; // #`
    constexpr unsigned int token_unsyntax               = 0x0008; // #,
    constexpr unsigned int token_unsyntax_splicing      = 0x0009; // #,@
    constexpr unsigned int token_ignore                 = 0x000A; // #_ | #;
    constexpr unsigned int token_block_comment          = 0x000B; // #| ... |#
    constexpr unsigned int token_line_comment           = 0x000C; // #! | ;
    constexpr unsigned int token_set_label              = 0x000D; // #(<digit>+)=
    constexpr unsigned int token_get_label              = 0x000E; // #(<digit>+)#
    constexpr unsigned int token_boolean                = 0x000F; // #[tT] | #[fF] | #true | #false
    constexpr unsigned int token_character              = 0x0010; // #\\<char-name> | #\\[xX]<hex-code>
    constexpr unsigned int token_integer                = 0x0011; // (#[eEiI])(#[bBoOdDxX])?[+-]?<digit>+
    constexpr unsigned int token_rational               = 0x0012; // (#[eEiI])(#[bBoOdDxX])?[+-]?<digit>+/<digit>+
    constexpr unsigned int token_real                   = 0x0013; // (#[eEiI])(#[bBoOdDxX])?[+-]?<digit>*\.<digit>+([eEsSfFdDlL][+-]?<digit>+)?
    constexpr unsigned int token_complex                = 0x0014; // (#[eEiI])(#[bBoOdDxX])?(<real>)?[+-](<ureal>)?i
    constexpr unsigned int token_identifier             = 0x0015; // <id-char>+
    constexpr unsigned int token_escaped_identifier     = 0x0016; // \|[^\|]*\|
    constexpr unsigned int token_string                 = 0x0017; // \"[^\"\\]*\"
    constexpr unsigned int token_escaped_string         = 0x0018; // \"[^\"]*\"
    constexpr unsigned int token_raw_string             = 0x0019; // #@<tag>\n ... <tag>@#\n
    constexpr unsigned int token_list_close             = 0x001A; // ) | ] | }
    constexpr unsigned int token_list_open              = 0x001B; // ( | [ | {
    constexpr unsigned int token_vector_open            = 0x001C; // #( | #[ | #{
    constexpr unsigned int token_bytevector_open        = 0x001D; // #u8( | #u8[ | #u8{
    constexpr unsigned int token_invalid_syntax         = 0x001E; // #<invalid>
    constexpr unsigned int token_invalid_character      = 0x001F; // #\\<invalid>
    constexpr unsigned int token_invalid_number         = 0x0020; // #([eEiI]|[bBoOdDxX])<invalid>
    constexpr unsigned int token_invalid_exactness      = 0x0021; // #[bBoOdDxX]#<invalid>
    constexpr unsigned int token_invalid_radix          = 0x0022; // #[eEiI]#<invalid>
    constexpr unsigned int token_unclosed_block_comment = 0x0023; // #| ...
    constexpr unsigned int token_unclosed_identifier    = 0x0024; // | ...
    constexpr unsigned int token_unclosed_raw_string    = 0x0025; // #@<tag>\n ...
    constexpr unsigned int token_unclosed_string        = 0x0026; // \" ...
    constexpr unsigned int token_unmatched_list_close   = 0x0027; // ) | ] | }
    constexpr unsigned int token_mismatched_list_close  = 0x0028; // ) | ] | }

    struct LIBUPCOREAPI token_number_info
    {
        uint_least8_t prefix_length;
        uint_least8_t exactness;
        uint_least8_t precision;
        int_least8_t radix;
    };

    struct LIBUPCOREAPI token_raw_info
    {
        size_t start_length:16;
        size_t end_length:16;
    };

    struct LIBUPCOREAPI token
    {
        unsigned int category:16;
        unsigned int type:16;
        union {
            bool bool_value;
            uint_least32_t char_value;
            unsigned int list_type;
            token_number_info number_info;
            token_raw_info raw_info;
        };
        char const* text;
        size_t length;
        uintmax_t column;
        uintmax_t line;
    };

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_int8(token const* UPRESTRICT tok, int_least8_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_uint8(token const* UPRESTRICT tok, uint_least8_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_int16(token const* UPRESTRICT tok, int_least16_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_uint16(token const* UPRESTRICT tok, uint_least16_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_int32(token const* UPRESTRICT tok, int_least32_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_uint32(token const* UPRESTRICT tok, uint_least32_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_int64(token const* UPRESTRICT tok, int_least64_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_uint64(token const* UPRESTRICT tok, uint_least64_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_float(token const* UPRESTRICT tok, float* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_double(token const* UPRESTRICT tok, double* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    int token_parse_long_double(token const* UPRESTRICT tok, long double* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_int8(token const* UPRESTRICT tok, int_least8_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_uint8(token const* UPRESTRICT tok, uint_least8_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_int16(token const* UPRESTRICT tok, int_least16_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_uint16(token const* UPRESTRICT tok, uint_least16_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_int32(token const* UPRESTRICT tok, int_least32_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_uint32(token const* UPRESTRICT tok, uint_least32_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_int64(token const* UPRESTRICT tok, int_least64_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_uint64(token const* UPRESTRICT tok, uint_least64_t* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_float(token const* UPRESTRICT tok, float* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_double(token const* UPRESTRICT tok, double* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_long_double(token const* UPRESTRICT tok, long double* UPRESTRICT result) noexcept;

    extern LIBUPCOREAPI
    int token_to_bool(token const* UPRESTRICT tok, bool* UPRESTRICT result) noexcept;

    struct LIBUPCOREAPI lexer
    {
        char const* first;
        char const* last;
        char const* cursor;
        char const* prev_cursor;
        uintmax_t column;
        uintmax_t line;
    };
        
    extern LIBUPCOREAPI
    int lexer_construct(lexer* UPRESTRICT lex, char const* UPRESTRICT text, size_t length) noexcept;

    extern LIBUPCOREAPI
    int lexer_rewind(lexer* lex) noexcept;

    extern LIBUPCOREAPI
    int lexer_peek(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept;

    extern LIBUPCOREAPI
    int lexer_read(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept;

    extern LIBUPCOREAPI UPPURE
    bool lexer_is_delimiter(uint_least32_t c) noexcept;

    inline UPALWAYSINLINE
    bool lexer_is_eof(lexer const* lex) noexcept {
        return lex->cursor == lex->last;
    }

    typedef void (*parser_error_handler)(char const* file, uintmax_t line, uintmax_t column, char const* message);

    struct LIBUPCOREAPI parser_token
    {
        slist_node node;
        token tok;
    };

    struct LIBUPCOREAPI parser
    {
        lexer lex;
        slist_node open_stack;
        slist_node unread_stack;
        uintmax_t prev_column;
        uintmax_t prev_line;
        parser_error_handler error_handler;
        parser_error_handler warning_handler;
        uintmax_t error_count;
        uintmax_t warning_count;
        allocator* alloc;
        char* filename;
        size_t filename_length;
        char* buffer;
        size_t buffer_length;
        bool buffer_allocated:1;
        bool loaded:1;
        bool error_brace_tokens:1;
        bool error_bracket_tokens:1;
        bool error_infix_tokens:1;
        bool error_label_tokens:1;
        bool error_prefix_tokens:1;
        bool ignore_comment_tokens:1;
        bool ignore_error_tokens:1;
        bool ignore_infix_tokens:1;
        bool ignore_prefix_tokens:1;
        bool unescape_identifiers:1;
        bool unescape_strings:1;
    };

    extern LIBUPCOREAPI
    int parser_construct(parser* UPRESTRICT par, allocator* UPRESTRICT alloc) noexcept;

    extern LIBUPCOREAPI
    int parser_destruct(parser* par) noexcept;

    extern LIBUPCOREAPI
    int parser_load_file(parser* UPRESTRICT par, char const* UPRESTRICT filename) noexcept;

#ifdef UP_HAS_STDC_WCHAR
    extern LIBUPCOREAPI
    int parser_load_file(parser* UPRESTRICT par, wchar_t const* UPRESTRICT filename) noexcept;
#endif

    extern LIBUPCOREAPI
    int parser_load_memory(parser* UPRESTRICT par, char const* UPRESTRICT filename, char const* UPRESTRICT text, size_t length) noexcept;

    extern LIBUPCOREAPI
    int parser_unload(parser* par) noexcept;

    extern LIBUPCOREAPI
    int parser_rewind(parser* par) noexcept;

    extern LIBUPCOREAPI
    int parser_peek(parser* UPRESTRICT par, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_read(parser* UPRESTRICT par, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_unread(parser* UPRESTRICT par, token const* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_peek_category(parser* UPRESTRICT par, unsigned int category, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_peek_type(parser* UPRESTRICT par, unsigned int type, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_peek_string(parser* UPRESTRICT par, char const* s, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_peek_symbol(parser* UPRESTRICT par, char const* id, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_read_category(parser* UPRESTRICT par, unsigned int category, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_read_type(parser* UPRESTRICT par, unsigned int type, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_read_string(parser* UPRESTRICT par, char const* str, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_read_symbol(parser* UPRESTRICT par, char const* id, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_expect_category(parser* UPRESTRICT par, unsigned int category, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_expect_type(parser* UPRESTRICT par, unsigned int type, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_expect_string(parser* UPRESTRICT par, char const* str, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_expect_symbol(parser* UPRESTRICT par, char const* id, token* UPRESTRICT token) noexcept;

    extern LIBUPCOREAPI
    int parser_error(parser* UPRESTRICT par, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI
    int parser_warning(parser* UPRESTRICT par, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void parser_default_error_handler(char const* file, uintmax_t line, uintmax_t column, char const* message) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void parser_default_warning_handler(char const* file, uintmax_t line, uintmax_t column, char const* message) noexcept;

    inline UPALWAYSINLINE
    void parser_set_error_handler(parser* par, parser_error_handler handler) noexcept {
        par->error_handler = handler;
    }

    inline UPALWAYSINLINE
    void parser_set_warning_handler(parser* par, parser_error_handler handler) noexcept {
        par->warning_handler = handler;
    }

    inline UPALWAYSINLINE
    bool parser_has_error_occurred(parser const* par) noexcept {
        return par->error_count > 0;
    }

    inline UPALWAYSINLINE
    bool parser_has_warning_occurred(parser const* par) noexcept {
        return par->warning_count > 0;
    }

    inline UPALWAYSINLINE
    bool parser_is_eof(parser const* par) noexcept {
        return lexer_is_eof(&par->lex);
    }
}}

#endif
