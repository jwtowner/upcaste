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

#ifndef UP_DETAIL_CSTDLIB_GCC_INL
#define UP_DETAIL_CSTDLIB_GCC_INL

#ifndef UP_CSTDLIB_HPP
#   error "Do not include this header directly. Instead include <up/cstdlib.hpp>"
#endif

#if !defined(malloca) && defined(UP_HAS_STDC_ALLOCA)
#   include <up/detail/malloca_alloca.inl>
#elif !defined(malloca)
#   include <up/detail/malloca_malloc.inl>
#endif

#if defined(UP_HAS_STDC_ALIGNED_ALLOC)
#   include <up/detail/aligned_alloc_c11.inl>
#elif defined(UP_HAS_POSIX_MEMALIGN)
#   include <up/detail/aligned_alloc_posix.inl>
#else
#   include <up/detail/aligned_alloc_generic.inl>
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE
    mul_t mul(int multiplicand, int multiplier) noexcept {
        union { long long product; mul_t retval; };
        product = (long long)multiplicand * multiplier;
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    lmul_t lmul(long multiplicand, long multiplier) noexcept {
#if LONG_MAX < LLONG_MAX
        union { long long product; lmul_t retval; };
        product = (long long)multiplicand * multiplier;
        return retval;
#else
        /*union { __int128 product; lmul_t retval; };
        product = (__int128)multiplicand * multiplier;*/
        lmul_t retval = { multiplicand, multiplier };
        return retval;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    llmul_t llmul(long long multiplicand, long long multiplier) noexcept {
        /*union { __int128 product; llmul_t retval; };
        product = (__int128)multiplicand * multiplier;*/
        llmul_t retval = { multiplicand, multiplier };
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    umul_t umul(unsigned int multiplicand, unsigned int multiplier) noexcept {
        union { unsigned long long product; umul_t retval; };
        product = (unsigned long long)multiplicand * multiplier;
        return retval;
    }

    inline UPALWAYSINLINE UPPURE
    ulmul_t ulmul(unsigned long multiplicand, unsigned long multiplier) noexcept {
#if ULONG_MAX < ULLONG_MAX
        union { unsigned long long product; ulmul_t retval; };
        product = (unsigned long long)multiplicand * multiplier;
        return retval;
#else
        /*union { unsigned __int128 product; ulmul_t retval; };
        product = (unsigned __int128)multiplicand * multiplier;*/
        ulmul_t retval = { multiplicand, multiplier };
        return retval;
#endif
    }

    inline UPALWAYSINLINE UPPURE
    ullmul_t ullmul(unsigned long long multiplicand, unsigned long long multiplier) noexcept {
        /*union { unsigned __int128 product; ullmul_t retval; };
        product = (unsigned __int128)multiplicand * multiplier;*/
        ullmul_t retval = { multiplicand, multiplier };
        return retval;
    }
}

#endif
