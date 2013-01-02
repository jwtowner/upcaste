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

#include "printf_internal.hpp"
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cfloat.hpp>
#include <up/cinttypes.hpp>
#include <up/climits.hpp>
#include <up/cmath.hpp>
#include <up/cstdint.hpp>
#include <up/cstring.hpp>

namespace up { namespace detail { namespace
{
    inline UPALWAYSINLINE UPPURE
    CHAR const* get_digit_table(int upper) noexcept {
        return upper ? STR("0123456789ABCDEF") : STR("0123456789abcdef");
    }

    inline UPALWAYSINLINE UPNONNULLALL
    CHAR* format_int_digits(
        CHAR* UPRESTRICT work_head,
        CHAR* UPRESTRICT work_tail,
        CHAR const* UPRESTRICT digit_table,
        int base,
        uintmax_t value
    )
    noexcept {
        switch (base) {
        case 16:
            do {
                assert(work_head < work_tail);
                *(--work_tail) = digit_table[value & 15];
                value >>= 4;
            }
            while (value != 0);
            break;
        case 8:
            do {
                assert(work_head < work_tail);
                *(--work_tail) = static_cast<CHAR>(value & 7) + CHR('0');
                value >>= 3;
            }
            while (value != 0);
            break;
        default:
            do {
                assert(work_head < work_tail);
                *(--work_tail) = static_cast<CHAR>(value % 10) + CHR('0');
                value /= 10;
            }
            while (value != 0);
            break;
        }
        return work_tail;
    }

    UPHIDDEN UPNONNULLALL
    CHAR* format_default_grouped_digits(
        CHAR* UPRESTRICT work_head,
        CHAR* UPRESTRICT work_tail,
        uintmax_t value
    )
    noexcept {
        int separator = 3;
        for (;;) {
            assert(work_head < work_tail);
            *(--work_tail) = static_cast<CHAR>(value % 10) + CHR('0');
            value /= 10;
            if (value == 0) {
                break;
            }
            if (--separator == 0) {
                assert(work_head < work_tail);
                *(--work_tail) = CHR(',');
                separator = 3;
            }
        }
        return work_tail;
    }

#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
    UPHIDDEN UPNONNULLALL
    CHAR* format_grouped_digits(
        CHAR* UPRESTRICT work_head,
        CHAR* UPRESTRICT work_tail,
        CHAR const* sep_start,
        CHAR const* sep_end,
        CHAR const* grouping,
        uintmax_t value
    )
    noexcept {
        CHAR group_count;
        CHAR const* sep;

        if (!grouping || (*grouping == CHAR_MAX) || (*grouping <= 0)) {
            goto stop_grouping;
        }
        
        group_count = *grouping++;
        while (UPLIKELY(work_head < work_tail)) {
            *(--work_tail) = static_cast<CHAR>(value % 10) + CHR('0');
            value /= 10;
            if (value == 0) {
                break;
            }
            if (--group_count == 0) {
                for (sep = sep_start; sep < sep_end; ++sep) {
                    if (UPUNLIKELY(work_head >= work_tail)) {
                        return work_tail;
                    }
                    *(--work_tail) = *sep;
                }
                if ((*grouping == CHAR_MAX) || (*grouping < 0)) {
                    goto stop_grouping;
                }
                else if (*grouping > 0) {
                    group_count = *grouping++;
                }
                else {
                    group_count = *(grouping - 1);
                }
            }
        }

        return work_tail;

    stop_grouping:

        while (UPLIKELY(work_head < work_tail)) {
            *(--work_tail) = static_cast<CHAR>(value % 10) + CHR('0');
            value /= 10;
            if (value == 0) {
                break;
            }
        }

        return work_tail;
    }
#endif

    UPHIDDEN UPNONNULLALL
    bool format_int(INFO* info, intmax_t value, int base, int width, int precision, unsigned int flags) noexcept {
        CHAR const* const digit_table = get_digit_table(flags & print_flag_upper);
        bool const noprecision = (precision == -1);
        CHAR* cursor, * end, * work_head;
        CHAR sign = 0, hexprefix = 0;
        int zeropad, spacepad;
        uintmax_t uvalue;

        // enough space for an octal representation of intmax_t plus grouping
        constexpr size_t work_buffer_length = (((sizeof(intmax_t) * CHAR_BIT) / 3) + 1) * 4;
        CHAR work_buffer[work_buffer_length];
        CHAR* const work_tail = work_buffer + work_buffer_length;

#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
        CHAR const* sep_start, * sep_end, * grouping;
        struct lconv const* lc;
#endif

        // determine sign character and compute unsigned value
        uvalue = value;
        if (flags & (print_flag_signed | print_flag_pointer)) {
            if (!(flags & print_flag_pointer) && (value < 0)) {
                sign = '-';
                uvalue = -value;
                --width;
            }
            else if (flags & print_flag_plus) {
                sign = '+';
                --width;
            }
            else if (flags & print_flag_space) {
                sign = ' ';
                --width;
            }
        }

        // format the digits into the work buffer, using grouping if necessary
        if (flags & print_flag_quote) {
#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
            lc = info->lc;
            if (!lc) {
                lc = localeconv();
                info->lc = lc;
                assert(lc);
            }
            grouping = lc->grouping;
            sep_start = lc->thousands_sep;
            sep_end = sep_start + (sep_start ? strlen(sep_start) : 0);
            work_head = format_grouped_digits(work_buffer, work_tail, sep_start, sep_end, grouping, uvalue);
#else
            work_head = format_default_grouped_digits(work_buffer, work_tail, uvalue);
#endif
        }
        else {
            work_head = format_int_digits(work_buffer, work_tail, digit_table, base, uvalue);
        }

        // compute amount of zero and space padding
        assert(static_cast<size_t>(work_tail - work_head) <= work_buffer_length);
        zeropad = static_cast<int>(work_tail - work_head);
        
        if (UPLIKELY(precision == -1)) {
            // set default precision
            precision = 1; 
        }
        else if (UPUNLIKELY((precision == 0) && (uvalue == 0))) {
            // the result of converting a zero value with a precision of zero is
            // no characters unless it's an octal digit in alternative form
            work_head = work_tail;
            zeropad = 0;
            if ((flags & print_flag_number) && (base == 8)) {
                precision = 1;
            }
        }
        
        if ((flags & print_flag_number) && (uvalue != 0)) {
            // handle conversion to alternative form
            if ((base == 8) && (precision <= zeropad)) {
                // increase precision if necessary for octal format
                precision = zeropad + 1;
            }
            else if (base == 16) {
                // output hex-prefix, no precision increase, but adjust padding
                hexprefix = (flags & print_flag_upper) ? 'X' : 'x';
                width -= 2;
            }
        }

        spacepad = width - ((precision >= zeropad) ? precision : zeropad);
        if (spacepad < 0) {
            spacepad = 0;
        }

        zeropad = precision - zeropad;
        if (zeropad < 0) {
            zeropad = 0;
        }

        if (flags & print_flag_minus) {
            // left justify
            spacepad = -spacepad;
        }
        else if ((flags & print_flag_zero) && noprecision) {
            // if the `0' and `-' flags both appear, the `0' flag is ignored
            zeropad += spacepad;
            spacepad = 0;
        }

        cursor = info->cursor;
        end = info->end;

        // leading spaces
        for ( ; spacepad > 0; --spacepad) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR(' ')))) {
                return false;
            }
        }
        
        // sign prefix
        if (sign) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, sign))) {
                return false;
            }
        }
        
        // hexadecimal prefix
        if (hexprefix) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR('0')))) {
                return false;
            }
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, hexprefix))) {
                return false;
            }
        }
        
        // lead zeros
        for ( ; zeropad > 0; --zeropad) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR('0')))) {
                return false;
            }
        }
        
        // actual number digits
        for ( ; work_head < work_tail; ++work_head) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, *work_head))) {
                return false;
            }
        }

        // trailing spaces
        for ( ; spacepad < 0; ++spacepad) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR(' ')))) {
                return false;
            }
        }

        info->cursor = cursor;
        return true;
    }

    UPHIDDEN UPNONNULLALL
    int decompose_style_a(uintmax_t* UPRESTRICT pintpart, uintmax_t* UPRESTRICT pfracpart, REAL fsignificand, int exponent, int precision) noexcept {
        REAL fintpart, ffracpart, ffracshift;
        uintmax_t intpart, fracpart;

        ffracpart = math::modf(fsignificand, &fintpart);
        intpart = static_cast<uintmax_t>(fintpart);

        // compute fractional part by shifting by a power of sixteen equal to precision
        ffracshift = math::exp2(static_cast<REAL>(precision + 3));
        ffracpart = math::round(ffracpart * ffracshift);
        if (ffracpart < ffracshift) {
            fracpart = static_cast<uintmax_t>(ffracpart);
        }
        else {
            // fraction part rounded up to whole number
            ++intpart;
            fracpart = 0;
            if (intpart == 16) {
                // adjust exponent if integer part rounded up to sixteen
                exponent += 4;
                intpart = 1;
            }
        }

        *pintpart = intpart;
        *pfracpart = fracpart;
        return exponent;
    }

    UPHIDDEN UPNONNULLALL
    int decompose_style_e(uintmax_t* UPRESTRICT pintpart, uintmax_t* UPRESTRICT pfracpart, REAL fsignificand, int exponent, int precision) noexcept {
        REAL fintpart, ffracpart, ffracshift;
        uintmax_t intpart, fracpart;

        ffracpart = math::modf(fsignificand, &fintpart);
        intpart = static_cast<uintmax_t>(fintpart);

        // compute fractional part by shifting by a power of ten equal to precision
        ffracshift = math::pow10(precision);
        ffracpart = math::round(ffracpart * ffracshift);
        if (ffracpart < ffracshift) {
            fracpart = static_cast<uintmax_t>(ffracpart);
        }
        else {
            // fraction part rounded up to whole number
            ++intpart;
            fracpart = 0;
            if (intpart == 10) {
                // adjust exponent if integer part rounded up to ten
                ++exponent;
                intpart = 1;
            }
        }

        *pintpart = intpart;
        *pfracpart = fracpart;
        return exponent;
    }

    UPHIDDEN UPNONNULLALL
    int decompose_style_f(uintmax_t* UPRESTRICT pintpart, uintmax_t* UPRESTRICT pfracpart, REAL fvalue, int exponent, int precision) noexcept {
        REAL fintpart, ffracpart, ffracshift;
        uintmax_t intpart, fracpart;
        int trailintzeros;

        if (exponent <= DECIMAL_DIG) {
            ffracpart = math::modf(fvalue, &fintpart);
            intpart = static_cast<uintmax_t>(fintpart);
            trailintzeros = 0;

            // compute fractional part by shifting by a power of ten equal to precision
            ffracshift = math::pow10(precision);
            ffracpart = math::round(ffracpart * ffracshift);
            if (ffracpart < ffracshift) {
                fracpart = static_cast<uintmax_t>(ffracpart);
            }
            else {
                // fraction part rounded up to whole number
                ++intpart;
                fracpart = 0;
            }
        }
        else {
            trailintzeros = exponent - (DECIMAL_DIG - 1);
            intpart = math::llround(fvalue / math::pow10<REAL>(trailintzeros));
            fracpart = 0;
        }

        *pintpart = intpart;
        *pfracpart = fracpart;
        return trailintzeros;
    }

    inline UPALWAYSINLINE UPNONNULLALL
    CHAR* format_significand_digits(
        CHAR* UPRESTRICT work_head,
        CHAR* UPRESTRICT work_tail,
        CHAR const* UPRESTRICT digit_table,
        int base,
        uintmax_t value
    )
    noexcept {
        if (base == 10) {
            do {
                assert(work_head < work_tail);
                *(--work_tail) = static_cast<CHAR>(value % 10) + CHR('0');
                value /= 10;
            }
            while (value != 0);
        }
        else {
            do {
                assert(work_head < work_tail);
                *(--work_tail) = digit_table[value & 15];
                value >>= 4;
            }
            while (value != 0);
        }
        return work_tail;
    }

    UPHIDDEN UPNONNULLALL
    bool format_real(INFO* UPRESTRICT info, REAL fvalue, int width, int precision, unsigned int flags) noexcept {
        CHAR const* const digit_table = get_digit_table(flags & print_flag_upper), * infnan;
        CHAR* cursor, * end, * temp, * int_head, * frac_head, * frac_tail, * exp_head;
        int base, exponent, fpclass, spacepad, zeropad, trailintzeros, leadfraczeros;
        CHAR sign, exponent_sign, exponent_char;
        uintmax_t intpart, fracpart;
        ptrdiff_t exponent_zeropad;
        REAL fsignificand;
        
        // enough space for sign, grouped integer part, fraction part and exponent
        constexpr size_t work_buffer_length = (DECIMAL_DIG * 5) + 32; 
        CHAR work_buffer[work_buffer_length];
        CHAR* const work_tail = work_buffer + work_buffer_length;

#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
        CHAR const* sep_start, * sep_end, * decimal_point, *decimal_point_end;
        size_t decimal_point_length;
        struct lconv const* lc;
#endif

        // set default precision if necessary
        spacepad = width;
        if (precision == -1) {
            precision = 6; 
        }

        // determine sign
        sign = 0;
        if (fvalue < REAL_ZERO) {
            fvalue = -fvalue;
            sign = CHR('-');
            --spacepad;
        }
        else if (flags & print_flag_plus) {
            sign = CHR('+');
            --spacepad;
        }
        else if (flags & print_flag_space) {
            sign = CHR(' ');
            --spacepad;
        }

        // check for infinity and nan
        fpclass = math::fpclassify(fvalue);
        if ((fpclass == FP_INFINITE) || (fpclass == FP_NAN)) {
            temp = work_buffer;
            if (sign != 0) {
                *temp++ = sign;
            }
            if (fpclass == FP_INFINITE) {
                infnan = (flags & print_flag_upper) ? STR("INF") : STR("inf");
            }
            else {
                infnan = (flags & print_flag_upper) ? STR("NAN") : STR("nan");
            }
            while (*infnan) {
                *temp++ = *infnan++;
            }
            return FORMAT_STRING(info, work_buffer, width, static_cast<int>(temp - work_buffer), flags);
        }
        
        // set up anticipating writing exponent into work buffer
        trailintzeros = 0;
        exp_head = work_tail;
        if (precision > DECIMAL_DIG) {
            precision = DECIMAL_DIG;
        }

        // decompose value into integer, fraction, and exponent parts based on style
        if (!(flags & print_flag_style_a)) {
            // f,F,e,E,g,G style conversions with decimal significand and decimal exponent
            base = 10;
            fsignificand = math::frexp10(fvalue, &exponent);
            switch (flags & (print_flag_style_e | print_flag_style_g)) {
            case print_flag_style_e:
                exponent = decompose_style_e(&intpart, &fracpart, fsignificand, exponent, precision);
                break;
            case print_flag_style_g:
                exponent = decompose_style_e(&intpart, &fracpart, fsignificand, exponent, precision);
                if (((precision + 1) <= exponent) || (exponent < -4)) {
                    flags |= print_flag_style_e;
                    break;
                }
                precision -= exponent;
                if (precision > DECIMAL_DIG) {
                    precision = DECIMAL_DIG;
                }
                // fallthrough
            default:
                trailintzeros = decompose_style_f(&intpart, &fracpart, fvalue, exponent, precision);
                spacepad -= trailintzeros;
                goto start_fraction;
            }
            exponent_char = (flags & print_flag_upper) ? CHR('E') : CHR('e');
            exponent_zeropad = 2 - (work_tail - exp_head);
        }
        else {
            // a,A conversion style, hexadecimal significand and binary exponent
            base = 16;
            fsignificand = math::frexp(fvalue, &exponent);
            exponent = decompose_style_a(&intpart, &fracpart, fsignificand, exponent, precision);
            exponent_char = (flags & print_flag_upper) ? CHR('P') : CHR('p');
            exponent_zeropad = 0;
        }

        // format exponent digits into work buffer
        exponent_sign = CHR('+');
        if (exponent < 0) {
            exponent = -exponent;
            exponent_sign = CHR('-');
        }

        do {
            assert(work_buffer < exp_head);
            *(--exp_head) = static_cast<CHAR>(exponent % 10) + CHR('0');
            exponent /= 10;
        }
        while (exponent != 0);

        // the exponent always contains at least one digits for a,A style conversions
        // and at least two digits for e,E,g,G style conversion, and only as many
        // more digits as necessary to represent the exponent
        for ( ; exponent_zeropad > 0; --exponent_zeropad) {
            *(--exp_head) = CHR('0');
        }

        // finish up with sign and exponent specifier
        *(--exp_head) = exponent_sign;
        *(--exp_head) = exponent_char;
        spacepad -= static_cast<int>(work_tail - exp_head);

    start_fraction:

        // format the fractional part into the work buffer
        frac_head = frac_tail = exp_head;
        if (fracpart != 0) {
            frac_head = format_significand_digits(work_buffer, frac_tail, digit_table, base, fracpart);
        }

        leadfraczeros = precision - static_cast<int>(exp_head - frac_head);
        if ((flags & (print_flag_style_g | print_flag_number)) == print_flag_style_g) {
            // for g,G conversions, trailing zeros are removed from the
            // fractional part unless the # flag was specified 
            if (frac_head < frac_tail) {
                for (temp = exp_head - 1; (temp >= frac_head) && (*temp == '0'); --temp) {
                    --frac_tail;
                    --precision;
                }
            }
            else {
                leadfraczeros = 0;
                precision = 0;
            }
        }

        // get decimal point string if using the system locale
#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
        lc = info->lc;
        if (!lc) {
            lc = localeconv();
            info->lc = lc;
            assert(lc);
        }

        decimal_point = lc->decimal_point;
        if (UPLIKELY(decimal_point && (*decimal_point != CHR('\0')))) {
            decimal_point_length = strlen(decimal_point);
            if (UPUNLIKELY(decimal_point_length > INT_MAX)) {
                errno = EOVERFLOW;
                return false;
            }
            decimal_point_end = decimal_point + decimal_point_length;
        }
        else {
            decimal_point = STR(".");
            decimal_point_end = decimal_point + 1;
        }
#endif

        // output decimal point if either the fractional part is non-zero
        // and/or the # flag was specified
        if ((precision > 0) || (flags & print_flag_number)) {
            flags |= print_flag_number;
#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
            spacepad -= static_cast<int>(decimal_point_length);
#else
            --spacepad;
#endif
        }

        // convert the integer part with optional grouping
        if (flags & print_flag_quote) {
#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
            sep_start = lc->thousands_sep;
            sep_end = sep_start + (sep_start ? strlen(sep_start) : 0);
            int_head = format_grouped_digits(work_buffer, frac_head, sep_start, sep_end, lc->grouping, intpart);
#else
            int_head = format_default_grouped_digits(work_buffer, frac_head, intpart);
#endif
        }
        else {
            int_head = format_significand_digits(work_buffer, frac_head, digit_table, base, intpart);
            if (base == 16) {
                *(--int_head) = (flags & print_flag_upper) ? CHR('x') : CHR('x');
                *(--int_head) = CHR('0');
            }
        }

        assert(int_head >= work_buffer);

        // update padding with final precision
        spacepad = spacepad - precision - static_cast<int>(frac_head - int_head);
        if (spacepad < 0) {
            spacepad = 0;
        }

        // if both the `0' and `-' flags appear, the `0' flag is ignored
        zeropad = 0;
        if (flags & print_flag_minus) {
            spacepad = -spacepad;
        }
        else if ((flags & print_flag_zero) && (spacepad > 0)) {
            zeropad = spacepad;
            spacepad = 0;
        }

        cursor = info->cursor;
        end = info->end;

        // leading spaces
        for ( ; spacepad > 0; --spacepad) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR(' ')))) {
                return false;
            }
        }

        // sign character
        if (sign) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, sign))) {
                return false;
            }
        }

        // leading zeros
        for ( ; zeropad > 0; --zeropad) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR('0')))) {
                return false;
            }
        }

        // integer part
        for ( ; int_head < frac_head; ++int_head) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, *int_head))) {
                return false;
            }
        }

        // trailing integer zeros
        for ( ; trailintzeros > 0; --trailintzeros) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR('0')))) {
                return false;
            }
        }

        // decimal point
        if (flags & print_flag_number) {
#if defined(UP_HAS_STDC_LOCALE) && defined(USE_STDC_LOCALE)
            for ( ; decimal_point < decimal_point_end; ++decimal_point) {
                if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, *decimal_point))) {
                    return false;
                }
            }
#else
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR('.')))) {
                return false;
            }
#endif
        }

        // leading fractional zeros
        for ( ; leadfraczeros > 0; --leadfraczeros) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR('0')))) {
                return false;
            }
        }

        // fractional part
        for ( ; frac_head < frac_tail; ++frac_head) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, *frac_head))) {
                return false;
            }
        }

        // exponent part
        for ( ; exp_head < work_tail; ++exp_head) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, *exp_head))) {
                return false;
            }
        }

        // trailing spaces
        for ( ; spacepad < 0; ++spacepad) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR(' ')))) {
                return false;
            }
        }

        info->cursor = cursor;
        return true;
    }

    UPHIDDEN
    CHAR const* dispatch(INFO* UPRESTRICT info, CHAR const* UPRESTRICT format) noexcept {
        bool success;
        unsigned int flags, new_precision, new_width;
        int base, precision, width;
        print_convert convert;
        UCHAR ch, digit, cvalue;
        CHAR const* strvalue;
        intmax_t value;
        size_t length;
        REAL fvalue;

        // parse zero or more flags (in any order)
        flags = 0;
        for (;;) {
            ch = *format++;
            switch (ch) {
            case '-':
                flags |= print_flag_minus;
                break;
            case '+':
                flags |= print_flag_plus;
                break;
            case ' ':
                flags |= print_flag_space;
                break;
            case '#':
                flags |= print_flag_number;
                break;
            case '0':
                flags |= print_flag_zero;
                break;
            case '\'':
                flags |= print_flag_quote;
                break;
            default:
                goto finished_flags;
            }
        }

    finished_flags:

        // parse optional minimum field width
        if (ch == '*') {
            width = va_arg(info->args, int);
            if (UPUNLIKELY(width < 0)) {
                flags |= print_flag_minus;
                width = -width;
            }
            ch = *format++;
        }
        else {
            width = 0;
            for (;;) {
                digit = ch - '0';
                if (digit >= 10) {
                    break;
                }
                new_width = (width * 10) + digit;
                if (UPUNLIKELY((new_width < ((unsigned int)width)) || (new_width > INT_MAX))) {
                    errno = EOVERFLOW;
                    return nullptr;
                }
                width = new_width;
                ch = *format++;
            }
        }

        // parse optional precision
        precision = -1;
        if (ch == '.') {
            ch = *format++;
            if (ch == '*') {
                precision = va_arg(info->args, int);
                if (UPUNLIKELY(precision < 0)) {
                    precision = -1;
                }
                ch = *format++;
            }
            else {
                precision = 0;
                for (;;) {
                    digit = ch - '0';
                    if (digit >= 10) {
                        break;
                    }
                    new_precision = (precision * 10) + digit;
                    if (UPUNLIKELY((new_precision < ((unsigned int)precision)) || (new_precision > INT_MAX))) {
                        errno = EOVERFLOW;
                        return nullptr;
                    }
                    precision = new_precision;
                    ch = *format++;
                }
            }
        }

        // parse optional length modifier
        switch (ch) {
        case 'h':
            ch = *format++;
            if (ch == 'h') {
                ch = *format++;
                convert = print_convert_char;
            }
            else {
                convert = print_convert_short;
            }
            break;
        case 'l':
            ch = *format++;
            if (ch == 'l') {
                ch = *format++;
                convert = print_convert_llong;
            }
            else {
                convert = print_convert_long;
            }
            break;
        case 'L':
            ch = *format++;
            convert = print_convert_ldouble;
            break;
        case 'j':
            ch = *format++;
            convert = print_convert_intmax;
            break;
        case 't':
            ch = *format++;
            convert = print_convert_ptrdiff;
            break;
        case 'z':
            ch = *format++;
            convert = print_convert_size;
            break;
        case 'I':
            // MSVC printf extension for ptrdiff_t/size/int32_t/int64_t/uint32_t/uint64_t
            ch = *format++;
            convert = print_convert_ptrdiff;
            if ((ch == '3') && (*format == '2')) {
                ch = *++format;
                convert = print_convert_int32;
            }
            else if ((ch == '6') && (*format == '4')) {
                ch = *++format;
                convert = print_convert_int64;
            }
            break;
        default:
            convert = print_convert_default;
            break;
        }

        // parse conversion specifier and perform conversion
        base = 0;
        switch (ch) {
        case 'd':
            // fallthrough
        case 'i':
            flags |= print_flag_signed;
            switch (convert) {
            case print_convert_char:
                value = static_cast<SCHAR>(va_arg(info->args, int));
                break;
            case print_convert_short:
                value = static_cast<short>(va_arg(info->args, int));
                break;
            case print_convert_long:
                value = va_arg(info->args, long);
                break;
            case print_convert_llong:
                value = va_arg(info->args, long long);
                break;
            case print_convert_size:
                value = va_arg(info->args, ssize_t);
                break;
            case print_convert_ptrdiff:
                value = va_arg(info->args, ptrdiff_t);
                break;
            case print_convert_int32:
                value = va_arg(info->args, int_least32_t);
                break;
            case print_convert_int64:
                value = va_arg(info->args, int_least64_t);
                break;
            case print_convert_intmax:
                value = va_arg(info->args, intmax_t);
                break;
            default:
                value = va_arg(info->args, int);
                break;
            }
            success = format_int(info, value, 10, width, precision, flags);
            break;
        case 'X':
            flags |= print_flag_upper;
            // fallthrough
        case 'x':
            base = 16;
            // fallthrough
        case 'o':
            flags &= ~print_flag_quote;
            if (base == 0) {
                base = 8;
            }
            // fallthrough
        case 'u':
            if (base == 0) {
                base = 10;
            }
            switch (convert) {
            case print_convert_char:
                value = static_cast<UCHAR>(va_arg(info->args, unsigned int));
                break;
            case print_convert_short:
                value = static_cast<unsigned short>(va_arg(info->args, unsigned int));
                break;
            case print_convert_long:
                value = va_arg(info->args, unsigned long);
                break;
            case print_convert_llong:
                value = va_arg(info->args, unsigned long long);
                break;
            case print_convert_size:
                value = va_arg(info->args, size_t);
                break;
            case print_convert_ptrdiff:
                value = va_arg(info->args, size_t);
                break;
            case print_convert_int32:
                value = va_arg(info->args, uint_least32_t);
                break;
            case print_convert_int64:
                value = va_arg(info->args, uint_least64_t);
                break;
            case print_convert_intmax:
                value = va_arg(info->args, uintmax_t);
                break;
            default:
                value = va_arg(info->args, unsigned int);
                break;
            }
            success = format_int(info, value, base, width, precision, flags);
            break;
        case 'A':
            flags |= print_flag_upper;
            // fallthrough
        case 'a':
            flags |= print_flag_style_a;
            flags &= ~print_flag_quote;
            if (convert == print_convert_ldouble) {
                fvalue = static_cast<REAL>(va_arg(info->args, long double));
            }
            else {
                fvalue = static_cast<REAL>(va_arg(info->args, double));
            }
            success = format_real(info, fvalue, width, precision, flags);
            break;
        case 'F':
            flags |= print_flag_upper;
            // fallthrough
        case 'f':
            if (convert == print_convert_ldouble) {
                fvalue = static_cast<REAL>(va_arg(info->args, long double));
            }
            else {
                fvalue = static_cast<REAL>(va_arg(info->args, double));
            }
            success = format_real(info, fvalue, width, precision, flags);
            break;
        case 'E':
            flags |= print_flag_upper;
            // fallthrough
        case 'e':
            flags |= print_flag_style_e;
            if (convert == print_convert_ldouble) {
                fvalue = static_cast<REAL>(va_arg(info->args, long double));
            }
            else {
                fvalue = static_cast<REAL>(va_arg(info->args, double));
            }
            success = format_real(info, fvalue, width, precision, flags);
            break;
        case 'G':
            flags |= print_flag_upper;
            // fallthrough
        case 'g':
            flags |= print_flag_style_g;
            if (convert == print_convert_ldouble) {
                fvalue = static_cast<REAL>(va_arg(info->args, long double));
            }
            else {
                fvalue = static_cast<REAL>(va_arg(info->args, double));
            }
            if (precision == 0) {
                precision = 1;
            }
            success = format_real(info, fvalue, width, precision, flags);
            break;
        case 'c':
            cvalue = static_cast<UCHAR>(va_arg(info->args, int));
            success = FORMAT_CHAR(info, cvalue);
            break;
        case 's':
            strvalue = va_arg(info->args, CHAR const*);
            success = FORMAT_STRING(info, strvalue, width, precision, flags);
            break;
        case 'p':
            strvalue = va_arg(info->args, CHAR const*);
            if (!strvalue) {
                success = FORMAT_STRING(info, "(nil)", width, -1, flags);
                break;
            }
            flags |= print_flag_number | print_flag_pointer;
            success = format_int(info, reinterpret_cast<uintptr_t>(strvalue), 16, width, precision, flags);
            break;
        case 'n':
            length = info->length + (info->cursor - info->buffer);
            switch (convert) {
            case print_convert_char:
                *va_arg(info->args, SCHAR*) = static_cast<SCHAR>(length);
                break;
            case print_convert_short:
                *va_arg(info->args, short*) = static_cast<short>(length);
                break;
            case print_convert_long:
                *va_arg(info->args, long*) = static_cast<long>(length);
                break;
            case print_convert_llong:
                *va_arg(info->args, long long*) = static_cast<long long>(length);
                break;
            case print_convert_size:
                *va_arg(info->args, ssize_t*) = static_cast<ssize_t>(length);
                break;
            case print_convert_ptrdiff:
                *va_arg(info->args, ptrdiff_t*) = static_cast<ptrdiff_t>(length);
                break;
            case print_convert_int32:
                *va_arg(info->args, int_least32_t*) = static_cast<int_least32_t>(length);
                break;
            case print_convert_int64:
                *va_arg(info->args, int_least64_t*) = static_cast<int_least64_t>(length);
                break;
            case print_convert_intmax:
                *va_arg(info->args, intmax_t*) = static_cast<intmax_t>(length);
                break;
            default:
                *va_arg(info->args, int*) = static_cast<int>(length);
                break;
            }
            success = true;
            break;
        case '%':
            success = FORMAT_CHAR(info, ch);
            break;
        default:
            success = true;
            break;
        }

        return success ? format : nullptr;
    }
}}}

namespace up { namespace detail
{
    UPHIDDEN
    int NAME(INFO* UPRESTRICT info, CHAR const* UPRESTRICT format) noexcept {
        size_t remaining, final_length;
        CHAR* cursor, * end, * buffer;
        CHAR ch;
        
        assert(format && info && (info->cursor || !(info->end - info->cursor)));
        end = info->end;

        for (;;) {
            cursor = info->cursor;
            for (ch = *format++; ch && (ch != '%'); ch = *format++) {
                if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, ch))) {
                    return -1;
                }
            }
            info->cursor = cursor;
            if (!ch) {
                break;
            }
            format = dispatch(info, format);
            if (UPUNLIKELY(!format)) {
                return -1;
            }
        }

        buffer = info->buffer;
        remaining = cursor - info->buffer;
        final_length = remaining + info->length;
        if (UPUNLIKELY(final_length > INT_MAX)) {
            errno = EOVERFLOW;
            return -1;
        }

        if (info->stream) {
            if (UPUNLIKELY(fwrite(buffer, sizeof(CHAR), remaining, info->stream) != remaining)) {
                return -1;
            }
            info->cursor = buffer;
            info->length = final_length;
        }
        else if (final_length < info->buffer_length) {
            buffer[final_length] = CHR('\0');
        }
        else if (info->buffer_length > 0) {
            final_length = info->buffer_length - 1;
            buffer[final_length] = CHR('\0');
        }

        return static_cast<int>(final_length);
    }
}}
