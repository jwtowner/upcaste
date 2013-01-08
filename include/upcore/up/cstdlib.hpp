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
#include <up/cstdint.hpp>
#include <stdlib.h>

#if (UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC)
#   include <up/detail/cstdlib_msvc.inl>
#else
#   if !defined(malloca) && defined(UP_HAS_STDC_ALLOCA)
#       include <up/detail/malloca_alloca.inl>
#   elif !defined(malloca)
#       include <up/detail/malloca_malloc.inl>
#   endif
#   if defined(UP_HAS_STDC_ALIGNED_ALLOC)
#       include <up/detail/aligned_alloc_c11.inl>
#   elif defined(UP_HAS_POSIX_MEMALIGN)
#       include <up/detail/aligned_alloc_posix.inl>
#   else
#       include <up/detail/aligned_alloc_generic.inl>
#   endif
#endif

#undef abs
#undef min
#undef max

namespace up
{
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

    using ::malloc;
    using ::calloc;
    using ::realloc;
    using ::free;

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

    using ::abort;
    using ::atexit;
    using ::exit;

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

    inline UPALWAYSINLINE UPNONNULLALL
    int syscmd(char const* cmd) noexcept {
        return ::system(cmd);
    }

    using ::getenv;
    using ::rand;
    using ::srand;
    using ::bsearch;
    using ::qsort;
    using ::abs;
    using ::labs;
    using ::llabs;
    using ::div_t;
    using ::ldiv_t;
    using ::lldiv_t;
    using ::div;
    using ::ldiv;
    using ::lldiv;
#ifdef UP_HAS_STDC_WCHAR
    using ::mblen;
    using ::mbtowc;
    using ::mbstowcs;
    using ::wctomb;
    using ::wcstombs;
#endif
}

#endif
