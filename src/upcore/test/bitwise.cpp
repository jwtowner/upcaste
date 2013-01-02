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

#include <up/bitwise.hpp>
#include <up/test.hpp>

namespace bitwise
{
    UP_TEST_CASE(tzcount) {
        require(up::tzcount(1u) == 0u);
        require(up::tzcount(2u) == 1u);
        require(up::tzcount(4u) == 2u);
        require(up::tzcount(8u) == 3u);

        for (unsigned int i = 4; i < UINT_BIT; ++i) {
            require(up::tzcount(1u << i) == i);
        }
    }

    UP_TEST_CASE(lzcount) {
        require(up::lzcount(1u) == (UINT_BIT - 1u));
        require(up::lzcount(2u) == (UINT_BIT - 2u));
        require(up::lzcount(4u) == (UINT_BIT - 3u));
        require(up::lzcount(8u) == (UINT_BIT - 4u));

        for (unsigned int i = 4; i < UINT_BIT; ++i) {
            require(up::lzcount(1u << i) == (UINT_BIT - i - 1));
        }
    }

    UP_TEST_CASE(popcount) {
        require(up::popcount(0u) == 0u);
        require(up::popcount(1u) == 1u);
        require(up::popcount(2u) == 1u);
        require(up::popcount(3u) == 2u);
        require(up::popcount(4u) == 1u);
        require(up::popcount(5u) == 2u);
        require(up::popcount(6u) == 2u);
        require(up::popcount(7u) == 3u);
        require(up::popcount(8u) == 1u);

        for (unsigned int i = 1; i < UINT_BIT; ++i) {
            unsigned int x = i << 1;
            require(up::popcount((x & (unsigned int)(-(int)x)) - 1) == up::tzcount(x));
        }
    }

    UP_TEST_CASE(logceil2) {
        require(up::logceil2(0u) == 0u);
        require(up::logceil2(1u) == 0u);
        require(up::logceil2(2u) == 1u);
        require(up::logceil2(3u) == 2u);
        require(up::logceil2(4u) == 2u);
        require(up::logceil2(5u) == 3u);
        require(up::logceil2(6u) == 3u);
        require(up::logceil2(7u) == 3u);
        require(up::logceil2(8u) == 3u);

        for (unsigned int i = 2; i < UINT_BIT; ++i) {
            require(up::logceil2((1u << i) - 1) == i);
        }
    }

    UP_TEST_CASE(logfloor2) {
        require(up::logfloor2(0u) == 0u);
        require(up::logfloor2(1u) == 0u);
        require(up::logfloor2(2u) == 1u);
        require(up::logfloor2(3u) == 1u);
        require(up::logfloor2(4u) == 2u);
        require(up::logfloor2(5u) == 2u);
        require(up::logfloor2(6u) == 2u);
        require(up::logfloor2(7u) == 2u);
        require(up::logfloor2(8u) == 3u);

        for (unsigned int i = 2; i < UINT_BIT; ++i) {
            require(up::logfloor2((1u << i) - 1) == (i - 1));
        }
    }

    UP_TEST_CASE(ceil2) {
        require(up::ceil2(0u) == 0u);
        require(up::ceil2(1u) == 1u);
        require(up::ceil2(2u) == 2u);
        require(up::ceil2(3u) == 4u);
        require(up::ceil2(4u) == 4u);
        require(up::ceil2(5u) == 8u);
        require(up::ceil2(6u) == 8u);
        require(up::ceil2(7u) == 8u);
        require(up::ceil2(8u) == 8u);

        for (unsigned int i = 2; i < UINT_BIT; ++i) {
            require(up::ceil2((1u << i) - 1) == (1u << i));
        }
    }

    UP_TEST_CASE(floor2) {
        require(up::floor2(0u) == 0u);
        require(up::floor2(1u) == 1u);
        require(up::floor2(2u) == 2u);
        require(up::floor2(3u) == 2u);
        require(up::floor2(4u) == 4u);
        require(up::floor2(5u) == 4u);
        require(up::floor2(6u) == 4u);
        require(up::floor2(7u) == 4u);
        require(up::floor2(8u) == 8u);

        for (unsigned int i = 2; i < UINT_BIT; ++i) {
            require(up::floor2((1u << i) - 1) == (1u << (i - 1)));
        }
    }
}
