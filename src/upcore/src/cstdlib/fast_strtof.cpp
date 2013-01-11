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

#define NAME                    fast_strtof
#define REAL                    float
#define REAL_ZERO               0.0f
#define REAL_ONE                1.0f
#define REAL_TWO                2.0f
#define REAL_TEN                10.0f
#define REAL_SIXTEEN            16.0f
#define REAL_EPSILON            FLT_EPSILON
#define REAL_INFINITY           FLT_INFINITY
#define REAL_NAN                FLT_NAN
#define REAL_DIG                FLT_DIG
#define REAL_MANT_DIG           FLT_MANT_DIG
#define REAL_MAX                FLT_MAX
#define REAL_MAX_10_EXP         FLT_MAX_10_EXP
#define REAL_MAX_EXP            FLT_MAX_EXP
#define REAL_MIN                FLT_MIN
#define REAL_MIN_10_EXP         FLT_MIN_10_EXP
#define REAL_MIN_EXP            FLT_MIN_EXP
#define ISSPACE(c)              ((c != '\0') && (c <= ' '))
#define ISRADIX(s)              (*s == '.')
#define RADIX_LENGTH            1
#define DECLARE_RADIX_VARIABLES
#define INIT_RADIX_VARIABLES
#include "strtoreal.inl"
