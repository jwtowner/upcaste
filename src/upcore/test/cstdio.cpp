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

#include <up/cstdio.hpp>
#include <up/cmath.hpp>
#include <up/clocale.hpp>
#include <up/climits.hpp>
#include <up/cstring.hpp>
#include <up/test.hpp>

namespace cstdio
{
    namespace
    {
        const char* float_fmt[] = {
            "%.16e",
            "%22.16e",
            "%022.16e",
            "%-22.16e",
            "%#+'022.16e",
            "foo|%#+0123.9E|bar",
            "%-123.9e",
            "%123.9e",
            "%+23.9e",
            "%+05.8e",
            "%-05.8e",
            "%05.8e",
            "%+5.8e",
            "%-5.8e",
            "% 5.8e",
            "%5.8e",
            "%+4.9e",
            "%+#010.0e",
            "%#10.1e",
            "%10.5e",
            "% 10.5e",
            "%5.0e",
            "%5.e",
            "%#5.0e",
            "%#5.e",
            "%3.2e",
            "%3.1e",
            "%-1.5e",
            "%1.5e",
            "%01.3e",
            "%1.e",
            "%.1e",
            "%#.0e",
            "%+.0e",
            "% .0e",
            "%.0e",
            "%#.e",
            "%+.e",
            "% .e",
            "%.e",
            "%4e",
            "%e",
            "%E",
            "% '022f",
            "%+'022f",
            "%-'22f",
            "%'22f",
            "%.16f",
            "%22.16f",
            "%022.16f",
            "%-22.16f",
            "%#+'022.16f",
            "foo|%#+0123.9F|bar",
            "%-123.9f",
            "%123.9f",
            "%+23.9f",
            "%+#010.0f",
            "%#10.1f",
            "%10.5f",
            "% 10.5f",
            "%+05.8f",
            "%-05.8f",
            "%05.8f",
            "%+5.8f",
            "%-5.8f",
            "% 5.8f",
            "%5.8f",
            "%5.0f",
            "%5.f",
            "%#5.0f",
            "%#5.f",
            "%+4.9f",
            "%3.2f",
            "%3.1f",
            "%-1.5f",
            "%1.5f",
            "%01.3f",
            "%1.f",
            "%.1f",
            "%#.0f",
            "%+.0f",
            "% .0f",
            "%.0f",
            "%#.f",
            "%+.f",
            "% .f",
            "%.f",
            "%4f",
            "%f",
            "%F",
            "% '022g",
            "%+'022g",
            "%-'22g",
            "%'22g",
            "%.16g",
            "%22.16g",
            "%022.16g",
            "%-22.16g",
            "%#+'022.16g",
            "foo|%#+0123.9G|bar",
            "%-123.9g",
            "%123.9g",
            "%+23.9g",
            "%+05.8g",
            "%-05.8g",
            "%05.8g",
            "%+5.8g",
            "%-5.8g",
            "% 5.8g",
            "%5.8g",
            "%+4.9g",
            "%+#010.0g",
            "%#10.1g",
            "%10.5g",
            "% 10.5g",
            "%5.0g",
            "%5.g",
            "%#5.0g",
            "%#5.g",
            "%3.2g",
            "%3.1g",
            "%-1.5g",
            "%1.5g",
            "%01.3g",
            "%1.g",
            "%.1g",
            "%#.0g",
            "%+.0g",
            "% .0g",
            "%.0g",
            "%#.g",
            "%+.g",
            "% .g",
            "%.g",
            "%4g",
            "%g",
            "%G",
            nullptr
        };

        double float_val[] = {
            -DBL_EPSILON,
            DBL_EPSILON,
            -DBL_MIN,
            DBL_MIN,
            -DBL_MAX,
            DBL_MAX,
            -DBL_INFINITY,
            DBL_INFINITY,
            -DBL_NAN,
            DBL_NAN,
            -4.136,
            -134.52,
            -5.04030201,
            -3410.01234,
            -999999.999999,
            -913450.29876,
            -913450.2,
            -91345.2,
            -9134.2,
            -913.2,
            -91.2,
            -9.2,
            -9.9,
            4.136,
            134.52,
            5.04030201,
            3410.01234,
            999999.999999,
            913450.29876,
            913450.2,
            91345.2,
            9134.2,
            913.2,
            91.2,
            9.2,
            9.9,
            9.96,
            9.996,
            9.9996,
            9.99996,
            9.999996,
            9.9999996,
            9.99999996,
            0.99999996,
            0.99999999,
            0.09999999,
            0.00999999,
            0.00099999,
            0.00009999,
            0.00000999,
            0.00000099,
            0.00000009,
            0.00000001,
            0.0000001,
            0.000001,
            0.00001,
            0.0001,
            0.001,
            0.01,
            0.1,
            1.0,
            1.5,
            -1.5,
            -1.0,
            -0.1,
            0
        };

        const char* int_fmt[] = {
            "foo|%0123d|bar",
            "% '0123d",
            "%+'0123d",
            "%-'123d",
            "%'123d",
            "%123.9d",
            "% 123.9d",
            "%+123.9d",
            "%-123.9d",
            "%0123d",
            "% 0123d",
            "%+0123d",
            "%-0123d",
            "%10.5d",
            "% 10.5d",
            "%+10.5d",
            "%-10.5d",
            "%010d",
            "% 010d",
            "%+010d",
            "%-010d",
            "%4.2d",
            "% 4.2d",
            "%+4.2d",
            "%-4.2d",
            "%04d",
            "% 04d",
            "%+04d",
            "%-04d",
            "%5.5d",
            "%+22.33d",
            "%01.3d",
            "%1.5d",
            "%-1.5d",
            "%44d",
            "%4d",
            "%4.0d",
            "%4.d",
            "%.44d",
            "%.4d",
            "%.0d",
            "%.d",
            "%d",
            nullptr
        };
        
        int int_val[] = {
            INT_MAX,
            INT_MIN,
            -91340,
            91340,
            341,
            134,
            -987654321,
            987654321,
            -123456789,
            123456789,
            0203,
            -1,
            1,
            0
        };
        
        const char* uint_fmt[] = {
            "foo|%0123u|bar",
            "% '0123u",
            "%+'0123u",
            "%-'123u",
            "%'123u",
            "%123.9u",
            "% 123.9u",
            "%+123.9u",
            "%-123.9u",
            "%0123u",
            "% 0123u",
            "%+0123u",
            "%-0123u",
            "%5.5u",
            "%+22.33u",
            "%01.3u",
            "%1.5u",
            "%-1.5u",
            "%44u",
            "%u",
            "foo|%#0123o|bar",
            "%#123.9o",
            "%# 123.9o",
            "%#+123.9o",
            "%#-123.9o",
            "%#0123o",
            "%# 0123o",
            "%#+0123o",
            "%#-0123o",
            "%#5.5o",
            "%#+22.33o",
            "%#01.3o",
            "%#1.5o",
            "%#-1.5o",
            "%#44o",
            "%#o",
            "%123.9o",
            "% 123.9o",
            "%+123.9o",
            "%-123.9o",
            "%0123o",
            "% 0123o",
            "%+0123o",
            "%-0123o",
            "%5.5o",
            "%+22.33o",
            "%01.3o",
            "%1.5o",
            "%-1.5o",
            "%44o",
            "%o",
            "foo|%#0123X|bar",
            "%#123.9x",
            "%# 123.9x",
            "%#+123.9x",
            "%#-123.9x",
            "%#0123x",
            "%# 0123x",
            "%#+0123x",
            "%#-0123x",
            "%#5.5x",
            "%#+22.33x",
            "%#01.3x",
            "%#1.5x",
            "%#-1.5x",
            "%#44x",
            "%#x",
            "%#X",
            "%123.9x",
            "% 123.9x",
            "%+123.9x",
            "%-123.9x",
            "%0123x",
            "% 0123x",
            "%+0123x",
            "%-0123x",
            "%5.5x",
            "%+22.33x",
            "%01.3x",
            "%1.5x",
            "%-1.5x",
            "%44x",
            "%x",
            "%X",
            nullptr
        };

        unsigned int uint_val[] = {
            UINT_MAX,
            91340,
            987654321,
            123456789,
            341,
            134,
            0203,
            1,
            0
        };

        const char* llong_fmt[] = {
            "foo|%0123lld|bar",
            "%123.9lld",
            "% 123.9lld",
            "%+123.9lld",
            "%-123.9lld",
            "%0123lld",
            "% 0123lld",
            "%+0123lld",
            "%-0123lld",
            "%5.5lld",
            "%+22.33lld",
            "%01.3lld",
            "%1.5lld",
            "%-1.5lld",
            "%44lld",
            "%lld",
            nullptr
        };
        
        long long llong_val[] = {
            LLONG_MAX,
            LLONG_MIN,
            -91340,
            91340,
            -987654321,
            987654321,
            -123456789,
            123456789,
            341,
            134,
            0203,
            -1,
            1,
            0
        };
        
        const char* string_fmt[] = {
            "foo|%10.10s|bar",
            "%-10.10s",
            "%10.10s",
            "%10.5s",
            "%5.10s",
            "%10.1s",
            "%1.10s",
            "%10.0s",
            "%0.10s",
            "%-42.5s",
            "%2.s",
            "%.10s",
            "%.1s",
            "%.0s",
            "%.s",
            "%4s",
            "%s",
            nullptr
        };

        const char* string_val[] = {
            "Hello",
            "Hello, world!",
            "Sound check: One, two, three.",
            "This string is a little longer than the other strings.",
            "1",
            "",
            nullptr
        };

        const char* pointer_fmt[] = {
            "foo|%p|bar",
            "%42p",
            "%p",
            "%123.9p",
            "% 123.9p",
            "%+123.9p",
            "%-123.9p",
            "%0123p",
            "% 0123p",
            "%+0123p",
            "%-0123p",
            "%5.5p",
            "%+22.33p",
            "%01.3p",
            "%1.5p",
            "%-1.5p",
            "%44p",
            "foo|%#0123p|bar",
            "%#123.9p",
            "%# 123.9p",
            "%#+123.9p",
            nullptr
        };

        void* pointer_val[] = {
            reinterpret_cast<void*>(0x23ABCDEF),
            reinterpret_cast<void*>(0x00ABCDEF),
            reinterpret_cast<void*>(0x01234567),
            reinterpret_cast<void*>(0x00000089),
            nullptr
        };

        template <class T>
        void generate_data(char const* outfile, char const** format, T const* values) {
            FILE* output;
            int retval;

            output = fopen(outfile, "wt");
            if (!output) {
                perror("could not open file");
                abort();
            }

            for (size_t i = 0; format[i] != nullptr; ++i) {
                for (size_t j = 0; !j || values[j - 1]; ++j) {
                    retval = fprintf(output, format[i], values[j]);
                    assert(retval >= 0);
                    retval = fputc('\n', output);
                    assert(retval == '\n');
                }
            }

            retval = fclose(output);
            assert(!retval);
        }

        template <class T>
        void compare_data(char const* infile, char const** format, T const* values) {
            FILE* input;
            char buf1[1024], buf2[1024];
            int r1, r2;
            char* s;

            input = up::fopen(infile, "rt");
            require(input);

            for (size_t i = 0; format[i] != nullptr; ++i) {
                for (size_t j = 0; !j || values[j - 1]; ++j) {
                    s = up::fgets(buf1, sizeof(buf1), input);
                    require(s);
                    r1 = (int)strlen(s);
                    if (s[r1 - 1] == '\n') {
                        s[--r1] = '\0';
                    }

                    r2 = up::fast_snprintf(buf2, sizeof(buf2), format[i], values[j]);
                    if (r1 != r2 || up::strcmp(buf1, buf2)) {
                        r2 = up::fast_snprintf(buf2, sizeof(buf2), format[i], values[j]);
                        require((r1 == r2) && !up::strcmp(buf1, buf2));
                    }
                }
            }

            up::fclose(input);
        }
    }

    UP_TEST_CASE(fast_snprintf_float) {
        compare_data("../../share/upcore/printf_float_data.txt", float_fmt, float_val);
    }

    UP_TEST_CASE(fast_snprintf_int) {
        compare_data("../../share/upcore/printf_int_data.txt", int_fmt, int_val);
    }
    
    UP_TEST_CASE(fast_snprintf_uint) {
        compare_data("../../share/upcore/printf_uint_data.txt", uint_fmt, uint_val);
    }
    
    UP_TEST_CASE(fast_snprintf_llong) {
        compare_data("../../share/upcore/printf_llong_data.txt", llong_fmt, llong_val);
    }
    
    UP_TEST_CASE(fast_snprintf_string) {
        compare_data("../../share/upcore/printf_string_data.txt", string_fmt, string_val);
    }

    UP_TEST_CASE(fast_snprintf_pointer) {
        compare_data("../../share/upcore/printf_pointer_data.txt", pointer_fmt, pointer_val);
    }
}

#if 0
int main() {
    setlocale(LC_ALL, "en_US");
    generate_data("../../share/upcore/printf_float_data.txt", float_fmt, float_val);
    generate_data("../../share/upcore/printf_int_data.txt", int_fmt, int_val);
    generate_data("../../share/upcore/printf_uint_data.txt", uint_fmt, uint_val);
    generate_data("../../share/upcore/printf_llong_data.txt", llong_fmt, llong_val);
    generate_data("../../share/upcore/printf_string_data.txt", string_fmt, string_val);
    generate_data("../../share/upcore/printf_pointer_data.txt", pointer_fmt, pointer_val);
    return 0;
}
#endif
