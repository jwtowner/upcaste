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
#include <up/cuchar.hpp>
#include <up/cuctype.hpp>

namespace up { namespace sexp { namespace
{
    alignas(UP_MAX_CACHE_LINE_SIZE) bool const ascii_delimiter_table[128] =
    {
         true,  true,  true,  true,  true,  true,  true,  true, // NUL, SOH, STX, ETX, EOT, ENQ, ACK, BEL,
         true,  true,  true,  true,  true,  true,  true,  true, //  BS, TAB,  LF,  VT,  FF,  CR,  SO,  SI,
         true,  true,  true,  true,  true,  true,  true,  true, // DLE, DC1, DC2, DC3, DC4, NAK, SYN, ETB,
         true,  true,  true,  true,  true,  true,  true,  true, // CAN,  EM, SUB, ESC,  FS,  GS,  RS,  US,
         true, false,  true, false, false, false, false,  true, // SPC,   !,   ",   #,   $,   %,   &,   ',
         true,  true, false, false,  true, false, false, false, //   (,   ),   *,   +,   ,,   -,   .,   /,
        false, false, false, false, false, false, false, false, //   0,   1,   2,   3,   4,   5,   6,   7,
        false, false, false,  true, false, false, false, false, //   8,   9,   :,   ;,   <,   =,   >,   ?,
        false, false, false, false, false, false, false, false, //   @,   A,   B,   C,   D,   E,   F,   G,
        false, false, false, false, false, false, false, false, //   H,   I,   J,   K,   L,   M,   N,   O,
        false, false, false, false, false, false, false, false, //   P,   Q,   R,   S,   T,   U,   V,   W,
        false, false, false,  true, false,  true, false, false, //   X,   Y,   Z,   [,   \,   ],   ^,   _,
         true, false, false, false, false, false, false, false, //   `,   a,   b,   c,   d,   e,   f,   g,
        false, false, false, false, false, false, false, false, //   h,   i,   j,   k,   l,   m,   n,   o,
        false, false, false, false, false, false, false, false, //   p,   q,   r,   s,   t,   u,   v,   w,
        false, false, false,  true,  true,  true, false,  true  //   x,   y,   z,   {,   |,   },   ~,  DEL
    };

    token_number_info const default_number_info =
    {
        0, exactness_unspecified, precision_unspecified, 10
    };

    token_number_info const exact_number_info =
    {
        2, exactness_exact, precision_unspecified, 10
    };

    token_number_info const inexact_number_info =
    {
        2, exactness_inexact, precision_unspecified, 10
    };
    
    token_number_info const binary_number_info =
    {
        2, exactness_unspecified, precision_unspecified, 2
    };
    
    token_number_info const octal_number_info =
    {
        2, exactness_unspecified, precision_unspecified, 8
    };
    
    token_number_info const decimal_number_info =
    {
        2, exactness_unspecified, precision_unspecified, 10
    };
    
    token_number_info const hexadecimal_number_info =
    {
        2, exactness_unspecified, precision_unspecified, 16 
    };

    inline UPALWAYSINLINE UPPURE
    bool lexer_fast_is_delimiter(uint_least32_t ch) noexcept {
        return ((ch < 0x80) && ascii_delimiter_table[ch]) || !isugraph(ch);
    }

    inline UPALWAYSINLINE
    const char* lexer_u8tou32(
        uint_least32_t* UPRESTRICT ch,
        const char* UPRESTRICT cursor,
        const char* UPRESTRICT last
    )
    noexcept {
        char32_t ch32 = *reinterpret_cast<unsigned char const*>(cursor);
        if (ch32 <= 0x7F) {
            ++cursor;
        }
        else {
            size_t const length = last - cursor;
            int const octet_count = u8tou32(&ch32, cursor, last - cursor); 
            if (octet_count > 0) {
                cursor += octet_count;
            }
            else {
                cursor = u8snerr(cursor, length);
                if (!cursor) {
                    cursor = last;
                }
            }
        }
        *ch = ch32;
        return cursor;
    }

    inline UPALWAYSINLINE
    uint_least32_t lexer_get_char(lexer* lex) noexcept {
        uint_least32_t ch = ueof;
        if (lex->cursor < lex->last) {
            lex->prev_cursor = lex->cursor;
            lex->cursor = lexer_u8tou32(&ch, lex->cursor, lex->last);
            ++lex->column;
        }
        return ch;
    }

    inline UPALWAYSINLINE
    void lexer_unget_char(lexer* lex, uint_least32_t ch) noexcept {
        if ((ch != ueof) && lex->prev_cursor) {
            lex->cursor = lex->prev_cursor;
            lex->prev_cursor = nullptr;
            if (!(lex->column--)) {
                lex->column = 0;
                --lex->line;
            }
        }
    }

    size_t lexer_read_until_delimiter(lexer* lex) noexcept {
        char const* const last = lex->last;
        char const* const start = lex->cursor;
        char const* cursor = start;
        char const* prev_cursor = lex->prev_cursor;
        uintmax_t column = lex->column;
        uint_least32_t ch;

        while (cursor < last) {
            prev_cursor = cursor;
            cursor = lexer_u8tou32(&ch, cursor, last);
            if (lexer_fast_is_delimiter(ch)) {
                cursor = prev_cursor;
                break;
            }
            ++column;
        }

        lex->cursor = cursor;
        lex->prev_cursor = prev_cursor;
        lex->column = column;
        return cursor - start;
    }

    void lexer_read_until_delimiter(lexer* lex, uint_least32_t first_ch) noexcept {
        if (lexer_fast_is_delimiter(first_ch)) {
            lexer_unget_char(lex, first_ch);
        }
        else {
            lexer_read_until_delimiter(lex);
        }
    }

    bool lexer_read_delimited_token(lexer* UPRESTRICT lex, char const* UPRESTRICT s, size_t n) noexcept {
        char const* const last = lex->last;
        char const* cursor = lex->cursor;
        uint_least32_t ch;

        if ((static_cast<size_t>(last - cursor) < n) || (fast_strncasecmp(cursor, s, n) != 0)) {
            return false;
        }

        cursor += n;
        if (cursor < last) {
            lexer_u8tou32(&ch, cursor, last);
            if (!lexer_fast_is_delimiter(ch)) {
                return false;
            }
        }
        
        lex->cursor = cursor;
        lex->prev_cursor = cursor - 1;
        lex->column += n;
        return true;
    }

    bool lexer_read_token_part(lexer* UPRESTRICT lex, char const* UPRESTRICT s, size_t n, uint_least32_t* result) noexcept {
        char const* const last = lex->last;
        char const* cursor = lex->cursor;
        char const* prev_cursor = lex->cursor;
        uintmax_t column = lex->column;
        uint_least32_t ch = ueof;

        if ((static_cast<size_t>(last - cursor) < n) || (fast_strncasecmp(cursor, s, n) != 0)) {
            return false;
        }

        prev_cursor = cursor + n - 1;
        cursor += n;
        column += n;
        
        if (cursor < last) {
            prev_cursor = cursor;
            cursor = lexer_u8tou32(&ch, cursor, last);
            if (!lexer_fast_is_delimiter(ch)) {
                return false;
            }
            ++column;
        }
        
        lex->cursor = cursor;
        lex->prev_cursor = prev_cursor;
        lex->column = column;
        *result = ch;
        return true;
    }

    inline UPALWAYSINLINE
    uint_least32_t lexer_read_digits_part(lexer* UPRESTRICT lex, int radix) noexcept {
        char const* const last = lex->last;
        char const* cursor = lex->cursor;
        char const* prev_cursor = lex->cursor;
        uintmax_t column = lex->column;
        uint_least32_t ch, retval = ueof;

        while (cursor < last) {
            prev_cursor = cursor;
            cursor = lexer_u8tou32(&ch, cursor, last);
            ++column;
            if (!isudigit(ch, radix)) {
                retval = ch;
                break;
            }
        }

        lex->cursor = cursor;
        lex->prev_cursor = prev_cursor;
        lex->column = column;
        return retval;
    }
    
    inline UPALWAYSINLINE
    unsigned int lexer_read_unquote(lexer* UPRESTRICT lex, unsigned int unquote, unsigned int unquote_splicing) noexcept {
        uint_least32_t ch = lexer_get_char(lex);
        if (ch == '@') {
            return unquote_splicing;
        }
        else {
            lexer_unget_char(lex, ch);
            return unquote;
        }
    }

    inline UPALWAYSINLINE
    int lexer_match_identifier(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        lexer_read_until_delimiter(lex);
        tok->category = category_symbol;
        tok->type = token_identifier;
        return sexp_success;
    }

    int lexer_match_escaped_identifier(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        char const* const last = lex->last;
        char const* cursor = lex->cursor;
        char const* prev_cursor = lex->prev_cursor;
        uintmax_t column = lex->column;
        uintmax_t line = lex->line;
        uint_least32_t ch;
        int retval;

        tok->category = category_symbol;
        tok->type = token_escaped_identifier;
        retval = sexp_success;

        for (;;) {
            if (cursor >= last) {
                tok->category = category_error;
                tok->type = token_unclosed_identifier;
                retval = sexp_badsyntax;
                break;
            }
            prev_cursor = cursor;
            cursor = lexer_u8tou32(&ch, cursor, last);
            ++column;
            if (ch == '|') {
                break;
            }
            if (ch == '\r') {
                column = 0;
                ++line;
                if ((cursor < last) && (*cursor == '\n')) {
                    ++cursor;
                }
                continue;
            }
            if ((('\n' <= ch) && (ch <= '\f')) || (ch == 0x0085) || ((ch | 0x0001) == 0x2029)) {
                column = 0;
                ++line;
                continue;
            }
        }

        lex->cursor = cursor;
        lex->prev_cursor = prev_cursor;
        lex->column = column;
        lex->line = line;
        return retval;
    }

    int lexer_match_block_comment(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        uint_least32_t ch;
        int retval;

        tok->category = category_comment;
        tok->type = token_block_comment;
        retval = sexp_success;
        
        for (;;) {
            ch = lexer_get_char(lex);
            if (ch == '|') {
                ch = lexer_get_char(lex);
                if (ch == '#') {
                    break;
                }
            }
            if (ch == ueof) {
                tok->category = category_error;
                tok->type = token_unclosed_block_comment;
                retval = sexp_badsyntax;
                break;
            }
        }

        return retval;
    }

    int lexer_match_line_comment(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        uint_least32_t ch;

        do {
            ch = lexer_get_char(lex);
            if ((('\n' <= ch) && (ch <= '\f')) || (ch == 0x0085) || ((ch | 0x0001) == 0x2029)) {
                lexer_unget_char(lex, ch);
                break;
            }
        }
        while (ch != ueof);

        tok->category = category_comment;
        tok->type = token_line_comment;
        return sexp_success;
    }

    inline UPALWAYSINLINE
    int lexer_match_boolean(lexer* UPRESTRICT lex, token* UPRESTRICT tok, char const* s, size_t n, bool value) noexcept {
        if (!lexer_read_delimited_token(lex, s, n)) {
            if (lexer_read_until_delimiter(lex) > 0) {
                tok->category = category_error;
                tok->type = token_invalid_syntax;
                return sexp_badsyntax;
            }
        }
        tok->category = category_number;
        tok->type = token_boolean;
        tok->bool_value = value;
        return sexp_success;
    }

    inline UPALWAYSINLINE
    bool lexer_match_character_name(
        lexer* UPRESTRICT lex,
        token* UPRESTRICT tok,
        uint_least32_t actual_ch,
        uint_least32_t expected_ch,
        char const* s,
        size_t n,
        uint_least32_t value
    )
    noexcept {
        if (((actual_ch & 0xFFFFFF5F) == expected_ch) && lexer_read_delimited_token(lex, s, n)) {
            tok->category = category_number;
            tok->type = token_character;
            tok->char_value = value;
            return true;
        }
        return false;
    }

    int lexer_match_character(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        uint_least32_t first, second, value;
        int digit;

        first = lexer_get_char(lex);
        second = lexer_get_char(lex);

        if ((second == '#') || lexer_fast_is_delimiter(second)) {
            lexer_unget_char(lex, second);
            tok->category = category_number;
            tok->type = token_character;
            tok->char_value = first;
            return sexp_success;
        }

        switch (first) {
        case 'a': case 'A':
            if (lexer_match_character_name(lex, tok, second, 'L', "ARM", 3, '\a')) {
                return sexp_success;
            }
            break;
        case 'b': case 'B':
            if (lexer_match_character_name(lex, tok, second, 'A', "CKSPACE", 7, '\b')) {
                return sexp_success;
            }
            break;
        case 'd': case 'D':
            if (lexer_match_character_name(lex, tok, second, 'E', "LETE", 4, 0x7F)) {
                return sexp_success;
            }
            break;
        case 'e': case 'E':
            if (lexer_match_character_name(lex, tok, second, 'S', "CAPE", 4, 0x1B)) {
                return sexp_success;
            }
            break;
        case 'p': case 'P':
            if (lexer_match_character_name(lex, tok, second, 'A', "GE", 2, '\f')) {
                return sexp_success;
            }
            break;
        case 'r': case 'R':
            if (lexer_match_character_name(lex, tok, second, 'E', "TURN", 4, '\r')) {
                return sexp_success;
            }
            break;
        case 's': case 'S':
            if (lexer_match_character_name(lex, tok, second, 'P', "ACE", 3, ' ')) {
                return sexp_success;
            }
            break;
        case 't': case 'T':
            if (lexer_match_character_name(lex, tok, second, 'A', "B", 1, '\t')) {
                return sexp_success;
            }
            break;
        case 'v': case 'V':
            if (lexer_match_character_name(lex, tok, second, 'T', "AB", 2, '\v')) {
                return sexp_success;
            }
            break;
        case 'n': case 'N':
            if ( lexer_match_character_name(lex, tok, second, 'E', "WLINE", 5, '\n')
                || lexer_match_character_name(lex, tok, second, 'U', "LL", 2, '\0') ) {
                return sexp_success;
            }
            break;
        case 'x': case 'X':
            digit = touxdigit(second);
            if (digit >= 0) {
                value = static_cast<uint_least32_t>(digit);

                for (;;) {
                    first = lexer_get_char(lex);
                    if (first == ueof) {
                        break;
                    }
                        
                    digit = touxdigit(first);
                    if (digit < 0) {
                        if ((first != '#') && !lexer_fast_is_delimiter(first)) {
                            goto invalid;
                        }
                        lexer_unget_char(lex, first);
                        break;
                    }

                    value = (value * 16) + static_cast<uint_least32_t>(digit);
                    if (value > 0x10FFFF) {
                        goto invalid;
                    }
                }

                tok->category = category_number;
                tok->type = token_character;
                tok->char_value = value;
                return sexp_success;
            }
            break;
        default:
            break;
        }

    invalid:

        lexer_read_until_delimiter(lex, first);
        tok->category = category_error;
        tok->type = token_invalid_character;
        return sexp_badsyntax;
    }

    int lexer_match_string(lexer* UPRESTRICT lex, token* UPRESTRICT tok, char const* start) noexcept {
        char const* cursor, * prev_cursor, * last;
        uintmax_t column, line;
        unsigned int category, type;
        uint_least32_t ch = ueof, prev_ch;
        int retval;

        cursor = lex->cursor;
        prev_cursor = lex->prev_cursor;
        last = lex->last;
        column = lex->column;
        line = lex->line;
        category = category_string;
        type = token_string;
        retval = sexp_success;

        while (cursor < last) {
            prev_ch = ch;
            prev_cursor = cursor;
            cursor = lexer_u8tou32(&ch, cursor, last);
            ++column;
            if ((ch == '\"') && (prev_ch != '\\')) {
                goto finished;
            }
            else if (ch == '\\') {
                type = token_escaped_string;
            }
            else if (ch == '\r') {
                column = 0;
                ++line;
                if ((cursor < last) && (*cursor == '\n')) {
                    ++cursor;
                }
            }
            else if ((('\n' <= ch) && (ch <= '\f')) || (ch == 0x0085) || ((ch | 0x0001) == 0x2029)) {
                column = 0;
                ++line;
            }
        }

        category = category_error;
        type = token_unclosed_string;
        retval = sexp_badsyntax;

    finished:

        lex->cursor = cursor;
        lex->prev_cursor = prev_cursor;
        lex->line = line;
        lex->column = column;
        tok->category = category;
        tok->type = type;
        tok->text = start;
        tok->length = static_cast<size_t>(lex->cursor - start);
        return retval;
    }

    int lexer_match_raw_string(lexer* UPRESTRICT lex, token* UPRESTRICT tok, char const* start) noexcept {
        char const* cursor, * prev_cursor, * last, * tag;
        size_t i, tag_length, start_length;
        uint_least32_t ch;
        uintmax_t line;
        bool match;

        tag = cursor = lex->cursor;
        last = lex->last;
        line = lex->line;

        // match opening tag
        for (;;) {
            if (cursor >= last) {
                goto eof;
            }

            prev_cursor = cursor;
            cursor = lexer_u8tou32(&ch, cursor, last);
            
            if (ch == '\r') {
                if ((cursor < last) && (*cursor == '\n')) {
                    ++cursor;
                }
                break;
            }
            else if ((('\n' <= ch) && (ch <= '\f')) || (ch == 0x0085) || ((ch | 0x0001) == 0x2029)) {
                break;
            }
        }

        ++line;
        tag_length = static_cast<size_t>(prev_cursor - tag);
        start_length = static_cast<size_t>(cursor - start);
        assert(start_length <= ((1 << 16) - 1));

        // match body
        for (;;) {
            if (static_cast<size_t>(last - cursor) <= (tag_length + 2)) {
                goto eof;
            }

            prev_cursor = cursor;
            for (i = 0, match = false; i < tag_length; ++cursor, ++i) {
                if (*cursor != tag[i]) {
                    break;
                }
            }

            if ((i >= tag_length) && (cursor[0] == '@') && (cursor[1] == '#')) {
                match = true;
                cursor += 2;
            }

            if (cursor >= last) {
                goto eof;
            }

            cursor = lexer_u8tou32(&ch, cursor, last);

            if (ch == '\r') {
                ch = '\n';
                if ((cursor < last) && (*cursor == '\n')) {
                    ++cursor;
                }
            }

            if ((('\n' <= ch) && (ch <= '\f')) || (ch == 0x0085) || ((ch | 0x0001) == 0x2029)) {
                ++line;
                if (match) {
                    break;
                }
            }
        }

        lex->cursor = cursor;
        lex->prev_cursor = nullptr;
        lex->line = line;
        lex->column = 0;
        tok->category = category_string;
        tok->type = token_raw_string;
        tok->raw_info.start_length = start_length;
        tok->raw_info.end_length = cursor - prev_cursor;
        tok->text = start;
        tok->length = static_cast<size_t>(cursor - start);
        return sexp_success;

    eof:

        lex->cursor = last;
        lex->prev_cursor = nullptr;
        lex->line = line;
        lex->column = 0;
        tok->category = category_error;
        tok->type = token_unclosed_raw_string;
        tok->text = start;
        tok->length = static_cast<size_t>(last - start);
        return sexp_badsyntax;
    }

    int lexer_match_bytevector(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        uint_least32_t ch;

        ch = lexer_get_char(lex);
        if (ch == '8') {
            ch = lexer_get_char(lex);
            if (ch == '(') {
                tok->category = category_open;
                tok->type = token_bytevector_open;
                tok->list_type = list_parenthesis;
                return sexp_success;
            }
            else if (ch == '[') {
                tok->category = category_open;
                tok->type = token_bytevector_open;
                tok->list_type = list_bracket;
                return sexp_success;
            }
            else if (ch == '{') {
                tok->category = category_open;
                tok->type = token_bytevector_open;
                tok->list_type = list_brace;
                return sexp_success;
            }
        }

        lexer_read_until_delimiter(lex, ch);
        tok->category = category_error;
        tok->type = token_invalid_syntax;
        return sexp_badsyntax;
    }

    uint_least32_t lexer_match_number_part(
        lexer* UPRESTRICT lex,
        uint_least32_t first_ch,
        unsigned int* UPRESTRICT number_type,
        token_number_info* UPRESTRICT number_info
    )
    noexcept {
        uint_least32_t ch = first_ch;
        unsigned int type, precision;
        int base;

        // read in optional number sign
        if ((ch == '+') || (ch == '-')) {
            ch = lexer_get_char(lex);
            if ((ch == 'i') && lexer_read_token_part(lex, "nf.0", 4, &ch)) {
                *number_type = token_real;
                return ch;
            }
            else if ((ch == 'n') && lexer_read_token_part(lex, "an.0", 4, &ch)) {
                *number_type = token_real;
                return ch;
            }
        }

        // read in digits before decimal place
        base = number_info->radix;
        type = token_none;
        if (isudigit(ch, base)) {
            ch = lexer_read_digits_part(lex, base);
            type = token_integer;

            // check for ratio divisor
            if (ch == '/') {
                // match integer divisor
                ch = lexer_get_char(lex);
                if (!isudigit(ch, base)) {
                    *number_type = token_identifier;
                    return ch;
                }

                ch = lexer_read_digits_part(lex, base);
                type = token_rational;
            }
        }

        // only decimal numbers can have a radix point or exponent
        if (base == 10) {
            // check for decimal place or ratio divisor
            if ((type != token_rational) && (ch == '.')) {
                // match fraction digits
                ch = lexer_read_digits_part(lex, 10);
                type = token_real;
            }

            if (type != token_identifier) {
                // check for exponent
                switch (ch) {
                case 'e': case 'E':
                case 'd': case 'D':
                    precision = precision_double;
                    break;
                case 's': case 'S':
                    precision = precision_half;
                    break;
                case 'f': case 'F':
                    precision = precision_single;
                    break;
                case 'l': case 'L':
                    precision = precision_extended;
                    break;
                default:
                    precision = precision_unspecified;
                    break;
                }

                if (precision != precision_unspecified) {
                    // match exponent sign
                    ch = lexer_get_char(lex);
                    if ((ch == '+') || (ch == '-')) {
                        ch = lexer_get_char(lex);
                    }

                    // match exponent digits
                    if (!isudigit(ch)) {
                        *number_type = token_identifier;
                        return ch;
                    }

                    ch = lexer_read_digits_part(lex, 10);
                    type = token_real;

                    // save precision if it's greater than current
                    if (number_info->precision < precision) {
                        number_info->precision = precision;
                    }
                }
            }
        }

        *number_type = type;
        return ch;
    }

    int lexer_match_number(
        lexer* UPRESTRICT lex,
        token* UPRESTRICT tok,
        token_number_info number_info,
        uint_least32_t first_ch
    )
    noexcept {
        uint_least32_t ch = first_ch;
        unsigned int type, imaginary_type;
        bool at_style, i_style;

        // match real part
        ch = lexer_match_number_part(lex, ch, &type, &number_info);
        if (type == token_identifier) {
            lexer_unget_char(lex, ch);
            return lexer_match_identifier(lex, tok);
        }

        // match imaginary part
        at_style = false;
        if (ch == '@') {
            at_style = true;
            ch = lexer_get_char(lex);
        }

        if (at_style || (ch == '+') || (ch == '-')) {
            ch = lexer_match_number_part(lex, ch, &imaginary_type, &number_info);
            if (imaginary_type == token_identifier) {
                lexer_unget_char(lex, ch);
                return lexer_match_identifier(lex, tok);
            }
            type = token_identifier;
        }
        
        i_style = ((ch & 0xFFFFFF5F) == 'I') ? true : false;
        if (!at_style && i_style) {
            type = token_complex;
            ch = lexer_get_char(lex);
        }
        else if (at_style && !i_style) {
            type = token_complex;
        }

        // match delimiter, completing the number token
        if (!lexer_fast_is_delimiter(ch)) {
            return lexer_match_identifier(lex, tok);
        }

        lexer_unget_char(lex, ch);

        // save number traits
        tok->category = category_number;
        tok->type = type;
        tok->number_info = number_info;
        return sexp_success;
    }

    int lexer_match_dot(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        uint_least32_t ch = lexer_get_char(lex);
        lexer_unget_char(lex, ch);

        if ((ch == '#') || lexer_fast_is_delimiter(ch)) {
            tok->category = category_infix;
            tok->type = token_dot;
            return sexp_success;
        }

        return lexer_match_number(lex, tok, default_number_info, '.');
    }

    int lexer_match_exactness_or_number(lexer* UPRESTRICT lex, token* UPRESTRICT tok, token_number_info number_info) noexcept {
        uint_least32_t ch;
        int retval;

        ch = lexer_get_char(lex);
        if (ch == '#') {
            ch = lexer_get_char(lex);
            switch (ch) {
            case 'e': case 'E':
                number_info.prefix_length += 2;
                number_info.exactness = exactness_exact;
                break;
            case 'i': case 'I':
                number_info.prefix_length += 2;
                number_info.exactness = exactness_inexact;
                break;
            default:
                tok->category = category_error;
                tok->type = token_invalid_exactness;
                return sexp_badsyntax;
            }
            ch = lexer_get_char(lex);
        }
        
        retval = lexer_match_number(lex, tok, number_info, ch);
        if (retval != sexp_success) {
            return retval;
        }

        if (tok->category != category_number) {
            tok->category = category_error;
            tok->type = token_invalid_number;
            return sexp_badsyntax;
        }

        return sexp_success;
    }

    int lexer_match_radix_or_number(lexer* UPRESTRICT lex, token* UPRESTRICT tok, token_number_info number_info) noexcept {
        uint_least32_t ch;
        int retval;

        ch = lexer_get_char(lex);
        if (ch == '#') {
            ch = lexer_get_char(lex);
            switch (ch) {
            case 'b': case 'B':
                number_info.prefix_length += 2;
                number_info.radix = 2;
                break;
            case 'o': case 'O':
                number_info.prefix_length += 2;
                number_info.radix = 8;
                break;
            case 'd': case 'D':
                number_info.prefix_length += 2;
                number_info.radix = 10;
                break;
            case 'x': case 'X':
                number_info.prefix_length += 2;
                number_info.radix = 16;
                break;
            default:
                tok->category = category_error;
                tok->type = token_invalid_radix;
                return sexp_badsyntax;
            }
            ch = lexer_get_char(lex);
        }

        retval = lexer_match_number(lex, tok, number_info, ch);
        if (retval != sexp_success) {
            return retval;
        }

        if (tok->category != category_number) {
            tok->category = category_error;
            tok->type = token_invalid_number;
            return sexp_badsyntax;
        }

        return sexp_success;
    }

    int lexer_match_label(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        uint_least32_t ch;
        
        ch = lexer_read_digits_part(lex, 10);
        if (ch == '#') {
            tok->category = category_label;
            tok->type = token_get_label;
            return sexp_success;
        }
        else if (ch == '=') {
            tok->category = category_label;
            tok->type = token_set_label;
            return sexp_success;
        }

        lexer_read_until_delimiter(lex, ch);
        tok->category = category_error;
        tok->type = token_invalid_syntax;
        return sexp_badsyntax;
    }

    int lexer_match_dispatch(lexer* UPRESTRICT lex, token* UPRESTRICT tok, char const* start) noexcept {
        int retval = sexp_success;
        uint_least32_t ch;

        ch = lexer_get_char(lex);
        switch (ch) {
        case ';': case '_':
            tok->category = category_prefix;
            tok->type = token_ignore;
            break;
        case '(':
            tok->category = category_open;
            tok->type = token_vector_open;
            tok->list_type = list_parenthesis;
            break;
        case '[':
            tok->category = category_open;
            tok->type = token_vector_open;
            tok->list_type = list_bracket;
            break;
        case '{':
            tok->category = category_open;
            tok->type = token_vector_open;
            tok->list_type = list_brace;
            break;
        case '\'':
            tok->category = category_prefix;
            tok->type = token_syntax;
            break;
        case '`':
            tok->category = category_prefix;
            tok->type = token_quasisyntax;
            break;
        case ',':
            tok->category = category_prefix;
            tok->type = lexer_read_unquote(lex, token_unsyntax, token_unsyntax_splicing);
            break;
        case '|':
            retval = lexer_match_block_comment(lex, tok);
            break;
        case '!':
            retval = lexer_match_line_comment(lex, tok);
            break;
        case '\\':
            retval = lexer_match_character(lex, tok);
            break;
        case '@':
            retval = lexer_match_raw_string(lex, tok, start);
            return retval;
        case 'u': case 'U':
            retval = lexer_match_bytevector(lex, tok);
            break;
        case 't': case 'T':
            retval = lexer_match_boolean(lex, tok, "RUE", 3, true);
            break;
        case 'f': case 'F':
            retval = lexer_match_boolean(lex, tok, "ALSE", 4, false);
            break;
        case 'e': case 'E':
            retval = lexer_match_radix_or_number(lex, tok, exact_number_info);
            break;
        case 'i': case 'I':
            retval = lexer_match_radix_or_number(lex, tok, inexact_number_info);
            break;
        case 'b': case 'B':
            retval = lexer_match_exactness_or_number(lex, tok, binary_number_info);
            break;
        case 'o': case 'O':
            retval = lexer_match_exactness_or_number(lex, tok, octal_number_info);
            break;
        case 'd': case 'D':
            retval = lexer_match_exactness_or_number(lex, tok, decimal_number_info);
            break;
        case 'x': case 'X':
            retval = lexer_match_exactness_or_number(lex, tok, hexadecimal_number_info);
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            retval = lexer_match_label(lex, tok);
            break;
        default:
            lexer_read_until_delimiter(lex, ch);
            tok->category = category_error;
            tok->type = token_invalid_syntax;
            retval = sexp_badsyntax;
            break;
        }

        tok->text = start;
        tok->length = static_cast<size_t>(lex->cursor - start);
        return retval;
    }
}}}

namespace up { namespace sexp
{
    LIBUPCOREAPI UPPURE
    bool lexer_is_delimiter(uint_least32_t uc) noexcept {
        return ((uc < 0x80) && ascii_delimiter_table[uc]) || !isugraph(uc);
    }

    LIBUPCOREAPI
    int lexer_construct(lexer* UPRESTRICT lex, char const* text, size_t length) noexcept {
        if (!lex || (!text && length)) {
            return sexp_badarg;
        }

        lex->first = text;
        lex->last = text + length;
        lex->cursor = text;
        lex->prev_cursor = nullptr;
        lex->column = 0;
        lex->line = 0;
        return sexp_success;
    }

    LIBUPCOREAPI
    int lexer_rewind(lexer* lex) noexcept {
        if (!lex || (lex->last < lex->first)) {
            return sexp_badarg;
        }

        lex->cursor = lex->first;
        lex->prev_cursor = nullptr;
        lex->column = 0;
        lex->line = 0;
        return sexp_success;
    }

    LIBUPCOREAPI
    int lexer_peek(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        if (!lex) {
            return sexp_badarg;
        }

        lexer const temp = *lex;
        int const retval = lexer_read(lex, tok);
        *lex = temp;
        return retval;
    }

    LIBUPCOREAPI
    int lexer_read(lexer* UPRESTRICT lex, token* UPRESTRICT tok) noexcept {
        int retval = sexp_success;
        uintmax_t column, line;
        char const* start;
        uint_least32_t ch;
        
        if (!lex || !tok) {
            return sexp_badarg;
        }

        for (;;) {
            start = lex->cursor;
            column = lex->column;
            line = lex->line;

            if (start >= lex->last) {
                tok->category = category_none;
                tok->type = token_none;
                retval = sexp_eof;
                goto finalize;
            }

            ++lex->column;
            lex->prev_cursor = start;
            ch = *reinterpret_cast<unsigned char const*>(lex->cursor++);

            switch (ch) {
            case '\0': case 0x01: case 0x02:
            case 0x03: case 0x04: case 0x05:
            case 0x06: case '\a': case '\b':
            case '\t': case 0x0E: case 0x0F:
            case 0x10: case 0x11: case 0x12:
            case 0x13: case 0x14: case 0x15:
            case 0x16: case 0x17: case 0x18:
            case 0x19: case 0x1A: case 0x1B:
            case 0x1C: case 0x1D: case 0x1E:
            case 0x1F: case  ' ': case 0x7F:
                break;
            case '\r':
                lex->column = 0;
                ++lex->line;
                if ((lex->cursor < lex->last) && (*lex->cursor == '\n')) {
                    ++lex->cursor;
                }
                break;
            case '\n': case '\v': case '\f':
                lex->column = 0;
                ++lex->line;
                break;
            case '(': 
                tok->category = category_open;
                tok->type = token_list_open;
                tok->list_type = list_parenthesis;
                goto finalize;
            case '[':
                tok->category = category_open;
                tok->type = token_list_open;
                tok->list_type = list_bracket;
                goto finalize;
            case '{':
                tok->category = category_open;
                tok->type = token_list_open;
                tok->list_type = list_brace;
                goto finalize;
            case ')':
                tok->category = category_close;
                tok->type = token_list_close;
                tok->list_type = list_parenthesis;
                goto finalize;
            case ']':
                tok->category = category_close;
                tok->type = token_list_close;
                tok->list_type = list_bracket;
                goto finalize;
            case '}':
                tok->category = category_close;
                tok->type = token_list_close;
                tok->list_type = list_brace;
                goto finalize;
            case '\'':
                tok->category = category_prefix;
                tok->type = token_quote;
                goto finalize;
            case '`':
                tok->category = category_prefix;
                tok->type = token_quasiquote;
                goto finalize;
            case ',':
                tok->category = category_prefix;
                tok->type = lexer_read_unquote(lex, token_unquote, token_unquote_splicing);
                goto finalize;
            case '#':
                retval = lexer_match_dispatch(lex, tok, start);
                goto done;
            case '"':
                retval = lexer_match_string(lex, tok, start);
                goto done;
            case ';':
                retval = lexer_match_line_comment(lex, tok);
                goto finalize;
            case '|':
                retval = lexer_match_escaped_identifier(lex, tok);
                goto finalize;
            case '!':  case '$':  case '%':
            case '&':  case '*':  case '/':
            case ':':  case '<':  case '=':
            case '>':  case '?':  case '@':
            case 'A':  case 'B':  case 'C':
            case 'D':  case 'E':  case 'F':
            case 'G':  case 'H':  case 'I':
            case 'J':  case 'K':  case 'L':
            case 'M':  case 'N':  case 'O':
            case 'P':  case 'Q':  case 'R':
            case 'S':  case 'T':  case 'U':
            case 'V':  case 'W':  case 'X':
            case 'Y':  case 'Z': case '\\':
            case '^':  case '_':  case 'a':
            case 'b':  case 'c':  case 'd':
            case 'e':  case 'f':  case 'g':
            case 'h':  case 'i':  case 'j':
            case 'k':  case 'l':  case 'm':
            case 'n':  case 'o':  case 'p':
            case 'q':  case 'r':  case 's':
            case 't':  case 'u':  case 'v':
            case 'w':  case 'x':  case 'y':
            case 'z':  case '~':
                retval = lexer_match_identifier(lex, tok);
                goto finalize;
            case '+':  case '-':
            case '0':  case '1':  case '2':  case '3':  case '4':
            case '5':  case '6':  case '7':  case '8':  case '9':
                retval = lexer_match_number(lex, tok, default_number_info, ch);
                goto finalize;
            case '.':
                retval = lexer_match_dot(lex, tok);
                goto finalize;
            default:
                char32_t ch32;
                size_t const length = lex->last - lex->prev_cursor;
                int const octet_count = u8tou32(&ch32, lex->prev_cursor, length);
                if (octet_count > 0) {
                    lex->cursor = lex->prev_cursor + octet_count;
                }
                else {
                    lex->cursor = u8snerr(lex->prev_cursor, length);
                    if (!lex->cursor) {
                        lex->cursor = lex->last;
                    }
                }
                if ((ch32 == 0x0085) || ((ch32 | 0x0001) == 0x2029)) {
                    lex->column = 0;
                    ++lex->line;
                }
                else if (isugraph(ch32)) {
                    retval = lexer_match_identifier(lex, tok);
                    goto finalize;
                }
                break;
            }
        }

    finalize:

        tok->text = start;
        tok->length = static_cast<size_t>(lex->cursor - start);

    done:

        tok->line = line;
        tok->column = column;
        return retval;
    }
}}
