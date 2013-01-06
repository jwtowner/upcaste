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

#ifndef UP_BYTESWAP_HPP
#define UP_BYTESWAP_HPP

#include <up/cstdint.hpp>
#if (UP_COMPILER == UP_COMPILER_GCC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_ARM)
#       include <up/detail/byteswap_gcc_arm.inl>
#   elif (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/byteswap_gcc_x86_x64.inl>
#   else
#       error "Platform does not have a byteswap implementation!"
#   endif
#elif (UP_COMPILER == UP_COMPILER_MSVC)
#   include <up/detail/byteswap_msvc.inl>
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE  int16_t byteswap(int16_t x) noexcept { return byteswap_int16(x); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap(int32_t x) noexcept { return byteswap_int32(x); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap(int64_t x) noexcept { return byteswap_int64(x); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap(uint16_t x) noexcept { return byteswap_uint16(x); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap(uint32_t x) noexcept { return byteswap_uint32(x); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap(uint64_t x) noexcept { return byteswap_uint64(x); }
    inline UPALWAYSINLINE UPPURE  int16_t byteswap(int16_t x, bool c) noexcept { return byteswap_int16(x, c); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap(int32_t x, bool c) noexcept { return byteswap_int32(x, c); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap(int64_t x, bool c) noexcept { return byteswap_int64(x, c); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap(uint16_t x, bool c) noexcept { return byteswap_uint16(x, c); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap(uint32_t x, bool c) noexcept { return byteswap_uint32(x, c); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap(uint64_t x, bool c) noexcept { return byteswap_uint64(x, c); }

#if (UP_BYTE_ORDER == UP_BIG_ENDIAN)
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_be(int16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_be(int32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_be(int64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_be(uint16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_be(uint32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_be(uint64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_le(int16_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_le(int32_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_le(int64_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_le(uint16_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_le(uint32_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_le(uint64_t x) noexcept { return byteswap(x); }
#else
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_be(int16_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_be(int32_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_be(int64_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_be(uint16_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_be(uint32_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_be(uint64_t x) noexcept { return byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_le(int16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_le(int32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_le(int64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_le(uint16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_le(uint32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_le(uint64_t x) noexcept { return x; }
#endif

#if (UP_BYTE_ORDER == UP_BIG_ENDIAN)
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_be_int16(int16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_be_int32(int32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_be_int64(int64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_be_uint16(uint16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_be_uint32(uint32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_be_uint64(uint64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_le_int16(int16_t x) noexcept { return byteswap_int16(x); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_le_int32(int32_t x) noexcept { return byteswap_int32(x); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_le_int64(int64_t x) noexcept { return byteswap_int64(x); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_le_uint16(uint16_t x) noexcept { return byteswap_uint16(x); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_le_uint32(uint32_t x) noexcept { return byteswap_uint32(x); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_le_uint64(uint64_t x) noexcept { return byteswap_uint64(x); }
#else
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_be_int16(int16_t x) noexcept { return byteswap_int16(x); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_be_int32(int32_t x) noexcept { return byteswap_int32(x); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_be_int64(int64_t x) noexcept { return byteswap_int64(x); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_be_uint16(uint16_t x) noexcept { return byteswap_uint16(x); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_be_uint32(uint32_t x) noexcept { return byteswap_uint32(x); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_be_uint64(uint64_t x) noexcept { return byteswap_uint64(x); }
    inline UPALWAYSINLINE UPPURE  int16_t byteswap_le_int16(int16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap_le_int32(int32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap_le_int64(int64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap_le_uint16(uint16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap_le_uint32(uint32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap_le_uint64(uint64_t x) noexcept { return x; }
#endif
}

#endif
