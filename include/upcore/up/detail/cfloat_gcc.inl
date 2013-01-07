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

#ifndef UP_DETAIL_CFLOAT_GCC_INL
#define UP_DETAIL_CFLOAT_GCC_INL

#ifndef UP_CFLOAT_HPP
#   error "Do not include this header directly. Instead include <up/cfloat.hpp>"
#endif

#ifndef DECIMAL_DIG
#   define DECIMAL_DIG __DECIMAL_DIG__
#endif

#ifndef FLT_EVAL_METHOD
#   define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif

#define FLT_INFINITY    __builtin_inff()
#define FLT_NAN         __builtin_nanf("")
#define DBL_INFINITY    __builtin_inf()
#define DBL_NAN         __builtin_nan("")
#define LDBL_INFINITY   __builtin_infl()
#define LDBL_NAN        __builtin_nanl("")

#endif
