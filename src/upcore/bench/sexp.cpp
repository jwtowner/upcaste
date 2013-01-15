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
#include <up/region.hpp>
#include <up/test.hpp>
#include <up/utility.hpp>

namespace sexp
{
    class sexp_context
	{
        up::sexp::parser par;
        up::sexp::lexer lex;

    public:

		sexp_context(char const* filename, up::allocator* alloc) {
            int retval = up::sexp::parser_construct(&par, alloc);
            verify(retval == up::sexp::sexp_success);

            retval = up::sexp::parser_load_file(&par, filename);
            verify(retval == up::sexp::sexp_success);

            lex = par.lex;
        }
        
		~sexp_context() {
            int retval = up::sexp::parser_destruct(&par);
            verify(retval == up::sexp::sexp_success);
        }

		up::sexp::parser* parser() {
            return &par;
        }
        
        up::sexp::lexer* lexer() {
            return &lex;
        }
        
        void setup() {
        }

        void teardown() {
            int retval = up::sexp::parser_rewind(&par);
            verify(retval == up::sexp::sexp_success);
            retval = up::sexp::lexer_rewind(&lex);
            verify(retval == up::sexp::sexp_success);
        }
	};

    UPNOINLINE
    void strlen_bench(sexp_context& ctx) {
        up::sexp::lexer* lex = ctx.lexer();
        size_t length = up::strnlen(lex->first, lex->last - lex->first);
        up::sink_dependency(length);
    }

    UPNOINLINE
    void u8sntou32slen_bench(sexp_context& ctx) {
        up::sexp::lexer* lex = ctx.lexer();
        size_t length = up::u8sntou32slen(lex->first, lex->last - lex->first);
        up::sink_dependency(length);
    }

    UPNOINLINE
    void u8tou32_bench(sexp_context& ctx) {
        up::sexp::lexer* lex = ctx.lexer();
        char const* char_ptr = lex->first;
        char const* end_ptr = lex->last;
        size_t length = 0;
        char32_t u32;
        int retval;

        for (;;) {
            retval = up::u8tou32(&u32, char_ptr, end_ptr - char_ptr);
            if (retval == 0) {
                break;
            }
            char_ptr += (retval > 0) ? retval : 1;
            ++length;
        }
        
        up::sink_dependency(u32);
        up::sink_dependency(length);
    }

    UPNOINLINE
    void lexer_bench(sexp_context& ctx) {
        up::sexp::lexer* lex = ctx.lexer();
        up::sexp::token tok;
        int retval;

        do {
            retval = up::sexp::lexer_read(lex, &tok);
            verify(retval >= up::sexp::sexp_badsyntax);
        }
        while (retval != up::sexp::sexp_eof);

        up::sink_dependency(tok);
        up::sink_dependency(retval);
    }

    UPNOINLINE
    void parser_bench(sexp_context& ctx) {
        up::sexp::parser* par = ctx.parser();
        up::sexp::token tok;
        int retval;

        do {
            retval = up::sexp::parser_read(par, &tok);
            verify(retval >= up::sexp::sexp_badsyntax);
        }
        while (retval != up::sexp::sexp_eof);

        up::sink_dependency(tok);
        up::sink_dependency(retval);
    }

    UP_TEST_BENCHMARK(modela) {
        sexp_context context("../../share/upcore/modela.sexp", nullptr);
        UP_TEST_RUN_BENCHMARK("strlen", 0, 200, &strlen_bench, context);
        UP_TEST_RUN_BENCHMARK("u8sntou32slen", 0, 200, &u8sntou32slen_bench, context);
        UP_TEST_RUN_BENCHMARK("u8tou32", 0, 200, &u8tou32_bench, context);
        UP_TEST_RUN_BENCHMARK("lexer_read", 0, 200, &lexer_bench, context);
        UP_TEST_RUN_BENCHMARK("parser_read", 0, 200, &parser_bench, context);
    }

    UP_TEST_BENCHMARK(modela_with_heap_region_allocator) {
        up::heap_region* region = up::heap_region_create();
        verify(region);

        {
            sexp_context context("../../share/upcore/modela.sexp", up::heap_region_allocator(region));
            strlen_bench(context); // make sure data is in cache
            UP_TEST_RUN_BENCHMARK("parser_read", 0, 200, &parser_bench, context);
        }

        up::heap_region_destroy(region);
    }

    UP_TEST_BENCHMARK(modele) {
        sexp_context context("../../share/upcore/modele.sexp", nullptr);
        UP_TEST_RUN_BENCHMARK("strlen", 0, 200, &strlen_bench, context);
        UP_TEST_RUN_BENCHMARK("u8sntou32slen", 0, 200, &u8sntou32slen_bench, context);
        UP_TEST_RUN_BENCHMARK("u8tou32", 0, 200, &u8tou32_bench, context);
        UP_TEST_RUN_BENCHMARK("lexer_read", 0, 200, &lexer_bench, context);
        UP_TEST_RUN_BENCHMARK("parser_read", 0, 200, &parser_bench, context);
    }

    UP_TEST_BENCHMARK(modele_with_heap_region_allocator) {
        up::heap_region* region = up::heap_region_create();
        verify(region);

        {
            sexp_context context("../../share/upcore/modele.sexp", up::heap_region_allocator(region));
            strlen_bench(context); // make sure data is in cache
            UP_TEST_RUN_BENCHMARK("parser_read", 0, 200, &parser_bench, context);
        }

        up::heap_region_destroy(region);
    }
}
