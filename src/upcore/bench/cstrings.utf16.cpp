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

#include <up/cuchar.hpp>
#include <up/cstdatomic.hpp>
#include <up/ctime.hpp>
#include <up/test.hpp>
#include <random>

namespace cstrings_utf16
{
	std::default_random_engine random_engine(static_cast<up::uint32_t>(up::time(nullptr) & 0xFFFFFFFF));

    enum data_source_generate_type
    {
        data_source_generate_garbage,
        data_source_generate_ascii,
        data_source_generate_unicode,
        data_source_generate_mixed,
    };
    
	class u16data_source
	{
        size_t result_;
		size_t size_;
		char16_t* data_;
        char16_t* scratch_;
        data_source_generate_type gen_type_;
        bool regenerate_;
        
    public:

		u16data_source(size_t sz, data_source_generate_type gen = data_source_generate_garbage, bool r = false)
        : result_(0),
        size_(sz),
        data_(new char16_t[sz + U16_CUR_MAX]),
        scratch_(new char16_t[(sz + U16_CUR_MAX) * U16_CUR_MAX * 2]),
        gen_type_(gen),
        regenerate_(r) {
            if (!r) {
                generate();
            }
        }
        
		~u16data_source() { delete[] data_; delete[] scratch_; }
        size_t result() const { return result_; }
        void result(size_t r) { result_ = r; }
		size_t size() const { return size_; }
		char16_t const* data() const { return data_; }
        char16_t* scratch() const { return scratch_; }
        void setup() { if (regenerate_) { generate(); } }
        void teardown() { }

    private:

        void generate() {
            if (gen_type_ == data_source_generate_garbage) {
                std::uniform_int_distribution<char16_t> dist(1, 0xFFFF);
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = dist(random_engine);
                }
            }
            else if (gen_type_ == data_source_generate_ascii) {
                std::uniform_int_distribution<char16_t> dist(1, 0x7F);
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = dist(random_engine);
                }
                
                require(!up::u16snchk(data_, size_));
            }
            else {
                char32_t min_char = (gen_type_ == data_source_generate_unicode) ? 0x80 : 0x01;
                std::uniform_int_distribution<char32_t> dist(min_char, 0x010FFFF);
                for (size_t i = 0; i < size_; ) {
                    char32_t codepoint = dist(random_engine);
                    char16_t u16buffer[U16_CUR_MAX];
                    int length = up::u32tou16(u16buffer, codepoint);
                    if (length <= 0) {
                        continue;
                    }

                    if ((i + length) > size_) {
                        for ( ; i < size_; ++i) {
                            data_[i] = 'a';
                        }
                    }
                    else {
                        for (int j = 0; j < length; ++i, ++j) {
                            data_[i] = u16buffer[j];
                        }
                    }
                }
                
                require(!up::u16snchk(data_, size_));
            }

            data_[size_ + 0] = 0;
            data_[size_ + 1] = 0;
        }
	};

    //
    // UTF-8 Iteration Benchmarks
    //

    UPNOINLINE void u16memlen_iteration(u16data_source& ds) {
        size_t count = up::u16slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u16memnlen_iteration(u16data_source& ds) {
        size_t count = up::u16snlen(ds.data(), ds.size());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u16len_iteration(u16data_source& ds) {
        char16_t const* char_ptr = ds.data();
        size_t count = 0;
        
        for (;;) {
            int length = up::u16len(char_ptr);
            if (length == 0) {
                break;
            }
            char_ptr += (length > 0) ? length : 1;
            ++count;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u16nlen_iteration(u16data_source& ds) {
        char16_t const* char_ptr = ds.data();
        char16_t const* end_ptr = ds.data() + ds.size() + U16_CUR_MAX;
        size_t count = 0;
        
        for (;;) {
            int length = up::u16nlen(char_ptr, end_ptr - char_ptr);
            if (length == 0) {
                break;
            }
            char_ptr += (length > 0) ? length : 1;
            ++count;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u16tou32_iteration(u16data_source& ds) {
        char16_t const* char_ptr = ds.data();
        char16_t const* end_ptr = ds.data() + ds.size() + U16_CUR_MAX;
        size_t count = 0;
        
        for (;;) {
            int length = up::u16tou32(nullptr, char_ptr, end_ptr - char_ptr);
            if (length == 0) {
                break;
            }
            char_ptr += (length > 0) ? length : 1;
            ++count;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u16slen_u8_iteration(u16data_source& ds) {
        size_t count = up::u16slen_u8(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u16snlen_u8_iteration(u16data_source& ds) {
        size_t count = up::u16snlen_u8(ds.data(), ds.size() + U16_CUR_MAX);
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u16slen_u32_iteration(u16data_source& ds) {
        size_t count = up::u16slen_u32(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u16snlen_u32_iteration(u16data_source& ds) {
        size_t count = up::u16snlen_u32(ds.data(), ds.size() + U16_CUR_MAX);
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UP_TEST_BENCHMARK(utf16_garbage_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u16data_source ds(size);
        
        UP_TEST_RUN_BENCHMARK("u16memlen", size, iterations, &u16memlen_iteration, ds);
        size_t u16memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16memnlen", size, iterations, &u16memnlen_iteration, ds);
        size_t u16memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16len", size, iterations, &u16len_iteration, ds);
        size_t u16len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16nlen", size, iterations, &u16nlen_iteration, ds);
        size_t u16nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16tou32", size, iterations, &u16tou32_iteration, ds);
        size_t u16tou32_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16slen_u8", size, iterations, &u16slen_u8_iteration, ds);
        size_t u16slen_u8_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u16snlen_u8", size, iterations, &u16snlen_u8_iteration, ds);
        size_t u16snlen_u8_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16slen_u32", size, iterations, &u16slen_u32_iteration, ds);
        size_t u16slen_u32_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u16snlen_u32", size, iterations, &u16snlen_u32_iteration, ds);
        size_t u16snlen_u32_result = ds.result();

        require(u16memlen_result == u16memnlen_result);
        require(u16memlen_result >= u16len_result);
        require(u16len_result == u16nlen_result);
        require(u16len_result == u16tou32_result);
        require(u16len_result == u16slen_u32_result);
        require(u16len_result == u16snlen_u32_result);
        require(u16slen_u8_result == u16snlen_u8_result);
    }
    
    UP_TEST_BENCHMARK(utf16_ascii_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u16data_source ds(size, data_source_generate_ascii);
        
        UP_TEST_RUN_BENCHMARK("u16memlen", size, iterations, &u16memlen_iteration, ds);
        size_t u16memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16memnlen", size, iterations, &u16memnlen_iteration, ds);
        size_t u16memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16len", size, iterations, &u16len_iteration, ds);
        size_t u16len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16nlen", size, iterations, &u16nlen_iteration, ds);
        size_t u16nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16tou32", size, iterations, &u16tou32_iteration, ds);
        size_t u16tou32_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16slen_u8", size, iterations, &u16slen_u8_iteration, ds);
        size_t u16slen_u8_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u16snlen_u8", size, iterations, &u16snlen_u8_iteration, ds);
        size_t u16snlen_u8_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16slen_u32", size, iterations, &u16slen_u32_iteration, ds);
        size_t u16slen_u32_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u16snlen_u32", size, iterations, &u16snlen_u32_iteration, ds);
        size_t u16snlen_u32_result = ds.result();

        require(u16memlen_result == u16memnlen_result);
        require(u16memlen_result >= u16len_result);
        require(u16len_result == u16nlen_result);
        require(u16len_result == u16tou32_result);
        require(u16len_result == u16slen_u32_result);
        require(u16len_result == u16snlen_u32_result);
        require(u16slen_u8_result == u16snlen_u8_result);
    }
    
    UP_TEST_BENCHMARK(utf16_unicode_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u16data_source ds(size, data_source_generate_unicode);
        
        UP_TEST_RUN_BENCHMARK("u16memlen", size, iterations, &u16memlen_iteration, ds);
        size_t u16memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16memnlen", size, iterations, &u16memnlen_iteration, ds);
        size_t u16memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16len", size, iterations, &u16len_iteration, ds);
        size_t u16len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16nlen", size, iterations, &u16nlen_iteration, ds);
        size_t u16nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16tou32", size, iterations, &u16tou32_iteration, ds);
        size_t u16tou32_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16slen_u8", size, iterations, &u16slen_u8_iteration, ds);
        size_t u16slen_u8_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u16snlen_u8", size, iterations, &u16snlen_u8_iteration, ds);
        size_t u16snlen_u8_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16slen_u32", size, iterations, &u16slen_u32_iteration, ds);
        size_t u16slen_u32_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u16snlen_u32", size, iterations, &u16snlen_u32_iteration, ds);
        size_t u16snlen_u32_result = ds.result();

        require(u16memlen_result == u16memnlen_result);
        require(u16memlen_result >= u16len_result);
        require(u16len_result == u16nlen_result);
        require(u16len_result == u16tou32_result);
        require(u16len_result == u16slen_u32_result);
        require(u16len_result == u16snlen_u32_result);
        require(u16slen_u8_result == u16snlen_u8_result);
    }
    
    //
    // UTF-8 Conversion Benchmarks
    //
    
    UPNOINLINE void u16stou8s_conversion(u16data_source& ds) {
        size_t count1 = up::u16slen_u8(ds.data());
        size_t count2 = up::u16stou8s(reinterpret_cast<char*>(ds.scratch()), ds.data(), count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u16stou8s_guess_conversion(u16data_source& ds) {
        size_t count1 = ds.size() * U8_CUR_MAX; // UTF-8 length <= UTF-16 length * U8_CUR_MAX
        size_t count2 = up::u16stou8s(reinterpret_cast<char*>(ds.scratch()), ds.data(), count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u16stou32s_conversion(u16data_source& ds) {
        size_t count1 = up::u16slen(ds.data());
        size_t count2 = up::u16stou32s(reinterpret_cast<char32_t*>(ds.scratch()), ds.data(), count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u16stou32s_guess_conversion(u16data_source& ds) {
        size_t count1 = ds.size(); // UTF-32 length <= UTF-16 length
        size_t count2 = up::u16stou32s(reinterpret_cast<char32_t*>(ds.scratch()), ds.data(), count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }

    UP_TEST_BENCHMARK(utf16_garbage_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u16data_source ds(size);
        
        UP_TEST_RUN_BENCHMARK("u16stou8s", size, iterations, &u16stou8s_conversion, ds);
        size_t u16stou8s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16stou32s", size, iterations, &u16stou32s_conversion, ds);
        size_t u16stou32s_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16stou8s_guess", size, iterations, &u16stou8s_guess_conversion, ds);
        size_t u16stou8s_guess_result = ds.result();
                
        UP_TEST_RUN_BENCHMARK("u16stou32s_guess", size, iterations, &u16stou32s_guess_conversion, ds);
        size_t u16stou32s_guess_result = ds.result();
        
        require(u16stou8s_result == u16stou8s_guess_result);
        require(u16stou32s_result == u16stou32s_guess_result);
    }
    
    UP_TEST_BENCHMARK(utf16_ascii_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u16data_source ds(size, data_source_generate_ascii);
        
        UP_TEST_RUN_BENCHMARK("u16stou8s", size, iterations, &u16stou8s_conversion, ds);
        size_t u16stou8s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16stou32s", size, iterations, &u16stou32s_conversion, ds);
        size_t u16stou32s_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16stou8s_guess", size, iterations, &u16stou8s_guess_conversion, ds);
        size_t u16stou8s_guess_result = ds.result();
                
        UP_TEST_RUN_BENCHMARK("u16stou32s_guess", size, iterations, &u16stou32s_guess_conversion, ds);
        size_t u16stou32s_guess_result = ds.result();
        
        require(u16stou8s_result == u16stou32s_result);
        require(u16stou8s_result == u16stou8s_guess_result);
        require(u16stou32s_result == u16stou32s_guess_result);
    }

    UP_TEST_BENCHMARK(utf16_unicode_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u16data_source ds(size, data_source_generate_unicode);
        
        UP_TEST_RUN_BENCHMARK("u16stou8s", size, iterations, &u16stou8s_conversion, ds);
        size_t u16stou8s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u16stou32s", size, iterations, &u16stou32s_conversion, ds);
        size_t u16stou32s_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u16stou8s_guess", size, iterations, &u16stou8s_guess_conversion, ds);
        size_t u16stou8s_guess_result = ds.result();
                
        UP_TEST_RUN_BENCHMARK("u16stou32s_guess", size, iterations, &u16stou32s_guess_conversion, ds);
        size_t u16stou32s_guess_result = ds.result();
        
        require(u16stou8s_result == u16stou8s_guess_result);
        require(u16stou32s_result == u16stou32s_guess_result);
    }
}
