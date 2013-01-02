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

#include <up/cmath.hpp>

namespace up { namespace math
{
    namespace
    {
        alignas(UP_MAX_CACHE_LINE_SIZE) float const pow10f_table[] =
        {
	         1e0f,  1e1f,  1e2f,  1e3f,  1e4f,  1e5f,  1e6f,  1e7f,  1e8f,  1e9f,
            1e10f, 1e11f, 1e12f, 1e13f, 1e14f, 1e15f, 1e16f, 1e17f, 1e18f, 1e19f,
            1e20f, 1e21f, 1e22f, 1e23f, 1e24f, 1e25f, 1e26f, 1e27f, 1e28f, 1e29f
        };

        constexpr int pow10f_table_size = static_cast<int>(sizeof(pow10f_table) / sizeof(pow10f_table[0]));
    }

    LIBUPCOREAPI UPPURE
    float pow10f(int n) noexcept {
        bool negative;
        float r;

        negative = false;
        if (n < 0) {
            negative = true;
            n = -n;
        }

        if (n < pow10f_table_size) {
            r = pow10f_table[n];
        }
        else {
            r = pow10f_table[pow10f_table_size - 1];
            for (;;) {
                n -= pow10f_table_size - 1;
                if (n < pow10f_table_size) {
                    r *= pow10f_table[n];
                    break;
                }
                r *= pow10f_table[pow10f_table_size - 1];
            }
        }

        return negative ? (1.0f / r) : r;
    }
}}
