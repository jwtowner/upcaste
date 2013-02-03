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

#ifndef UP_CSTDLIB_HPP
#define UP_CSTDLIB_HPP

#include <up/allocator.hpp>
#include <up/climits.hpp>
#include <up/cstdint.hpp>
#include <stdlib.h>

#undef abs
#undef min
#undef max

namespace up
{
    using ::abs;
    using ::labs;
    using ::llabs;
    using ::div_t;
    using ::ldiv_t;
    using ::lldiv_t;
    using ::div;
    using ::ldiv;
    using ::lldiv;
    using ::rand;
    using ::srand;
    using ::bsearch;
    using ::qsort;
    using ::malloc;
    using ::calloc;
    using ::realloc;
    using ::free;
    using ::abort;
    using ::atexit;
    using ::exit;
    using ::getenv;
#ifdef UP_HAS_STDC_WCHAR
    using ::mblen;
    using ::mbtowc;
    using ::mbstowcs;
    using ::wctomb;
    using ::wcstombs;
#endif

    inline UPALWAYSINLINE UPNONNULLALL
    int syscmd(char const* cmd) noexcept {
        return ::system(cmd);
    }

#ifndef UP_HAS_STDC_QUICK_EXIT
    extern LIBUPCOREAPI UPNONNULL(1)
    void at_quick_exit(void (UPCDECL * func)()) noexcept;
    
    extern LIBUPCOREAPI UPNORETURN
    void quick_exit(int status) noexcept;
#else
    using ::at_quick_exit;
    using ::quick_exit;
#endif

#ifndef UP_HAS_STDC_UNSAFE_EXIT
    extern LIBUPCOREAPI UPNORETURN
    void force_exit(int status) noexcept;
#else
    inline UPALWAYSINLINE UPNORETURN
    void force_exit(int status) noexcept {
        ::_Exit(status);
    }
#endif

#ifndef UP_HAS_STDC_STRTOF
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    float strtof(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept;
#else
    using ::strtof;
#endif

#ifndef UP_HAS_STDC_STRTOD
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    double strtod(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept;
#else
    using ::strtod;
#endif

#ifndef UP_HAS_STDC_STRTOLD
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    long double strtold(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept;
#else
    using ::strtold;
#endif

    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    float fast_strtof(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept;

    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    double fast_strtod(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept;
    
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    long double fast_strtold(char const* UPRESTRICT str, char** UPRESTRICT endptr) noexcept;

    using ::strtol;
    using ::strtoul;

#ifndef UP_HAS_STDC_STRTOLL
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    long long strtoll(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;
#else
    using ::strtoll;
#endif

#ifndef UP_HAS_STDC_STRTOULL
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    unsigned long long strtoull(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;
#else
    using ::strtoull;
#endif

    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    int_least32_t fast_strtoi32(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;
    
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    uint_least32_t fast_strtou32(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;

    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    int_least64_t fast_strtoi64(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;
   
    extern LIBUPCOREAPI UPNONNULL(1) UPWARNRESULT
    uint_least64_t fast_strtou64(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;

    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT
    long fast_strtol(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept {
#if LONG_MAX <= INT_LEAST32_MAX
        return fast_strtoi32(str, endptr, base);
#else
        return fast_strtoi64(str, endptr, base);
#endif
    }

    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT
    unsigned long fast_strtoul(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept {
#if ULONG_MAX <= UINT_LEAST32_MAX
        return fast_strtou32(str, endptr, base);
#else
        return fast_strtou64(str, endptr, base);
#endif
    }

    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT
    long long fast_strtoll(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept {
        return fast_strtoi64(str, endptr, base);
    }
   
    inline UPALWAYSINLINE UPNONNULL(1) UPWARNRESULT
    unsigned long long fast_strtoull(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept {
        return fast_strtou64(str, endptr, base);
    }

#if UP_BYTE_ORDER == UP_LITTLE_ENDIAN
#   define UP_DETAIL_DEFINE_MUL_T(Name, Type) \
    struct LIBUPCOREAPI Name \
    { \
        Type low; \
        Type high; \
    };
#else
#   define UP_DETAIL_DEFINE_MUL_T(Name, Type) \
    struct LIBUPCOREAPI Name \
    { \
        Type high; \
        Type low; \
    };
#endif
    UP_DETAIL_DEFINE_MUL_T(mul_t, int)
    UP_DETAIL_DEFINE_MUL_T(lmul_t, long)
    UP_DETAIL_DEFINE_MUL_T(llmul_t, long long)
    UP_DETAIL_DEFINE_MUL_T(umul_t, unsigned int)
    UP_DETAIL_DEFINE_MUL_T(ulmul_t, unsigned long)
    UP_DETAIL_DEFINE_MUL_T(ullmul_t, unsigned long long)
#   undef UP_DETAIL_DEFINE_MUL_T
}

#if UP_COMPILER == UP_COMPILER_CLANG
#   include <up/detail/cstdlib_clang.inl>
#elif UP_COMPILER == UP_COMPILER_GCC
#   include <up/detail/cstdlib_gcc.inl>
#elif UP_COMPILER == UP_COMPILER_MSVC
#   include <up/detail/cstdlib_msvc.inl>
#else
#   error "Compiler not currently supported!"
#endif

namespace up
{
    class LIBUPCOREAPI malloc_allocator : public allocator
    {
    public:

        static malloc_allocator* instance() noexcept;
        malloc_allocator() noexcept;
        virtual ~malloc_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n = 0) noexcept;
    };

    class LIBUPCOREAPI aligned_malloc_allocator : public allocator
    {
    public:

        aligned_malloc_allocator() noexcept;
        explicit aligned_malloc_allocator(size_t alignment) noexcept;
        virtual ~aligned_malloc_allocator() noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate(size_t n) noexcept;
        virtual UPALLOC UPWARNRESULT void* allocate_zero(size_t n, size_t s) noexcept;
        virtual void deallocate(void* p, size_t n = 0) noexcept;
        size_t alignment() const noexcept;
        void alignment(size_t value) noexcept;

    private:

        size_t alignment_;
    };

#if UP_STDC_EXTENSIONS != UP_STDC_EXTENSIONS_MSVC
    inline UPALWAYSINLINE UPPURE long abs(long x) noexcept { return labs(x); }
    inline UPALWAYSINLINE UPPURE long long abs(long long x) noexcept { return llabs(x); }
    inline UPALWAYSINLINE UPPURE ldiv_t div(long x, long y) noexcept { return ldiv(x, y); }
    inline UPALWAYSINLINE UPPURE lldiv_t div(long long x, long long y) noexcept { return lldiv(x, y); }
#endif
    inline UPALWAYSINLINE UPPURE lmul_t mul(long x, long y) noexcept { return lmul(x, y); }
    inline UPALWAYSINLINE UPPURE ulmul_t umul(unsigned long x, unsigned long y) noexcept { return ulmul(x, y); }
    inline UPALWAYSINLINE UPPURE llmul_t mul(long long x, long long y) noexcept { return llmul(x, y); }
    inline UPALWAYSINLINE UPPURE ullmul_t umul(unsigned long long x, unsigned long long y) noexcept { return ullmul(x, y); }

#if INT_LEAST32_MAX == INT_MAX
    typedef div_t i32div_t;
    typedef mul_t i32mul_t;
    typedef umul_t u32mul_t;
#elif INT_LEAST32_MAX == LONG_MAX
    typedef ldiv_t i32div_t;
    typedef lmul_t i32mul_t;
    typedef ulmul_t u32mul_t;
#elif INT_LEAST32_MAX == LLONG_MAX
    typedef lldiv_t i32div_t;
    typedef llmul_t i32mul_t;
    typedef ullmul_t u32mul_t;
#endif

    inline UPALWAYSINLINE UPPURE int_least32_t i32abs(int_least32_t x) noexcept { return abs(x); }
    inline UPALWAYSINLINE UPPURE i32div_t i32div(int_least32_t x, int_least32_t y) noexcept { return div(x, y); }
    inline UPALWAYSINLINE UPPURE i32mul_t i32mul(int_least32_t x, int_least32_t y) noexcept { return mul(x, y); }
    inline UPALWAYSINLINE UPPURE u32mul_t u32mul(uint_least32_t x, uint_least32_t y) noexcept { return umul(x, y); }

#if INT_LEAST64_MAX == LONG_MAX
    typedef ldiv_t i64div_t;
    typedef lmul_t i64mul_t;
    typedef ulmul_t u64mul_t;
#elif INT_LEAST64_MAX == LLONG_MAX
    typedef lldiv_t i64div_t;
    typedef llmul_t i64mul_t;
    typedef ullmul_t u64mul_t;
#endif

    inline UPALWAYSINLINE UPPURE int_least64_t i64abs(int_least64_t x) noexcept { return abs(x); }
    inline UPALWAYSINLINE UPPURE i64div_t i64div(int_least64_t x, int_least64_t y) noexcept { return div(x, y); }
    inline UPALWAYSINLINE UPPURE i64mul_t i64mul(int_least64_t x, int_least64_t y) noexcept { return mul(x, y); }
    inline UPALWAYSINLINE UPPURE u64mul_t u64mul(uint_least64_t x, uint_least64_t y) noexcept { return umul(x, y); }
}

#endif
