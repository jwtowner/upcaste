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

#include <up/math/int4.hpp>
#include <up/test.hpp>

namespace math_int4
{
    using namespace ::up::math;

    UP_TEST_CASE(broadcast) {
        int4 const a = broadcast<int4>(-10);
        require(-10 == getx(a));
        require(-10 == gety(a));
        require(-10 == getz(a));
        require(-10 == getw(a));

        int32_t const c = 2;
        int4 const b = loadbroadcast<int4>(&c);
        require(2 == getx(b));
        require(2 == gety(b));
        require(2 == getz(b));
        require(2 == getw(b));
    }

    UP_TEST_CASE(make) {
        int4 const a = make<int4>(1, 2, 3, 4);
        require(1 == get(a, 0));
        require(2 == get(a, 1));
        require(3 == get(a, 2));
        require(4 == get(a, 3));

        int4 const b = make<int4>(1, 2, 3);
        require(1 == get(b, 0));
        require(2 == get(b, 1));
        require(3 == get(b, 2));
        require(0 == get(b, 3));

        int4 const c = make<int4>(1, 2);
        require(1 == get(c, 0));
        require(2 == get(c, 1));
        require(0 == get(c, 2));
        require(0 == get(c, 3));
    }

    UP_TEST_CASE(put) {
        int4 a = make<int4>(1, 2, 3, 4);
        a = put(a, -4, 0);
        a = put(a, -3, 1);
        a = put(a, -2, 2);
        a = put(a, -1, 3);
        require(-4 == getx(a));
        require(-3 == gety(a));
        require(-2 == getz(a));
        require(-1 == getw(a));
        a = put(a, -1, 0);
        a = put(a, -2, 1);
        a = put(a, -3, 2);
        a = put(a, -4, 3);
        require(-1 == getx(a));
        require(-2 == gety(a));
        require(-3 == getz(a));
        require(-4 == getw(a));
    }

    UP_TEST_CASE(splat) {
        int4 const v = make<int4>(1, 2, 3, 4);
        int4 a, b, c, d;
        
        a = splat(v, 0);
        b = splat(v, 1);
        c = splat(v, 2);
        d = splat(v, 3);
        require(all(isequal(a, broadcast<int4>(1))));
        require(all(isequal(b, broadcast<int4>(2))));
        require(all(isequal(c, broadcast<int4>(3))));
        require(all(isequal(d, broadcast<int4>(4))));
        
        a = splatx(v);
        b = splaty(v);
        c = splatz(v);
        d = splatw(v);
        require(all(isequal(a, broadcast<int4>(1))));
        require(all(isequal(b, broadcast<int4>(2))));
        require(all(isequal(c, broadcast<int4>(3))));
        require(all(isequal(d, broadcast<int4>(4))));
    }

    UP_TEST_CASE(arithmetic) {
        int4 a = make<int4>(1, -2, 3, 5);

        a = add(a, broadcast<int4>(1));
        require(all(isequal(a, make<int4>(2, -1, 4, 6))));

        a = sub(a, broadcast<int4>(3));
        require(all(isequal(a, make<int4>(-1, -4, 1, 3))));

        a = mul(a, broadcast<int4>(2));
        require(all(isequal(a, make<int4>(-2, -8, 2, 6))));

        a = div(a, broadcast<int4>(-2));
        require(all(isequal(a, make<int4>(1, 4, -1, -3))));
    }

    UP_TEST_CASE(add_sat) {
        int4 a = make<int4>(-1, -2, 3, 5);
        int4 b;

        b = add_sat(a, uniform<int4>::max);
        require(all(isequal(b, make<int4>(INT32_MAX - 1, INT32_MAX - 2, INT32_MAX, INT32_MAX))));

        b = add_sat(a, uniform<int4>::min);
        require(all(isequal(b, make<int4>(INT32_MIN, INT32_MIN, INT32_MIN + 3, INT32_MIN + 5))));

        b = add_sat(uniform<int4>::max, uniform<int4>::max);
        require(all(isequal(b, make<int4>(INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX))));

        b = add_sat(uniform<int4>::min, uniform<int4>::min);
        require(all(isequal(b, make<int4>(INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN))));
    }

    UP_TEST_CASE(sub_sat) {
        int4 a = make<int4>(-1, -2, 3, 5);
        int4 b;

        b = sub_sat(a, uniform<int4>::max);
        require(all(isequal(b, make<int4>(INT32_MIN, INT32_MIN, -INT32_MAX + 3, -INT32_MAX + 5))));

        b = sub_sat(a, uniform<int4>::min);
        require(all(isequal(b, make<int4>(INT32_MAX, INT32_MAX - 1, INT32_MAX, INT32_MAX))));

        b = sub_sat(uniform<int4>::max, uniform<int4>::min);
        require(all(isequal(b, make<int4>(INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX))));

        b = sub_sat(uniform<int4>::min, uniform<int4>::max);
        require(all(isequal(b, make<int4>(INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN))));
    }

    UP_TEST_CASE(neg) {
        int4 a = make<int4>(-1, INT32_MIN, 3, INT32_MAX);
        a = neg(a);
        require(all(isequal(a, make<int4>(1, INT32_MIN, -3, -INT32_MAX))));
    }

    UP_TEST_CASE(neg_sat) {
        int4 a = make<int4>(-1, INT32_MIN, 3, INT32_MAX);
        a = neg_sat(a);
        require(all(isequal(a, make<int4>(1, INT32_MAX, -3, -INT32_MAX))));
    }

    UP_TEST_CASE(abs) {
        int4 a = make<int4>(-1, INT32_MIN, 0, 5);
        a = abs(a);
        require(all(isequal(a, make<int4>(1, INT32_MIN, 0, 5))));
    }

    UP_TEST_CASE(abs_sat) {
        int4 a = make<int4>(-1, INT32_MIN, 0, 5);
        a = abs_sat(a);
        require(all(isequal(a, make<int4>(1, INT32_MAX, 0, 5))));
    }

    UP_TEST_CASE(nabs) {
        int4 a = make<int4>(-1, INT32_MIN, 0, 5);
        a = nabs(a);
        require(all(isequal(a, make<int4>(-1, INT32_MIN, 0, -5))));
    }

    UP_TEST_CASE(nabs_sat) {
        int4 a = make<int4>(-1, INT32_MIN, INT32_MAX, 5);
        a = nabs_sat(a);
        require(all(isequal(a, make<int4>(-1, INT32_MIN, -INT32_MAX, -5))));
    }

    UP_TEST_CASE(abs_diff) {
        int4 a = make<int4>(-1, -2, 0, 5);
        int4 b = make<int4>(-10, 8, 5, -20);
        int4 c;

        c = abs_diff(a, b);
        require(all(isequal(c, make<int4>(9, 10, 5, 25))));

        c = abs_diff(b, a);
        require(all(isequal(c, make<int4>(9, 10, 5, 25))));
    }

    UP_TEST_CASE(hadd) {
        int4 c;

        c = hadd(make<int4>(-1, -2, 0, 5), make<int4>(-10, 8, 5, -20));
        require(all(isequal(c, make<int4>(-6, 3, 2, -8))));

        c = hadd(make<int4>(16, 31, 7, 8), make<int4>(24, 12, 18, 9));
        require(all(isequal(c, make<int4>(20, 21, 12, 8))));

        c = hadd(uniform<int4>::one, uniform<int4>::zero);
        require(all(isequal(c, uniform<int4>::zero)));

        c = hadd(uniform<int4>::min, uniform<int4>::min);
        require(all(isequal(c, uniform<int4>::min)));

        c = hadd(uniform<int4>::max, uniform<int4>::max);
        require(all(isequal(c, uniform<int4>::max)));
    }

    UP_TEST_CASE(rhadd) {
        int4 c;

        c = rhadd(make<int4>(-1, -2, 0, 5), make<int4>(-10, 8, 5, -20));
        require(all(isequal(c, make<int4>(-5, 3, 3, -7))));

        c = rhadd(make<int4>(16, 31, 7, 8), make<int4>(24, 12, 18, 9));
        require(all(isequal(c, make<int4>(20, 22, 13, 9))));

        c = rhadd(uniform<int4>::one, uniform<int4>::zero);
        require(all(isequal(c, uniform<int4>::one)));

        c = rhadd(uniform<int4>::min, uniform<int4>::min);
        require(all(isequal(c, uniform<int4>::min)));

        c = rhadd(uniform<int4>::max, uniform<int4>::max);
        require(all(isequal(c, uniform<int4>::max)));
    }

    UP_TEST_CASE(rotate) {
        int4 a, b;

        a = make<int4>(0xEFEFEFEF, 0xFEFEFEFE, 0x01080108, 0x80108010);

        b = rotate(a, 1);
        require(all(isequal(b, make<int4>(0xDFDFDFDF, 0xFDFDFDFD, 0x02100210, 0x00210021))));

        b = rotate(a, 4);
        require(all(isequal(b, make<int4>(0xFEFEFEFE, 0xEFEFEFEF, 0x10801080, 0x01080108))));

        b = rotate(a, broadcast<int4>(1));
        require(all(isequal(b, make<int4>(0xDFDFDFDF, 0xFDFDFDFD, 0x02100210, 0x00210021))));

        b = rotate(a, broadcast<int4>(4));
        require(all(isequal(b, make<int4>(0xFEFEFEFE, 0xEFEFEFEF, 0x10801080, 0x01080108))));
    }

    UP_TEST_CASE(clamp) {
        int4 a, b, b_expected;
        a = make<int4>(1, -2, 3, -5);
        b_expected = make<int4>(1, -1, 2, -1);
        b = clamp(a, broadcast<int4>(-1), broadcast<int4>(2));
        require(all(isequal(b, b_expected)));
    }

    UP_TEST_CASE(max) {
        int4 a, b, c, c_expected;
        a = make<int4>(1, -3, 2, 3);
        b = make<int4>(100, -99, 2, 2);
        c_expected = make<int4>(100, -3, 2, 3);

        c = max(a, b);
        require(all(isequal(c, c_expected)));

        c = max(b, a);
        require(all(isequal(c, c_expected)));
    }

    UP_TEST_CASE(min) {
        int4 a, b, c, c_expected;
        a = make<int4>(1, -3, 2, 3);
        b = make<int4>(100, -99, 2, 2);
        c_expected = make<int4>(1, -99, 2, 2);
        
        c = min(a, b);
        require(all(isequal(c, c_expected)));

        c = min(b, a);
        require(all(isequal(c, c_expected)));
    }
}
