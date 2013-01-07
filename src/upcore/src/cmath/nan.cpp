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

#include <up/prolog.hpp>

#ifndef UP_HAS_STDC_MATH_C99

#include <up/cmath.hpp>
#include <up/cstdlib.hpp>
#include <up/ieee754.hpp>

namespace up { namespace math
{
    LIBUPCOREAPI UPNONNULLALL
    double nan(char const* tag) noexcept {
#if (DBL_MANT_DIG == 53) && (FLT_RADIX == 2)
        ieee754_binary64 result;
        int64_t tag_value;
        
        tag_value = fast_strtoi64(tag, nullptr, 0);
        if (tag_value < 0) {
            tag_value = 0;
        }
        else if (tag_value > 0x0007FFFFFFFFFFFFull) {
            tag_value = 0x0007FFFFFFFFFFFFull;
        }

        result.ieee_nan.negative = 0;
        result.ieee_nan.exponent = 0x7FF;
        result.ieee_nan.quiet_nan = 1;
        result.ieee_nan.mantissa = static_cast<unsigned long long>(tag_value);
        return result.d;
#else
#   error "Double-precision floating-point format not yet supported!";
#endif
    }
}}

#endif
