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

#ifndef UP_DETAIL_BYTESWAP_GCC_ARM_INL
#define UP_DETAIL_BYTESWAP_GCC_ARM_INL

#ifndef UP_BYTESWAP_HPP
#   error "Do not include this file directly. Instead include <up/byteswap.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    int16_t byteswap_int16(int16_t value) noexcept {
#if (UP_COMPILER == UP_COMPILER_GCC) && (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev16 %0, %0" : "+r" (value) );
        return value;
#else
        union { int16_t s16; uint16_t u16; } r = { value };
        r.u16 = (r.u16 <<8) | (r.u16 >> 8);
        return r.s16;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint16_t byteswap_uint16(uint16_t value) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev16 %0, %0" : "+r" (value) );
#else
        value = (value <<8) | (value >> 8);
#endif
        return value;
    }

    inline UPALWAYSINLINE UPPURE
    int32_t byteswap_int32(int32_t value) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev %0, %0" : "+r" (value) );
        return value;
#else
        union { int32_t s32; uint32_t u32; } r = { value };
        r.u32 = ((r.u32 ^ (r.u32 >> 16 | (r.u32 <<16))) & 0xFF00FFFFU)>> 8) ^ (r.u32 >> 8 | r.u32 <<24);
        return r.s32;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t byteswap_uint32(uint32_t value) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ ( "rev %0, %0" : "+r" (value) );
#else
        value = ((value ^ (value >> 16 | (value <<16))) & 0xFF00FFFFU)>> 8) ^ (value >> 8 | value <<24);
#endif
        return value;
    }

    inline UPALWAYSINLINE UPPURE
    int64_t byteswap_int64(int64_t value) noexcept {
        union { uint32_t input32[2]; int64_t output64; } r;
        r.input32[0] = static_cast<uint32_t>(value >> 32);
        r.input32[1] = static_cast<uint32_t>(value);
        r.input32[0] = byteswap(r.input32[0]);
        r.input32[1] = byteswap(r.input32[1]);
        return r.output64;
    }

    inline UPALWAYSINLINE UPPURE
    uint64_t byteswap_uint64(uint64_t value) noexcept {
        union { uint32_t input32[2]; uint64_t output64; } r;
        r.input32[0] = static_cast<uint32_t>(value >> 32);
        r.input32[1] = static_cast<uint32_t>(value);
        r.input32[0] = byteswap(r.input32[0]);
        r.input32[1] = byteswap(r.input32[1]);
        return r.output64;
    }

    inline UPALWAYSINLINE UPPURE
    int16_t byteswap_int16(int16_t value, bool condition) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "rev16ne %0, %0"
            : "+r" (value)
            : "r" (condition)
            : "cc"
        );

        return value;
#else
        union { int16_t s16; uint16_t u16; } r = { value };

        if (condition) {
            r.u16 = (r.u16 <<8) | (r.u16 >> 8);
        }
        
        return r.s16;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint16_t byteswap_uint16(uint16_t value, bool condition) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "rev16ne %0, %0"
            : "+r" (value)
            : "r" (condition)
            : "cc"
        );
#else
        if (condition) {
            value = (value <<8) | (value >> 8);
        }
#endif
        return value;
    }
 
     inline UPALWAYSINLINE UPPURE
     int32_t byteswap_int32(int32_t value, bool condition) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "revne %0, %0"
            : "+r" (value)
            : "r" (condition)
            : "cc"
        );

        return value;
#else
        union { int32_t s32; uint32_t u32; } r = { value };

        if (condition) {
            r.u32 = (((r.u32 ^ (r.u32 >> 16 | (r.u32 <<16))) & 0xFF00FFFFU) >> 8) ^ (r.u32 >> 8 | r.u32 <<24);
        }
        
        return r.s32;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    uint32_t byteswap_uint32(uint32_t value, bool condition) noexcept {
#if (UP_ARCHITECTURE_ARM_VERSION >= 6)
        __asm__ (
            "teq %1, #0\n\t"
            "revne %0, %0"
            : "+r" (value)
            : "r" (condition)
            : "cc"
        );
#else
        if (condition) {
            value = (((value ^ (value >> 16 | (value <<16))) & 0xFF00FFFFU) >> 8) ^ (value >> 8 | value <<24);
        }
#endif
        return value;
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
