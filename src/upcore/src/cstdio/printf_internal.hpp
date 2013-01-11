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

#ifndef UP_CSTDIO_PRINTF_INTERNAL_HPP
#define UP_CSTDIO_PRINTF_INTERNAL_HPP

#include <up/cstdarg.hpp>
#include <up/cstdio.hpp>
#ifdef UP_HAS_STDC_LOCALE
#   include <up/clocale.hpp>
#endif

namespace up { namespace detail
{
    // conversion type
    enum print_convert
    {
        print_convert_default,
        print_convert_char,
        print_convert_short,
        print_convert_long,
        print_convert_llong,
        print_convert_ldouble,
        print_convert_size,
        print_convert_ptrdiff,
        print_convert_int32,
        print_convert_int64,
        print_convert_intmax
    };

    // conversion flags
    enum print_flag
    {
        print_flag_minus    = 0x0001,
        print_flag_plus     = 0x0002,
        print_flag_space    = 0x0004,
        print_flag_number   = 0x0008,
        print_flag_zero     = 0x0010,
        print_flag_quote    = 0x0020,
        print_flag_upper    = 0x0040,
        print_flag_signed   = 0x0080,
        print_flag_pointer  = 0x0100,
        print_flag_style_a  = 0x0200,
        print_flag_style_e  = 0x0400,
        print_flag_style_g  = 0x0800
    };

    struct UPHIDDEN print_info
    {
        char* cursor;
        char* end;
        char* buffer;
        size_t buffer_length;
        size_t length;
        va_list args;
        FILE* stream;
#ifdef UP_HAS_STDC_LOCALE
        lconv const* lc;
#endif
    };

    extern UPHIDDEN UPNONNULLALL
    int fast_print_format(print_info* UPRESTRICT info, char const* UPRESTRICT format) noexcept;

    extern UPHIDDEN UPNONNULLALL
    int print_format(print_info* UPRESTRICT info, char const* UPRESTRICT format) noexcept;

    extern UPHIDDEN UPNONNULLALL
    ptrdiff_t print_format_flush(print_info* UPRESTRICT info) noexcept;

    extern UPHIDDEN UPNONNULL(1)
    bool print_format_string(print_info* UPRESTRICT info, char const* UPRESTRICT s, int width, int precision, unsigned int flags) noexcept;

    inline UPALWAYSINLINE UPNONNULL(1)
    bool print_format_char(print_info* UPRESTRICT info, char*& cursor, char*& end, char ch) noexcept {
        while (cursor >= end) {
            if (!end) {
                ++cursor;
                return true;
            }
            info->cursor = cursor;
            if (UPUNLIKELY(print_format_flush(info) < 0)) {
                return false;
            }
            cursor = info->cursor;
            end = info->end;
        }
        *cursor++ = ch;
        return true;
    }

    inline UPALWAYSINLINE UPNONNULL(1)
    bool print_format_char(print_info* UPRESTRICT info, char ch) noexcept {
        char* cursor = info->cursor, * end = info->end;
        if (UPUNLIKELY(!print_format_char(info, cursor, end, ch))) {
            return false;
        }
        info->cursor = cursor;
        return true;
    }
}}

#endif
