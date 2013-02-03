//
//  Upcaste Performance Libraries
//  Copyright c 2012-2013 Jesse W. Towner
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following cs:
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

#ifndef UP_DETAIL_BYTESWAP_CLANG_INL
#define UP_DETAIL_BYTESWAP_CLANG_INL

#ifndef UP_BYTESWAP_HPP
#   error "Do not include this file directly, instead include <up/byteswap.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    int16_t i16byteswap(int16_t x) noexcept {
        uint16_t result = __builtin_bswap16(reinterpret_cast<uint16_t&>(x));
        return reinterpret_cast<int16_t&>(result);
    }

    inline UPALWAYSINLINE UPPURE 
    uint16_t u16byteswap(uint16_t x) noexcept {
        return __builtin_bswap16(x);
    }

    inline UPALWAYSINLINE UPPURE
    int32_t i32byteswap(int32_t x) noexcept {
        uint32_t result = __builtin_bswap32(reinterpret_cast<uint32_t&>(x));
        return reinterpret_cast<int32_t&>(result);
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t u32byteswap(uint32_t x) noexcept {
        return __builtin_bswap32(x);
    }

    inline UPALWAYSINLINE UPPURE
    int64_t i64byteswap(int64_t x) noexcept {
        uint64_t result = __builtin_bswap64(reinterpret_cast<uint64_t&>(x));
        return reinterpret_cast<int64_t&>(result);
    }

    inline UPALWAYSINLINE UPPURE
    uint64_t u64byteswap(uint64_t x) noexcept {
        return __builtin_bswap64(x);
    }

    inline UPALWAYSINLINE UPPURE
    int16_t i16byteswap(int16_t x, bool c) noexcept {
        return c ? i16byteswap(x) : x;
    }

    inline UPALWAYSINLINE UPPURE
    uint16_t u16byteswap(uint16_t x, bool c) noexcept {
        return c ? u16byteswap(x) : x;
    }

    inline UPALWAYSINLINE UPPURE
    int32_t i32byteswap(int32_t x, bool c) noexcept {
        return c ? i32byteswap(x) : x;
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t u32byteswap(uint32_t x, bool c) noexcept {
        return c ? u32byteswap(x) : x;
    }

    inline UPALWAYSINLINE UPPURE
    int64_t i64byteswap(int64_t x, bool c) noexcept {
        return c ? i64byteswap(x) : x;
    }

    inline UPALWAYSINLINE UPPURE
    uint64_t u64byteswap(uint64_t x, bool c) noexcept {
        return c ? u64byteswap(x) : x;
    }
}

#endif

