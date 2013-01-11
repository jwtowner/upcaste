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

#define NAME                    fast_strtod
#define REAL                    double
#define REAL_ZERO               0.0
#define REAL_ONE                1.0
#define REAL_TWO                2.0
#define REAL_TEN                10.0
#define REAL_SIXTEEN            16.0
#define REAL_EPSILON            DBL_EPSILON
#define REAL_INFINITY           DBL_INFINITY
#define REAL_NAN                DBL_NAN
#define REAL_DIG                DBL_DIG
#define REAL_MANT_DIG           DBL_MANT_DIG
#define REAL_MAX                DBL_MAX
#define REAL_MAX_10_EXP         DBL_MAX_10_EXP
#define REAL_MAX_EXP            DBL_MAX_EXP
#define REAL_MIN                DBL_MIN
#define REAL_MIN_10_EXP         DBL_MIN_10_EXP
#define REAL_MIN_EXP            DBL_MIN_EXP
#define ISSPACE(c)              ((c != '\0') && (c <= ' '))
#define ISRADIX(s)              (*s == '.')
#define RADIX_LENGTH            1
#define DECLARE_RADIX_VARIABLES
#define INIT_RADIX_VARIABLES
#include "strtoreal.inl"
