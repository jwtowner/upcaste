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
#include <up/cstdlib.hpp>
#include <up/cwchar.hpp>
#include <up/cwctype.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    RTYPE NAME(CHAR const* UPRESTRICT str, CHAR** UPRESTRICT endptr, int base) noexcept {
        CHAR const* nptr, * startptr;
        UTYPE max_value, overflow_value, shift_value, new_value, value;
        UCHAR digit, digit_range, letter_range;
        bool negative;

        assert(str && (!base || ((2 <= base) && (base <= 36))));

        // skip whitespace
        for (nptr = str; ISSPACE(*nptr); ++nptr) ;

        // determine sign, if any
        value = 0;
        max_value = UTYPE_MAX;
        overflow_value = UTYPE_OVERFLOW;
        negative = false;

        if (*nptr == '-') {
            max_value = UTYPE_NEG_MAX;
            overflow_value = UTYPE_NEG_OVERFLOW;
            negative = true;
            ++nptr;
        }
        else if (*nptr == '+') {
            ++nptr;
        }

        // determine base and skip base prefix
        if (base == 0) {
            base = 10;
            if (*nptr == '0') {
                base -= 2;
                ++nptr;
                if (fast_iseqlower(*nptr, 'x')) {
                    base <<= 1;
                    ++nptr;
                }
            }
        }
        else if ((base == 16) && (*nptr == '0') && fast_iseqlower(*(nptr + 1), 'x')) {
            nptr += 2;
        }

        shift_value = base;

        // determine allowed digit/letter ranges
        if (base < 10) {
            digit_range = static_cast<UCHAR>(base);
            letter_range = 0;
        }
        else {
            digit_range = 10;
            letter_range = static_cast<UCHAR>(base - 10);
        }

        // read in the digits and accumulate final unsigned result, checking for overflow
        for (startptr = nptr; ; ++nptr) {
            digit = *nptr - '0';
            if (digit >= digit_range) {
                digit = (*nptr & 0xDF) - 'A';
                if (digit >= letter_range) {
                    break;
                }
                digit += 10;
            }

            new_value = (value * shift_value) + static_cast<UTYPE>(digit);
            if ((value <= new_value) && (new_value <= max_value)) {
                value = new_value;
                continue;
            }

            for (++nptr; (static_cast<UCHAR>(*nptr - '0') < digit_range) || (static_cast<UCHAR>((*nptr & 0xDF) - 'A') < letter_range); ++nptr) ;
            value = overflow_value;
            errno = ERANGE;
            break;
        }

        // check for no conversion
        if (nptr == startptr) {
            nptr = str;
            errno = EINVAL;
        }

        // save nptr and return final result
        if (endptr) {
            *endptr = const_cast<CHAR*>(nptr);
        }

        return negative ? static_cast<RTYPE>(-static_cast<STYPE>(value)) : static_cast<RTYPE>(value);
    }
}
