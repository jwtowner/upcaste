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

#ifndef UP_PROLOG_MSVC_HPP
#define UP_PROLOG_MSVC_HPP

//
// versions check
//
#if (_MSC_VER < 1600)
#   error "MSVC++ versions prior to MSVC++ 2010 are not supported."
#elif (_MSC_VER > 1700)
#   error "Unknown MSVC++ compiler version"
#endif

//
// ignored warnings
//
#if (_MSC_VER <= 1700)
#   pragma warning(disable:4231) // nonstandard extension used : 'extern' before template explicit instantiation
#   pragma warning(disable:4530) // C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#endif

//
// dependencies
//
#include <intrin.h>

//
// compiler configuration
//
#define UP_COMPILER UP_COMPILER_MSVC

//
// C++11 feature detection
//
#if (_MSC_VER < 1600)
#   define UP_NO_AUTO_DECLARATIONS
#   define UP_NO_AUTO_MULTIDECLARATIONS
#   define UP_NO_DECLTYPE
#   define UP_NO_LAMBDAS
#   define UP_NO_NULLPTR
#   define UP_NO_RVALUE_REFERENCES
#   define UP_NO_STATIC_ASSERT
#endif

#define UP_NO_ALIGNAS
#define UP_NO_ALIGNOF
#define UP_NO_CONSTEXPR
#define UP_NO_DEFAULTED_FUNCTIONS
#define UP_NO_DELETED_FUNCTIONS
#define UP_NO_EXPLICIT_CONVERSION_OPERATORS
#define UP_NO_SPECIALIZED_EXTERN_TEMPLATES
#define UP_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define UP_NO_INCONSISTENT_EXPLICIT_INSTANTIATIONS
#define UP_NO_INITIALIZER_LISTS
#define UP_NO_NATIVE_CHAR16_T
#define UP_NO_NATIVE_CHAR32_T
#define UP_NO_NOEXCEPT
#define UP_NO_NOEXCEPT_FUNCTION_PTR
#define UP_NO_RAW_LITERALS
#define UP_NO_REFERENCE_QUALIFIED_FUNCTIONS
#define UP_NO_SCOPED_ENUMS
#define UP_NO_SFINAE_EXPR
#define UP_NO_TEMPLATE_ALIASES
#define UP_NO_THREAD_LOCAL
#define UP_NO_UNICODE_LITERALS
#define UP_NO_VARIADIC_TEMPLATES

#ifdef UP_NO_ALIGNAS
#   define alignas(expression) __declspec(align(expression))
#endif
#ifdef UP_NO_ALIGNOF
#   define alignof __alignof
#endif
#ifdef UP_NO_CONSTEXPR
#   define constexpr const
#endif
#ifdef UP_NO_NOEXCEPT
#   define noexcept
#endif
#ifdef UP_NO_THREAD_LOCAL
#   define thread_local __declspec(thread)
#endif

//
// C++98 feature detection
//
#ifndef _CPPUNWIND
#   define UP_NO_EXCEPTIONS
#endif
#ifndef _CPPRTTI
#   define UP_NO_RTTI
#endif
#ifndef _NATIVE_WCHAR_T_DEFINED
#   define UP_NO_NATIVE_WCHAR_T
#endif

//
// library feature detection
//
#if (_MSC_VER >= 1700)
#   define UP_HAS_STDC_MAX_ALIGN_T
#endif

#undef  UP_HAS_STDC_INTTYPES_C99
#undef  UP_HAS_STDC_INTTYPES_CXX11
#define UP_HAS_STDC_LOCALE
#undef  UP_HAS_STDC_MATH_C99
#undef  UP_HAS_STDC_MATH_CXX11
#undef  UP_HAS_STDC_STDALIGN
#undef  UP_HAS_STDC_STDBOOL
#undef  UP_HAS_STDC_THREADS
#define UP_HAS_STDC_TIME
#undef  UP_HAS_STDC_UCHAR_C11
#undef  UP_HAS_STDC_UCHAR_CXX11
#define UP_HAS_STDC_WCHAR
#define UP_HAS_STDC_ALLOCA
#undef  UP_HAS_STDC_ALIGNED_ALLOC
#undef  UP_HAS_STDC_SNPRINTF
#undef  UP_HAS_STDC_STRTOF
#undef  UP_HAS_STDC_STRTOD
#undef  UP_HAS_STDC_STRTOLD
#undef  UP_HAS_STDC_STRTOLL
#undef  UP_HAS_STDC_STRTOULL
#undef  UP_HAS_STDC_QUICK_EXIT
#undef  UP_HAS_STDC_TIMESPEC_GET
#undef  UP_HAS_STDC_UNSAFE_EXIT
#undef  UP_HAS_STDC_WCSTOF
#undef  UP_HAS_STDC_WCSTOD
#undef  UP_HAS_STDC_WCSTOLD
#undef  UP_HAS_STDC_WCSTOLL
#undef  UP_HAS_STDC_WCSTOULL
#undef  UP_HAS_STDC_VFSCANF
#undef  UP_HAS_STDC_VSCANF
#define UP_HAS_STDC_VSNPRINTF
#undef  UP_HAS_STDC_VSSCANF
#undef  UP_HAS_POSIX_THREADS
#undef  UP_HAS_POSIX_ASPRINTF
#undef  UP_HAS_POSIX_VASPRINTF
#define UP_HAS_POSIX_FSEEKO
#define UP_HAS_POSIX_FTELLO
#undef  UP_HAS_POSIX_GETDELIM
#undef  UP_HAS_POSIX_GETLINE
#undef  UP_HAS_POSIX_GETWDELIM
#undef  UP_HAS_POSIX_GETWLINE
#define UP_HAS_POSIX_LOCALE
#undef  UP_HAS_POSIX_MEMALIGN
#undef  UP_HAS_POSIX_MEMSTREAM
#define UP_HAS_POSIX_MEMCCPY
#undef  UP_HAS_POSIX_WMEMCCPY
#undef  UP_HAS_POSIX_SSIZE_T
#undef  UP_HAS_POSIX_STPCPY
#undef  UP_HAS_POSIX_STPNCPY
#define UP_HAS_POSIX_STRCASECMP
#define UP_HAS_POSIX_STRNCASECMP
#define UP_HAS_POSIX_STRDUP
#define UP_HAS_POSIX_STRNLEN
#define UP_HAS_POSIX_STRTOK_R
#undef  UP_HAS_POSIX_WCPCPY
#undef  UP_HAS_POSIX_WCPNCPY
#define UP_HAS_POSIX_WCSCASECMP
#define UP_HAS_POSIX_WCSNCASECMP
#define UP_HAS_POSIX_WCSDUP
#define UP_HAS_POSIX_WCSNLEN
#undef  UP_HAS_GNU_MEMPCPY
#undef  UP_HAS_GNU_WMEMPCPY
#undef  UP_HAS_GNU_STRNDUP
#undef  UP_HAS_GNU_WCSNDUP
#undef  UP_HAS_BSD_MEMSET_PATTERN
#define UP_HAS_MSVC_XTHREADS
#define UP_HAS_MSVC_XTIME

//
// compiler built-ins
//
#define UPABORT __debugbreak
#define UPASSUME(expr) __assume(expr)
#define UPLIKELY(expr) (expr)
#define UPUNLIKELY(expr) (expr)
#define UPPREFETCH(address, rw, locality) _mm_prefetch((address), (locality))
#define UPIGNORE(expr) ((void)(expr))

//
// function and parameter attributes
//
#define UPALLOC __declspec(restrict)
#define UPCOLD
#define UPDEPRECATED(msg) __declspec(deprecated(msg))
#define UPALWAYSINLINE __forceinline
#define UPHIDDENINLINE
#define UPHOT
#define UPNOINLINE __declspec(noinline)
#define UPNONNULLALL
#define UPNONNULL(...)
#define UPNORETURN __declspec(noreturn)
#define UPPRINTF(format, args)
#define UPPURE __declspec(noalias)
#define UPRESTRICT __restrict
#define UPSCANF(format, args)
#define UPUSED
#define UPWARNRESULT

//
// calling conventions
//
#define UPCDECL __cdecl
#define UPFASTCALL __fastcall
#define UPSTDCALL __stdcall
#define UPTHISCALL __thiscall

//
// shared/static library visibility
//
#define UPEXPORT __declspec(dllexport)
#define UPIMPORT __declspec(dllimport)
#define UPVISIBLE
#define UPHIDDEN
#define UPEXPORTEXTERN
#define UPIMPORTEXTERN extern
#define UPEXPORTEXCEPT
#define UPIMPORTEXCEPT

#endif
