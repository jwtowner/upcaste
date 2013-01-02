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


#include <up/math/uint4.hpp>
#include <up/test.hpp>


namespace math_uint4
{
    using namespace ::up::math;

    UP_TEST_CASE(broadcast) {
        uint4 const a = broadcast<uint4>(10);
        require(10 == getx(a));
        require(10 == gety(a));
        require(10 == getz(a));
        require(10 == getw(a));

        uint32_t const c = 2;
        uint4 const b = loadbroadcast<uint4>(&c);
        require(2 == getx(b));
        require(2 == gety(b));
        require(2 == getz(b));
        require(2 == getw(b));
    }

    UP_TEST_CASE(make) {
        uint4 const a = make<uint4>(1, 2, 3, 4);
        require(1 == get(a, 0));
        require(2 == get(a, 1));
        require(3 == get(a, 2));
        require(4 == get(a, 3));

        uint4 const b = make<uint4>(1, 2, 3);
        require(1 == get(b, 0));
        require(2 == get(b, 1));
        require(3 == get(b, 2));
        require(0 == get(b, 3));

        uint4 const c = make<uint4>(1, 2);
        require(1 == get(c, 0));
        require(2 == get(c, 1));
        require(0 == get(c, 2));
        require(0 == get(c, 3));
    }

    UP_TEST_CASE(put) {
        uint4 a = make<uint4>(10, 20, 30, 40);
        a = put(a, 4, 0);
        a = put(a, 3, 1);
        a = put(a, 2, 2);
        a = put(a, 1, 3);
        require(4 == getx(a));
        require(3 == gety(a));
        require(2 == getz(a));
        require(1 == getw(a));
        a = put(a, 1, 0);
        a = put(a, 2, 1);
        a = put(a, 3, 2);
        a = put(a, 4, 3);
        require(1 == getx(a));
        require(2 == gety(a));
        require(3 == getz(a));
        require(4 == getw(a));
    }

    UP_TEST_CASE(splat) {
        uint4 const v = make<uint4>(1, 2, 3, 4);
        uint4 a, b, c, d;
        
        a = splat(v, 0);
        b = splat(v, 1);
        c = splat(v, 2);
        d = splat(v, 3);
        require(all(isequal(a, broadcast<uint4>(1))));
        require(all(isequal(b, broadcast<uint4>(2))));
        require(all(isequal(c, broadcast<uint4>(3))));
        require(all(isequal(d, broadcast<uint4>(4))));
        
        a = splatx(v);
        b = splaty(v);
        c = splatz(v);
        d = splatw(v);
        require(all(isequal(a, broadcast<uint4>(1))));
        require(all(isequal(b, broadcast<uint4>(2))));
        require(all(isequal(c, broadcast<uint4>(3))));
        require(all(isequal(d, broadcast<uint4>(4))));
    }

    UP_TEST_CASE(arithmetic) {
        uint4 a = make<uint4>(10, 20, 30, 50);

        a = add(a, broadcast<uint4>(1));
        require(all(isequal(a, make<uint4>(11, 21, 31, 51))));

        a = sub(a, broadcast<uint4>(3));
        require(all(isequal(a, make<uint4>(8, 18, 28, 48))));

        a = mul(a, broadcast<uint4>(2));
        require(all(isequal(a, make<uint4>(16, 36, 56, 96))));

        a = div(a, broadcast<uint4>(4));
        require(all(isequal(a, make<uint4>(4, 9, 14, 24))));
    }

    UP_TEST_CASE(add_sat) {
        uint4 a = make<uint4>(0, 1, 3, 5);
        uint4 b;

        b = add_sat(a, uniform<uint4>::max);
        require(all(isequal(b, broadcast<uint4>(UINT32_MAX))));

        b = add_sat(a, make<uint4>(UINT32_MAX, 50, UINT32_MAX, 50));
        require(all(isequal(b, make<uint4>(UINT32_MAX, 51, UINT32_MAX, 55))));

        b = add_sat(uniform<uint4>::max, uniform<uint4>::max);
        require(all(isequal(b, broadcast<uint4>(UINT32_MAX))));

        b = add_sat(uniform<uint4>::max, a);
        require(all(isequal(b, broadcast<uint4>(UINT32_MAX))));
    }
/*
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
*/
    UP_TEST_CASE(abs_diff) {
        uint4 a = make<uint4>(1, 2, 0, 5);
        uint4 b = make<uint4>(10, 8, 5, 20);
        uint4 c;

        c = abs_diff(a, b);
        require(all(isequal(c, make<uint4>(9, 6, 5, 15))));

        c = abs_diff(b, a);
        require(all(isequal(c, make<uint4>(9, 6, 5, 15))));
    }

    UP_TEST_CASE(hadd) {
        uint4 c;

        c = hadd(make<uint4>(16, 31, 7, 8), make<uint4>(24, 12, 18, 9));
        require(all(isequal(c, make<uint4>(20, 21, 12, 8))));

        c = hadd(uniform<uint4>::one, uniform<uint4>::zero);
        require(all(isequal(c, uniform<uint4>::zero)));

        c = hadd(uniform<uint4>::zero, uniform<uint4>::zero);
        require(all(isequal(c, uniform<uint4>::zero)));

        c = hadd(uniform<uint4>::max, uniform<uint4>::max);
        require(all(isequal(c, uniform<uint4>::max)));
    }

    UP_TEST_CASE(rhadd) {
        uint4 c;

        c = rhadd(make<uint4>(1, 2, 0, 5), make<uint4>(10, 8, 5, 20));
        require(all(isequal(c, make<uint4>(6, 5, 3, 13))));

        c = rhadd(uniform<uint4>::one, uniform<uint4>::zero);
        require(all(isequal(c, uniform<uint4>::one)));

        c = rhadd(uniform<uint4>::zero, uniform<uint4>::zero);
        require(all(isequal(c, uniform<uint4>::zero)));

        c = rhadd(uniform<uint4>::max, uniform<uint4>::max);
        require(all(isequal(c, uniform<uint4>::max)));
    }

    UP_TEST_CASE(rotate) {
        uint4 a, b;

        a = make<uint4>(0xEFEFEFEF, 0xFEFEFEFE, 0x01080108, 0x80108010);

        b = rotate(a, 1);
        require(all(isequal(b, make<uint4>(0xDFDFDFDF, 0xFDFDFDFD, 0x02100210, 0x00210021))));

        b = rotate(a, 4);
        require(all(isequal(b, make<uint4>(0xFEFEFEFE, 0xEFEFEFEF, 0x10801080, 0x01080108))));

        b = rotate(a, broadcast<uint4>(1));
        require(all(isequal(b, make<uint4>(0xDFDFDFDF, 0xFDFDFDFD, 0x02100210, 0x00210021))));

        b = rotate(a, broadcast<uint4>(4));
        require(all(isequal(b, make<uint4>(0xFEFEFEFE, 0xEFEFEFEF, 0x10801080, 0x01080108))));
    }

    UP_TEST_CASE(clamp) {
        uint4 a, b, b_expected;
        a = make<uint4>(1, 20, 13, 50);
        b_expected = make<uint4>(10, 20, 13, 22);
        b = clamp(a, broadcast<uint4>(10), broadcast<uint4>(22));
        require(all(isequal(b, b_expected)));
    }

    UP_TEST_CASE(max) {
        uint4 a, b, c, c_expected;
        a = make<uint4>(1, 30, 12, 55);
        b = make<uint4>(100, 13, 44, 2);
        c_expected = make<uint4>(100, 30, 44, 55);

        c = max(a, b);
        require(all(isequal(c, c_expected)));

        c = max(b, a);
        require(all(isequal(c, c_expected)));
    }

    UP_TEST_CASE(min) {
        uint4 a, b, c, c_expected;
        a = make<uint4>(1, 30, 12, 55);
        b = make<uint4>(100, 13, 44, 2);
        c_expected = make<uint4>(1, 13, 12, 2);
        
        c = min(a, b);
        require(all(isequal(c, c_expected)));

        c = min(b, a);
        require(all(isequal(c, c_expected)));
    }
}

