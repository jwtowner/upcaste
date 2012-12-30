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

#include <up/cuctype.hpp>
#include <up/cuchar.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace up
{
    LIBUPCOREAPI UPPURE
    int isualnum(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 

        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & (C1_ALPHA | C1_DIGIT)) != 0) : 0;
    }

    LIBUPCOREAPI UPPURE
    int isualpha(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 
        
        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & C1_ALPHA) != 0) : 0;
    }

    LIBUPCOREAPI UPPURE
    int isublank(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 
        
        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & C1_BLANK) != 0) : 0;
    }

    LIBUPCOREAPI UPPURE
    int isucntrl(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 
        
        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & C1_CNTRL) != 0) : 0;
    }

    LIBUPCOREAPI UPPURE
    int isugraph(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 
        
        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & ~(C1_BLANK | C1_CNTRL | C1_DEFINED | C1_SPACE)) != 0) : 0;
    }
    
    LIBUPCOREAPI UPPURE
    int isuprint(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 
        
        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & ~(C1_CNTRL | C1_DEFINED)) != 0) : 0;
    }
    
    LIBUPCOREAPI UPPURE
    int isupunct(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 

        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & C1_PUNCT) != 0) : 0;
    }
    
    LIBUPCOREAPI UPPURE
    int isuspace(uint_least32_t uc) noexcept {
        WCHAR buffer[U16_CUR_MAX];
        WORD type[U16_CUR_MAX]; 

        int count = u32towc(buffer, uc);
        if (count <= 0) {
            return 0;
        }

        BOOL result = ::GetStringTypeW(CT_CTYPE1, buffer, count, type);
        return result ? ((type[0] & C1_SPACE) != 0) : 0;
    }
}
