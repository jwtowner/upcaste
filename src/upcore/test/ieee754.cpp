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

#include <up/ieee754.hpp>
#include <up/cstdint.hpp>
#include <up/test.hpp>

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(disable:4127) // conditional expression is constant
#endif

namespace ieee754
{
    UP_TEST_CASE(ieee754) {
        require(sizeof(up::ieee754_binary16) == sizeof(uint16_t));
        require(sizeof(up::ieee754_binary32) == sizeof(uint32_t));
        require(sizeof(up::ieee754_binary64) == sizeof(uint64_t));
        require(sizeof(up::ieee754_binary128) == (sizeof(uint64_t) * 2));
#if (LDBL_MANT_DIG == 64) && (FLT_RADIX == 2)
        require(sizeof(up::ieee754_binary96) == sizeof(long double));
#else
        require(sizeof(up::ieee754_binary96) == (sizeof(uint32_t) * 4));
#endif
    }
}
