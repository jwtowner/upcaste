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

#include <up/cinttypes.hpp>
#include <up/log.hpp>
#include <up/random.hpp>
#include <up/test.hpp>

namespace prng
{
    UP_TEST_CASE(generate_canonical) {
        up::xorshift64_engine<uint_least32_t, 13, 7, 17> engine32;
        up::xorshift64_engine<uint_least64_t, 13, 7, 17> engine64;
        long double sum;
        double d;
        float f;
        size_t i;
        
        for (sum = 0.0l, i = 1; i <= 32768; sum += f, ++i) {
            f = ::up::generate_canonical<float, FLT_MANT_DIG>(engine32);
            require((0.0f <= f) && (f < 1.0f));
        }

        require_approx(sum / 32768.0l, 0.5l, 0.01l);

        for (sum = 0.0l, i = 1; i <= 32768; sum += f, ++i) {
            f = ::up::generate_canonical<float, FLT_MANT_DIG>(engine64);
            require((0.0f <= f) && (f < 1.0f));
        }

        require_approx(sum / 32768.0l, 0.5l, 0.01l);

        for (sum = 0.0l, i = 1; i <= 32768; sum += d, ++i) {
            d = ::up::generate_canonical<double, DBL_MANT_DIG>(engine32);
            require((0.0 <= d) && (d < 1.0));
        }

        require_approx(sum / 32768.0l, 0.5l, 0.01l);

        for (sum = 0.0l, i = 1; i <= 32768; sum += d, ++i) {
            d = ::up::generate_canonical<float, DBL_MANT_DIG>(engine64);
            require((0.0 <= d) && (d < 1.0));
        }

        require_approx(sum / 32768.0l, 0.5l, 0.01l);
    }

    UP_TEST_CASE(generate_random_seed) {
        uint_least32_t seed;
        uint_least32_t seq[4];

        seed = 0;
        up::generate_random_seed(seq, 4, &seed, 1);
        require(seq[0] == 0xb0a842fd);
        require(seq[1] == 0x04290315);
        require(seq[2] == 0x75a0adfa);
        require(seq[3] == 0x1fb83b0f);

        seed = 1;
        up::generate_random_seed(seq, 4, &seed, 1);
        require(seq[0] == 0x4dc239dd);
        require(seq[1] == 0x529c8ec8);
        require(seq[2] == 0x60e79bf6);
        require(seq[3] == 0x624f7451);

        seed = 521288629;
        up::generate_random_seed(seq, 4, &seed, 1);
        require(seq[0] == 0xb887f987);
        require(seq[1] == 0x6e42c737);
        require(seq[2] == 0xd6c4c175);
        require(seq[3] == 0xf71737b9);
    }

    UP_TEST_CASE(xorshift64_engine) {
        up::default_xorshift64_engine engine;
        require(engine.min() == ((UINT_FAST32_MAX > 0xFFFFFFFF) ? 1 : 0))
        require(engine.max() == UINT_FAST32_MAX);

#ifdef UP_DEBUG
        up::log_event(up::log_level_info, "First 128 values...\n\n");
        for (size_t i = 1; i <= 128; ++i)  {
            up::log_eventf(up::log_level_info, "%#.8" PRIxFAST32 " ", static_cast<uintmax_t>(engine()));
            if ((i % 4) == 0) {
                up::log_event(up::log_level_info, "\n");
            }
        }
#else
        engine.discard(128);
#endif

#if UINT_FAST32_MAX == 0xFFFFFFFF
        require(engine() == 0xfb699180);
#else
        require(engine() == 0xda8b4c09021984c3ull);
#endif
    }

    UP_TEST_CASE(xorshift128_engine) {
        up::default_xorshift128_engine engine;
        require(engine.min() == 0)
        require(engine.max() == UINT_FAST32_MAX);
        
#ifdef UP_DEBUG
        up::log_event(up::log_level_info, "First 128 values...\n\n");
        for (size_t i = 1; i <= 128; ++i)  {
            up::log_eventf(up::log_level_info, "%#.8" PRIxFAST32 " ", static_cast<uintmax_t>(engine()));
            if ((i % 4) == 0) {
                up::log_event(up::log_level_info, "\n");
            }
        }
#else
        engine.discard(128);
#endif

#if UINT_FAST32_MAX == 0xFFFFFFFF
        require(engine() == 0xc72a4e0a);
#else
        require(engine() == 0x60f38d031f5c90b1);
#endif
    }

    UP_TEST_CASE(uniform_int_distribution) {
        up::default_random_engine engine;
        long double sum;
        size_t i;
        int x;

        static_assert(up::is_same<int, up::uniform_int_distribution<int>::result_type>::value, "unexpected type");

        up::uniform_int_distribution<int> dist0(0, 0);
        require((dist0.a() == 0) && (dist0.b() == 0));
        require((dist0.min() == 0) && (dist0.max() == 0));

        for (sum = 0.0l, i = 1; i <= 1024; sum += x, ++i) {
            x = dist0(engine);
            require(x == 0);
        }

        require_approx(sum / 1024.0l, 0.0l, LDBL_EPSILON);

        up::uniform_int_distribution<int> dist1(0, 10);
        require((dist1.a() == 0) && (dist1.b() == 10));
        require((dist1.min() == 0) && (dist1.max() == 10));

        for (sum = 0.0l, i = 1; i <= 16384; sum += x, ++i) {
            x = dist1(engine);
            require((x >= 0) && (x <= 10));
        }

        require_approx(sum / 16384.0l, 5.0l, 0.05l);

        up::uniform_int_distribution<int> dist2(-10, 0);
        require((dist2.a() == -10) && (dist2.b() == 0));
        require((dist2.min() == -10) && (dist2.max() == 0));

        for (sum = 0.0l, i = 1; i <= 16384; sum += x, ++i) {
            x = dist2(engine);
            require((x >= -10) && (x <= 0));
        }

        require_approx(sum / 16384.0l, -5.0l, 0.05l);

        up::uniform_int_distribution<int> dist3(-10, 10);
        require((dist3.a() == -10) && (dist3.b() == 10));
        require((dist3.min() == -10) && (dist3.max() == 10));

        for (sum = 0.0l, i = 1; i <= 16384; sum += x, ++i) {
            x = dist3(engine);
            require((x >= -10) && (x <= 10));
        }

        require_approx(sum / 16384.0l, 0.0l, 0.05l);

        up::uniform_int_distribution<int> dist4;
        require((dist4.a() == 0) && (dist4.b() == INT_MAX));
        require((dist4.min() == 0) && (dist4.max() == INT_MAX));

        for (sum = 0.0l, i = 1; i <= (INT_MAX / 8); sum += x, ++i) {
            x = dist4(engine);
            require((x >= 0) && (x <= INT_MAX));
        }

        require_approx(sum / (INT_MAX / 8.0l), (INT_MAX / 2.0l), 50000.0l);
    }

    UP_TEST_CASE(uniform_llong_distribution) {
        typedef long long llong;
        up::default_random_engine engine;
        long double sum;
        llong x;
        size_t i;

        static_assert(up::is_same<llong, up::uniform_int_distribution<llong>::result_type>::value, "unexpected type");

        up::uniform_int_distribution<llong> dist0(0, 0);
        require((dist0.a() == 0) && (dist0.b() == 0));
        require((dist0.min() == 0) && (dist0.max() == 0));

        for (sum = 0.0l, i = 1; i <= 1024; sum += x, ++i) {
            x = dist0(engine);
            require(x == 0);
        }

        require_approx(sum / 1024.0l, 0.0l, LDBL_EPSILON);

        up::uniform_int_distribution<llong> dist1(-100, 100);
        require((dist1.a() == -100) && (dist1.b() == 100));
        require((dist1.min() == -100) && (dist1.max() == 100));

        for (sum = 0.0l, i = 1; i <= 16384; sum += x, ++i) {
            x = dist1(engine);
            require((x >= -100) && (x <= 100));
        }

        require_approx(sum / 16384.0l, 0.0l, 1.0l);

        up::uniform_int_distribution<llong> dist2;
        require((dist2.a() == 0) && (dist2.b() == LLONG_MAX));
        require((dist2.min() == 0) && (dist2.max() == LLONG_MAX));

        for (sum = 0.0l, i = 1; i <= (INT_MAX / 8); sum += x, ++i) {
            x = dist2(engine);
            require((x >= 0) && (x <= LLONG_MAX));
        }

        require_approx(sum / (INT_MAX / 8.0l), (LLONG_MAX / 2.0l), 4.0e+14);
    }

    UP_TEST_CASE(uniform_ullong_distribution) {
        typedef unsigned long long ullong;
        up::default_random_engine engine;
        long double sum;
        ullong x;
        size_t i;

        static_assert(up::is_same<ullong, up::uniform_int_distribution<ullong>::result_type>::value, "unexpected type");

        up::uniform_int_distribution<ullong> dist0(0, 0);
        require((dist0.a() == 0) && (dist0.b() == 0));
        require((dist0.min() == 0) && (dist0.max() == 0));

        for (sum = 0.0l, i = 1; i <= 1024; sum += x, ++i) {
            x = dist0(engine);
            require(x == 0);
        }

        require_approx(sum / 1024.0l, 0.0l, LDBL_EPSILON);

        up::uniform_int_distribution<ullong> dist1(0, 100);
        require((dist1.a() == 0) && (dist1.b() == 100));
        require((dist1.min() == 0) && (dist1.max() == 100));

        for (sum = 0.0l, i = 1; i <= 16384; sum += x, ++i) {
            x = dist1(engine);
            require(x <= 100);
        }

        require_approx(sum / 16384.0l, 50.0l, 1.0l);

        up::uniform_int_distribution<ullong> dist2;
        require((dist2.a() == 0) && (dist2.b() == ULLONG_MAX));
        require((dist2.min() == 0) && (dist2.max() == ULLONG_MAX));

        for (sum = 0.0l, i = 1; i <= (INT_MAX / 8); sum += x, ++i) {
            x = dist2(engine);
            require(x <= ULLONG_MAX);
        }

        require_approx(sum / (INT_MAX / 8.0l), (ULLONG_MAX / 2.0l), 4.0e+14);
    }

    UP_TEST_CASE(uniform_float_distribution) {
        up::default_random_engine engine;
        long double sum;
        float x;
        size_t i;

        static_assert(up::is_same<float, up::uniform_real_distribution<float>::result_type>::value, "unexpected type");

        up::uniform_real_distribution<float> dist0(0.0f, 0.0f);
        require((dist0.a() == 0.0f) && (dist0.b() == 0.0f));
        require((dist0.min() == 0.0f) && (dist0.max() == 0.0f));

        for (sum = 0.0l, i = 1; i <= 1024; sum += x, ++i) {
            x = dist0(engine);
            require(x == 0.0f);
        }

        require_approx(sum / 1024.0l, 0.0l, LDBL_EPSILON);

        up::uniform_real_distribution<float> dist1;
        require((dist1.a() == 0.0f) && (dist1.b() == 1.0f));
        require((dist1.min() == 0.0f) && (dist1.max() == 1.0f));

        for (sum = 0.0l, i = 1; i <= 32768; sum += x, ++i) {
            x = dist1(engine);
            require((0.0f <= x) && (x < 1.0f));
        }

        require_approx(sum / 32768.0l, 0.5l, 0.01l);

        up::uniform_real_distribution<float> dist2(-128.0f, 128.0f);
        require((dist2.a() == -128.0f) && (dist2.b() == 128.0f));
        require((dist2.min() == -128.0f) && (dist2.max() == 128.0f));

        for (sum = 0.0l, i = 1; i <= 32768; sum += x, ++i) {
            x = dist2(engine);
            require((-128.0f <= x) && (x < 128.0f));
        }

        require_approx(sum / 32768.0l, 0.0l, 1.0l);

        /*
        up::uniform_real_distribution<float> dist3(-(FLT_MAX / 2.0f), (FLT_MAX / 2.0f));
        require((dist3.a() == -(FLT_MAX / 2.0f)) && (dist3.b() == (FLT_MAX / 2.0f)));
        require((dist3.min() == -(FLT_MAX / 2.0f)) && (dist3.max() == (FLT_MAX / 2.0f)));

        for (sum = 0.0l, i = 1; i <= (INT_MAX / 8); sum += x, ++i) {
            x = dist3(engine);
            require((-(FLT_MAX / 2.0f) <= x) && (x < (FLT_MAX / 2.0f)));
        }

        require_approx(sum / (INT_MAX / 8.0l), 0.0l, 1e+14);
        */
    }
}
