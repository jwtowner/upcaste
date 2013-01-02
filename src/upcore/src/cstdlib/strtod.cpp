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

#ifndef UP_HAS_STDC_STRTOD

#define NAME strtod
#define RTYPE double
#define ZERO_VALUE 0.0
#define ONE_VALUE 1.0
#define TWO_VALUE 2.0
#define TEN_VALUE 10.0
#define SIXTEEN_VALUE 16.0
#define EPSILON_VALUE DBL_EPSILON
#define MAX_VALUE DBL_MAX
#define INFINITY_VALUE DBL_INFINITY
#define NAN_VALUE DBL_NAN
#define NAN_TAG(tag) math::nan(tag)
#define ISSPACE(c) isspace(c)
#define ISRADIX(s) (strncmp(s, radix, radix_length) == 0)
#define RADIX_LENGTH radix_length

#define DECLARE_RADIX_VARIABLES \
    struct lconv* locale_data; \
    char const* radix; \
    size_t radix_length; \

#define INIT_RADIX_VARIABLES \
    locale_data = localeconv(); \
    assert(locale_data); \
    radix = locale_data->decimal_point; \
    assert(radix); \
    radix_length = strlen(radix); \
    assert(radix_length > 0); \

#include "strtoreal.inl"

#endif
