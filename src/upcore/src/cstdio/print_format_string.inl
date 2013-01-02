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
#include <up/cstring.hpp>
#include <up/climits.hpp>

namespace up { namespace detail
{
    UPHIDDEN UPNONNULLALL
    bool NAME(INFO* UPRESTRICT info, CHAR const* UPRESTRICT s, int width, int precision, unsigned int flags) noexcept {
        int padlength, slength;
        CHAR* cursor, * end;

        // determine string length based on precision
        if (s) {
            slength = static_cast<int>(strnlen(s, (precision == -1) ? INT_MAX : precision));
        }
        else if ((precision == -1) || (precision >= 6)) {
            s = STR("(null)");
            slength = 6;
        }
        else {
            slength = 0;
        }

        // determine pad length
        padlength = width - slength;
        if (padlength < 0) {
            padlength = 0;
        }
        if (flags & print_flag_minus) {
            padlength = -padlength;
        }

        cursor = info->cursor;
        end = info->end;

        // leading spaces
        for ( ; padlength > 0; --padlength) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR(' ')))) {
                return false;
            }
        }

        // string characters
        for ( ; slength > 0; ++s, --slength) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, *s))) {
                return false;
            }
        }

        // trailing spaces
        for ( ; padlength < 0; ++padlength) {
            if (UPUNLIKELY(!FORMAT_CHAR(info, cursor, end, CHR(' ')))) {
                return false;
            }
        }

        info->cursor = cursor;
        return true;
    }
}}
