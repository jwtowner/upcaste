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

#include <up/hash.hpp>
#include <up/cstdatomic.hpp>
#include <up/ctime.hpp>
#include <up/test.hpp>
#include <random>

#if !defined(UP_NO_SIMD) && defined(UP_SIMD_SSE_4_2)
#   include <immintrin.h>
#endif

namespace memory_hash
{
    /*std::default_random_engine random_engine(static_cast<up::uint32_t>(up::time(nullptr) & 0xFFFFFFFF));

	struct data_source
	{
    public:

		typedef uint32_t const* iterator;
		data_source(size_t sz, bool r = false) : size_(sz), data_(new uint32_t[sz]), regenerate_(r) { if (!r) { generate(); } }
		~data_source() { delete[] data_; }
		size_t size() const { return size_; }
		uint32_t operator[](size_t n) const { return data_[n]; }
		uint32_t const* begin() const { return data_; }
		uint32_t const* end() const { return data_ + size_; }
        void setup() { if (regenerate_) { generate(); } }
        void teardown() { }

    private:

        void generate() {
            std::uniform_int_distribution<uint32_t> dist(1, std::numeric_limits<uint32_t>::max());
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = dist(random_engine);
            }
        }

		size_t size_;
		uint32_t* data_;
        bool regenerate_;
	};
    
    template <class Hasher>
    UPNOINLINE void hash(data_source const& ds) {
        Hasher h;
        h.accumulate(ds.begin(), ds.size() * sizeof(uint32_t));
        typename Hasher::result_type result = h.finalize();
        up::sink_dependency(result);
    }
    
    UP_TEST_BENCHMARK(hash) {
        unsigned int const iterations = 1000;
        for (size_t i = 0; i <= 20; ++i) {
            unsigned int const size = 1 << i;
            data_source ds(size);
            UP_TEST_RUN_BENCHMARK("default", size, iterations, &hash<up::hasher>, ds);
            UP_TEST_RUN_BENCHMARK("crc32", size, iterations, &hash<up::crc32_hasher>, ds);
            UP_TEST_RUN_BENCHMARK("crc32_64", size, iterations, &hash<up::crc32_64_hasher>, ds);
#if !defined(UP_NO_SIMD) && defined(UP_SIMD_SSE_4_2)
            UP_TEST_RUN_BENCHMARK("crc32_sse42", size, iterations, &hash<up::crc32_sse42_hasher>, ds);
            UP_TEST_RUN_BENCHMARK("crc32_64_sse42", size, iterations, &hash<up::crc32_64_sse42_hasher>, ds);
#endif
            UP_TEST_RUN_BENCHMARK("fnv1a_jesteress", size, iterations, &hash<up::fnv1a_jesteress_hasher>, ds);
            UP_TEST_RUN_BENCHMARK("fnv1a_jesteress_64", size, iterations, &hash<up::fnv1a_jesteress_64_hasher>, ds);
        }
    }*/
}
