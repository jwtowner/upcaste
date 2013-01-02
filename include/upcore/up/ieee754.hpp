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

#ifndef UP_IEEE754_HPP
#define UP_IEEE754_HPP

#include <up/cfloat.hpp>

namespace up
{
    constexpr int ieee754_binary16_bias = 0x0F;
    constexpr int ieee754_binary32_bias = 0x7F;
    constexpr int ieee754_binary64_bias = 0x3FF;
    constexpr int ieee754_binary96_bias = 0x3FFF;
    constexpr int ieee754_binary128_bias = 0x3FFF;

    union LIBUPCOREAPI ieee754_binary16
    {
        unsigned short u;
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned short mantissa:10;
            unsigned short exponent:5;
            unsigned short negative:1;
#else
            unsigned short negative:1;
            unsigned short exponent:5;
            unsigned short mantissa:10;
#endif
        } ieee;
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned short mantissa:9;
            unsigned short quiet_nan:1;
            unsigned short exponent:5;
            unsigned short negative:1;
#else
            unsigned short negative:1;
            unsigned short exponent:5;
            unsigned short quiet_nan:1;
            unsigned short mantissa:9;
#endif
        } ieee_nan;
    };

    union LIBUPCOREAPI ieee754_binary32
    {
        unsigned int u;
#if (FLT_MANT_DIG == 24) && (FLT_RADIX == 2)
        float f;
#endif
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned int mantissa:23;
            unsigned int exponent:8;
            unsigned int negative:1;
#else
            unsigned int negative:1;
            unsigned int exponent:8;
            unsigned int mantissa:23;
#endif
        } ieee;
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned int mantissa:22;
            unsigned int quiet_nan:1;
            unsigned int exponent:8;
            unsigned int negative:1;
#else
            unsigned int negative:1;
            unsigned int exponent:8;
            unsigned int quiet_nan:1;
            unsigned int mantissa:22;
#endif
        } ieee_nan;
    };

    union LIBUPCOREAPI ieee754_binary64
    {
        unsigned long long u;
#if (DBL_MANT_DIG == 53) && (DBL_RADIX == 2)
        double d;
#endif
#if (LDBL_MANT_DIG == 53) && (LDBL_RADIX == 2)
        long double ld;
#endif
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
	        unsigned long long mantissa:52;
	        unsigned long long exponent:11;
            unsigned long long negative:1;
#else
            unsigned long long negative:1;
	        unsigned long long exponent:11;
	        unsigned long long mantissa:52;
#endif
        } ieee;
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
	        unsigned long long mantissa:51;
            unsigned long long quiet_nan:1;
	        unsigned long long exponent:11;
            unsigned long long negative:1;
#else
            unsigned long long negative:1;
	        unsigned long long exponent:11;
            unsigned long long quiet_nan:1;
	        unsigned long long mantissa:51;
#endif
        } ieee_nan;
    };

    union LIBUPCOREAPI ieee754_binary96
    {
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned long long lo:64;
            unsigned long long hi:32;
#else
            unsigned long long hi:32;
            unsigned long long lo:64;
#endif
        } u;
#if (LDBL_MANT_DIG == 64) && (LDBL_RADIX == 2)
        long double ld;
#endif
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
	        unsigned long long mantissa:64;
	        unsigned long long exponent:15;
            unsigned long long negative:1;
            unsigned long long empty:16;
#else
            unsigned long long negative:1;
	        unsigned long long exponent:15;
            unsigned long long empty:16;
            unsigned long long mantissa:64;
#endif
        } ieee;
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
  	        unsigned long long mantissa:62;
            unsigned long long quiet_nan:1;
            unsigned long long one:1;
	        unsigned long long exponent:15;
            unsigned long long negative:1;
            unsigned long long empty:16;
#else
            unsigned long long negative:1;
	        unsigned long long exponent:15;
            unsigned long long empty:16;
            unsigned long long one:1;
            unsigned long long quiet_nan:1;
            unsigned long long mantissa:62;
#endif
        } ieee_nan;
    };

    union LIBUPCOREAPI ieee754_binary128
    {
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned long long lo;
            unsigned long long hi;
#else
            unsigned long long hi;
            unsigned long long lo;
#endif
        } u;
#if (LDBL_MANT_DIG == 113) && (LDBL_RADIX == 2)
        long double ld;
#endif
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned long long mantissa1:64;
            unsigned long long mantissa0:48;
	        unsigned long long exponent:15;
            unsigned long long negative:1;
#else
            unsigned long long negative:1;
	        unsigned long long exponent:15;
	        unsigned long long mantissa0:48;
            unsigned long long mantissa1:64;
#endif
        } ieee;
        struct {
#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
            unsigned long long mantissa1:64;
            unsigned long long mantissa0:47;
            unsigned long long quiet_nan:1;
            unsigned long long exponent:15;
            unsigned long long negative:1;
#else
            unsigned long long negative:1;
	        unsigned long long exponent:15;
            unsigned long long quiet_nan:1;
            unsigned long long mantissa0:47;
            unsigned long long mantissa1:64;
#endif
        } ieee_nan;
    };
}

#endif
