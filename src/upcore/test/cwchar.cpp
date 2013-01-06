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

#include <up/prolog.hpp>

#if defined(UP_HAS_STDC_WCHAR) && defined(UP_HAS_STDC_LOCALE)

#include <up/cwchar.hpp>
#include <up/clocale.hpp>
#include <up/test.hpp>

namespace cwchar
{
    UP_TEST_CASE(mbsnrtowcs) {
        up::mbstate_t state;
        char const* src, * orig_src;
        char const* input = "have you read your k&r today?";
        size_t const input_length = strlen(input);
        wchar_t const* expected_output = L"have you read your k&r today?";
        wchar_t output[64];

        require(nullptr != up::setlocale(LC_ALL, "C"));
        up::memset(&state, 0, sizeof(state));

        orig_src = src = "";
        require((0 == up::mbsnrtowcs(nullptr, &src, 0, 0, &state)) && (src == orig_src));

        orig_src = src = "";
        require((0 == up::mbsnrtowcs(nullptr, &src, 1, 0, &state)) && (src == orig_src));

        orig_src = src = input;
        require((input_length == up::mbsnrtowcs(nullptr, &src, input_length, 0, &state)) && (src == orig_src));

        src = input;
        require((input_length == up::mbsnrtowcs(nullptr, &src, input_length + 1, 0, &state)) && (src == orig_src));

        up::wmemset(output, L't', sizeof(output) / sizeof(wchar_t));
        orig_src = src = input;
        require((input_length == up::mbsnrtowcs(output, &src, input_length, input_length, &state)) && (src != nullptr) && (src != orig_src));
        require(0 == up::wcsncmp(output, expected_output, input_length));
        require(L't' == output[30]);

        up::wmemset(output, L't', sizeof(output) / sizeof(wchar_t));
        orig_src = src = input;
        require((input_length == up::mbsnrtowcs(output, &src, input_length + 1, input_length, &state)) && (src != nullptr) && (src != orig_src));
        require(0 == up::wcsncmp(output, expected_output, input_length));
        require(L't' == output[30]);

        up::wmemset(output, L't', sizeof(output) / sizeof(wchar_t));
        orig_src = src = input;
        require((input_length == up::mbsnrtowcs(output, &src, input_length + 1, sizeof(output) / sizeof(wchar_t), &state)) && (src == nullptr));
        require(0 == up::wcscmp(output, expected_output));
        require(L'\0' == output[input_length]);
    }

    UP_TEST_CASE(wcsnrtombs) {
        up::mbstate_t state;
        wchar_t const* src, * orig_src;
        wchar_t const* input = L"have you read your sicp today?";
        size_t const input_length = wcslen(input);
        char const* expected_output = "have you read your sicp today?";
        char output[64];

        require(nullptr != up::setlocale(LC_ALL, "C"));
        up::memset(&state, 0, sizeof(state));

        orig_src = src = L"";
        require((0 == up::wcsnrtombs(nullptr, &src, 0, 0, &state)) && (src == orig_src));

        orig_src = src = L"";
        require((0 == up::wcsnrtombs(nullptr, &src, 1, 0, &state)) && (src == orig_src));

        orig_src = src = input;
        require((input_length == up::wcsnrtombs(nullptr, &src, input_length, 0, &state)) && (src == orig_src));

        src = input;
        require((input_length == up::wcsnrtombs(nullptr, &src, input_length + 1, 0, &state)) && (src == orig_src));

        up::memset(output, 't', sizeof(output));
        orig_src = src = input;
        require((input_length == up::wcsnrtombs(output, &src, input_length, input_length, &state)) && (src != nullptr) && (src != orig_src));
        require(0 == up::strncmp(output, expected_output, input_length));
        require('t' == output[30]);

        up::memset(output, 't', sizeof(output));
        orig_src = src = input;
        require((input_length == up::wcsnrtombs(output, &src, input_length + 1, input_length, &state)) && (src != nullptr) && (src != orig_src));
        require(0 == up::strncmp(output, expected_output, input_length));
        require('t' == output[30]);

        up::memset(output, 't', sizeof(output));
        orig_src = src = input;
        require((input_length == up::wcsnrtombs(output, &src, input_length + 1, sizeof(output), &state)) && (src == nullptr));
        require(0 == up::strcmp(output, expected_output));
        require('\0' == output[input_length]);
    }
}

#endif
