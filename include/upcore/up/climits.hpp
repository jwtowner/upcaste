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

#ifndef UP_CLIMITS_HPP
#define UP_CLIMITS_HPP

#include <up/prolog.hpp>
#include <climits>

#define SHORT_BIT (sizeof(short) * CHAR_BIT)
#define INT_BIT (sizeof(int) * CHAR_BIT)
#define LONG_BIT (sizeof(long) * CHAR_BIT)
#define LLONG_BIT (sizeof(long long) * CHAR_BIT)
#define UCHAR_BIT (sizeof(unsigned char) * CHAR_BIT)
#define USHORT_BIT (sizeof(unsigned short) * CHAR_BIT)
#define UINT_BIT (sizeof(unsigned int) * CHAR_BIT)
#define ULONG_BIT (sizeof(unsigned long) * CHAR_BIT)
#define ULLONG_BIT (sizeof(unsigned long long) * CHAR_BIT)
#define SIZE_BIT (sizeof(size_t) * CHAR_BIT)
#define SSIZE_BIT (sizeof(size_t) * CHAR_BIT)
#define PTRDIFF_BIT (sizeof(ptrdiff_t) * CHAR_BIT)
#define WCHAR_BIT (sizeof(wchar_t) * CHAR_BIT)

#endif
