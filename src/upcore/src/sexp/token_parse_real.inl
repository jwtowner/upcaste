//
//  Upcaste Performance Libraries
//  Copyright (C) 2012 Jesse W. Towner
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
#include <up/cfloat.hpp>
#include <up/cmath.hpp>
#include <up/sexp.hpp>

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(disable:4756) // overflow in constant arithmetic
#endif

namespace up { namespace sexp
{
    LIBUPCOREAPI UPNONNULLALL
    int NAME(token const* UPRESTRICT tok, REAL* UPRESTRICT result) noexcept {
        REAL mantissa = ZERO_VALUE, scale = ONE_VALUE, mantissa_shift, fraction_shift, temp;
        bool negative = false, inverse = false;
        unsigned char digit, radix;
        char const* ptr, * end_ptr;
        int retval;
        char ch;

        assert(result && tok && tok->text && ((tok->type == token_integer) || (tok->type == token_real)));
        assert((tok->number_info.exactness <= exactness_inexact));
        assert((tok->number_info.radix >= 2) && (tok->number_info.radix <= 36));
        assert((tok->number_info.prefix_length < tok->length));

        ptr = tok->text + tok->number_info.prefix_length;
        end_ptr = tok->text + tok->length;
        
        // read in optional sign
        ch = *ptr;
        if ((ch == '+') || (ch == '-')) {
            if (ch == '-') {
                negative = true;
            }
            
            // check for infinity or NaN
            ch = *(++ptr);
            if (ch == 'i') {
                *result = negative ? -INF_VALUE : INF_VALUE;
                return sexp_success;
            }
            else if (ch == 'n') {
                *result = NAN_VALUE;
                return sexp_success;
            }
        }

        // read in the digits and accumulate mantissa portion
        radix = tok->number_info.radix;
        mantissa_shift = ((REAL)radix);

        for ( ; ptr < end_ptr; ++ptr) {
            ch = *ptr;
            digit = ch - '0';
            if (digit >= 10) {
                digit = ((ch & 0xDF) - 'A') + 10;
                if (digit >= radix) {
                    break;
                }
            }

            mantissa = (mantissa * mantissa_shift) + ((REAL)digit);
        }
        
        // read in optional fraction after decimal place
        if (ch == '.') {
            fraction_shift = mantissa_shift;
            for (++ptr; ptr < end_ptr; ++ptr) {
                ch = *ptr;
                digit = ch - '0';
                if (digit >= 10) {
                    digit = ((ch & 0x5F) - 'A') + 10;
                    if (digit >= radix) {
                        break;
                    }
                }

                mantissa += ((REAL)digit) / fraction_shift;
                fraction_shift *= mantissa_shift;
            }
        }

        // read in optional exponent
        if (ptr < end_ptr) {
            ch = *(++ptr);
            if ((ch == '+') || (ch == '-')) {
                if (ch == '-') {
                    inverse = true;
                }
                ++ptr;
            }

            scale = ZERO_VALUE;

            for ( ; ptr < end_ptr; ++ptr) {
                ch = *ptr;
                digit = ch - '0';
                if (digit >= 10) {
                    digit = ((ch & 0xDF) - 'A') + 10;
                }

                scale = (scale * mantissa_shift) + ((REAL)digit);
            }

            scale = up::math::pow(mantissa_shift, scale);
        }

        // compute final value, testing for overflow/underflow
        temp = inverse ? (mantissa / scale) : (mantissa * scale);
        retval = sexp_success;

        if ((temp < EPSILON_VALUE) && (mantissa >= EPSILON_VALUE)) {
            temp = ZERO_VALUE;
            retval = sexp_underflow;
        }
        else if (temp > MAX_VALUE) {
            temp = INF_VALUE;
            retval = sexp_overflow;
        }

        *result = negative ? -temp : temp;
        return retval;
    }
}}
