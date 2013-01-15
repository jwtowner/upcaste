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

#ifndef UP_CCTYPE_HPP
#define UP_CCTYPE_HPP

#include <up/prolog.hpp>
#include <ctype.h>

#undef isalnum
#undef isalpha
#undef isascii
#undef isblank
#undef iscntrl
#undef isdigit
#undef islower
#undef isgraph
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef tolower
#undef toupper

namespace up
{
    using ::isalnum;
    using ::isalpha;
    using ::iscntrl;
    using ::isdigit;
    using ::islower;
    using ::isgraph;
    using ::isprint;
    using ::ispunct;
    using ::isspace;
    using ::isupper;
    using ::isxdigit;
    using ::tolower;
    using ::toupper;

    inline UPALWAYSINLINE UPPURE
    int fast_isblank(int c) noexcept {
        return (c == ' ') | (c == '\t');
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_isspace(int c) noexcept {
        return (c == ' ') || ((((unsigned int)c) - ((unsigned int)'\t')) <= ((unsigned int)('\t' - '\r')));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_iscntrl(int c) noexcept {
        return (c <= 0x1F) | (c == 0x7F);
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_isdigit(int c) noexcept {
        return (((unsigned int)c) - ((unsigned int)'0')) <= ((unsigned int)('9' - '0'));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_isgraph(int c) noexcept {
        return (((unsigned int)c) - ((unsigned int)'!')) <= ((unsigned int)('~' - '!'));
    }
   
    inline UPALWAYSINLINE UPPURE
    int fast_isprint(int c) noexcept {
        return (((unsigned int)c) - ((unsigned int)' ')) <= ((unsigned int)('~' - ' '));
    }
   
    inline UPALWAYSINLINE UPPURE
    int fast_islower(int c) noexcept {
        return (((unsigned int)c) - ((unsigned int)'a')) <= ((unsigned int)('z' - 'a'));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_isupper(int c) noexcept {
        return (((unsigned int)c) - ((unsigned int)'A')) <= ((unsigned int)('Z' - 'A'));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_isalpha(int c) noexcept {
        return ((((unsigned int)c) | 0x20) - ((unsigned int)'a')) <= ((unsigned int)('z' - 'a'));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_isalnum(int c) noexcept {
        return (((((unsigned int)c) | 0x20) - ((unsigned int)'a')) <= ((unsigned int)('z' - 'a')))
            | ((((unsigned int)c) - ((unsigned int)'0')) <= ((unsigned int)('9' - '0')));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_ispunct(int c) noexcept {
        return ((((unsigned int)c) - ((unsigned int)'!')) <= ((unsigned int)('~' - '!')))
            & (((((unsigned int)c) | 0x20) - ((unsigned int)'a')) > ((unsigned int)('z' - 'a')))
            & ((((unsigned int)c) - ((unsigned int)'0')) > ((unsigned int)('9' - '0')));
    }

    inline UPALWAYSINLINE UPPURE
    int fast_isxdigit(int c) noexcept {
        return ((((unsigned int)c | 0x20) - ((unsigned int)'a')) <= ((unsigned int)('f' - 'a')))
            | ((((unsigned int)c) - ((unsigned int)'0')) <= ((unsigned int)('9' - '0')));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_tolower(int c) noexcept {
        return (int)(((unsigned int)c) ^ ((~(((unsigned int)fast_isupper(c)) - 1)) & 0x20));
    }
    
    inline UPALWAYSINLINE UPPURE
    int fast_toupper(int c) noexcept {
        return (int)(((unsigned int)c) ^ ((~(((unsigned int)fast_islower(c)) - 1)) & 0x20));
    }

    inline UPALWAYSINLINE UPPURE
    int fast_todigit(int c) noexcept {
        unsigned int dvalue = c - ((unsigned int)'0');
        unsigned int dmask = (dvalue <= ((unsigned int)('9' - '0')));
        return (int)((dvalue & (~dmask + 1)) | (dmask - 1));
    }

    inline UPALWAYSINLINE UPPURE
    int fast_toxdigit(int c) noexcept {
        unsigned int nvalue = c - ((unsigned int)'0');
        unsigned int avalue = (c | 0x20) - ((unsigned int)'a');
        unsigned int nmask = (nvalue <= ((unsigned int)('9' - '0')));
        unsigned int amask = (avalue <= ((unsigned int)('f' - 'a')));
        return (int)((nvalue & (~nmask + 1)) | ((avalue + 10) & (~amask + 1)) | ((nmask | amask) - 1));
    }

    inline UPALWAYSINLINE UPPURE
    int fast_todigit(int c, int base) noexcept {
#ifdef UP_EXPENSIVE_BRANCHING
        unsigned int nvalue = c - ((unsigned int)'0');
        unsigned int avalue = (c | 0x20) - ((unsigned int)'a');
        unsigned int nmask = (nvalue <= ((unsigned int)('9' - '0')));
        unsigned int amask = (avalue <= ((unsigned int)('z' - 'a')));
        unsigned int result = (nvalue & (~nmask + 1)) | ((avalue + 10) & (~amask + 1)) | ((nmask | amask) - 1);
        unsigned int rmask = (result < ((unsigned int)base));
        return (int)((result & (~rmask + 1)) | (rmask - 1));
#else
        unsigned int digit_range = base;
        unsigned int letter_range = 0;
        if (base > 10) {
            digit_range = 10;
            letter_range = base - 10;
        }
        unsigned int digit = ((unsigned int)c) - ((unsigned int)'0');
        if (digit < digit_range) {
            return (int)digit;
        }
        digit = ((((unsigned int)c) | 0x20) - ((unsigned int)'a'));
        if (digit < letter_range) {
            return (int)(digit + 10);
        }
        return -1;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    int fast_isdigit(int c, int base) noexcept {
#ifdef UP_EXPENSIVE_BRANCHING
        return (0 <= fast_todigit(c, base));
#else
        unsigned int digit_range = base;
        unsigned int letter_range = 0;
        if (base > 10) {
            digit_range = 10;
            letter_range = base - 10;
        }
        unsigned int digit = ((unsigned int)c) - ((unsigned int)'0');
        if (digit < digit_range) {
            return 1;
        }
        digit = ((((unsigned int)c) | 0x20) - ((unsigned int)'a'));
        if (digit < letter_range) {
            return 1;
        }
        return 0;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    int fast_iseqlower(int c, int expected) noexcept {
        return (c | 0x20) == expected;
    }

    inline UPALWAYSINLINE UPPURE
    int fast_isequpper(int c, int expected) noexcept {
        return (c & 0xDF) == expected;
    }
}

#if defined(UP_HAS_POSIX_LOCALE) && (UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC)
#   include <up/detail/cctype_msvc.inl>
#elif defined(UP_HAS_POSIX_LOCALE) && (UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_POSIX)
#   include <up/detail/cctype_posix.inl>
#else
#   error No POSIX locale extensions compatability layer for target platform!
#endif

#endif
