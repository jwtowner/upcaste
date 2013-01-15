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
#include <up/cstring.hpp>
#include <up/test.hpp>

namespace
{

char const* sample =
"#! simple sample program                                           \n"
"                                                                   \n"
"(define [palindrome? x]                                            \n"
"  (define (check 'left 'right)                                     \n"
"    (if (>= 'left 'right)                                          \v"
"        #true                                                    \r\n"
"        (and (char=? (string-ref x 'left) (string-ref x 'right))   \n"
"             (check (add1 'left) (sub1 'right)))))                 \n"
"             #| (check (add1 'right)                               \n"
"                       (sub1 'left))))) |#                         \n"
"  (check 0 (sub1 (string-length x))))                              \n"
"                                                                   \n"
"; test our palindrome function                                     \n"
"(let [(arg (car (command-line-arguments)))]                        \n"
"  (display                                                         \n"
"   (string-append arg                                              \n"
"    (if [palindrome? arg]                                          \n"
"     \" is a palindrome\\n\"                                       \n"
"     \" isn't a palindrome\\n\"))))                                \n";

char const* invalid_sample =
"(define matrix?                            \n"
"  (lambda [x)]                             \n" // <-- closing list mismatch
"    (and (vector? x)                       \n"
"         (> (vector-length x) 0)           \n"
"         (vector? (vector-ref x 0)))))     \n";

char const* invalid_sample2 =
"(define matrix?                            \n"
"  (lambda (x)                              \n"
"    (and (vector? x)                       \n"
"         (> (vector-length x) 0)           \n"
"         (vector? (vector-ref x 0))))      \n"; // <-- premature end of file

}

namespace sexp_parser
{
    UP_TEST_CASE(construct_destruct) {
        up::sexp::parser par;
        int retval;

        retval = up::sexp::parser_construct(&par, nullptr);
        require(retval == up::sexp::sexp_success);
        require(par.alloc == up::malloc_allocator::instance());

        retval = up::sexp::parser_destruct(&par);
        require(retval == up::sexp::sexp_success);
    }

    UP_TEST_CASE(load_and_verify_memory) {
        up::sexp::parser par;
        int retval;

        retval = up::sexp::parser_construct(&par);
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_load_memory(&par, "sample.scm", sample, up::strlen(sample));
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_skip_end(&par);
        require(retval == up::sexp::sexp_success);
        require(!up::sexp::parser_has_error_occurred(&par));

        retval = up::sexp::parser_destruct(&par);
        require(retval == up::sexp::sexp_success);
    }

    UP_TEST_CASE(load_and_verify_invalid_memory) {
        up::sexp::parser par;
        int retval;

        retval = up::sexp::parser_construct(&par);
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_load_memory(&par, "sample.scm", invalid_sample, up::strlen(invalid_sample));
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_skip_end(&par);
        require(retval == up::sexp::sexp_success);
        require(up::sexp::parser_has_error_occurred(&par));

        retval = up::sexp::parser_load_memory(&par, "sample2.scm", invalid_sample2, up::strlen(invalid_sample2));
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_skip_end(&par);
        require(retval == up::sexp::sexp_success);
        require(up::sexp::parser_has_error_occurred(&par));

        retval = up::sexp::parser_destruct(&par);
        require(retval == up::sexp::sexp_success);
    }

    UP_TEST_CASE(load_and_verify_small_file) {
        up::sexp::parser par;
        int retval;

        retval = up::sexp::parser_construct(&par, up::malloc_allocator::instance());
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_load_file(&par, "../../share/upcore/modela.sexp");
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_skip_end(&par);
        require(retval == up::sexp::sexp_success);
        require(!up::sexp::parser_has_error_occurred(&par));

        retval = up::sexp::parser_destruct(&par);
        require(retval == up::sexp::sexp_success);
    }

    UP_TEST_CASE(load_and_verify_large_file) {
        up::sexp::parser par;
        int retval;

        retval = up::sexp::parser_construct(&par, up::malloc_allocator::instance());
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_load_file(&par, "../../share/upcore/modele.sexp");
        require(retval == up::sexp::sexp_success);

        retval = up::sexp::parser_skip_end(&par);
        require(retval == up::sexp::sexp_success);
        require(!up::sexp::parser_has_error_occurred(&par));

        retval = up::sexp::parser_destruct(&par);
        require(retval == up::sexp::sexp_success);
    }
}
