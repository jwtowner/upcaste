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
#include <up/ctime.hpp>
#include <up/test.hpp>
#include <up/utility.hpp>
#include <random>

namespace cstrings_utf32
{
    std::default_random_engine random_engine(static_cast<up::uint32_t>(up::time(nullptr) & 0xFFFFFFFF));

    enum data_source_generate_type
    {
        data_source_generate_garbage,
        data_source_generate_ascii,
        data_source_generate_unicode,
        data_source_generate_mixed,
    };

	class u32data_source
	{
        size_t result_;
		size_t size_;
		char32_t* data_;
        char32_t* scratch_;
        data_source_generate_type gen_type_;
        bool regenerate_;

    public:

		u32data_source(size_t sz, data_source_generate_type gen = data_source_generate_garbage, bool r = false)
        : result_(0),
        size_(sz),
        data_(new char32_t[sz + 1]),
        scratch_(new char32_t[(sz + 1) * 2]),
        gen_type_(gen),
        regenerate_(r) {
            if (!r) {
                generate();
            }
        }
        
		~u32data_source() { delete[] data_; delete[] scratch_; }
        size_t result() const { return result_; }
        void result(size_t r) { result_ = r; }
		size_t size() const { return size_; }
		char32_t const* data() const { return data_; }
        char32_t* scratch() const { return scratch_; }
        void setup() { if (regenerate_) { generate(); } }
        void teardown() { }

    private:

        void generate() {
            if (gen_type_ == data_source_generate_garbage) {
                std::uniform_int_distribution<char32_t> dist(1, 0x017FFFF);
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = static_cast<char32_t>(dist(random_engine));
                }
            }
            else if (gen_type_ == data_source_generate_ascii) {
                std::uniform_int_distribution<char32_t> dist(1, 0x7F);
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = static_cast<char32_t>(dist(random_engine));
                }
            }
            else {
                char32_t min_char = (gen_type_ == data_source_generate_unicode) ? 0x80 : 0x01;
                std::uniform_int_distribution<char32_t> dist(min_char, 0x010FFFF);
                for (size_t i = 0; i < size_; ++i) {
                    char32_t c = static_cast<char32_t>(dist(random_engine));
                    if (0xD800 <= c && c < 0xE000) {
                        c += 0x801;
                    }
                    data_[i] = c;
                }
            }

            data_[size_] = 0;
        }
	};
    
    //
    // UTF-32 Iteration Benchmarks
    //

    UPNOINLINE void u32memlen_iteration(u32data_source& ds) {
        size_t count = up::u32slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32memnlen_iteration(u32data_source& ds) {
        size_t count = up::u32snlen(ds.data(), ds.size());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32len_iteration(u32data_source& ds) {
        char32_t const* char_ptr = ds.data();
        size_t count = 0;
        
        for (;;) {
            int length = up::u32len(char_ptr);
            if (length == 0) {
                break;
            }
            char_ptr += (length > 0) ? length : 1;
            ++count;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32nlen_iteration(u32data_source& ds) {
        char32_t const* char_ptr = ds.data();
        char32_t const* end_ptr = ds.data() + ds.size() + 1;
        size_t count = 0;
        
        for (;;) {
            int length = up::u32nlen(char_ptr, end_ptr - char_ptr);
            if (length == 0) {
                break;
            }
            char_ptr += (length > 0) ? length : 1;
            ++count;
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u32tou8_iteration(u32data_source& ds) {
        char32_t const* char_ptr = ds.data();
        size_t count = 0;
        
        for ( ; ; ++char_ptr, ++count) {
            int length = up::u32tou8(nullptr, *char_ptr);
            if (length == 0) {
                break;
            }
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u32tou16_iteration(u32data_source& ds) {
        char32_t const* char_ptr = ds.data();
        size_t count = 0;
        
        for ( ; ; ++char_ptr, ++count) {
            int length = up::u32tou16(nullptr, *char_ptr);
            if (length == 0) {
                break;
            }
        }
        
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32slen_iteration(u32data_source& ds) {
        size_t count = up::u32slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32snlen_iteration(u32data_source& ds) {
        size_t count = up::u32snlen(ds.data(), ds.size() + 1);
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u32slen_u8_iteration(u32data_source& ds) {
        size_t count = up::u32stou8slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32snlen_u8_iteration(u32data_source& ds) {
        size_t count = up::u32sntou8slen(ds.data(), ds.size() + 1);
        ds.result(count);
        up::sink_dependency(count);
    }
    
    UPNOINLINE void u32slen_u16_iteration(u32data_source& ds) {
        size_t count = up::u32stou16slen(ds.data());
        ds.result(count);
        up::sink_dependency(count);
    }

    UPNOINLINE void u32snlen_u16_iteration(u32data_source& ds) {
        size_t count = up::u32sntou16slen(ds.data(), ds.size() + 1);
        ds.result(count);
        up::sink_dependency(count);
    }


    UP_TEST_BENCHMARK(utf32_garbage_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u32data_source ds(size);
        
        UP_TEST_RUN_BENCHMARK("u32memlen", size, iterations, &u32memlen_iteration, ds);
        size_t u32memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32memnlen", size, iterations, &u32memnlen_iteration, ds);
        size_t u32memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32len", size, iterations, &u32len_iteration, ds);
        size_t u32len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32nlen", size, iterations, &u32nlen_iteration, ds);
        size_t u32nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32tou8", size, iterations, &u32tou8_iteration, ds);
        size_t u32tou8_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32tou16", size, iterations, &u32tou16_iteration, ds);
        size_t u32tou16_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32slen", size, iterations, &u32slen_iteration, ds);
        size_t u32slen_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32snlen", size, iterations, &u32snlen_iteration, ds);
        size_t u32snlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou8slen", size, iterations, &u32slen_u8_iteration, ds);
        size_t u32slen_u8_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32sntou8slen", size, iterations, &u32snlen_u8_iteration, ds);
        size_t u32snlen_u8_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou16slen", size, iterations, &u32slen_u16_iteration, ds);
        size_t u32slen_u16_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32sntou16slen", size, iterations, &u32snlen_u16_iteration, ds);
        size_t u32snlen_u16_result = ds.result();
        
        require(u32memlen_result == u32memnlen_result);
        require(u32memlen_result >= u32len_result);
        
        require(u32len_result == u32nlen_result);
        require(u32len_result == u32tou8_result);
        require(u32len_result == u32tou16_result);
        require(u32len_result == u32slen_result);
        require(u32len_result == u32snlen_result);
        
        require(u32slen_u8_result == u32snlen_u8_result);
        require(u32slen_u16_result == u32snlen_u16_result);
    }
    
    UP_TEST_BENCHMARK(utf32_ascii_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u32data_source ds(size, data_source_generate_ascii);
        
        UP_TEST_RUN_BENCHMARK("u32memlen", size, iterations, &u32memlen_iteration, ds);
        size_t u32memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32memnlen", size, iterations, &u32memnlen_iteration, ds);
        size_t u32memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32len", size, iterations, &u32len_iteration, ds);
        size_t u32len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32nlen", size, iterations, &u32nlen_iteration, ds);
        size_t u32nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32tou8", size, iterations, &u32tou8_iteration, ds);
        size_t u32tou8_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32tou16", size, iterations, &u32tou16_iteration, ds);
        size_t u32tou16_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32slen", size, iterations, &u32slen_iteration, ds);
        size_t u32slen_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32snlen", size, iterations, &u32snlen_iteration, ds);
        size_t u32snlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou8slen", size, iterations, &u32slen_u8_iteration, ds);
        size_t u32slen_u8_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32sntou8slen", size, iterations, &u32snlen_u8_iteration, ds);
        size_t u32snlen_u8_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou16slen", size, iterations, &u32slen_u16_iteration, ds);
        size_t u32slen_u16_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32sntou16slen", size, iterations, &u32snlen_u16_iteration, ds);
        size_t u32snlen_u16_result = ds.result();

        require(u32memlen_result == u32memnlen_result);
        require(u32memlen_result >= u32len_result);
        
        require(u32len_result == u32nlen_result);
        require(u32len_result == u32tou8_result);
        require(u32len_result == u32tou16_result);
        require(u32len_result == u32slen_result);
        require(u32len_result == u32snlen_result);
        
        require(u32slen_u8_result == u32snlen_u8_result);
        require(u32slen_u16_result == u32snlen_u16_result);
    }
    
    UP_TEST_BENCHMARK(utf32_unicode_iteration) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u32data_source ds(size, data_source_generate_unicode);
        
        UP_TEST_RUN_BENCHMARK("u32memlen", size, iterations, &u32memlen_iteration, ds);
        size_t u32memlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32memnlen", size, iterations, &u32memnlen_iteration, ds);
        size_t u32memnlen_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32len", size, iterations, &u32len_iteration, ds);
        size_t u32len_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32nlen", size, iterations, &u32nlen_iteration, ds);
        size_t u32nlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32tou8", size, iterations, &u32tou8_iteration, ds);
        size_t u32tou8_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32tou16", size, iterations, &u32tou16_iteration, ds);
        size_t u32tou16_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32slen", size, iterations, &u32slen_iteration, ds);
        size_t u32slen_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32snlen", size, iterations, &u32snlen_iteration, ds);
        size_t u32snlen_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou8slen", size, iterations, &u32slen_u8_iteration, ds);
        size_t u32slen_u8_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32sntou8slen", size, iterations, &u32snlen_u8_iteration, ds);
        size_t u32snlen_u8_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou16slen", size, iterations, &u32slen_u16_iteration, ds);
        size_t u32slen_u16_result = ds.result();
   
        UP_TEST_RUN_BENCHMARK("u32sntou16slen", size, iterations, &u32snlen_u16_iteration, ds);
        size_t u32snlen_u16_result = ds.result();
        
        require(u32memlen_result == u32memnlen_result);
        require(u32memlen_result >= u32len_result);
        
        require(u32len_result == u32nlen_result);
        require(u32len_result == u32tou8_result);
        require(u32len_result == u32tou16_result);
        require(u32len_result == u32slen_result);
        require(u32len_result == u32snlen_result);

        require(u32slen_u8_result == u32snlen_u8_result);
        require(u32slen_u16_result == u32snlen_u16_result);
    }

    //
    // UTF-32 Conversion Benchmarks
    //
    
    UPNOINLINE void u32stou8s_conversion(u32data_source& ds) {
        char32_t const* src = ds.data();
        size_t count1 = up::u32stou8slen(ds.data());
        size_t count2 = up::u32stou8s(reinterpret_cast<char*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u32stou8s_guess_conversion(u32data_source& ds) {
        char32_t const* src = ds.data();
        size_t count1 = ds.size() * up::u8_cur_max; // UTF-8 length <= UTF-32 length * up::u8_cur_max
        size_t count2 = up::u32stou8s(reinterpret_cast<char*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u32stou16s_conversion(u32data_source& ds) {
        char32_t const* src = ds.data();
        size_t count1 = up::u32stou16slen(ds.data());
        size_t count2 = up::u32stou16s(reinterpret_cast<char16_t*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UPNOINLINE void u32stou16s_guess_conversion(u32data_source& ds) {
        char32_t const* src = ds.data();
        size_t count1 = ds.size() * up::u16_cur_max; // UTF-16 length <= UTF-32 length * up::u16_cur_max
        size_t count2 = up::u32stou16s(reinterpret_cast<char16_t*>(ds.scratch()), &src, count1 + 1);
        ds.result(count2);
        up::sink_dependency(count1);
        up::sink_dependency(count2);
    }
    
    UP_TEST_BENCHMARK(utf32_garbage_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u32data_source ds(size);
        
        UP_TEST_RUN_BENCHMARK("u32stou8s", size, iterations, &u32stou8s_conversion, ds);
        size_t u32stou8s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou16s", size, iterations, &u32stou16s_conversion, ds);
        size_t u32stou16s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou8s_guess", size, iterations, &u32stou8s_guess_conversion, ds);
        size_t u32stou8s_guess_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32stou16s_guess", size, iterations, &u32stou16s_guess_conversion, ds);
        size_t u32stou16s_guess_result = ds.result();
        
        require(u32stou8s_result == u32stou8s_guess_result);
        require(u32stou16s_result == u32stou16s_guess_result);
    }

    UP_TEST_BENCHMARK(utf32_ascii_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u32data_source ds(size, data_source_generate_ascii);
        
        UP_TEST_RUN_BENCHMARK("u32stou8s", size, iterations, &u32stou8s_conversion, ds);
        size_t u32stou8s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou16s", size, iterations, &u32stou16s_conversion, ds);
        size_t u32stou16s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou8s_guess", size, iterations, &u32stou8s_guess_conversion, ds);
        size_t u32stou8s_guess_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32stou16s_guess", size, iterations, &u32stou16s_guess_conversion, ds);
        size_t u32stou16s_guess_result = ds.result();
        
        require(u32stou8s_result == u32stou8s_guess_result);
        require(u32stou16s_result == u32stou16s_guess_result);
        require(u32stou8s_guess_result == u32stou16s_guess_result);
    }

    UP_TEST_BENCHMARK(utf32_unicode_conversion) {
        unsigned int const iterations = 100;
        unsigned int const size = 2048 * 1024;
        u32data_source ds(size, data_source_generate_unicode);
        
        UP_TEST_RUN_BENCHMARK("u32stou8s", size, iterations, &u32stou8s_conversion, ds);
        size_t u32stou8s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou16s", size, iterations, &u32stou16s_conversion, ds);
        size_t u32stou16s_result = ds.result();
        
        UP_TEST_RUN_BENCHMARK("u32stou8s_guess", size, iterations, &u32stou8s_guess_conversion, ds);
        size_t u32stou8s_guess_result = ds.result();

        UP_TEST_RUN_BENCHMARK("u32stou16s_guess", size, iterations, &u32stou16s_guess_conversion, ds);
        size_t u32stou16s_guess_result = ds.result();
        
        require(u32stou8s_result == u32stou8s_guess_result);
        require(u32stou16s_result == u32stou16s_guess_result);
    }
}
