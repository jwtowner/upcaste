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

#include <up/math/int4.hpp>
#include <up/math/float4.hpp>
#include <up/test.hpp>

namespace math_float4
{
    using namespace ::up::math;

    inline float4 relative_error(float4 const a, float4 const b) noexcept {
        float4 u = add(copysign(uniform<float4>::epsilon, a), a);
        float4 v = add(copysign(uniform<float4>::epsilon, b), b);
        return fdim(uniform<float4>::one, div(u, v));
    }

    UP_TEST_CASE(broadcast) {
        float4 const a = broadcast<float4>(-10.0f);
        require_approx(-10.0f, getx(a), FLT_EPSILON);
        require_approx(-10.0f, gety(a), FLT_EPSILON);
        require_approx(-10.0f, getz(a), FLT_EPSILON);
        require_approx(-10.0f, getw(a), FLT_EPSILON);

        float const c = 2.0f;
        float4 const b = loadbroadcast<float4>(&c);
        require_approx(2.0f, getx(b), FLT_EPSILON);
        require_approx(2.0f, gety(b), FLT_EPSILON);
        require_approx(2.0f, getz(b), FLT_EPSILON);
        require_approx(2.0f, getw(b), FLT_EPSILON);
    }

    UP_TEST_CASE(make) {
        float4 const a = make<float4>(1.0f, 2.0f, 3.0f, 4.0f);
        require_approx(1.0f, get(a, 0), FLT_EPSILON);
        require_approx(2.0f, get(a, 1), FLT_EPSILON);
        require_approx(3.0f, get(a, 2), FLT_EPSILON);
        require_approx(4.0f, get(a, 3), FLT_EPSILON);
    }

    UP_TEST_CASE(put) {
        float4 a = make<float4>(1.0f, 2.0f, 3.0, 4.0f);
        a = put(a, -4.0f, 0);
        a = put(a, -3.0f, 1);
        a = put(a, -2.0f, 2);
        a = put(a, -1.0f, 3);
        require_approx(-4.0f, getx(a), FLT_EPSILON);
        require_approx(-3.0f, gety(a), FLT_EPSILON);
        require_approx(-2.0f, getz(a), FLT_EPSILON);
        require_approx(-1.0f, getw(a), FLT_EPSILON);
        a = putx(a, -1.0f);
        a = puty(a, -2.0f);
        a = putz(a, -3.0f);
        a = putw(a, -4.0f);
        require_approx(-1.0f, getx(a), FLT_EPSILON);
        require_approx(-2.0f, gety(a), FLT_EPSILON);
        require_approx(-3.0f, getz(a), FLT_EPSILON);
        require_approx(-4.0f, getw(a), FLT_EPSILON);
    }

    UP_TEST_CASE(splat) {
        float4 const v = make<float4>(1.0f, 2.0f, 3.0f, 4.0f);
        float4 a, b, c, d;
        
        a = splat(v, 0);
        b = splat(v, 1);
        c = splat(v, 2);
        d = splat(v, 3);
        require(all(isequal(a, broadcast<float4>(1.0f))));
        require(all(isequal(b, broadcast<float4>(2.0f))));
        require(all(isequal(c, broadcast<float4>(3.0f))));
        require(all(isequal(d, broadcast<float4>(4.0f))));
        
        a = splatx(v);
        b = splaty(v);
        c = splatz(v);
        d = splatw(v);
        require(all(isequal(a, broadcast<float4>(1.0f))));
        require(all(isequal(b, broadcast<float4>(2.0f))));
        require(all(isequal(c, broadcast<float4>(3.0f))));
        require(all(isequal(d, broadcast<float4>(4.0f))));
    }

    UP_TEST_CASE(isequal) {
        float4 a = make<float4>(1.0f, 2.0f, 3.0f, 4.0f);
        float4 b = make<float4>(1.0f, 2.0f, 3.0f, 4.0f);
        float4 c = broadcast<float4>(1.0f);
        float4 d = broadcast<float4>(-1.0f);
        int4 ab = isequal(a, b);
        int4 ac = isequal(a, c);
        int4 ad = isequal(a, d);
        require(all(ab));
        require(any(ab));
        require(!all(ac));
        require(any(ac));
        require(!all(ad));
        require(!any(ad));
    }

    UP_TEST_CASE(isnotequal) {
        float4 a = make<float4>(1.0f, 2.0f, 3.0f, 4.0f);
        float4 b = make<float4>(1.0f, 2.0f, 3.0f, 4.0f);
        float4 c = broadcast<float4>(1.0f);
        float4 d = broadcast<float4>(-1.0f);
        int4 ab = isnotequal(a, b);
        int4 ac = isnotequal(a, c);
        int4 ad = isnotequal(a, d);
        require(!all(ab));
        require(!any(ab));
        require(!all(ac));
        require(any(ac));
        require(all(ad));
        require(any(ad));
    }

    UP_TEST_CASE(isinf) {
        require(all(isinf(uniform<float4>::infinity)));
        require(all(isinf(neg((float4)uniform<float4>::infinity))));
        require(!any(isinf(uniform<float4>::quiet_nan)));
        require(!any(isinf(uniform<float4>::zero)));
        require(!any(isinf(uniform<float4>::negative_zero)));
        require(!any(isinf(uniform<float4>::one)));
        require(!any(isinf(uniform<float4>::negative_one)));
    }

    UP_TEST_CASE(isnan) {
        require(all(isnan(uniform<float4>::quiet_nan)));
        require(!any(isnan(uniform<float4>::infinity)));
        require(!any(isnan(uniform<float4>::zero)));
        require(!any(isnan(uniform<float4>::negative_zero)));
        require(!any(isnan(uniform<float4>::one)));
        require(!any(isnan(uniform<float4>::negative_one)));
    }
  
    UP_TEST_CASE(arithmetic) {
        float4 a = make<float4>(0.2f, -2.5f, 1.85f, 3.0f);

        a = add(a, broadcast<float4>(1.0f));
        require(all(islessequal(fdim(a, make<float4>(1.2f, -1.5f, 2.85f, 4.0f)), uniform<float4>::epsilon)));

        a = sub(a, broadcast<float4>(3.5f));
        require(all(islessequal(fdim(a, make<float4>(-2.3f, -5.0f, -0.65f, 0.5f)), uniform<float4>::epsilon)));

        a = neg(a);
        require(all(islessequal(fdim(a, make<float4>(2.3f, 5.0f, 0.65f, -0.5f)), uniform<float4>::epsilon)));

        a = mul(a, broadcast<float4>(2.0f));
        require(all(islessequal(fdim(a, make<float4>(4.6f, 10.0f, 1.3f, -1.0f)), mul(uniform<float4>::epsilon, broadcast<float4>(4.0f)))));

        a = div(a, broadcast<float4>(-2.0f));
        require(all(islessequal(fdim(a, make<float4>(-2.3f, -5.0f, -0.65f, 0.5f)), mul(uniform<float4>::epsilon, broadcast<float4>(4.0f)))));
    }
    
    UP_TEST_CASE(rsqrt) {
        float4 const epsilon8 = broadcast<float4>(3.90625000e-3f);
        float4 const epsilon10 = broadcast<float4>(9.76562500e-4f);
        float4 const epsilon24 = uniform<float4>::epsilon;
        float4 a;

        a = native_rsqrt(broadcast<float4>(4.0f));
        require(all(islessequal(fdim(a, broadcast<float4>(0.5f)), epsilon8)));

        a = half_rsqrt(broadcast<float4>(4.0f));
        require(all(islessequal(fdim(a, broadcast<float4>(0.5f)), epsilon10)));
        
        a = rsqrt(broadcast<float4>(4.0f));
        require(all(islessequal(fdim(a, broadcast<float4>(0.5f)), epsilon24)));
    }

    UP_TEST_CASE(clamp) {
        float4 a, b, b_expected;
        a = make<float4>(0.2f, -2.5f, 1.85f, 3.0f);
        b_expected = make<float4>(0.2f, -1.0f, 1.85f, 2.0f);
        b = clamp(a, broadcast<float4>(-1.0f), broadcast<float4>(2.0f));
        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(max) {
        float4 a, b, c, c_expected;
        a = make<float4>(0.2f, -2.5f, 1.85f, 3.0f);
        b = make<float4>(100.2f, -99.5f, 1.95f, 2.95f);
        c_expected = make<float4>(100.2f, -2.5f, 1.95f, 3.0f);

        c = max(a, b);
        require(all(islessequal(fdim(c, c_expected), uniform<float4>::epsilon)));

        c = max(b, a);
        require(all(islessequal(fdim(c, c_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(min) {
        float4 a, b, c, c_expected;
        a = make<float4>(0.2f, -2.5f, 1.85f, 3.0f);
        b = make<float4>(100.2f, -99.5f, 1.95f, 2.95f);
        c_expected = make<float4>(0.2f, -99.5f, 1.85f, 2.95f);
        
        c = min(a, b);
        require(all(islessequal(fdim(c, c_expected), uniform<float4>::epsilon)));

        c = min(b, a);
        require(all(islessequal(fdim(c, c_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(ceil) {
        float4 a, b, b_expected;

        a = make<float4>(1.2f, 2.5f, 3.8f, 3.0f);
        b_expected = make<float4>(2.0f, 3.0f, 4.0f, 3.0f);
        b = ceil(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));

        a = make<float4>(-1.2f, 3.5f, -3.8f, 2.6f);
        b_expected = make<float4>(-1.0f, 4.0f, -3.0f, 3.0f);
        b = ceil(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(floor) {
        float4 a, b, b_expected;

        a = make<float4>(1.2f, 2.5f, 3.8f, 3.0f);
        b_expected = make<float4>(1.0f, 2.0f, 3.0f, 3.0f);
        b = floor(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));

        a = make<float4>(-1.2f, 3.5f, -3.8f, 2.6f);
        b_expected = make<float4>(-2.0f, 3.0f, -4.0f, 2.0f);
        b = floor(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(rint) {
        float4 a, b, b_expected;

        a = make<float4>(1.2f, 2.5f, 3.8f, 3.0f);
        b_expected = make<float4>(1.0f, 2.0f, 4.0f, 3.0f);
        b = round(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));

        a = make<float4>(-1.2f, 3.5f, -3.8f, 2.6f);
        b_expected = make<float4>(-1.0f, 4.0f, -4.0f, 3.0f);
        b = round(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(round) {
        float4 a, b, b_expected;

        a = make<float4>(1.2f, 2.5f, 3.8f, 3.0f);
        b_expected = make<float4>(1.0f, 2.0f, 4.0f, 3.0f);
        b = round(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));

        a = make<float4>(-1.2f, 3.5f, -3.8f, 2.6f);
        b_expected = make<float4>(-1.0f, 4.0f, -4.0f, 3.0f);
        b = round(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));
    }

    UP_TEST_CASE(trunc) {
        float4 a, b, b_expected;
        
        a = make<float4>(1.2f, 2.5f, 3.8f, 3.0f);
        b_expected = make<float4>(1.0f, 2.0f, 3.0f, 3.0f);
        b = trunc(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));

        a = make<float4>(-1.2f, 3.5f, -3.8f, 2.6f);
        b_expected = make<float4>(-1.0f, 3.0f, -3.0f, 2.0f);
        b = trunc(a);

        require(all(islessequal(fdim(b, b_expected), uniform<float4>::epsilon)));
    }
/*
    UP_TEST_CASE(nextafter) {
        float4 a;

        a = nextafter(uniform<float4>::zero, uniform<float4>::one);
        require(all(isequal(a, uniform<float4>::epsilon)));

        a = nextafter(uniform<float4>::zero, uniform<float4>::negative_one);
        require(all(isequal(a, neg((float4)uniform<float4>::epsilon))));

        a = nextafter(uniform<float4>::one, uniform<float4>::one);
        require(all(isequal(a, uniform<float4>::one_plus_epsilon)));

        a = nextafter(uniform<float4>::one, uniform<float4>::negative_one);
        require(all(isequal(a, uniform<float4>::one_minus_epsilon)));

        a = nextafter(broadcast<float4>(10000.0f), uniform<float4>::one);
        require(all(isnotequal(a, broadcast<float4>(10000.0f))));

        a = nextafter(broadcast<float4>(10000.0f), uniform<float4>::negative_one);
        require(all(isnotequal(a, broadcast<float4>(10000.0f))));

        a = nextafter(uniform<float4>::infinity, uniform<float4>::one);
        require(all(isinf(a)));

        a = nextafter(uniform<float4>::infinity, uniform<float4>::negative_one);
        require(all(isinf(a)));

        a = nextafter(uniform<float4>::quiet_nan, uniform<float4>::one);
        require(all(isnan(a)));

        a = nextafter(uniform<float4>::quiet_nan, uniform<float4>::negative_one);
        require(all(isnan(a)));

        a = nextafter(uniform<float4>::one, uniform<float4>::quiet_nan);
        require(all(isnan(a)));

        a = nextafter(uniform<float4>::infinity, uniform<float4>::quiet_nan);
        require(all(isnan(a)));
    }
*/
    UP_TEST_CASE(native_trigonometry) {
        float4 const epsilon10 = broadcast<float4>(0.0009765625f);
        float4 a, b, c, d;
        float angle, ss, sc, x;

        // native_acos
        for (int i = -16; i <= 16; ++i) {
            x = static_cast<float>(i) / 16.0f;
            a = native_acos(broadcast<float4>(x));
            b = broadcast<float4>(acos(x));
            require(all(islessequal(fdim(a, b), epsilon10)));
        }

        // native_cos
        for (int i = -16; i <= 16; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 16.0f;
            a = native_cos(broadcast<float4>(angle));
            b = broadcast<float4>(cos(angle));
            require(all(islessequal(fdim(a, b), epsilon10)));
        }

        // native_sin
        for (int i = -16; i <= 16; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 16.0f;
            a = native_sin(broadcast<float4>(angle));
            b = broadcast<float4>(sin(angle));
            require(all(islessequal(fdim(a, b), epsilon10)));
        }

        // native_sincos
        for (int i = -16; i <= 16; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 16.0f;
            a = native_sincos(broadcast<float4>(angle), &c);
            ss = sincos(angle, &sc);
            b = broadcast<float4>(ss);
            d = broadcast<float4>(sc);
            require(all(islessequal(fdim(a, b), epsilon10)));
            require(all(islessequal(fdim(c, d), epsilon10)));
        }

        // native_tan
/*        for (int i = -16; i <= 16; ++i) {
            angle = FLT_PI_2 * static_cast<float>(i) / 16.0f;
            a = native_tan(broadcast<float4>(angle));
            b = div(sin(broadcast<float4>(angle)), cos(broadcast<float4>(angle)));
            require(all(islessequal(fdim(a, b), epsilon10)));
        }*/
    }

    UP_TEST_CASE(half_trigonometry) {
        float4 const epsilon10 = broadcast<float4>(0.0009765625f);
        float4 a, b, c, d;
        float angle, ss, sc;

        // half_cos
        for (int i = -128; i <= 128; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 8.0f;
            a = half_cos(broadcast<float4>(angle));
            b = broadcast<float4>(cos(angle));
            require(all(islessequal(fdim(a, b), epsilon10)));
        }

        // half_sin
        for (int i = -128; i <= 128; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 8.0f;
            a = half_sin(broadcast<float4>(angle));
            b = broadcast<float4>(sin(angle));
            require(all(islessequal(fdim(a, b), epsilon10)));
        }

        // half_sincos
        for (int i = -128; i <= 128; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 8.0f;
            a = half_sincos(broadcast<float4>(angle), &c);
            ss = sincos(angle, &sc);
            b = broadcast<float4>(ss);
            d = broadcast<float4>(sc);
            require(all(islessequal(fdim(a, b), epsilon10)));
            require(all(islessequal(fdim(c, d), epsilon10)));
        }
    }

    UP_TEST_CASE(trigonometry) {
        float4 const epsilon = uniform<float4>::epsilon;
        float4 const epsilon4 = mul(broadcast<float4>(4.0f), epsilon);
        float4 a, b, c, d;
        float angle, ss, sc;

        // cos
        for (int i = -16; i <= 16; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 8.0f;
            a = cos(broadcast<float4>(angle));
            b = broadcast<float4>(cos(angle));
            require(all(islessequal(fdim(a, b), ((-8 <= i) && (i <= 8)) ? epsilon : epsilon4)));
        }

        // sin
        for (int i = -16; i <= 16; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 8.0f;
            a = sin(broadcast<float4>(angle));
            b = broadcast<float4>(sin(angle));
            require(all(islessequal(fdim(a, b), ((-8 <= i) && (i <= 8)) ? epsilon : epsilon4)));
        }
        
        // sincos
        for (int i = -48; i <= 48; ++i) {
            angle = FLT_PI * static_cast<float>(i) / 8.0f;
            a = sincos(broadcast<float4>(angle), &c);
            ss = sincos(angle, &sc);
            b = broadcast<float4>(ss);
            d = broadcast<float4>(sc);
            require(all(islessequal(fdim(a, b), ((-8 <= i) && (i <= 8)) ? epsilon : epsilon4)));
            require(all(islessequal(fdim(c, d), ((-8 <= i) && (i <= 8)) ? epsilon : epsilon4)));
        }
    }

    UP_TEST_CASE(geometry2) {
        float4 a, b, q, r;
        
        a = make<float4>(1.0f, 2.5f);
        b = make<float4>(-5.0f, 10.0f);
 
        q = cross2(a, b);
        r = broadcast<float4>(22.5f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = cross2(b, a);
        r = broadcast<float4>(-22.5f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = dot2(a, b);
        r = broadcast<float4>(20.0f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = dot2(b, a);
        r = broadcast<float4>(20.0f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = rlength2(b);
        r = broadcast<float4>(1.0f / sqrt(125.0f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = length2(b);
        r = broadcast<float4>(sqrt(125.0f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = distance2(a, b);
        r = broadcast<float4>(sqrt(92.25f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = normalize2(a);
        r = make<float4>(1.0f/sqrt(1.0f+(2.5f*2.5f)), 2.5f/sqrt(1.0f+(2.5f*2.5f)));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
    }
    
    UP_TEST_CASE(fast_geometry2) {
        float const epsilon10 = 0.0009765625f;
        float4 a, b, q, r;
        
        a = make<float4>(1.0f, 2.5f);
        b = make<float4>(-5.0f, 10.0f);

        q = fast_rlength2(b);
        r = broadcast<float4>(1.0f / sqrt(125.0f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));

        q = fast_length2(b);
        r = broadcast<float4>(sqrt(125.0f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
        
        q = fast_distance2(a, b);
        r = broadcast<float4>(sqrt(92.25f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
        
        q = fast_normalize2(a);
        r = make<float4>(1.0f/sqrt(1.0f+(2.5f*2.5f)), 2.5f/sqrt(1.0f+(2.5f*2.5f)));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
    }

    UP_TEST_CASE(geometry3) {
        float4 a, b, q, r;
        
        a = make<float4>(1.0f, 2.5f, 6.0f);
        b = make<float4>(-5.0f, 10.0f, 20.0f);
 
        q = cross3(a, b);
        r = make<float4>(-10.0f, -50.0f, 22.5f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = cross3(b, a);
        r = make<float4>(10.0f, 50.0f, -22.5f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = dot3(a, b);
        r = broadcast<float4>(140.0f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = dot3(b, a);
        r = broadcast<float4>(140.0f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = rlength3(b);
        r = broadcast<float4>(1.0f/sqrt(525.0f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = length3(b);
        r = broadcast<float4>(sqrt(525.0f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = distance3(a, b);
        r = broadcast<float4>(sqrt(288.25f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = normalize3(a);
        r = make<float4>(1.0f/sqrt(43.25f), 2.5f/sqrt(43.25f), 6.0f/sqrt(43.25f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
    }
    
    UP_TEST_CASE(fast_geometry3) {
        float const epsilon10 = 0.0009765625f;
        float4 a, b, q, r;
        
        a = make<float4>(1.0f, 2.5f, 6.0f);
        b = make<float4>(-5.0f, 10.0f, 20.0f);

        q = fast_rlength3(b);
        r = broadcast<float4>(1.0f/sqrt(525.0f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));

        q = fast_length3(b);
        r = broadcast<float4>(sqrt(525.0f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
        
        q = fast_distance3(a, b);
        r = broadcast<float4>(sqrt(288.25f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
        
        q = fast_normalize3(a);
        r = make<float4>(1.0f/sqrt(43.25f), 2.5f/sqrt(43.25f), 6.0f/sqrt(43.25f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
    }

    UP_TEST_CASE(geometry4) {
        float4 a, b, q, r;
        
        a = make<float4>(1.0f, 2.5f, 6.0f, -4.0f);
        b = make<float4>(-5.0f, 10.0f, 20.0f, 8.0f);
        //c = make<float4>(4.0f, -3.0f, -5.0f, 2.0f);
 
/*        q = cross4(a, b, c);
        r = make<float4>(104.0f, -472.0f, 231.0f, 77.5f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = cross4(c, b, a);
        r = make<float4>(-104.0f, 472.0f, -231.0f, -77.5f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
*/

        q = dot4(a, b);
        r = broadcast<float4>(108.0f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
        
        q = dot4(b, a);
        r = broadcast<float4>(108.0f);
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = rlength4(b);
        r = broadcast<float4>(1.0f/sqrt(589.0f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = length4(b);
        r = broadcast<float4>(sqrt(589.0f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = distance4(a, b);
        r = broadcast<float4>(sqrt(432.25f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));

        q = normalize4(a);
        r = make<float4>(1.0f/sqrt(59.25f), 2.5f/sqrt(59.25f), 6.0f/sqrt(59.25f), -4.0f/sqrt(59.25f));
        require(all(islessequal(fdim(q, r), uniform<float4>::epsilon)));
    }
    
    UP_TEST_CASE(fast_geometry4) {
        float const epsilon10 = 0.0009765625f;
        float4 a, b, q, r;
        
        a = make<float4>(1.0f, 2.5f, 6.0f, -4.0f);
        b = make<float4>(-5.0f, 10.0f, 20.0f, 8.0f);

        q = fast_rlength4(b);
        r = broadcast<float4>(0.0412042822f);
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));

        q = fast_length4(b);
        r = broadcast<float4>(24.269322199f);
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
        
        q = fast_distance4(a, b);
        r = broadcast<float4>(20.790622886f);
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
        
        q = fast_normalize4(a);
        r = make<float4>(1.0f/sqrt(59.25f), 2.5f/sqrt(59.25f), 6.0f/sqrt(59.25f), -4.0f/sqrt(59.25f));
        require(all(islessequal(fdim(q, r), broadcast<float4>(epsilon10))));
    }
}
