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

#if defined(UP_HAS_STDC_WCHAR) && !defined(UP_HAS_STDC_INTTYPES_C99) && !defined(UP_HAS_STDC_INTTYPES_CXX11)

#include <up/cinttypes.hpp>

#define NAME wcstoumax
#define RTYPE uintmax_t
#define STYPE intmax_t
#define UTYPE uintmax_t
#define UTYPE_MAX ((UTYPE)UINTMAX_MAX)
#define UTYPE_NEG_MAX (((UTYPE)INTMAX_MAX) + 1)
#define UTYPE_OVERFLOW UTYPE_MAX
#define UTYPE_NEG_OVERFLOW 1
#define CHAR wchar_t
#define UCHAR wint_t
#define ISSPACE(c) iswspace(c)
#include "../cstdlib/strtoint.inl"

#endif
