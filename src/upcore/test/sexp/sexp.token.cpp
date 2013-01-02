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

namespace sexp_token
{
    namespace
    {
        up::sexp::token make_token(char const* text) noexcept {
            up::sexp::lexer lex;
            up::sexp::token tok;
            int retval;

            retval = up::sexp::lexer_construct(&lex, text, up::strlen(text));
            require(retval == up::sexp::sexp_success);

            retval = up::sexp::lexer_read(&lex, &tok);
            require(retval == up::sexp::sexp_success);

            return tok;
        }
    }
    
    UP_TEST_CASE(parse_integer) {
        constexpr size_t num_tokens = 23;
        up::sexp::token tokens[num_tokens];
        int retval, expected_retval;
        
        char const* const input[num_tokens] =
        {
            "0", "+0", "-0", "123", "+123", "-123", "16384", "-19554",
            "899156294", "-899156294", "#b1001", "#b+1101", "#B-1010", "#o731", "#O+155", "#o-40716235",
            "#x8a", "#xFF", "#x-ef", "#X7FCD0051", "#X+FfCdb469", "21258912512512515125", "-21258912512512515125"
        };

        for (size_t i = 0; i < num_tokens; ++i) {
            tokens[i] = make_token(input[i]);
        }

        int32_t const expected_i32[num_tokens] =
        {
            0, 0, 0, 123, 123, -123, 16384, -19554,
            899156294, -899156294, 9, 13, -10, 473, 109, -8625309,
            138, 255, -239, 2144141393, INT32_MAX, INT32_MAX, INT32_MIN
        };

        int32_t i32;

        for (size_t i = 0; i < num_tokens; ++i) {
            expected_retval = (i < (num_tokens - 3)) ? up::sexp::sexp_success : up::sexp::sexp_overflow;
            retval = up::sexp::token_parse_int32(&tokens[i], &i32);
            require((retval == expected_retval) && (i32 == expected_i32[i]));
        }
    }
}
