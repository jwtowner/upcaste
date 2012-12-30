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

#ifndef UP_DETAIL_CINTTYPES_MSVC_INL
#define UP_DETAIL_CINTTYPES_MSVC_INL

#ifndef UP_CINTTYPES_HPP
#   error "Do not include this header directly. Instead include <up/cinttypes.hpp>"
#endif

#define PRId8 "c"
#define PRIi8 "c"
#define PRIo8 "o"
#define PRIu8 "u"
#define PRIx8 "x"
#define PRIX8 "X"
#define PRId16 "hd"
#define PRIi16 "hi"
#define PRIo16 "ho"
#define PRIu16 "hu"
#define PRIx16 "hx"
#define PRIX16 "hX"
#define PRId32 "d"
#define PRIi32 "i"
#define PRIo32 "o"
#define PRIu32 "u"
#define PRIx32 "x"
#define PRIX32 "X"
#define PRId64 "I64d"
#define PRIi64 "I64i"
#define PRIo64 "I64o"
#define PRIu64 "I64u"
#define PRIx64 "I64x"
#define PRIX64 "I64X"
#define SCNd8 "c"
#define SCNi8 "c"
#define SCNo8 "c"
#define SCNu8 "c"
#define SCNx8 "c"
#define SCNd16 "hd"
#define SCNi16 "hi"
#define SCNo16 "ho"
#define SCNu16 "hu"
#define SCNx16 "hx"
#define SCNd32 "d"
#define SCNi32 "i"
#define SCNo32 "o"
#define SCNu32 "u"
#define SCNx32 "x"
#define SCNd64 "I64d"
#define SCNi64 "I64i"
#define SCNo64 "I64o"
#define SCNu64 "I64u"
#define SCNx64 "I64x"
#define PRIdLEAST8 "c"
#define PRIiLEAST8 "c"
#define PRIoLEAST8 "o"
#define PRIuLEAST8 "u"
#define PRIxLEAST8 "x"
#define PRIXLEAST8 "X"
#define PRIdLEAST16 "hd"
#define PRIiLEAST16 "hi"
#define PRIoLEAST16 "ho"
#define PRIuLEAST16 "hu"
#define PRIxLEAST16 "hx"
#define PRIXLEAST16 "hX"
#define PRIdLEAST32 "d"
#define PRIiLEAST32 "i"
#define PRIoLEAST32 "o"
#define PRIuLEAST32 "u"
#define PRIxLEAST32 "x"
#define PRIXLEAST32 "X"
#define PRIdLEAST64 "I64d"
#define PRIiLEAST64 "I64i"
#define PRIoLEAST64 "I64o"
#define PRIuLEAST64 "I64u"
#define PRIxLEAST64 "I64x"
#define PRIXLEAST64 "I64X"
#define SCNdLEAST8 "c"
#define SCNiLEAST8 "c"
#define SCNoLEAST8 "c"
#define SCNuLEAST8 "c"
#define SCNxLEAST8 "c"
#define SCNdLEAST16 "hd"
#define SCNiLEAST16 "hi"
#define SCNoLEAST16 "ho"
#define SCNuLEAST16 "hu"
#define SCNxLEAST16 "hx"
#define SCNdLEAST32 "d"
#define SCNiLEAST32 "i"
#define SCNoLEAST32 "o"
#define SCNuLEAST32 "u"
#define SCNxLEAST32 "x"
#define SCNdLEAST64 "I64d"
#define SCNiLEAST64 "I64i"
#define SCNoLEAST64 "I64o"
#define SCNuLEAST64 "I64u"
#define SCNxLEAST64 "I64x"
#define PRIdFAST8 "c"
#define PRIiFAST8 "c"
#define PRIoFAST8 "o"
#define PRIuFAST8 "u"
#define PRIxFAST8 "x"
#define PRIXFAST8 "X"
#define PRIdFAST16 "d"
#define PRIiFAST16 "i"
#define PRIoFAST16 "o"
#define PRIuFAST16 "u"
#define PRIxFAST16 "x"
#define PRIXFAST16 "X"
#define PRIdFAST32 "d"
#define PRIiFAST32 "i"
#define PRIoFAST32 "o"
#define PRIuFAST32 "u"
#define PRIxFAST32 "x"
#define PRIXFAST32 "X"
#define PRIdFAST64 "I64d"
#define PRIiFAST64 "I64i"
#define PRIoFAST64 "I64o"
#define PRIuFAST64 "I64u"
#define PRIxFAST64 "I64x"
#define PRIXFAST64 "I64X"
#define SCNdFAST8 "c"
#define SCNiFAST8 "c"
#define SCNoFAST8 "c"
#define SCNuFAST8 "c"
#define SCNxFAST8 "c"
#define SCNdFAST16 "d"
#define SCNiFAST16 "i"
#define SCNoFAST16 "o"
#define SCNuFAST16 "u"
#define SCNxFAST16 "x"
#define SCNdFAST32 "d"
#define SCNiFAST32 "i"
#define SCNoFAST32 "o"
#define SCNuFAST32 "u"
#define SCNxFAST32 "x"
#define SCNdFAST64 "I64d"
#define SCNiFAST64 "I64i"
#define SCNoFAST64 "I64o"
#define SCNuFAST64 "I64u"
#define SCNxFAST64 "I64x"
#define PRIdMAX "I64d"
#define PRIiMAX "I64i"
#define PRIoMAX "I64o"
#define PRIuMAX "I64u"
#define PRIxMAX "I64x"
#define PRIXMAX "I64X"
#define SCNdMAX "I64d"
#define SCNiMAX "I64i"
#define SCNoMAX "I64o"
#define SCNuMAX "I64u"
#define SCNxMAX "I64x"

#if UINTPTR_MAX == UINT32_MAX
#   define PRIdPTR PRId32
#   define PRIiPTR PRIi32
#   define PRIoPTR PRIo32
#   define PRIuPTR PRIu32
#   define PRIxPTR PRIx32
#   define PRIXPTR PRIX32
#   define SCNdPTR SCNd32
#   define SCNiPTR SCNi32
#   define SCNoPTR SCNo32
#   define SCNuPTR SCNu32
#   define SCNxPTR SCNx32
#else
#   define PRIdPTR PRId64
#   define PRIiPTR PRIi64
#   define PRIoPTR PRIo64
#   define PRIuPTR PRIu64
#   define PRIxPTR PRIx64
#   define PRIXPTR PRIX64
#   define SCNdPTR SCNd64
#   define SCNiPTR SCNi64
#   define SCNoPTR SCNo64
#   define SCNuPTR SCNu64
#   define SCNxPTR SCNx64
#endif

namespace up
{
    struct LIBUPCOREAPI imaxdiv_t
    {
        intmax_t quot;
        intmax_t rem;
    };

    inline UPALWAYSINLINE UPPURE
    intmax_t imaxabs(intmax_t j) noexcept {
        return (j < 0) ? -j : j;
    }

    inline UPALWAYSINLINE UPPURE
    imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom) noexcept {
        imaxdiv_t r = { numer / denom, numer % denom };
        return r;
    }

    extern LIBUPCOREAPI
    intmax_t strtoimax(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;

    extern LIBUPCOREAPI
    uintmax_t strtoumax(char const* UPRESTRICT str, char** UPRESTRICT endptr, int base) noexcept;

#ifdef UP_HAS_STDC_WCHAR
    extern LIBUPCOREAPI
    intmax_t wcstoimax(wchar_t const* UPRESTRICT str, wchar_t** UPRESTRICT endptr, int base) noexcept;
    
    extern LIBUPCOREAPI
    uintmax_t wcstoumax(wchar_t const* UPRESTRICT str, wchar_t** UPRESTRICT endptr, int base) noexcept;
#endif
}

#endif
