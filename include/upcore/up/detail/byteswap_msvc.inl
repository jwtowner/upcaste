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

#ifndef UP_DETAIL_BYTESWAP_MSVC_INL
#define UP_DETAIL_BYTESWAP_MSVC_INL

#ifndef UP_BYTESWAP_HPP
#   error "Do not include this file directly, instead include <up/byteswap.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    int16_t byteswap_int16(int16_t value) noexcept {
        uint16_t result = ::_byteswap_ushort(reinterpret_cast<uint16_t&>(value));
        return reinterpret_cast<int16_t&>(result);
    }

    inline UPALWAYSINLINE UPPURE 
    uint16_t byteswap_uint16(uint16_t value) noexcept {
        return ::_byteswap_ushort(value);
    }

    inline UPALWAYSINLINE UPPURE
    int32_t byteswap_int32(int32_t value) noexcept {
        uint32_t result = ::_byteswap_ulong(reinterpret_cast<uint32_t&>(value));
        return reinterpret_cast<int32_t&>(result);
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t byteswap_uint32(uint32_t value) noexcept {
        return ::_byteswap_ulong(value);
    }

    inline UPALWAYSINLINE UPPURE
    int64_t byteswap_int64(int64_t value) noexcept {
        uint64_t result = ::_byteswap_uint64(reinterpret_cast<uint64_t&>(value));
        return reinterpret_cast<int64_t&>(result);
    }

    inline UPALWAYSINLINE UPPURE
    uint64_t byteswap_uint64(uint64_t value) noexcept {
        return ::_byteswap_uint64(value);
    }

    inline UPALWAYSINLINE UPPURE
    int16_t byteswap_int16(int16_t value, bool condition) noexcept {
        return (condition) ? byteswap_int16(value) : value;
    }

    inline UPALWAYSINLINE UPPURE
    uint16_t byteswap_uint16(uint16_t value, bool condition) noexcept {
        return (condition) ? byteswap_uint16(value) : value;
    }

    inline UPALWAYSINLINE UPPURE
    int32_t byteswap_int32(int32_t value, bool condition) noexcept {
        return (condition) ? byteswap_int32(value) : value;
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t byteswap_uint32(uint32_t value, bool condition) noexcept {
        return (condition) ? byteswap_uint32(value) : value;
    }

    inline UPALWAYSINLINE UPPURE
    int64_t byteswap_int64(int64_t value, bool condition) noexcept {
        return (condition) ? byteswap_int64(value) : value;
    }

    inline UPALWAYSINLINE UPPURE
    uint64_t byteswap_uint64(uint64_t value, bool condition) noexcept {
        return (condition) ? byteswap_uint64(value) : value;
    }
}

#endif
