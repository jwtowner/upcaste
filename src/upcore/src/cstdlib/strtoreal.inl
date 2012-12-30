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
#include <up/cctype.hpp>
#include <up/cerrno.hpp>
#include <up/clocale.hpp>
#include <up/cmath.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    RTYPE NAME(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept {
        DECLARE_RADIX_VARIABLES
        char const* ptr, * startptr, * prevptr, * estartptr, * eprevptr;
        RTYPE value, mantissa, scale, mantissa_shift, fraction_shift, exponent_base;
        unsigned char digit, exponent, mantissa_range;
        bool negative, inverse;
        size_t tag_length;
        char* tag;

        assert(str);

        // skip whitespace
        for (ptr = str; ISSPACE(*ptr); ++ptr) ;

        // determine sign, if any
        negative = false;
        if (*ptr == '-') {
            negative = true;
            ++ptr;
        }
        else if (*ptr == '+') {
            ++ptr;
        }

        // check for infinity or nan
        if (fast_iseqlower(*ptr, 'i') && fast_iseqlower(*(ptr + 1), 'n') && fast_iseqlower(*(ptr + 2), 'f')) {
            ptr += 3;
            if (!fast_strncasecmp(ptr, "inity", 5)) {
                ptr += 5;
            }
            value = INFINITY_VALUE;
            goto finished;
        }
        else if (fast_iseqlower(*ptr, 'n') && fast_iseqlower(*(ptr + 1), 'a') && fast_iseqlower(*(ptr + 2), 'n')) {
            ptr += 3;
            if (*ptr != '(') {
                value = NAN_VALUE;
            }
            else {
                startptr = ++ptr;
                ptr = strchr(ptr, ')');
                if (!ptr) {
                    goto error;
                }
                tag_length = ptr++ - startptr;
                tag = static_cast<char*>(malloca(tag_length + 1));
                if (!tag) {
                    value = NAN_VALUE;
                    goto finished;
                }
                *static_cast<char*>(mempcpy(tag, startptr, tag_length)) = '\0';
                value = NAN_TAG(tag);
                freea(tag);
            }
            goto finished;
        }

        // setup default state
        inverse = false;
        scale = ONE_VALUE;
        mantissa = ZERO_VALUE;
        mantissa_shift = TEN_VALUE;
        mantissa_range = 0;
        exponent_base = TEN_VALUE;
        exponent = 'e';
        INIT_RADIX_VARIABLES
        
        // determine hexadecimal base
        if ((*ptr == '0') && fast_iseqlower(*(ptr + 1), 'x')) {
            mantissa_shift = SIXTEEN_VALUE;
            mantissa_range = 6;
            exponent_base = TWO_VALUE;
            exponent = 'p';
            ptr += 2;
        }

        // read in the integer portion of the number
        for (startptr = prevptr = ptr; ; ++ptr) {
            digit = *ptr - '0';
            if (digit >= 10) {
                digit = (*ptr & 0xDF) - 'A';
                if (digit >= mantissa_range) {
                    break;
                }
                digit += 10;
            }
            mantissa = (mantissa * mantissa_shift) + static_cast<RTYPE>(digit);
        }

        // read in optional fraction after radix character
        if (ISRADIX(ptr)) {
            fraction_shift = mantissa_shift;
            for (startptr += RADIX_LENGTH, ptr += RADIX_LENGTH; ; ++ptr) {
                digit = *ptr - '0';
                if (digit >= 10) {
                    digit = (*ptr & 0xDF) - 'A';
                    if (digit >= mantissa_range) {
                        break;
                    }
                    digit += 10;
                }
                mantissa += static_cast<RTYPE>(digit) / fraction_shift;
                fraction_shift *= mantissa_shift;
            }
        }

        // read in optional exponent
        if (fast_iseqlower(*ptr, exponent) && ((ptr != startptr) || (ptr == prevptr))) {
            eprevptr = ptr++;
            if (*ptr == '-') {
                inverse = true;
                ++ptr;
            }
            else if (*ptr == '+') {
                ++ptr;
            }
            
            scale = ZERO_VALUE;

            for (estartptr = ptr; ; ++ptr) {
                digit = *ptr - '0';
                if (digit >= 10) {
                    break;
                }
                scale = (scale * TEN_VALUE) + static_cast<RTYPE>(digit);
            }

            if (ptr != estartptr) {
                scale = math::pow(exponent_base, scale);
            }
            else {
                scale = ONE_VALUE;
                ptr = eprevptr;
            }
        }

        // compute final value if conversion occured, testing for overflow/underflow
        if (ptr != startptr) {
            value = inverse ? (mantissa / scale) : (mantissa * scale);
            if ((value <= EPSILON_VALUE) && (mantissa > EPSILON_VALUE)) {
                value = EPSILON_VALUE;
                errno = ERANGE;
            }
            else if (value > MAX_VALUE) {
                value = INFINITY_VALUE;
                errno = ERANGE;
            }

            goto finished;
        }
       
    error:

        value = ZERO_VALUE;
        errno = EINVAL;
        negative = false;
        ptr = str;

    finished:

        // save ptr and return final result
        if (endptr) {
            *endptr = const_cast<char*>(ptr);
        }

        return negative ? -value : value;
    }
}
