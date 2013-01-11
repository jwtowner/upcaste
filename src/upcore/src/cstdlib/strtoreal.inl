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
#include <up/cctype.hpp>
#include <up/cerrno.hpp>
#include <up/clocale.hpp>
#include <up/cmath.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/cstdio.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    REAL NAME(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept {
        DECLARE_RADIX_VARIABLES
        char const* ptr, * startptr, * prevptr, * estartptr, * eprevptr;
        REAL value, mantissa, scale, mantissa_shift, fraction_shift;
        unsigned char digit, exponent_char, mantissa_range;
        int exponent, exponent_max;
        bool sign, inverse;
        size_t tag_length;
        char* tag;

        assert(str);

        // skip whitespace
        for (ptr = str; ISSPACE(*ptr); ++ptr) ;

        // determine sign, if any
        sign = false;
        if (*ptr == '-') {
            sign = true;
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
            value = REAL_INFINITY;
            goto finished;
        }
        else if (fast_iseqlower(*ptr, 'n') && fast_iseqlower(*(ptr + 1), 'a') && fast_iseqlower(*(ptr + 2), 'n')) {
            ptr += 3;
            if (*ptr != '(') {
                value = REAL_NAN;
            }
            else {
                startptr = ++ptr;
                ptr = strchr(ptr, ')');
                if (UPUNLIKELY(!ptr)) {
                    goto error;
                }
                tag_length = ptr++ - startptr;
                tag = static_cast<char*>(malloca(tag_length + 1));
                if (UPUNLIKELY(!tag)) {
                    value = REAL_NAN;
                    goto finished;
                }
                *static_cast<char*>(mempcpy(tag, startptr, tag_length)) = '\0';
                value = math::nan<REAL>(tag);
                freea(tag);
            }
            goto finished;
        }

        // setup default state
        inverse = false;
        scale = REAL_ONE;
        mantissa = REAL_ZERO;
        mantissa_shift = REAL_TEN;
        mantissa_range = 0;
        exponent_max = REAL_MAX_10_EXP;
        exponent_char = 'e';
        exponent = 0;
        INIT_RADIX_VARIABLES
        
        // determine hexadecimal base
        if ((*ptr == '0') && fast_iseqlower(*(ptr + 1), 'x')) {
            mantissa_shift = REAL_SIXTEEN;
            mantissa_range = 6;
            exponent_max = REAL_MAX_EXP;
            exponent_char = 'p';
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
            mantissa = (mantissa * mantissa_shift) + static_cast<REAL>(digit);
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
                mantissa += static_cast<REAL>(digit) / fraction_shift;
                fraction_shift *= mantissa_shift;
            }
        }

        // read in optional exponent
        if (fast_iseqlower(*ptr, exponent_char) && ((ptr != startptr) || (ptr == prevptr))) {
            eprevptr = ptr++;
            if (*ptr == '-') {
                inverse = true;
                ++ptr;
            }
            else if (*ptr == '+') {
                ++ptr;
            }

            for (estartptr = ptr; ; ++ptr) {
                digit = *ptr - '0';
                if (digit >= 10) {
                    break;
                }
                exponent = (exponent * 10) + static_cast<int>(digit);
                if (UPUNLIKELY(exponent > exponent_max)) {
                    // overflow occurred, consume rest of input
                    for (++ptr; static_cast<char>(*ptr - '0') < 10; ++ptr) ;
                    value = inverse ? REAL_MIN : REAL_INFINITY;
                    errno = ERANGE;
                    goto finished;
                }
            }

            if (UPUNLIKELY(ptr == estartptr)) {
                inverse = false;
                ptr = eprevptr;
            }
            
            if (inverse) {
                exponent = -exponent;
            }

            if (exponent_char == 'e') {
                scale = math::pow10<REAL>(exponent);
            }
            else {
                scale = math::pow(REAL_TWO, exponent);
            }
        }

        if (UPUNLIKELY(ptr == startptr)) {
            goto error;
        }

        // conversion from text successful, compute final value
        value = mantissa * scale;
        if ((value < REAL_MIN) && (mantissa >= REAL_MIN)) {
            // underflow occurred
            value = REAL_MIN;
            errno = ERANGE;
        }
        else if ((value > REAL_MAX) && (mantissa <= REAL_MAX)) {
            // overflow occurred
            value = REAL_INFINITY;
            errno = ERANGE;
        }

    finished:

        // save ptr and return final result
        if (endptr) {
            *endptr = const_cast<char*>(ptr);
        }

        return sign ? -value : value;

    error:

        value = REAL_ZERO;
        errno = EINVAL;
        sign = false;
        ptr = str;
        goto finished;
    }
}
