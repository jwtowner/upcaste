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

#ifndef UP_DETAIL_CSTDLIB_MSVC_INL
#define UP_DETAIL_CSTDLIB_MSVC_INL

#ifndef UP_CSTDLIB_HPP
#   error "Do not include this header directly. Instead include <up/cstdlib.hpp>"
#endif

#define malloca(n) _malloca(n)
#define freea(p) _freea(p)

namespace up
{
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    void* aligned_alloc(size_t alignment, size_t size) noexcept {
        return _aligned_malloc(size, alignment);
    }
    
    inline UPALWAYSINLINE
    void aligned_free(void* ptr) noexcept {
        _aligned_free(ptr);
    }

    inline UPALWAYSINLINE UPPURE
    mul_t mul(int multiplicand, int multiplier) noexcept {
        union { long long product; mul_t retval; };
        product = __emul(multiplicand, multiplier);
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    lmul_t lmul(long multiplicand, long multiplier) noexcept {
        union { long long product; lmul_t retval; };
        product = __emul(multiplicand, multiplier);
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    llmul_t llmul(long long multiplicand, long long multiplier) noexcept {
        llmul_t retval;
#ifdef UP_ARCHITECTURE_64BIT
        retval.low = _mul128(multiplicand, multiplier, &retval.high);
#else
         __asm {
            mov eax, dword ptr [multiplicand]
            imul dword ptr [multiplier]
            mov [retval], eax
            mov [retval + 4], edx
            mov eax, dword ptr [multiplicand + 4]
            imul dword ptr [multiplier]
            add dword ptr [retval + 4], eax
            adc edx, 0
            mov dword ptr [retval + 8], edx
            mov dword ptr [retval + 12], 0
            adc dword ptr [retval + 12], 0
            mov eax, dword ptr [multiplicand]
            imul dword ptr [multiplier + 4]
            add dword ptr [retval + 4], eax
            adc dword ptr [retval + 8], edx
            adc dword ptr [retval + 12], 0
            mov eax, dword ptr [multiplicand + 4]
            imul dword ptr [multiplier + 4]
            add dword ptr [retval + 8], eax
            adc dword ptr [retval + 12], edx
         }
#endif
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    umul_t umul(unsigned int multiplicand, unsigned int multiplier) noexcept {
        union { unsigned long long product; umul_t retval; };
        product = __emulu(multiplicand, multiplier);
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    ulmul_t ulmul(unsigned long multiplicand, unsigned long multiplier) noexcept {
        union { unsigned long long product; ulmul_t retval; };
        product = __emulu(multiplicand, multiplier);
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    ullmul_t ullmul(unsigned long long multiplicand, unsigned long long multiplier) noexcept {
        ullmul_t retval;
#ifdef UP_ARCHITECTURE_64BIT
        retval.low = _umul128(multiplicand, multiplier, &retval.high);
#else
         __asm {
            mov eax, dword ptr [multiplicand]
            mul dword ptr [multiplier]
            mov [retval], eax
            mov [retval + 4], edx
            mov eax, dword ptr [multiplicand + 4]
            mul dword ptr [multiplier]
            add dword ptr [retval + 4], eax
            adc edx, 0
            mov dword ptr [retval + 8], edx
            mov dword ptr [retval + 12], 0
            adc dword ptr [retval + 12], 0
            mov eax, dword ptr [multiplicand]
            mul dword ptr [multiplier + 4]
            add dword ptr [retval + 4], eax
            adc dword ptr [retval + 8], edx
            adc dword ptr [retval + 12], 0
            mov eax, dword ptr [multiplicand + 4]
            mul dword ptr [multiplier + 4]
            add dword ptr [retval + 8], eax
            adc dword ptr [retval + 12], edx
         }
#endif
        return retval;
    }
}

#endif
