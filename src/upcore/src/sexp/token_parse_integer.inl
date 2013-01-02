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

#include <up/cassert.hpp>
#include <up/sexp.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI UPNONNULLALL
    int NAME(token const* UPRESTRICT tok, RTYPE* UPRESTRICT result) noexcept {
        UTYPE max_value, overflow_value, shift_value, new_value, value, digit;
        char const* ptr, * end_ptr;
        bool negative;
        int retval;

        assert(result && tok && tok->text && (tok->type == token_integer));
        assert((tok->number_info.exactness <= exactness_inexact));
        assert((tok->number_info.radix >= 2) && (tok->number_info.radix <= 36));
        assert((tok->number_info.prefix_length < tok->length));

        ptr = tok->text + tok->number_info.prefix_length;
        end_ptr = tok->text + tok->length;
        shift_value = tok->number_info.radix;
        max_value = UTYPE_MAX;
        overflow_value = UTYPE_OVERFLOW;
        negative = false;
        retval = sexp_success;
        value = 0;
        
        // determine sign, if any
        if (*ptr == '-') {
            max_value = UTYPE_NEG_MAX;
            overflow_value = UTYPE_NEG_OVERFLOW;
            negative = true;
            ++ptr;
        }
        else if (*ptr == '+') {
            ++ptr;
        }

        // read in the digits and accumulate final unsigned result
        for ( ; ptr < end_ptr; ++ptr) {
            digit = *ptr - '0';
            if (digit >= 10) {
                digit = ((*ptr & 0xDF) - 'A') + 10;
            }

            new_value = (value * shift_value) + digit;
            if ((new_value < value) || (new_value > max_value)) {
                retval = sexp_overflow;
                value = overflow_value;
                break;
            }

            value = new_value;
        }

        *result = negative ? ((RTYPE)(-((STYPE)value))) : ((RTYPE)value);
        return retval;
    }
}}
