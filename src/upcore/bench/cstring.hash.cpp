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

#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/random.hpp>
#include <up/test.hpp>
#include <up/utility.hpp>

namespace cstring_hash
{
    using up::memhash;

    UPNOINLINE UPPURE UPNONNULL(1)
    uint32_t fnv1a_yorrike(void const* key, size_t n, uint32_t seed = UINT32_C(0x811C9DC5)) noexcept {
        constexpr uint32_t hashprime32 = UINT32_C(709607);
        uint32_t const* k32, * k32_end;
        uint32_t hashcode1, hashcode2;
        uintptr_t key_addr;

        hashcode1 = seed;
        hashcode2 = static_cast<uint32_t>(n);

        for (k32 = static_cast<uint32_t const*>(key), k32_end = k32 + ((n >> 2) & ~3); k32 < k32_end; k32 += 4) {
            hashcode1 = (hashcode1 ^ (up::u32rotl(*(k32 + 0), 5) ^ *(k32 + 1))) * hashprime32;
            hashcode2 = (hashcode2 ^ (up::u32rotl(*(k32 + 2), 5) ^ *(k32 + 3))) * hashprime32;
        }

        key_addr = reinterpret_cast<uintptr_t>(k32);
            
        if (n & 8) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint32_t const*>(key_addr)) * hashprime32;
            hashcode2 = (hashcode2 ^ *reinterpret_cast<uint32_t const*>(key_addr + 4)) * hashprime32;
            key_addr += 8;
        }

        if (n & 4) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint16_t const*>(key_addr)) * hashprime32;
            hashcode2 = (hashcode2 ^ *reinterpret_cast<uint16_t const*>(key_addr + 2)) * hashprime32;
            key_addr += 4;
        }

        if (n & 2) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint16_t const*>(key_addr)) * hashprime32;
            key_addr += 2;
        }
            
        if (n & 1) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint8_t const*>(key_addr)) * hashprime32;
        }

        hashcode1 = (hashcode1 ^ up::u32rotl(hashcode2, 5)) * hashprime32;
        return hashcode1 ^ (hashcode1 >> 16);
    }

    UPNOINLINE UPPURE UPNONNULL(1)
    uint64_t fnv1a_tesla(void const* key, size_t n, uint64_t seed = UINT64_C(0xCBF29CE484222325)) noexcept {
        constexpr uint64_t hashprime64 = UINT64_C(11400714819323198393);
        uint64_t const* k64, * k64_end;
        uint64_t hashcode1, hashcode2;
        uintptr_t key_addr;

        hashcode1 = seed;
        hashcode2 = n;

        for (k64 = static_cast<uint64_t const*>(key), k64_end = k64 + ((n >> 3) & ~3); k64 < k64_end; k64 += 4) {
            hashcode1 = (hashcode1 ^ (up::u64rotl(*(k64 + 0), 5) ^ *(k64 + 1))) * hashprime64;
            hashcode2 = (hashcode2 ^ (up::u64rotl(*(k64 + 2), 5) ^ *(k64 + 3))) * hashprime64;
        }

        key_addr = reinterpret_cast<uintptr_t>(k64);
            
        if (n & 16) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint64_t const*>(key_addr)) * hashprime64;
            hashcode2 = (hashcode2 ^ *reinterpret_cast<uint64_t const*>(key_addr + 8)) * hashprime64;
            key_addr += 16;
        }

        if (n & 8) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint32_t const*>(key_addr)) * hashprime64;
            hashcode2 = (hashcode2 ^ *reinterpret_cast<uint32_t const*>(key_addr + 4)) * hashprime64;
            key_addr += 8;
        }

        if (n & 4) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint16_t const*>(key_addr)) * hashprime64;
            hashcode2 = (hashcode2 ^ *reinterpret_cast<uint16_t const*>(key_addr + 2)) * hashprime64;
            key_addr += 4;
        }

        if (n & 2) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint16_t const*>(key_addr)) * hashprime64;
            key_addr += 2;
        }
            
        if (n & 1) {
            hashcode1 = (hashcode1 ^ *reinterpret_cast<uint8_t const*>(key_addr)) * hashprime64;
        }

        hashcode1 = (hashcode1 ^ up::u64rotl(hashcode2, 5)) * hashprime64;
        return hashcode1 ^ (hashcode1 >> 32);
    }

	struct data_source
	{
    public:

		typedef uint32_t const* iterator;
		data_source(size_t sz, bool r = false) : size_(sz), data_(new uint8_t[sz]), regenerate_(r) { if (!r) { generate(); } }
		~data_source() { delete[] data_; }
		size_t size() const { return size_; }
		uint8_t operator[](size_t n) const { return data_[n]; }
		uint8_t const* begin() const { return data_; }
		uint8_t const* end() const { return data_ + size_; }
        void setup() { if (regenerate_) { generate(); } }
        void teardown() { }

    private:

        void generate() {
            up::default_random_engine random_engine(static_cast<up::uint_least32_t>(up::time(nullptr) & UINT_LEAST32_MAX));
            up::uniform_int_distribution<uint8_t> dist(1, UINT8_MAX);
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = dist(random_engine);
            }
        }

		size_t size_;
		uint8_t* data_;
        bool regenerate_;
	};

#define DEFINE_BENCH_HASH(Function, Type) \
    UPNOINLINE \
    void UPCONCATENATE(bench_, Function)(data_source const& ds) { \
        Type result = Function(ds.begin(), ds.size()); \
        up::sink_dependency(result); \
    }

    DEFINE_BENCH_HASH(memhash, uint_least64_t);
    DEFINE_BENCH_HASH(fnv1a_yorrike, uint_least32_t);
    DEFINE_BENCH_HASH(fnv1a_tesla, uint_least64_t);

    UP_TEST_BENCHMARK(small_keys) {
        unsigned int const iterations = 100000;

        for (unsigned int i = 0; i < 16; ++i) {
            data_source ds(i);
            UP_TEST_RUN_BENCHMARK("memhash", i, iterations, &bench_memhash, ds);
            UP_TEST_RUN_BENCHMARK("fnv1a_yorrike", i, iterations, &bench_fnv1a_yorrike, ds);
            UP_TEST_RUN_BENCHMARK("fnv1a_tesla", i, iterations, &bench_fnv1a_tesla, ds);
        }
    }

    UP_TEST_BENCHMARK(big_keys) {
        unsigned int const iterations = 1000;

        for (unsigned int i = 6; i <= 24; ++i) {
            unsigned int const size = 1 << i;
            data_source ds(size);
            UP_TEST_RUN_BENCHMARK("memhash", size, iterations, &bench_memhash, ds);
            UP_TEST_RUN_BENCHMARK("fnv1a_yorrike", size, iterations, &bench_fnv1a_yorrike, ds);
            UP_TEST_RUN_BENCHMARK("fnv1a_tesla", size, iterations, &bench_fnv1a_tesla, ds);
        }
    }
}
