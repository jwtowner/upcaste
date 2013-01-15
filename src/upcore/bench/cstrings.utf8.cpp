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

#include <up/cuchar.hpp>
#include <up/random.hpp>
#include <up/test.hpp>

namespace cstrings_utf8
{
    enum data_source_generate_type
    {
        data_source_generate_garbage,
        data_source_generate_ascii,
        data_source_generate_unicode,
        data_source_generate_mixed,
    };
    
	class data_source
	{
        size_t result_;
		size_t size_;
		char* data_;
        char* scratch_;
        data_source_generate_type gen_type_;
        bool regenerate_;
        
    public:

		data_source(size_t sz, data_source_generate_type gen = data_source_generate_garbage, bool r = false)
        : result_(0),
        size_(sz),
        data_(new char[sz + up::u8_cur_max]),
        scratch_(new char[(sz + up::u8_cur_max) * up::u8_cur_max * 4]),
        gen_type_(gen),
        regenerate_(r) {
            if (!r) {
                generate();
            }
        }
        
		~data_source() { delete[] data_; delete[] scratch_; }
        size_t result() const { return result_; }
        void result(size_t r) { result_ = r; }
		size_t size() const { return size_; }
		char const* data() const { return data_; }
        char* scratch() const { return scratch_; }
        void setup() { if (regenerate_) { generate(); } }
        void teardown() { }

    private:

        void generate() {
            up::default_random_engine random_engine(static_cast<up::uint_least32_t>(up::time(nullptr) & UINT_LEAST32_MAX));

            if (gen_type_ == data_source_generate_garbage) {
                up::uniform_int_distribution<unsigned char> dist(1, 0xFF);
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = static_cast<char>(dist(random_engine));
                }
            }
            else if (gen_type_ == data_source_generate_ascii) {
                up::uniform_int_distribution<unsigned char> dist(1, 0x7F);
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = static_cast<char>(dist(random_engine));
                }
                
                require(!up::u8snchk(data_, size_));
            }
            else {
                char32_t min_char = (gen_type_ == data_source_generate_unicode) ? 0x80 : 0x01;
                up::uniform_int_distribution<char32_t> dist(min_char, 0x010FFFF);
                for (size_t i = 0; i < size_; ) {
                    char32_t codepoint = dist(random_engine);
                    char u8buffer[up::u8_cur_max];
                    int length = up::u32tou8(u8buffer, codepoint);
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
                            data_[i] = u8buffer[j];
                        }
                    }
                }
                
                require(!up::u8snchk(data_, size_));
            }

            data_[size_ + 0] = 0;
            data_[size_ + 1] = 0;
            data_[size_ + 2] = 0;
            data_[size_ + 3] = 0;
        }
	};

    //
    // UTF-8 Iteration Benchmarks
    //

    UPNOINLINE void u8memlen_iteration(data_source& ds) {
        size_t count = up::u8slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u8memnlen_iteration(data_source& ds) {
        size_t count = up::u8snlen(ds.data(), ds.size());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u8len_iteration(data_source& ds) {
        char const* char_ptr = ds.data();
        size_t count = 0;
        
        for (;;) {
            int length = up::u8len(char_ptr);
            if (length == 0) {
                break;
            }
            ++count;
            char_ptr += (length > 0) ? length : -length;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u8nlen_iteration(data_source& ds) {
        char const* char_ptr = ds.data();
        char const* end_ptr = ds.data() + ds.size() + up::u8_cur_max;
        size_t count = 0;
        
        for (;;) {
            int length = up::u8nlen(char_ptr, end_ptr - char_ptr);
            if (length == 0) {
                break;
            }
            ++count;
            char_ptr += (length > 0) ? length : -length;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u8tou32_iteration(data_source& ds) {
        char const* char_ptr = ds.data();
        char const* end_ptr = ds.data() + ds.size() + up::u8_cur_max;
        size_t count = 0;
        
        for (;;) {
            int length = up::u8tou32(nullptr, char_ptr, end_ptr - char_ptr);
            if (length == 0) {
                break;
            }
            ++count;
            char_ptr += (length > 0) ? length : -length;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u8slen_u16_iteration(data_source& ds) {
        size_t count = up::u8stou16slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u8snlen_u16_iteration(data_source& ds) {
        size_t count = up::u8sntou16slen(ds.data(), ds.size() + up::u8_cur_max);
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u8slen_u32_iteration(data_source& ds) {
        size_t count = up::u8stou32slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u8snlen_u32_iteration(data_source& ds) {
        size_t count = up::u8sntou32slen(ds.data(), ds.size() + up::u8_cur_max);
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UP_TEST_BENCHMARK(utf8_garbage_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        data_source ds(size);
        
        UP_TEST_RUN_BENCHMARK("u8memlen", size, iterations, &u8memlen_iteration, ds);
        size_t u8memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8memnlen", size, iterations, &u8memnlen_iteration, ds);
        size_t u8memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8len", size, iterations, &u8len_iteration, ds);
        size_t u8len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8nlen", size, iterations, &u8nlen_iteration, ds);
        size_t u8nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8tou32", size, iterations, &u8tou32_iteration, ds);
        size_t u8tou32_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou16slen", size, iterations, &u8slen_u16_iteration, ds);
        size_t u8slen_u16_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u8sntou16slen", size, iterations, &u8snlen_u16_iteration, ds);
        size_t u8snlen_u16_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou32slen", size, iterations, &u8slen_u32_iteration, ds);
        size_t u8slen_u32_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u8sntou32slen", size, iterations, &u8snlen_u32_iteration, ds);
        size_t u8snlen_u32_result = ds.result();
        
        require(u8memlen_result == u8memnlen_result);
        require(u8memlen_result >= u8len_result);
        require(u8len_result == u8nlen_result);
        require(u8len_result == u8tou32_result);
        require(u8len_result == u8slen_u32_result);
        require(u8len_result == u8snlen_u32_result);
        require(u8slen_u16_result == u8snlen_u16_result);
    }
    
    UP_TEST_BENCHMARK(utf8_ascii_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        data_source ds(size, data_source_generate_ascii);
        
        UP_TEST_RUN_BENCHMARK("u8memlen", size, iterations, &u8memlen_iteration, ds);
        size_t u8memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8memnlen", size, iterations, &u8memnlen_iteration, ds);
        size_t u8memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8len", size, iterations, &u8len_iteration, ds);
        size_t u8len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8nlen", size, iterations, &u8nlen_iteration, ds);
        size_t u8nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8tou32", size, iterations, &u8tou32_iteration, ds);
        size_t u8tou32_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou16slen", size, iterations, &u8slen_u16_iteration, ds);
        size_t u8slen_u16_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u8sntou16slen", size, iterations, &u8snlen_u16_iteration, ds);
        size_t u8snlen_u16_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou32slen", size, iterations, &u8slen_u32_iteration, ds);
        size_t u8slen_u32_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u8sntou32slen", size, iterations, &u8snlen_u32_iteration, ds);
        size_t u8snlen_u32_result = ds.result();
        
        require(u8memlen_result == u8memnlen_result);
        require(u8memlen_result >= u8len_result);
        
        require(u8len_result == u8nlen_result);
        require(u8len_result == u8tou32_result);
        require(u8len_result == u8slen_u32_result);
        require(u8len_result == u8snlen_u32_result);
        
        require(u8slen_u16_result == u8snlen_u16_result);
    }
    
    UP_TEST_BENCHMARK(utf8_unicode_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        data_source ds(size, data_source_generate_unicode);
        
        UP_TEST_RUN_BENCHMARK("u8memlen", size, iterations, &u8memlen_iteration, ds);
        size_t u8memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8memnlen", size, iterations, &u8memnlen_iteration, ds);
        size_t u8memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8len", size, iterations, &u8len_iteration, ds);
        size_t u8len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8nlen", size, iterations, &u8nlen_iteration, ds);
        size_t u8nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8tou32", size, iterations, &u8tou32_iteration, ds);
        size_t u8tou32_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou16slen", size, iterations, &u8slen_u16_iteration, ds);
        size_t u8slen_u16_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u8sntou16slen", size, iterations, &u8snlen_u16_iteration, ds);
        size_t u8snlen_u16_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou32slen", size, iterations, &u8slen_u32_iteration, ds);
        size_t u8slen_u32_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u8sntou32slen", size, iterations, &u8snlen_u32_iteration, ds);
        size_t u8snlen_u32_result = ds.result();
        
        require(u8memlen_result == u8memnlen_result);
        require(u8memlen_result >= u8len_result);
        
        require(u8len_result == u8nlen_result);
        require(u8len_result == u8tou32_result);
        require(u8len_result == u8slen_u32_result);
        require(u8len_result == u8snlen_u32_result);
        
        require(u8slen_u16_result == u8snlen_u16_result);
    }
    
    //
    // UTF-8 Conversion Benchmarks
    //
    
    UPNOINLINE void u8stou16s_conversion(data_source& ds) {
        char const* src = ds.data();
        size_t count1 = up::u8stou16slen(ds.data());
        size_t count2 = up::u8stou16s(reinterpret_cast<char16_t*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u8stou16s_guess_conversion(data_source& ds) {
        char const* src = ds.data();
        size_t count1 = ds.size(); // UTF-16 length <= UTF-8 length
        size_t count2 = up::u8stou16s(reinterpret_cast<char16_t*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u8stou32s_conversion(data_source& ds) {
        char const* src = ds.data();
        size_t count1 = up::u8stou32slen(ds.data());
        size_t count2 = up::u8stou32s(reinterpret_cast<char32_t*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u8stou32s_guess_conversion(data_source& ds) {
        char const* src = ds.data();
        size_t count1 = ds.size(); // UTF-32 length <= UTF-8 length
        size_t count2 = up::u8stou32s(reinterpret_cast<char32_t*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }

    UP_TEST_BENCHMARK(utf8_ascii_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        data_source ds(size, data_source_generate_ascii);
        
        UP_TEST_RUN_BENCHMARK("u8stou16s", size, iterations, &u8stou16s_conversion, ds);
        size_t u8stou16s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8stou32s", size, iterations, &u8stou32s_conversion, ds);
        size_t u8stou32s_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u8stou16s_guess", size, iterations, &u8stou16s_guess_conversion, ds);
        size_t u8stou16s_guess_result = ds.result();
                
        UP_TEST_RUN_BENCHMARK("u8stou32s_guess", size, iterations, &u8stou32s_guess_conversion, ds);
        size_t u8stou32s_guess_result = ds.result();
        
        require(u8stou16s_result == u8stou32s_result);
        require(u8stou16s_result == u8stou16s_guess_result);
        require(u8stou32s_result == u8stou32s_guess_result);
    }

    UP_TEST_BENCHMARK(utf8_unicode_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        data_source ds(size, data_source_generate_unicode);
        
        UP_TEST_RUN_BENCHMARK("u8stou16s", size, iterations, &u8stou16s_conversion, ds);
        size_t u8stou16s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8stou32s", size, iterations, &u8stou32s_conversion, ds);
        size_t u8stou32s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8stou16s_guess", size, iterations, &u8stou16s_guess_conversion, ds);
        size_t u8stou16s_guess_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u8stou32s_guess", size, iterations, &u8stou32s_guess_conversion, ds);
        size_t u8stou32s_guess_result = ds.result();
        
        require(u8stou16s_result == u8stou16s_guess_result);
        require(u8stou32s_result == u8stou32s_guess_result);
    }
}
