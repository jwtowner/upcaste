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
#include <cstdint>

#ifndef UP_HAS_POSIX_SSIZE_T
#   define SSIZE_MAX (SIZE_MAX / 2)
#endif

namespace up
{
    using ::std::int_least8_t;
    using ::std::int_least16_t;
    using ::std::int_least32_t;
    using ::std::int_least64_t;
    using ::std::uint_least8_t;
    using ::std::uint_least16_t;
    using ::std::uint_least32_t;
    using ::std::uint_least64_t;
    using ::std::int_fast8_t;
    using ::std::int_fast16_t;
    using ::std::int_fast32_t;
    using ::std::int_fast64_t;
    using ::std::uint_fast8_t;
    using ::std::uint_fast16_t;
    using ::std::uint_fast32_t;
    using ::std::uint_fast64_t;
    using ::std::int8_t;
    using ::std::int16_t;
    using ::std::int32_t;
    using ::std::int64_t;
    using ::std::uint8_t;
    using ::std::uint16_t;
    using ::std::uint32_t;
    using ::std::uint64_t;
    using ::std::intmax_t;
    using ::std::uintmax_t;
    using ::std::intptr_t;
	using ::std::uintptr_t;
}

#endif
