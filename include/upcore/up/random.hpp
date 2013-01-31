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

#ifndef UP_RANDOM_HPP
#define UP_RANDOM_HPP

#include <up/bitwise.hpp>
#include <up/cstdlib.hpp>
#include <up/limits.hpp>
#include <up/utility.hpp>

namespace up
{
    template <class UIntType, size_t A, size_t B, size_t C>
    class xorshift64_engine;

    template <class UIntType, size_t A, size_t B, size_t C>
    class xorshift128_engine;

    typedef xorshift64_engine<uint_fast32_t, 13, 7, 17> default_xorshift64_engine;
    typedef xorshift128_engine<uint_fast32_t, 11, 19, 8> default_xorshift128_engine;
    typedef default_xorshift128_engine default_random_engine;

    template <class IntType>
    struct uniform_int_distribution_param;

    template <class IntType = int>
    class uniform_int_distribution;

    template <class RealType, size_t bits, class Engine>
    UPHIDDENINLINE
    RealType generate_canonical(Engine& engine) {
        constexpr uintmax_t iR = (uintmax_t)Engine::result_range + (uintmax_t)1;
        constexpr size_t max_logR = numeric_limits<typename Engine::result_type>::digits;
        constexpr size_t logR = (iR > 0) ? static_floorlog2<uintmax_t, iR>::value : max_logR;
        constexpr size_t digits = numeric_limits<RealType>::digits;
        constexpr size_t b = (digits < bits) ? digits : bits;
        constexpr size_t k = (b / logR) + (b % logR != 0) + (b == 0);
        RealType const R = (RealType)Engine::result_range + (RealType)1;
        RealType R_k, S;
        size_t i;

        S = (RealType)(engine() - Engine::result_min);
        for (i = 1, R_k = R; i < k; ++i, R_k *= R) {
            S += (RealType)(engine() - Engine::result_min) * R_k;
        }

        return S / R_k;
    }

    extern LIBUPCOREAPI UPNONNULLALL
    void generate_random_seed(
        uint_least32_t* UPRESTRICT dst,
        size_t dst_length,
        uint_least32_t const* UPRESTRICT src,
        size_t src_length
    )
    noexcept;
}

namespace up { namespace detail
{
    template <class UIntType>
    struct is_big_seed : integral_constant<bool, (1 == (((sizeof(UIntType) * CHAR_BIT) + 31) / 32))> { };

    template <class Engine, class UIntType>
    UPALWAYSINLINE
    void seed_engine(Engine& e, UIntType s, true_type) {
        constexpr size_t n = ((sizeof(UIntType) * CHAR_BIT) + 31) / 32;
        uint_least32_t seq[n];
        for (size_t i = 0; i < n; ++i) {
            seq[n] = static_cast<uint_least32_t>(s & 0xFFFFFFFF);
            s >>= 32;
        }
        e.seed(seq, n);
    }

    template <class Engine, class UIntType>
    UPALWAYSINLINE
    void seed_engine(Engine& e, UIntType s, false_type) {
        uint_least32_t seq = static_cast<uint_least32_t>(s & 0xFFFFFFFF);
        e.seed(&seq, 1);
    }

    template <class Engine, class IntType>
    class uniform_int_generator
    {
        UPNONCOPYABLE(uniform_int_generator);
    
    public:

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(push)
#   pragma warning(disable:4307) // '+' : integral constant overflow
#endif

        typedef typename make_unsigned<IntType>::type UIntType;
        typedef typename Engine::result_type EngineType;
        typedef typename common_type<EngineType, UIntType>::type WorkType;
        static constexpr WorkType engine_range = static_cast<WorkType>(Engine::result_range) + 1;
        static constexpr size_t engine_bits = static_floorlog2<WorkType, engine_range>::value;
        static constexpr size_t engine_max_bits = numeric_limits<EngineType>::digits;
        static constexpr size_t work_max_bits = numeric_limits<WorkType>::digits;
        static constexpr size_t max_bucket_count = (sizeof(WorkType) + (sizeof(EngineType) - 1)) / sizeof(EngineType);
        static constexpr bool fast_path = ((Engine::result_range + static_cast<EngineType>(1)) == 0) && (max_bucket_count <= 2);

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(pop)
#endif

        static_assert(engine_max_bits >= engine_bits, "invalid engine_bits"); 
        static_assert(work_max_bits >= engine_max_bits, "invalid engine_max_bits"); 
        static_assert(max_bucket_count > 0, "invalid max_bucket_count"); 

        UPALWAYSINLINE
        uniform_int_generator(Engine& engine_, WorkType work_range_)
        : engine(engine_),
        work_range(work_range_),
        work_bits((work_range_ > 0) ? static_cast<size_t>(::up::ceillog2(work_range_)) : work_max_bits),
        bucket_count((work_bits + (engine_bits - 1)) / engine_bits),
        bucket_bits(work_bits / bucket_count) {
        }

        UPALWAYSINLINE
        WorkType generate() {
            return generate(integral_constant<size_t, fast_path ? (max_bucket_count - 1) : 2>());
        }

        UPALWAYSINLINE
        WorkType generate(integral_constant<size_t, 0>) {
            EngineType const bucket_mask = EngineType(~0) >> (engine_max_bits - bucket_bits);
            WorkType sample;

            do {
                sample = engine() & bucket_mask;
            }
            while ((sample >= work_range) && (work_range > 0));

            return sample;
        }

        UPALWAYSINLINE
        WorkType generate(integral_constant<size_t, 1>) {
            WorkType work, bucket_range, upper_bucket_range;
            EngineType sample, bucket_mask, upper_bucket_mask;

            upper_bucket_mask = EngineType(~0) >> (engine_max_bits - bucket_bits);
            upper_bucket_range = (engine_range >> bucket_bits) << bucket_bits;

            if (bucket_count <= 1) {
                do {
                    work = engine() & upper_bucket_mask;
                }
                while ((work >= work_range) && (work_range > 0));
                return work;
            }

            if ((work_bits % bucket_count) > 0) {
                bucket_range = (bucket_bits < (work_max_bits - 1))
                    ? (engine_range >> (bucket_bits + 1)) << (bucket_bits + 1)
                    : 0;
                bucket_mask = (bucket_bits < (engine_max_bits - 1))
                    ? (EngineType(~0) >> (engine_max_bits - (bucket_bits + 1))) 
                    : EngineType(~0);
                ++bucket_bits;
            }
            else {
                bucket_range = upper_bucket_range;
                bucket_mask = upper_bucket_mask;
            }

            do {
                do { sample = engine(); } while (sample >= upper_bucket_range);
                work = sample & upper_bucket_mask;
                do { sample = engine(); } while (sample >= bucket_range);
                work = (work << bucket_bits) | (sample & bucket_mask);
            }
            while ((work >= work_range) && (work_range > 0));

            return work;
        }

        UPALWAYSINLINE
        WorkType generate(integral_constant<size_t, 2>) {
            WorkType upper_bucket_range;
            if (bucket_bits < work_max_bits) {
                upper_bucket_range = (engine_range >> bucket_bits) << bucket_bits;
            }
            else {
                upper_bucket_range = 0;
            }

            if ((engine_range - upper_bucket_range) > (upper_bucket_range / bucket_count)) {
                bucket_bits = work_bits / ++bucket_count;
                if (bucket_bits < work_max_bits) {
                    upper_bucket_range = (engine_range >> bucket_bits) << bucket_bits;
                }
                else {
                    upper_bucket_range = 0;
                }
            }

            size_t const upper_bucket_count = bucket_count - (work_bits % bucket_count);

            EngineType const upper_bucket_mask = (bucket_bits > 0)
                ? (EngineType(~0) >> (engine_max_bits - bucket_bits))
                : EngineType(0);
        
            WorkType const bucket_range = (bucket_bits < (work_max_bits - 1))
                ? (engine_range >> (bucket_bits + 1)) << (bucket_bits + 1)
                : 0;

            EngineType const bucket_mask = (bucket_bits < (engine_max_bits - 1))
                ? (EngineType(~0) >> (engine_max_bits - (bucket_bits + 1))) 
                : EngineType(~0);

            EngineType sample;
            WorkType work;
            
            do {
                work = 0;

                for (size_t k = 0; k < upper_bucket_count; ++k) {
                    do {
                        sample = engine() - Engine::result_min;
                    }
                    while (sample >= upper_bucket_range);
                    
                    if (bucket_bits < work_max_bits) {
                        work <<= bucket_bits;
                    }
                    else {
                        work = 0;
                    }

                    work += sample & upper_bucket_mask;
                }

                for (size_t k = upper_bucket_count; k < bucket_count; ++k) {
                    do {
                        sample = engine() - Engine::result_min;
                    }
                    while (sample >= bucket_range);

                    if (bucket_bits < (work_max_bits - 1)) {
                        work <<= bucket_bits + 1;
                    }
                    else {
                        work = 0;
                    }

                    work += sample & bucket_mask;
                }
            }
            while ((work >= work_range) && (work_range > 0));

            return work;
        }

    private:

        Engine& engine;
        WorkType const work_range;
        size_t const work_bits;
        size_t bucket_count;
        size_t bucket_bits;
    };
}}

namespace up
{
    template <class UIntType, size_t A, size_t B, size_t C>
    class UPVISIBLE xorshift64_engine
    {
        static_assert(sizeof(UIntType) <= sizeof(uint_least64_t), "UIntType is too large");
        static_assert(A != C, "shift constants A and C must not be the same");

    public:

        typedef UIntType result_type;
        static constexpr UIntType default_seed = 1u;
        static constexpr UIntType result_min = sizeof(UIntType) / sizeof(uint_least64_t);
        static constexpr UIntType result_range = static_cast<UIntType>(~0) - result_min;
        static UPALWAYSINLINE UPCONSTEXPR UIntType min() { return result_min; }
        static UPALWAYSINLINE UPCONSTEXPR UIntType max() { return result_min + result_range; }

        UPHIDDENINLINE
        explicit xorshift64_engine(UIntType s = default_seed) {
            seed(s);
        }

        UPHIDDENINLINE
        xorshift64_engine(uint_least32_t const* seq, size_t n) {
            seed(seq, n);
        }

        UPHIDDENINLINE
        void seed(UIntType s = default_seed) {
            typename common_type<UIntType, uint_least32_t>::type const s32 = s;
            seed(reinterpret_cast<uint_least32_t const*>(&s32), sizeof(s32) / sizeof(uint_least32_t));
        }

        UPHIDDENINLINE
        void seed(uint_least32_t const* seq, size_t n) {
            uint_least32_t r[5];
            generate_random_seed(r, 5, seq, n);
            state = static_cast<uint_least64_t>(r[3]) | (static_cast<uint_least64_t>(r[4]) << 32);
        }

        UPHIDDENINLINE
        UIntType operator()() {
            state ^= state << A;
            state ^= state >> B;
            state ^= state << C;
            return static_cast<UIntType>(state & static_cast<UIntType>(~0));
        }

        UPHIDDENINLINE
        void discard(unsigned long long n) {
            for ( ; n > 0; --n) {
                operator()();
            }
        }

    private:

        uint_least64_t state;
    };

    template <class UIntType, size_t A, size_t B, size_t C>
    class LIBUPCOREAPI xorshift128_engine
    {
        static_assert(sizeof(UIntType) <= sizeof(uint_least32_t[4]), "UIntType is too large");
        static_assert(A != C, "shift constants A and C must not be the same");

    public:

        typedef UIntType result_type;
        static constexpr UIntType default_seed = 1u;
        static constexpr UIntType result_min = sizeof(UIntType) / sizeof(uint_least32_t[4]);
        static constexpr UIntType result_range = static_cast<UIntType>(~0) - result_min;
        static UPALWAYSINLINE UPCONSTEXPR UIntType min() { return result_min; }
        static UPALWAYSINLINE UPCONSTEXPR UIntType max() { return result_min + result_range; }

        UPHIDDENINLINE
        explicit xorshift128_engine(UIntType s = default_seed) {
            seed(s);
        }

        UPHIDDENINLINE
        xorshift128_engine(uint_least32_t const* seq, size_t n) {
            seed(seq, n);
        }

        UPHIDDENINLINE
        void seed(UIntType s = default_seed) {
            typename common_type<UIntType, uint_least32_t>::type const s32 = s;
            seed(reinterpret_cast<uint_least32_t const*>(&s32), sizeof(s32) / sizeof(uint_least32_t));
        }

        UPHIDDENINLINE
        void seed(uint_least32_t const* seq, size_t n) {
            generate_random_seed(state, 4, seq, n);
        }

        UPHIDDENINLINE
        UIntType operator()() {
            uint_least32_t t = state[0] ^ (state[0] << A);
            state[0] = state[1]; state[1] = state[2]; state[2] = state[3];
            state[3] = state[3] ^ (state[3] >> B) ^ (t ^ (t >> C));
            return finalize(
                integral_constant
                <
                    size_t,
                    ((sizeof(UIntType) + (sizeof(uint_least32_t) - 1)) / sizeof(uint_least32_t)) - 1
                >()
            );
        }

        UPHIDDENINLINE
        void discard(unsigned long long n) {
            for ( ; n > 0; --n) {
                operator()();
            }
        }

    private:
        
        UPALWAYSINLINE
        UIntType finalize(integral_constant<size_t, 0>) {
            return static_cast<UIntType>(state[3] & result_range);
        }
        
        UPALWAYSINLINE
        UIntType finalize(integral_constant<size_t, 1>) {
            return ((static_cast<UIntType>(state[2]) << 32) | static_cast<UIntType>(state[3])) & result_range;
        }

        UPALWAYSINLINE
        UIntType finalize(integral_constant<size_t, 2>) {
            return ((static_cast<UIntType>(state[1]) << 64) | (static_cast<UIntType>(state[2]) << 32)
                | static_cast<UIntType>(state[3])) & result_range;
        }

        UPALWAYSINLINE
        UIntType finalize(integral_constant<size_t, 3>) {
            return ((static_cast<UIntType>(state[0]) << 96) | (static_cast<UIntType>(state[1]) << 64)
                | (static_cast<UIntType>(state[2]) << 32) | static_cast<UIntType>(state[3])) & result_range;
        }

        uint_least32_t state[4];
    };

    template <class IntType>
    struct UPVISIBLE uniform_int_distribution_param
    {
        typedef IntType result_type;
        typedef typename make_unsigned<result_type>::type unsigned_result_type;

        result_type result_min;
        unsigned_result_type result_range;

        UPHIDDENINLINE
        explicit uniform_int_distribution_param(result_type a = 0, result_type b = numeric_limits<result_type>::max()) {
            if (a > b) {
                swap(a, b);
            }
            result_min = a;
            result_range = static_cast<unsigned_result_type>(b - a);
        }

        UPHIDDENINLINE result_type a() const { return result_min; }
        UPHIDDENINLINE result_type b() const { return result_min + static_cast<result_type>(result_range); }
    };

    template <class IntType>
    UPHIDDENINLINE
    bool operator==(uniform_int_distribution_param<IntType> const& x, uniform_int_distribution_param<IntType> const& y) {
        return x.result_min == y.result_min && x.range == y.result_range;
    }

    template <class IntType>
    UPHIDDENINLINE
    bool operator!=(uniform_int_distribution_param<IntType> const& x, uniform_int_distribution_param<IntType> const& y) {
        return x.result_min != y.result_min || x.range != y.result_range;
    }

    template <class IntType>
    class UPVISIBLE uniform_int_distribution
    {
    public:
        
        typedef IntType result_type;
        typedef uniform_int_distribution_param<IntType> param_type;
        
        UPHIDDENINLINE
        explicit uniform_int_distribution(result_type a = 0, result_type b = numeric_limits<result_type>::max())
        : param_(a, b) {
        }

        UPHIDDENINLINE
        explicit uniform_int_distribution(param_type const& p)
        : param_(p) {
        }

        UPHIDDENINLINE result_type min() const { return param_.a(); }
        UPHIDDENINLINE result_type max() const { return param_.b(); }
        UPHIDDENINLINE result_type a() const { return param_.a(); }
        UPHIDDENINLINE result_type b() const { return param_.b(); }
        UPHIDDENINLINE param_type param() const { return param_; }
        UPHIDDENINLINE void param(const param_type& p) { param_ = p; }

        template <class Engine>
        UPHIDDENINLINE
        result_type operator()(Engine& engine) {
            return (*this)(engine, param_);
        }

        template <class Engine>
        result_type operator()(Engine& engine, param_type const& p);

    private:

        param_type param_;
    };

    template <class IntType>
    UPHIDDENINLINE
    bool operator==(uniform_int_distribution<IntType> const& x, uniform_int_distribution<IntType> const& y) {
        return x.param() == y.param();
    }

    template <class IntType>
    UPHIDDENINLINE
    bool operator!=(uniform_int_distribution<IntType> const& x, uniform_int_distribution<IntType> const& y) {
        return x.param() != y.param();
    }

    template <class IntType> template <class Engine>
    typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::operator()(Engine& engine, param_type const& p) {
        typedef detail::uniform_int_generator<Engine, IntType> Generator;
        typedef typename Generator::WorkType WorkType;
        WorkType const work_range = static_cast<WorkType>(p.result_range) + 1;
        if (work_range == 1) {
            return p.result_min;
        }
        else if (work_range == Generator::engine_range) {
            return static_cast<result_type>(engine() - Engine::result_min) + p.result_min;
        }
        else {
            Generator generator(engine, work_range);
            return static_cast<result_type>(generator.generate()) + p.result_min;
        }
    }

    template <class RealType>
    struct UPVISIBLE uniform_real_distribution_param
    {
        typedef RealType result_type;

        result_type result_min;
        result_type result_range;

        UPHIDDENINLINE
        explicit uniform_real_distribution_param(result_type a = 0, result_type b = 1) {
            if (a > b) {
                swap(a, b);
            }
            result_min = a;
            result_range = b - a;
        }

        UPHIDDENINLINE result_type a() const { return result_min; }
        UPHIDDENINLINE result_type b() const { return result_min + static_cast<result_type>(result_range); }
    };

    template <class RealType>
    UPHIDDENINLINE
    bool operator==(uniform_real_distribution_param<RealType> const& x, uniform_real_distribution_param<RealType> const& y) {
        return x.result_min == y.result_min && x.range == y.result_range;
    }

    template <class RealType>
    UPHIDDENINLINE
    bool operator!=(uniform_real_distribution_param<RealType> const& x, uniform_real_distribution_param<RealType> const& y) {
        return x.result_min != y.result_min || x.range != y.result_range;
    }

    template <class RealType>
    class UPVISIBLE uniform_real_distribution
    {
    public:

        typedef RealType result_type;
        typedef uniform_real_distribution_param<RealType> param_type;

    public:

        UPHIDDENINLINE
        explicit uniform_real_distribution(result_type a = 0, result_type b = 1)
        : param_(a, b) {
        }

        UPHIDDENINLINE
        explicit uniform_real_distribution(param_type const& p)
        : param_(p) {
        }

        UPHIDDENINLINE result_type min() const { return param_.a(); }
        UPHIDDENINLINE result_type max() const { return param_.b(); }
        UPHIDDENINLINE result_type a() const { return param_.a(); }
        UPHIDDENINLINE result_type b() const { return param_.b(); }
        UPHIDDENINLINE param_type param() const { return param_; }
        UPHIDDENINLINE void param(const param_type& p) { param_ = p; }

        template <class Engine>
        UPHIDDENINLINE
        result_type operator()(Engine& engine) {
            return (*this)(engine, param_);
        }

        template <class Engine>
        result_type operator()(Engine& engine, param_type const& p);

    private:

        param_type param_;
    };

    template <class RealType>
    UPHIDDENINLINE
    bool operator==(uniform_real_distribution<RealType> const& x, uniform_int_distribution<RealType> const& y) {
        return x.param() == y.param();
    }

    template <class RealType>
    UPHIDDENINLINE
    bool operator!=(uniform_real_distribution<RealType> const& x, uniform_int_distribution<RealType> const& y) {
        return x.param() != y.param();
    }

    template <class RealType> template <class Engine>
    typename uniform_real_distribution<RealType>::result_type
    uniform_real_distribution<RealType>::operator()(Engine& engine, param_type const& p) {
        return (p.result_range * ::up::generate_canonical<RealType, numeric_limits<RealType>::digits>(engine)) + p.result_min;
    }
}

#endif
