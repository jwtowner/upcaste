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

#include <up/cctype.hpp>
#include <up/climits.hpp>
#include <up/cmath.hpp>
#include <up/cstdio.hpp>
#include <up/cstring.hpp>
#include <up/random.hpp>
#include <up/sparsemap.hpp>
#include <up/log.hpp>
#include <up/test.hpp>

namespace cstring_hash
{
    namespace
    {
        typedef uint64_t (* hashfunc_t)(void const*, size_t);

        uint64_t memhash16(void const* p, size_t n) noexcept {
            return up::memhash(p, n, 0) & UINT16_MAX;
        }

        uint64_t memhash32(void const* p, size_t n) noexcept {
            return up::memhash(p, n, 0) & UINT32_MAX;
        }

        uint64_t memhash64(void const* p, size_t n) noexcept {
            return up::memhash(p, n, 0);
        }

        void do_length_test(hashfunc_t func) {
            constexpr size_t key_size = 64;
            char key[key_size];

            constexpr size_t num_hashcodes = key_size + (key_size - 1);
            uint64_t hashcodes[num_hashcodes];

            // make sure that keys with different lengths of zero bytes produce different results
            memset(key, 0, key_size);
            for (size_t i = 0; i < key_size; ++i) {
                hashcodes[i] = (*func)(key, i);
            }

            // make sure that keys with different lengths of non-zeor bytes produce different results
            memset(key, 42, key_size);
            for (size_t i = 1; i < key_size; ++i) {
                hashcodes[i + key_size - 1] = (*func)(key, i);
            }

            // we should have no collisions
            for (size_t i = 0; i < num_hashcodes; ++i) {
                for (size_t j = i + 1; j < num_hashcodes; ++j) {
                    require(hashcodes[i] != hashcodes[j]);
                }
            }
        }

        bool do_avalanche_test(char const* name, hashfunc_t func, uint64_t mask) {
            constexpr size_t MAXPAIR = 80;
            constexpr size_t MAXLEN = 100;
            
            uint8_t qa[MAXLEN+2], qb[MAXLEN+2], * a = &qa[1], * b = &qb[1];
            uint64_t c, d, i, j = 0, k, l, z;
            uint64_t e, f, g, h;
            uint64_t x, y;
            uint64_t hlen;
            bool retval = true;

            up::log_eventf(up::log_level_info, "%s avalanche test, at most %zu trials needed...\n", name, MAXPAIR / 2);

            // for each bit, test if flipping bit produces change in output bit
            for (hlen = 0; hlen < MAXLEN; ++hlen) {
                z = 0;
                for (i = 0; i < hlen; ++i) {
                    for (j = 0; j < 8; ++j) {
                        e = f = g = h = x = y = mask;
                        for (k = 0; k < MAXPAIR; k += 2) {
                            // reset keys
                            for (l = 0; l < (hlen + 1); ++l) {
                                a[l] = b[l] = 0;
                            }
                            
                            // setup a and b as two keys differing in only one bit
                            a[i] ^= (k << j);
                            a[i] ^= (k >> (8 - j));
                            c = (*func)(a, static_cast<size_t>(hlen));
                            
                            b[i] ^= ((k + 1) << j);
                            b[i] ^= ((k + 1) >> (8 - j));
                            d = (*func)(b, static_cast<size_t>(hlen));

                            // update status masks
                            e &= c ^ d;
                            f &= ~(c ^ d);
                            g &= c;
                            h &= ~c;
                            x &= d;
                            y &= ~d;
                    
                            if (!((e | f | g | h | x | y) & mask)) {
                                break;
                            }
                        }
                        if (k > z) {
                            z = k;
                        }
                        if (k >= MAXPAIR) {
                            retval = false;
                            up::log_eventf(
                                up::log_level_info,
                                "\tMix Error: %.8llx %.8llx %.8llx %.8llx %.8llx %.8llx i %lld j %lld len %lld\n",
                                e, f, g, h, x, y, i, j, hlen
                            );
                        }
                        if (z >= MAXPAIR) {
                            goto done;
                        }
                    }
                }
        
            done:

                if (z < MAXPAIR) {
                    up::log_eventf(up::log_level_info, "\tMix Success: %2lld bytes, required %lld trials\n", i, z / 2);
                }
            }

            return retval;
        }

        bool do_correlation_test(char const* name, hashfunc_t func, size_t bits, size_t size, size_t trials) noexcept {
            up::default_random_engine random_engine(static_cast<uint_least32_t>(up::time(nullptr) & UINT_LEAST32_MAX));
            up::uniform_int_distribution<uint8_t> state_distribution(0, UINT8_MAX);

            int* t = static_cast<int*>(up::calloc(sizeof(int) * bits * size * 8, 1));
            uint8_t* state = static_cast<uint8_t*>(up::calloc(size, 1));
            uint8_t* save = static_cast<uint8_t*>(up::calloc(size, 1));

            bool retval = true;
            size_t i, j, k;
            uint64_t inb, outb;
            double x, y, ssq = 0;
            double maxt = 50.0, mint = 50.0;
            double sfactor = 4.0 * 64 / up::math::sqrt(static_cast<double>(trials));

            up::log_eventf(up::log_level_info, "%s 1st order correlation test...\n", name);

            for (i = 0; i < trials; i++) {
                for (j = 0; j < size; j++) {
                    state[j] = state_distribution(random_engine);
                }
                up::memcpy(save, state, size);
                inb = (*func)(state, size);
                for (k = 0; k < (8 * size); k++) {
                    up::memcpy(state, save, size);
                    state[k / 8] ^= 1 << (k & 7);
                    outb = (*func)(state, size) ^ inb;
                    for (j = 0; j < bits; j++) {
                        t[k * bits + j] += static_cast<int>(outb & 1);
                        outb /= 2;
                    }
                }
            }

            for (i = 0; i < (8 * size); i++) {
                for (j = 0; j < bits; j++) {
                    x = t[i * bits + j] * 100.0 / static_cast<double>(trials);
                    if (x > maxt) {
                        maxt = x;
                    }
                    if (x < mint) {
                        mint = x;
                    }

                    y = up::math::fabs(x - 50.0);
                    if (retval && (y > sfactor)) {
                        up::log_eventf(up::log_level_info, "\tBad value %f (%d %d)\n", x, i, j);
                        retval = false;
                    }
            
                    ssq += (x - 50.0) * (x - 50.0);
                }
            }
    
            ssq /= static_cast<double>(bits * 8 * size);
            up::log_eventf(up::log_level_info, "\tMax %f Min %f Variance %f sfactor %f\n", maxt, mint, ssq, sfactor);

            up::free(save);
            up::free(state);
            up::free(t);
            
            return retval;
        }

        struct hash
        {
            size_t operator()(uint64_t x) const {
#if SIZE_MAX >= UINT64_MAX
                return x;
#else
                return static_cast<size_t>(x >> 32) ^ static_cast<size_t>(x & UINT32_MAX);
#endif
            }
        };

        struct equal
        {
            bool operator()(uint64_t x, uint64_t y) const {
                return x == y;
            }
        };

        void do_dictionary_test(char const* name, hashfunc_t func) {
            up::log_eventf(up::log_level_info, "%s dictionary test...\n", name);

            up::allocator* alloc = up::malloc_allocator::instance();
            up::sparsemap<uint64_t, size_t, hash, equal> map;
            char* line_buffer = nullptr;
            int retval, total_words = 0, total_collisions = 0;
            size_t line_buffer_length = 0;
            ssize_t length;

            up::FILE* input = up::fopen("../../share/upcore/english.txt", "rt");
            require(input);

            retval = up::sparsemap_construct(map, alloc, 16384);
            require(retval == up::sparse_success);

            while ((length = up::getline(&line_buffer, &line_buffer_length, input)) >= 0) {
                while ((length > 0) && up::fast_isspace(line_buffer[length - 1])) {
                    --length;
                }

                uint64_t key = (*func)(line_buffer, length);
                up::sparseresult<uint64_t, size_t> result = up::sparsemap_insert(map, alloc, key, size_t(1));
                if (!result.success) {
                    require(result.record);
                    ++result.record->value;
                    up::log_eventf(up::log_level_info, "\t'%.*s' collides %llx\n", length, line_buffer, key);
                    ++total_collisions;
                }
                ++total_words;
            }

            up::sparsemap_destruct(map, alloc, alloc);
            up::free(line_buffer);
            up::fclose(input);

            up::log_eventf(up::log_level_info, "\ttotal words: %d\n", total_words);
            up::log_eventf(up::log_level_info, "\ttotal collisions: %d\n", total_collisions);
        }
    }

    UP_TEST_CASE(avalanche) {
        do_avalanche_test("memhash16", &memhash16, UINT16_MAX);
        do_avalanche_test("memhash32", &memhash32, UINT32_MAX);
        do_avalanche_test("memhash64", &memhash64, UINT64_MAX);
    }

    UP_TEST_CASE(correlation) {
        (void)do_correlation_test;
        /*for (size_t i = 4; i < 8; ++i) {
            do_correlation_test("memhash16", &memhash16, 16, i, 1000000);
            do_correlation_test("memhash32", &memhash32, 32, i, 1000000);
            do_correlation_test("memhash64", &memhash64, 64, i, 1000000);
        }*/
    }

    UP_TEST_CASE(dictionary) {
        do_dictionary_test("memhash32", &memhash32);
        do_dictionary_test("memhash64", &memhash64);
    }

    UP_TEST_CASE(length) {
        do_length_test(&memhash16);
        do_length_test(&memhash32);
        do_length_test(&memhash64);
    }

    uint64_t const reference_values[100] = 
    {
        0x0000000000000000ull, 0xda201cb486d49b4full, 0xb485071ce3c0b6a4ull, 0x752b4933342f74faull,
        0x12e5276834e3186aull, 0x19cddfd02fc92d39ull, 0x49181102fdd50bfdull, 0xbace5de983e2f1beull,
        0x491ff87a3403b419ull, 0xa95dab9ca4f16091ull, 0x58f61ed52dec820dull, 0x84ca3fd980295e2eull,
        0xb6809e0d21c48921ull, 0xccedf6b0755abcc6ull, 0x3ad63f7d13c197e0ull, 0x71d7c4cdd287d73bull,
        0x993a186be0fc10bbull, 0x03bb44ef3346af9dull, 0xdce40668244495b1ull, 0x6ea0b82805d38584ull,
        0xb6923818b72d8988ull, 0xb273889bceb91e71ull, 0xff3de6c11e22d9e8ull, 0x74af3ef88168497bull,
        0xf916d7401399b348ull, 0xad6291ea0f49465eull, 0xf6d4cc79a8c84d2cull, 0x6c3d06a41b173455ull,
        0x2f97297e6851654full, 0x4db1be96139b124eull, 0x00a2ad07df4c425cull, 0xdce85c35aaced712ull,
        0x6b597ec09db532d3ull, 0xca8e121ba4400d88ull, 0x2f94553ef9a59b75ull, 0xcf50125d34cfd14full,
        0xbee5030105891153ull, 0x202681c44045c748ull, 0xfe5048210a89a43bull, 0x1cb342f65533d42aull,
        0x9c28430ac1d88a2eull, 0xaf9c675147c0c6a9ull, 0xab3bf176c85ac3d7ull, 0xcbd9120395187963ull,
        0xacd9cb45b218c726ull, 0x1d39a25a31d4fe0bull, 0xcfc8781388e2af9full, 0x1a7e15ffc4fbe041ull,
        0x083767ed981e7237ull, 0x49a38ede0a0514bbull, 0x765c90f91410d26aull, 0x7cd6e76f6e0ea765ull,
        0x181ae48963edeaddull, 0xe8ffbdf93aa9e802ull, 0x0ef7b6f872661ed5ull, 0xb3013a677bb47d16ull,
        0xf390885184a94e4dull, 0x76d1f1ae2e1709f6ull, 0x1262e8de3f408366ull, 0x792956dfc6bd07ddull,
        0x7a4eb41cd8d67bb1ull, 0xb59e570d9ae6b80aull, 0xe940f07e58a07302ull, 0x5e28787bef77836dull,
        0xdc008ddb490209a1ull, 0xe0bc46cfd1dc37ffull, 0x0770e74f00c3d089ull, 0x670cfc2f8778dce7ull,
        0x4507d0835b10ee5full, 0x0abdde384bcffce2ull, 0x980ad58fe98efedcull, 0x84da72fc08a0ee90ull,
        0x3dcc5a4671de820eull, 0xacd11697e70af8adull, 0x885e74f64be4de65ull, 0xec981bee82f90e8dull,
        0x6bd18a48a3bb3185ull, 0x7e2a085c4f9a09feull, 0x8728b479525089d1ull, 0x137a98fa8e1ce119ull,
        0x27905b7e952c9cd2ull, 0xcb5fde42c4156661ull, 0x496172b3c342fbb8ull, 0xe9da2230605bb61aull,
        0xa4572dcc434c84a4ull, 0xe5a2a8d6d11f0fe5ull, 0xc4cff660e394be32ull, 0xa64bd4af517df17eull,
        0x3bcbff62efdcc69aull, 0x9e954e05719ff968ull, 0xfeaec79efe59d605ull, 0xc6c5ee4c28b96c83ull,
        0x484d23115b0805edull, 0x39752a8259ad358aull, 0x5830766e6ec0a617ull, 0x16a8024c464c0344ull,
        0x74a4cbd5abf8bd09ull, 0x8df52f0f1c332570ull, 0xd7c84cee202e170cull, 0x2a4d51002499cc42ull
    };

    UP_TEST_CASE(reference) {
        up::xorshift128_engine<uint32_t, 11, 19, 8> random_engine;
        up::uniform_int_distribution<uint8_t> state_distribution(0, UINT8_MAX);
        uint8_t data[100];

        for (size_t i = 0; i < 100; ++i) {
            for (size_t j = 0; j < i; ++j) {
                data[j] = state_distribution(random_engine);
            }
            uint64_t x = up::memhash(data, i);
            require(x == reference_values[i]);
        }
    }
}
