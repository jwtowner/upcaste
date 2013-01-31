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

#ifndef UP_DETAIL_BYTESWAP_GCC_ARM_INL
#define UP_DETAIL_BYTESWAP_GCC_ARM_INL

#ifndef UP_BYTESWAP_HPP
#   error "Do not include this file directly. Instead include <up/byteswap.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    int16_t i16byteswap(int16_t x) noexcept {
#if (UP_COMPILER == UP_COMPILER_GCC) && (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev16 %0, %0" : "+r" (x) );
        return x;
#else
        union { int16_t s16; uint16_t u16; } r = { x };
        r.u16 = (r.u16 << 8) | (r.u16 >> 8);
        return r.s16;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint16_t u16byteswap(uint16_t x) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev16 %0, %0" : "+r" (x) );
#else
        x = (x << 8) | (x >> 8);
#endif
        return x;
    }

    inline UPALWAYSINLINE UPPURE
    int32_t i32byteswap(int32_t x) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev %0, %0" : "+r" (x) );
        return x;
#else
        union { int32_t s32; uint32_t u32; } r = { x };
        r.u32 = (((r.u32 ^ (r.u32 >> 16 | r.u32 << 16)) & 0xFF00FFFFU) >> 8) ^ (r.u32 >> 8 | r.u32 << 24);
        return r.s32;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t u32byteswap(uint32_t x) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev %0, %0" : "+r" (x) );
#else
        x = (((x ^ (x >> 16 | x << 16)) & 0xFF00FFFFU) >> 8) ^ (x >> 8 | x << 24);
#endif
        return x;
    }

    inline UPALWAYSINLINE UPPURE
    int64_t i64byteswap(int64_t x) noexcept {
        union { uint32_t u32[2]; int64_t u64; } r;
        r.u32[0] = static_cast<uint32_t>(x >> 32);
        r.u32[1] = static_cast<uint32_t>(x);
        r.u32[0] = byteswap(r.u32[0]);
        r.u32[1] = byteswap(r.u32[1]);
        return r.u64;
    }

    inline UPALWAYSINLINE UPPURE
    uint64_t u64byteswap(uint64_t x) noexcept {
        union { uint32_t u32[2]; uint64_t u64; } r;
        r.u32[0] = static_cast<uint32_t>(x >> 32);
        r.u32[1] = static_cast<uint32_t>(x);
        r.u32[0] = byteswap(r.u32[0]);
        r.u32[1] = byteswap(r.u32[1]);
        return r.u64;
    }

    inline UPALWAYSINLINE UPPURE
    int16_t i16byteswap(int16_t x, bool c) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "rev16ne %0, %0"
            : "+r" (x)
            : "r" (c)
            : "cc"
        );

        return x;
#else
        union { int16_t s16; uint16_t u16; } r = { x };
        if (c) {
            r.u16 = (r.u16 << 8) | (r.u16 >> 8);
        }
        return r.s16;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint16_t u16byteswap(uint16_t x, bool c) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "rev16ne %0, %0"
            : "+r" (x)
            : "r" (c)
            : "cc"
        );
#else
        if (c) {
            x = (x << 8) | (x >> 8);
        }
#endif
        return x;
    }
 
     inline UPALWAYSINLINE UPPURE
     int32_t i32byteswap(int32_t x, bool c) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "revne %0, %0"
            : "+r" (x)
            : "r" (c)
            : "cc"
        );
        return x;
#else
        union { int32_t s32; uint32_t u32; } r = { x };
        if (c) {
            r.u32 = (((r.u32 ^ (r.u32 >> 16 | r.u32 << 16)) & 0xFF00FFFFU) >> 8) ^ (r.u32 >> 8 | r.u32 << 24);
        }
        return r.s32;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t u32byteswap(uint32_t x, bool c) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "revne %0, %0"
            : "+r" (x)
            : "r" (c)
            : "cc"
        );
#else
        if (c) {
            x = (((x ^ (x >> 16 | x << 16)) & 0xFF00FFFFU) >> 8) ^ (x >> 8 | x << 24);
        }
#endif
        return x;
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
