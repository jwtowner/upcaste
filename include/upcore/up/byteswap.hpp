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
#if UP_COMPILER == UP_COMPILER_CLANG
#   include <up/detail/byteswap_clang.inl>
#elif UP_COMPILER == UP_COMPILER_GCC
#   if UP_ARCHITECTURE == UP_ARCHITECTURE_ARM
#       include <up/detail/byteswap_gcc_arm.inl>
#   elif (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/byteswap_gcc_x86_x64.inl>
#   else
#       error "Platform does not have a byteswap implementation!"
#   endif
#elif UP_COMPILER == UP_COMPILER_MSVC
#   include <up/detail/byteswap_msvc.inl>
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE  int16_t byteswap(int16_t x) noexcept { return i16byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap(int32_t x) noexcept { return i32byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap(int64_t x) noexcept { return i64byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap(uint16_t x) noexcept { return u16byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap(uint32_t x) noexcept { return u32byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap(uint64_t x) noexcept { return u64byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int16_t byteswap(int16_t x, bool c) noexcept { return i16byteswap(x, c); }
    inline UPALWAYSINLINE UPPURE  int32_t byteswap(int32_t x, bool c) noexcept { return i32byteswap(x, c); }
    inline UPALWAYSINLINE UPPURE  int64_t byteswap(int64_t x, bool c) noexcept { return i64byteswap(x, c); }
    inline UPALWAYSINLINE UPPURE uint16_t byteswap(uint16_t x, bool c) noexcept { return u16byteswap(x, c); }
    inline UPALWAYSINLINE UPPURE uint32_t byteswap(uint32_t x, bool c) noexcept { return u32byteswap(x, c); }
    inline UPALWAYSINLINE UPPURE uint64_t byteswap(uint64_t x, bool c) noexcept { return u64byteswap(x, c); }

#if UP_BYTE_ORDER == UP_BIG_ENDIAN
    inline UPALWAYSINLINE UPPURE  int16_t i16byteswap_be(int16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int32_t i32byteswap_be(int32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int64_t i64byteswap_be(int64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint16_t u16byteswap_be(uint16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint32_t u32byteswap_be(uint32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint64_t u64byteswap_be(uint64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int16_t i16byteswap_le(int16_t x) noexcept { return i16byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int32_t i32byteswap_le(int32_t x) noexcept { return i32byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int64_t i64byteswap_le(int64_t x) noexcept { return i64byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint16_t u16byteswap_le(uint16_t x) noexcept { return u16byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint32_t u32byteswap_le(uint32_t x) noexcept { return u32byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint64_t u64byteswap_le(uint64_t x) noexcept { return u64byteswap(x); }
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
    inline UPALWAYSINLINE UPPURE  int16_t i16byteswap_be(int16_t x) noexcept { return i16byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int32_t i32byteswap_be(int32_t x) noexcept { return i32byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int64_t i64byteswap_be(int64_t x) noexcept { return i64byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint16_t u16byteswap_be(uint16_t x) noexcept { return u16byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint32_t u32byteswap_be(uint32_t x) noexcept { return u32byteswap(x); }
    inline UPALWAYSINLINE UPPURE uint64_t u64byteswap_be(uint64_t x) noexcept { return u64byteswap(x); }
    inline UPALWAYSINLINE UPPURE  int16_t i16byteswap_le(int16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int32_t i32byteswap_le(int32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE  int64_t i64byteswap_le(int64_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint16_t u16byteswap_le(uint16_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint32_t u32byteswap_le(uint32_t x) noexcept { return x; }
    inline UPALWAYSINLINE UPPURE uint64_t u64byteswap_le(uint64_t x) noexcept { return x; }
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
}

#endif
