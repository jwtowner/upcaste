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

#include <up/math/float4.hpp>
#include <up/random.hpp>
#include <up/test.hpp>
#include <random>

namespace math_float4
{
    class float4_source
	{
		size_t size_;
		up::math::float4* input_;
        up::math::float4* output_;
        bool regenerate_;
        float min_;
        float max_;

    public:

		float4_source(size_t sz, bool r = false)
        : size_(sz),
        input_(new up::math::float4[sz]),
        output_(new up::math::float4[sz]),
        regenerate_(r),
        min_(-8.0f * FLT_PI),
        max_(8.0f * FLT_PI) {
            if (!r) {
                generate();
            }
        }

        float4_source(size_t sz, float l, float u, bool r = false)
        : size_(sz),
        input_(new up::math::float4[sz]),
        output_(new up::math::float4[sz]),
        regenerate_(r),
        min_(l),
        max_(u) {
            if (!r) {
                generate();
            }
        }
         
		~float4_source() { delete[] output_; delete[] input_; }
		size_t size() const { return size_; }
		up::math::float4 const* input() const { return input_; }
        up::math::float4* output() const { return output_; }
        void setup() { if (regenerate_) { generate(); } }
        void teardown() { }

    private:

        void generate() {
            up::default_random_engine random_engine(static_cast<up::uint_least32_t>(up::time(nullptr) & UINT_LEAST32_MAX));
            up::uniform_real_distribution<float> dist(min_, max_);
            for (size_t i = 0; i < size_; ++i) {
                up::math::storea4(up::math::make<up::math::float4>(dist(random_engine), dist(random_engine), dist(random_engine), dist(random_engine)), i, input_);
            }
        }
	};


    //
    // recip
    //

    UPNOINLINE void recip_div_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::div(up::math::uniform<up::math::float4>::one, a);
            output[i] = a;
        }
    }

    UPNOINLINE void recip_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::recip(a);
            output[i] = a;
        }
    }

    UPNOINLINE void half_recip_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::half_recip(a);
            output[i] = a;
        }
    }

    UPNOINLINE void native_recip_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::native_recip(a);
            output[i] = a;
        }
    }

    UP_TEST_BENCHMARK(recip) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size);
        UP_TEST_RUN_BENCHMARK("div", size, iterations, &recip_div_iteration, ds);
        UP_TEST_RUN_BENCHMARK("recip", size, iterations, &recip_iteration, ds);
        UP_TEST_RUN_BENCHMARK("half_recip", size, iterations, &half_recip_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_recip", size, iterations, &native_recip_iteration, ds);
    }

    //
    // rsqrt
    //

    UPNOINLINE void sqrt_div_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::sqrt(a);
            a = up::math::div(up::math::uniform<up::math::float4>::one, a);
            output[i] = a;
        }
    }

    UPNOINLINE void rsqrt_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::rsqrt(a);
            output[i] = a;
        }
    }

    UPNOINLINE void half_rsqrt_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::half_rsqrt(a);
            output[i] = a;
        }
    }

    UPNOINLINE void native_rsqrt_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::native_rsqrt(a);
            output[i] = a;
        }
    }

    UP_TEST_BENCHMARK(rsqrt) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size);
        UP_TEST_RUN_BENCHMARK("sqrt_div", size, iterations, &sqrt_div_iteration, ds);
        UP_TEST_RUN_BENCHMARK("rsqrt", size, iterations, &rsqrt_iteration, ds);
        UP_TEST_RUN_BENCHMARK("half_rsqrt", size, iterations, &half_rsqrt_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_rsqrt", size, iterations, &native_rsqrt_iteration, ds);
    }
     
    //
    // acos
    //

    UPNOINLINE void acos_float_iteration(float4_source& ds) {
        float const* input = reinterpret_cast<float const*>(ds.input());
        float* output = reinterpret_cast<float*>(ds.output());
        size_t const count = ds.size() * 4;

        for (size_t i = 0; i < count; ++i) {
            float a = input[i];
            a = up::math::acos(a);
            output[i] = a;
        }
    }

    UPNOINLINE void native_acos_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::native_acos(a);
            output[i] = a;
        }
    }

    UP_TEST_BENCHMARK(acos) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size, -1.0f, 1.0f);
        UP_TEST_RUN_BENCHMARK("acosf", size, iterations, &acos_float_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_acosf4", size, iterations, &native_acos_iteration, ds);
    }

    //
    // cos
    //

    UPNOINLINE void cos_float_iteration(float4_source& ds) {
        float const* input = reinterpret_cast<float const*>(ds.input());
        float* output = reinterpret_cast<float*>(ds.output());
        size_t const count = ds.size() * 4;

        for (size_t i = 0; i < count; ++i) {
            float a = input[i];
            a = up::math::cos(a);
            output[i] = a;
        }
    }

    UPNOINLINE void cos_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::cos(a);
            output[i] = a;
        }
    }

    UPNOINLINE void half_cos_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::half_cos(a);
            output[i] = a;
        }
    }

    UPNOINLINE void native_cos_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::native_cos(a);
            output[i] = a;
        }
    }

    UP_TEST_BENCHMARK(cos) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size);
        UP_TEST_RUN_BENCHMARK("cosf", size, iterations, &cos_float_iteration, ds);
        UP_TEST_RUN_BENCHMARK("cosf4", size, iterations, &cos_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("half_cosf4", size, iterations, &half_cos_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_cosf4", size, iterations, &native_cos_float4_iteration, ds);
    }
    
    //
    // sin
    //

    UPNOINLINE void sin_float_iteration(float4_source& ds) {
        float const* input = reinterpret_cast<float const*>(ds.input());
        float* output = reinterpret_cast<float*>(ds.output());
        size_t const count = ds.size() * 4;

        for (size_t i = 0; i < count; ++i) {
            float a = input[i];
            a = up::math::sin(a);
            output[i] = a;
        }
    }

    UPNOINLINE void sin_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::sin(a);
            output[i] = a;
        }
    }

    UPNOINLINE void half_sin_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::half_sin(a);
            output[i] = a;
        }
    }

    UPNOINLINE void native_sin_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::native_sin(a);
            output[i] = a;
        }
    }

    UP_TEST_BENCHMARK(sin) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size);
        UP_TEST_RUN_BENCHMARK("sinf", size, iterations, &sin_float_iteration, ds);
        UP_TEST_RUN_BENCHMARK("sinf4", size, iterations, &sin_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("half_sinf4", size, iterations, &half_sin_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_cosf4", size, iterations, &native_sin_float4_iteration, ds);
    }

    //
    // sincos
    //

    UPNOINLINE void sincos_float_iteration(float4_source& ds) {
        float const* input = reinterpret_cast<float const*>(ds.input());
        float* output = reinterpret_cast<float*>(ds.output());
        size_t const count = ds.size() * 4;

        for (size_t i = 0; i < count; ++i) {
            float a = input[i], b, c;
            b = up::math::sincos(a, &c);
            output[i] = b + c;
        }
    }

    UPNOINLINE void sincos_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i], b, c;
            b = up::math::sincos(a, &c);
            output[i] = up::math::add(b, c);
        }
    }

    UPNOINLINE void half_sincos_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i], b, c;
            b = up::math::half_sincos(a, &c);
            output[i] = up::math::add(b, c);
        }
    }

    UPNOINLINE void native_sincos_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i], b, c;
            b = up::math::native_sincos(a, &c);
            output[i] = up::math::add(b, c);
        }
    }

    UP_TEST_BENCHMARK(sincos) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size);
        UP_TEST_RUN_BENCHMARK("sincosf", size, iterations, &sincos_float_iteration, ds);
        UP_TEST_RUN_BENCHMARK("sincosf4", size, iterations, &sincos_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("half_sincosf4", size, iterations, &half_sincos_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_sincosf4", size, iterations, &native_sincos_float4_iteration, ds);
    }


    //
    // tan
    //

    UPNOINLINE void tan_float_iteration(float4_source& ds) {
        float const* input = reinterpret_cast<float const*>(ds.input());
        float* output = reinterpret_cast<float*>(ds.output());
        size_t const count = ds.size() * 4;

        for (size_t i = 0; i < count; ++i) {
            float a = input[i];
            a = up::math::tan(a);
            output[i] = a;
        }
    }

    UPNOINLINE void tan_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::tan(a);
            output[i] = a;
        }
    }

    UPNOINLINE void half_tan_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::half_tan(a);
            output[i] = a;
        }
    }

    UPNOINLINE void native_tan_float4_iteration(float4_source& ds) {
        up::math::float4 const* input = ds.input();
        up::math::float4* output = ds.output();
        size_t const count = ds.size();

        for (size_t i = 0; i < count; ++i) {
            up::math::float4 a = input[i];
            a = up::math::native_tan(a);
            output[i] = a;
        }
    }

    UP_TEST_BENCHMARK(tan) {
        unsigned int const iterations = 100;
        unsigned int const size = 1024 * 1024;
        float4_source ds(size);
        UP_TEST_RUN_BENCHMARK("tanf", size, iterations, &tan_float_iteration, ds);
        UP_TEST_RUN_BENCHMARK("tanf4", size, iterations, &tan_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("half_tanf4", size, iterations, &half_tan_float4_iteration, ds);
        UP_TEST_RUN_BENCHMARK("native_tanf4", size, iterations, &native_tan_float4_iteration, ds);
    }
}
