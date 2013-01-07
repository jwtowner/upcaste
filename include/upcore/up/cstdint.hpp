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

#ifndef UP_CSTDINT_HPP
#define UP_CSTDINT_HPP

#include <up/prolog.hpp>

#ifndef __STDC_CONSTANT_MACROS
#   define __STDC_CONSTANT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#   define __STDC_LIMIT_MACROS
#endif

#include <stdint.h>

#ifndef SSIZE_MAX
#   define SSIZE_MAX (SIZE_MAX/2)
#endif
#ifndef SSIZE_MIN
#   define SSIZE_MIN (-SSIZE_MAX-1)
#endif

namespace up
{
    using ::int_least8_t;
    using ::int_least16_t;
    using ::int_least32_t;
    using ::int_least64_t;
    using ::uint_least8_t;
    using ::uint_least16_t;
    using ::uint_least32_t;
    using ::uint_least64_t;
    using ::int_fast8_t;
    using ::int_fast16_t;
    using ::int_fast32_t;
    using ::int_fast64_t;
    using ::uint_fast8_t;
    using ::uint_fast16_t;
    using ::uint_fast32_t;
    using ::uint_fast64_t;
    using ::int8_t;
    using ::int16_t;
    using ::int32_t;
    using ::int64_t;
    using ::uint8_t;
    using ::uint16_t;
    using ::uint32_t;
    using ::uint64_t;
    using ::intmax_t;
    using ::uintmax_t;
    using ::intptr_t;
	using ::uintptr_t;
}

#endif
