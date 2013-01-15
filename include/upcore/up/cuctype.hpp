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

#ifndef UP_CUCTYPE_HPP
#define UP_CUCTYPE_HPP

#include <up/cstdint.hpp>

namespace up
{
    extern LIBUPCOREAPI UPPURE int isualnum(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isualpha(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isublank(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isucntrl(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isugraph(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isuprint(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isupunct(uint_least32_t uc) noexcept;
    extern LIBUPCOREAPI UPPURE int isuspace(uint_least32_t uc) noexcept;

    inline UPALWAYSINLINE UPPURE
    int isudigit(uint_least32_t uc) noexcept {
        return (uc - ((uint_least32_t)'0')) <= ((uint_least32_t)('9' - '0'));
    }

    inline UPALWAYSINLINE UPPURE
    int isuxdigit(uint_least32_t uc) noexcept {
        return ((uc - ((uint_least32_t)'0')) <= ((uint_least32_t)('9' - '0')))
            | (((uc | 0x20) - ((uint_least32_t)'a')) <= ((uint_least32_t)('f' - 'a')));
    }

    inline UPALWAYSINLINE UPPURE
    int toudigit(uint_least32_t uc) noexcept {
        uint_least32_t dvalue = uc - ((uint_least32_t)'0');
        uint_least32_t dmask = (dvalue <= ((uint_least32_t)('9' - '0')));
        return (int)((dvalue & (~dmask + 1)) | (dmask - 1));
    }

    inline UPALWAYSINLINE UPPURE
    int touxdigit(uint_least32_t uc) noexcept {
        uint_least32_t nvalue = uc - ((uint_least32_t)'0');
        uint_least32_t avalue = (uc | 0x20) - ((uint_least32_t)'a');
        uint_least32_t nmask = (nvalue <= ((uint_least32_t)('9' - '0')));
        uint_least32_t amask = (avalue <= ((uint_least32_t)('f' - 'a')));
        return (int)((nvalue & (~nmask + 1)) | ((avalue + 10) & (~amask + 1)) | ((nmask | amask) - 1));
    }

    inline UPALWAYSINLINE UPPURE
    int toudigit(uint_least32_t uc, int base) noexcept {
#ifdef UP_EXPENSIVE_BRANCHING
        uint_least32_t nvalue = uc - ((uint_least32_t)'0');
        uint_least32_t avalue = (uc | 0x20) - ((uint_least32_t)'a');
        uint_least32_t nmask = (nvalue <= ((uint_least32_t)('9' - '0')));
        uint_least32_t amask = (avalue <= ((uint_least32_t)('z' - 'a')));
        uint_least32_t result = (nvalue & (~nmask + 1)) | ((avalue + 10) & (~amask + 1)) | ((nmask | amask) - 1);
        uint_least32_t rmask = (result < ((uint_least32_t)base));
        return (int)((result & (~rmask + 1)) | (rmask - 1));
#else
        uint_least32_t digit_range = base;
        uint_least32_t letter_range = 0;
        if (base > 10) {
            digit_range = 10;
            letter_range = base - 10;
        }
        uint_least32_t digit = uc - '0';
        if (digit < digit_range) {
            return (int)digit;
        }
        digit = ((uc | 0x20) - 'a');
        if (digit < letter_range) {
            return (int)(digit + 10);
        }
        return -1;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    int isudigit(uint_least32_t uc, int base) noexcept {
#ifdef UP_EXPENSIVE_BRANCHING
        return (0 <= toudigit(uc, base));
#else
        uint_least32_t digit_range = base;
        uint_least32_t letter_range = 0;
        if (base > 10) {
            digit_range = 10;
            letter_range = base - 10;
        }
        uint_least32_t digit = uc - '0';
        if (digit < digit_range) {
            return 1;
        }
        digit = ((uc | 0x20) - 'a');
        if (digit < letter_range) {
            return 1;
        }
        return 0;
#endif
    }
}

#endif
