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
#include <up/cuchar.hpp>
#include <up/test.hpp>

namespace sexp_lexer
{
    UP_TEST_CASE(is_delimiter) {
        require(up::sexp::lexer_is_delimiter(up::ueof));
        require(up::sexp::lexer_is_delimiter('\t'));
        require(up::sexp::lexer_is_delimiter('\v'));
        require(up::sexp::lexer_is_delimiter('\r'));
        require(up::sexp::lexer_is_delimiter('\n'));
        require(up::sexp::lexer_is_delimiter('\f'));
        require(up::sexp::lexer_is_delimiter(' '));
        require(up::sexp::lexer_is_delimiter('('));
        require(up::sexp::lexer_is_delimiter(')'));
        require(up::sexp::lexer_is_delimiter('['));
        require(up::sexp::lexer_is_delimiter(']'));
        require(up::sexp::lexer_is_delimiter('{'));
        require(up::sexp::lexer_is_delimiter('}'));
        require(up::sexp::lexer_is_delimiter('|'));
        require(up::sexp::lexer_is_delimiter('\"'));
        require(up::sexp::lexer_is_delimiter('\''));
        require(up::sexp::lexer_is_delimiter('`'));
        require(up::sexp::lexer_is_delimiter(','));
        require(up::sexp::lexer_is_delimiter(';'));
        require(!up::sexp::lexer_is_delimiter('#'));
        require(!up::sexp::lexer_is_delimiter('~'));
        require(!up::sexp::lexer_is_delimiter('@'));
    }

    UP_TEST_CASE(construct) {
        up::sexp::lexer lex; 
        int result;

        result = up::sexp::lexer_construct(nullptr, nullptr, 0);
        require(result == up::sexp::sexp_badarg);

        result = up::sexp::lexer_construct(&lex, nullptr, 5);
        require(result == up::sexp::sexp_badarg);

        result = up::sexp::lexer_construct(&lex, nullptr, 0);
        require((result == up::sexp::sexp_success) && !lex.first && !lex.last);

        result = up::sexp::lexer_construct(&lex, "hello", 5);
        require((result == up::sexp::sexp_success) && !up::strcmp(lex.first, "hello"));
    }

    UP_TEST_CASE(read_error) {
        char const* text = "atom";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(nullptr, nullptr);
        require(result == up::sexp::sexp_badarg);

        result = up::sexp::lexer_read(&lex, nullptr);
        require(result == up::sexp::sexp_badarg);

        result = up::sexp::lexer_read(nullptr, &tok);
        require(result == up::sexp::sexp_badarg);
    }

    UP_TEST_CASE(read_list) {
        char const* const text = "([ {} ])";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "(", tok.length));
        require((tok.type == up::sexp::token_list_open) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "[", tok.length));
        require((tok.type == up::sexp::token_list_open) && (tok.list_type == up::sexp::list_bracket));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "{", tok.length));
        require((tok.type == up::sexp::token_list_open) && (tok.list_type == up::sexp::list_brace));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "}", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_brace));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "]", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_bracket));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, ")", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_vector) {
        char const* const text = "#(#[ #{} ])";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#(", tok.length));
        require((tok.type == up::sexp::token_vector_open) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#[", tok.length));
        require((tok.type == up::sexp::token_vector_open) && (tok.list_type == up::sexp::list_bracket));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#{", tok.length));
        require((tok.type == up::sexp::token_vector_open) && (tok.list_type == up::sexp::list_brace));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "}", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_brace));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "]", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_bracket));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, ")", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_bytevector) {
        char const* const text = "#u8(#u8[ #U8{ #u_( #U8} ])";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#u8(", tok.length));
        require((tok.type == up::sexp::token_bytevector_open) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#u8[", tok.length));
        require((tok.type == up::sexp::token_bytevector_open) && (tok.list_type == up::sexp::list_bracket));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#U8{", tok.length));
        require((tok.type == up::sexp::token_bytevector_open) && (tok.list_type == up::sexp::list_brace));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && !up::strncmp(tok.text, "#u_", tok.length));
        require(tok.type == up::sexp::token_invalid_syntax);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "(", tok.length));
        require((tok.type == up::sexp::token_list_open) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && !up::strncmp(tok.text, "#U8", tok.length));
        require(tok.type == up::sexp::token_invalid_syntax);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "}", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_brace));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "]", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_bracket));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, ")", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }


    UP_TEST_CASE(read_label) {
        char const* const text = "#1234567890='(foo . #1234567890#)";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#1234567890=", tok.length));
        require(tok.type == up::sexp::token_set_label);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "'", tok.length));
        require(tok.type == up::sexp::token_quote);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "(", tok.length));
        require((tok.type == up::sexp::token_list_open) && (tok.list_type == up::sexp::list_parenthesis));


        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "foo", tok.length));
        require(tok.type == up::sexp::token_identifier);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, ".", tok.length));
        require(tok.type == up::sexp::token_dot);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, "#1234567890#", tok.length));
        require(tok.type == up::sexp::token_get_label);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && !up::strncmp(tok.text, ")", tok.length));
        require((tok.type == up::sexp::token_list_close) && (tok.list_type == up::sexp::list_parenthesis));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_identifier) {
        char const* const text = "atom +a:t_om -at$o#m+ @AT\\OM?\n:atom :-at#o:m";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "atom", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "+a:t_om", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "-at$o#m+", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "@AT\\OM?", tok.length));

        require(lex.line == 0);
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, ":atom", tok.length));
        require(lex.line == 1);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, ":-at#o:m", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_escaped_identifier) {
        char const* const text = "|| |\x045;| |a\"'`,,@([{}])# \f\v\tb\nc\r\nd| | ab ";
        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_escaped_identifier) && !up::strncmp(tok.text, "||", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_escaped_identifier) && !up::strncmp(tok.text, "|\x045;|", tok.length));

        require(lex.line == 0);
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_escaped_identifier) && !up::strncmp(tok.text, "|a\"'`,,@([{}])# \f\v\tb\nc\r\nd|", tok.length));
        require(lex.line == 4);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_unclosed_identifier) && !up::strncmp(tok.text, "| ab ", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_character) {
        char const* text =
            "#\\a #\\b #\\c #\\x #\\ #\\; #\\# #\\\\ #\\alarm #\\backspace #\\delETE "
            "#\\escape #\\NEWLINE #\\null #\\page #\\return #\\space #\\tab #\\vtab #\\x85 #\\x0190 "
            "#\\aa #\\newline9 #\\x0124G #\\x200101 #\\x100100014400";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 'a'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 'b'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 'c'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 'x'));
        
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == ' '));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == ';'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '#'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\\'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\a'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\b'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 0x007F));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 0x001B));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\n'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\0'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\f'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\r'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == ' '));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\t'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == '\v'));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 0x0085));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_character) && (tok.char_value == 0x0190));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_character));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_character));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_character));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_character));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_character));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_string) {
        char const* text = "\"data\" \"da\\\"ta\" \"data\n\v\f\r\ndata\" \"data ";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_string) && !up::strncmp(tok.text, "\"data\"", tok.length));
        
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_escaped_string) && !up::strncmp(tok.text, "\"da\\\"ta\"", tok.length));

        require(lex.line == 0);
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_string) && !up::strncmp(tok.text, "\"data\n\v\f\r\ndata\"", tok.length));
        require(lex.line == 4);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_unclosed_string) && !up::strncmp(tok.text, "\"data ", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_raw_string) {
        char const* text = "#@\nda\"t#@a@#\r\n #@tag\r\n\"da\"@tatag 123\ntag@#\n #@\ndata@# ";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_raw_string) && !up::strncmp(tok.text, "#@\nda\"t#@a@#\r\n", tok.length));
        require((tok.raw_info.start_length == 3) && (tok.raw_info.end_length == 4));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_raw_string) && !up::strncmp(tok.text, "#@tag\r\n\"da\"@tatag 123\ntag@#\n", tok.length));
        require((tok.raw_info.start_length == 7) && (tok.raw_info.end_length == 6));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_unclosed_raw_string) && !up::strncmp(tok.text, "#@\ndata@# ", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_true) {
        char const* text = "#t #T #true #TRUE";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == true) && !up::strncmp(tok.text, "#t", tok.length));
        
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == true) && !up::strncmp(tok.text, "#T", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == true) && !up::strncmp(tok.text, "#true", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == true) && !up::strncmp(tok.text, "#TRUE", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_false) {
        char const* text = "#f #F #false #FALSE";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == false) && !up::strncmp(tok.text, "#f", tok.length));
        
        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == false) && !up::strncmp(tok.text, "#F", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == false) && !up::strncmp(tok.text, "#false", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_boolean) && (tok.bool_value == false) && !up::strncmp(tok.text, "#FALSE", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_integer) {
        char const* text =
            "1234567890 +1234567890 -1234567890 0 1 001 +123a "
            "#d1234567890 #d+1234567890 #d-1234567890 #d0 #d1 #D001 #d+123a "
            "#b0101 #b+0 #b-1 #B+1203 #o0145 #o+36 #O-12 #o+6578";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "1234567890", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "+1234567890", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "-1234567890", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "1", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "001", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "+123a", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#d1234567890", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#d+1234567890", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#d-1234567890", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#d0", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#d1", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#D001", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_number) && !up::strncmp(tok.text, "#d+123a", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#b0101", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 2));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#b+0", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 2));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#b-1", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 2));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_number) && !up::strncmp(tok.text, "#B+1203", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#o0145", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 8));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#o+36", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 8));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_integer) && !up::strncmp(tok.text, "#O-12", tok.length));
        require((tok.number_info.prefix_length == 2) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 8));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_badsyntax) && (tok.type == up::sexp::token_invalid_number) && !up::strncmp(tok.text, "#o+6578", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_ratio) {
        char const* text = "12345/67890 +1234/567890 -123456/7890 0/1 001/2 1/a";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_rational) && !up::strncmp(tok.text, "12345/67890", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_rational) && !up::strncmp(tok.text, "+1234/567890", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_rational) && !up::strncmp(tok.text, "-123456/7890", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_rational) && !up::strncmp(tok.text, "0/1", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_rational) && !up::strncmp(tok.text, "001/2", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "1/a", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_real) {
        char const* text =
            "1234567890.01234 +1234567890.56789 -1234567890.25 0.1 1.0 001.001 "
            "1234.5e44 1234.5E+44 1234.5e-123456789 1234e44 1234E+44 1234e-123456789 "
            ".01234 +.56789 -.25 .1 +.0 .001 +123. 12/33E-4 +inf.0 -inf.0 +nan.0 -nan.0 "
            "-nan.0e +inf.1 21e 21e+ 1234e44.55";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234567890.01234", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "+1234567890.56789", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "-1234567890.25", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "0.1", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1.0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "001.001", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234.5e44", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234.5E+44", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234.5e-123456789", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234e44", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234E+44", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "1234e-123456789", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, ".01234", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "+.56789", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "-.25", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, ".1", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "+.0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, ".001", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "+123.", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "12/33E-4", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "+inf.0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "-inf.0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "+nan.0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_real) && !up::strncmp(tok.text, "-nan.0", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "-nan.0e", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "+inf.1", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "21e", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "21e+", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_identifier) && !up::strncmp(tok.text, "1234e44.55", tok.length));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }

    UP_TEST_CASE(read_complex) {
        char const* text =
            "+i -i +4i -99i +44/3i 1+i 1-i 1.5e+5+44.0e-4i .5-44.0e-4i +4-44/3i 55e4-.4i ";

        up::sexp::lexer lex;
        up::sexp::token tok;
        int result;

        result = up::sexp::lexer_construct(&lex, text, up::strlen(text));
        require(result == up::sexp::sexp_success);

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "+i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "-i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "+4i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "-99i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "+44/3i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "1+i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "1-i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "1.5e+5+44.0e-4i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, ".5-44.0e-4i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "+4-44/3i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require((result == up::sexp::sexp_success) && (tok.type == up::sexp::token_complex) && !up::strncmp(tok.text, "55e4-.4i", tok.length));
        require((tok.number_info.prefix_length == 0) && (tok.number_info.exactness == up::sexp::exactness_unspecified) && (tok.number_info.radix == 10));

        result = up::sexp::lexer_read(&lex, &tok);
        require(result == up::sexp::sexp_eof);
    }
}
