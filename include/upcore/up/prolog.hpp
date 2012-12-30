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

#ifndef UP_PROLOG_HPP
#define UP_PROLOG_HPP

//
// environment configuration
//

#define UP_ARCHITECTURE_ARM          1
#define UP_ARCHITECTURE_ARM_64       2
#define UP_ARCHITECTURE_LLVM         3
#define UP_ARCHITECTURE_POWER        4
#define UP_ARCHITECTURE_POWER_64     5
#define UP_ARCHITECTURE_X86          6
#define UP_ARCHITECTURE_X64          7

#define UP_LITTLE_ENDIAN             1
#define UP_BIG_ENDIAN                2

#define UP_COMPILER_CLANG            1
#define UP_COMPILER_GCC              2
#define UP_COMPILER_ICC              3
#define UP_COMPILER_MSVC             4

#define UP_PLATFORM_FREEBSD          1
#define UP_PLATFORM_IOS              2
#define UP_PLATFORM_LINUX            3
#define UP_PLATFORM_MACOSX           4
#define UP_PLATFORM_NETBSD           5
#define UP_PLATFORM_OPENBSD          6
#define UP_PLATFORM_WINDOWS          7

#define UP_BASESYSTEM_BSD            1
#define UP_BASESYSTEM_DARWIN         2
#define UP_BASESYSTEM_LINUX          3
#define UP_BASESYSTEM_WINDOWS        4

#define UP_STDC_EXTENSIONS_MSVC      1
#define UP_STDC_EXTENSIONS_POSIX     2

#define UP_FILESYSTEM_NONE           0
#define UP_FILESYSTEM_POSIX          1
#define UP_FILESYSTEM_WINDOWS        2

#define UP_SOCKETS_NONE              0
#define UP_SOCKETS_BSD               1
#define UP_SOCKETS_WINSOCK           2

#define UP_TCHAR_CHAR                1
#define UP_TCHAR_WCHAR               2

#ifndef UP_NO_PROLOG_HOST_HEADER
#   include <up/prolog/host.hpp>
#endif

#ifndef UP_PLATFORM
#   if defined(__apple__) || defined(__APPLE__) || defined(__apple_cc__) || defined(__APPLE_CC__)
#       include <up/prolog/apple.hpp>
#   elif defined(__linux) || defined(__linux__)
#       include <up/prolog/linux.hpp>
#   elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WIN64__)
#       include <up/prolog/windows.hpp>
#   else
#       error "Platform not supported by Upcaste!"
#   endif
#endif

#ifndef UP_COMPILER
#   if defined(__clang__)
#       include <up/prolog/clang.hpp>
#   elif defined(__GNUC__)
#       include <up/prolog/gcc.hpp>
#   elif defined(_MSC_VER)
#	    include <up/prolog/msvc.hpp>
#   else
#       error "Compiler not supported by Upcaste!"
#   endif
#endif

//
// compiler compatability macros
//

#ifndef UP_NO_CONSTEXPR
#   define UPCONSTEXPR constexpr
#else
#   define UPCONSTEXPR
#endif

#ifndef UP_NO_NOEXCEPT
#   define UPNOEXCEPT(...) noexcept(__VA_ARGS__)
#else
#   define UPNOEXCEPT(...)
#endif

#ifdef UP_NO_DEFAULTED_FUNCTIONS
#   define UPDEFAULTCTOR(type) inline UPHIDDENINLINE type() { }
#else
#   define UPDEFAULTCTOR(type) inline UPHIDDENINLINE type() = default
#endif

#ifdef UP_NO_DELETED_FUNCTIONS
#   define UPNONCOPYABLE(type) \
    private: \
        UPHIDDEN type(type const&); \
        UPHIDDEN type& operator=(type const&)
#else
#   define UPNONCOPYABLE(type) \
        type(type const&) = delete; \
        type& operator=(type const&) = delete
#endif

#ifndef UP_NO_EXCEPTIONS
#   define UPTRY try
#   define UPCATCH(arg, statement) catch(arg) { statement; }
#   define UPCATCH_RETHROW(arg, statement) catch(arg) { statement; throw; }
#   define UPCATCH_IGNORE(arg) catch(arg) { }
#   define UPCATCHANY(statement) catch(...) { statement; }
#   define UPCATCHANY_RETHROW(statement) catch(...) { statement; throw; }
#   define UPCATCHANY_IGNORE() catch(...) { }
#   define UPTHROW(throw_statement) throw_statement
#   define UPTHROWIF(condition, throw_statement) { if ((condition)) { throw_statement; } }
#else
#   define UPTRY
#   define UPCATCH(arg, statement)
#   define UPCATCH_RETHROW(arg, statement)
#   define UPCATCH_IGNORE(arg)
#   define UPCATCHANY(statement)
#   define UPCATCHANY_RETHROW(statement)
#   define UPCATCHANY_IGNORE()
#   define UPTHROW(throw_statement) ((void)0)
#   define UPTHROWIF(condition, throw_statement) UPIGNORE(condition)
#endif

#ifndef UP_NO_SCOPED_ENUMS
#   define UPENUMBEGIN(name) enum class name
#   define UPENUMEND ;
#   define UPENUMTYPE(name) name
#else
#   define UPENUMBEGIN(name) class UPHIDDEN name { public: UPALWAYSINLINE name(){} template <class T> UPALWAYSINLINE name(T t):e_(t){} enum enum_t
#   define UPENUMEND ; inline UPALWAYSINLINE operator enum_t() const { return e_; } private: enum_t e_; };
#   define UPENUMTYPE(name) name::enum_type
#endif

#ifndef UPCONCATENATE
#   define UP_DETAIL_CONCATENATE_1(x, y) x##y
#   define UP_DETAIL_CONCATENATE_2(x, y) UP_DETAIL_CONCATENATE_1(x, y) 
#   define UPCONCATENATE(x, y) UP_DETAIL_CONCATENATE_2(x, y)
#endif

#ifndef UPSTRINGIZE
#   define UP_DETAIL_STRINGIZE_1(x) #x
#   define UP_DETAIL_STRINGIZE_2(x) UP_DETAIL_STRINGIZE_1(x)
#   define UPSTRINGIZE(x) UP_DETAIL_STRINGIZE_2(x)
#endif

#if defined(UP_HAS_STDC_WCHAR) && !defined(UPWSTRINGIZE)
#   define UP_DETAIL_WSTRINGIZE_1(x) #x
#   define UP_DETAIL_WSTRINGIZE_2(x) UP_DETAIL_WSTRINGIZE_1(x)
#   define UPWSTRINGIZE(x) UPCONCATENATE(L, UP_DETAIL_WSTRINGIZE_2(x))
#endif

//
// tchar_t null-terminated string macros
//

#ifndef UP_TCHAR
#   if defined(UP_HAS_STDC_WCHAR) && (UP_PLATFORM == UP_PLATFORM_WINDOWS) && defined(_UNICODE)
#       define UP_TCHAR UP_TCHAR_WCHAR
#   else
#       define UP_TCHAR UP_TCHAR_CHAR
#   endif
#endif

#if UP_TCHAR == UP_TCHAR_CHAR
#   define UPTC(x) x
#   define UPTS(x) x
#   define UPTSTRINGIZE(x) UPSTRINGIZE(x)
#elif UP_TCHAR == UP_TCHAR_WCHAR
#   define UPTC(x) UPCONCATENATE(L, x)
#   define UPTS(x) UPCONCATENATE(L, x)
#   define UPTSTRINGIZE(x) UPWSTRINGIZE(x)
#endif

//
// shared library configuration
//

#ifdef UP_LINKAGE_SHARED
#   ifdef LIBUPCORE_EXPORT_SYMBOLS
#       define LIBUPCOREAPI UPEXPORT
#       define LIBUPCOREEXTERN UPEXPORTEXTERN
#       define LIBUPCOREEXCEPTAPI UPEXPORTEXCEPT
#   else
#       define LIBUPCOREAPI UPIMPORT
#       define LIBUPCOREEXTERN UPIMPORTEXTERN
#       define LIBUPCOREEXCEPTAPI UPIMPORTEXCEPT
#   endif
#else
#   define LIBUPCOREAPI
#   define LIBUPCOREEXTERN extern
#   define LIBUPCOREEXCEPTAPI
#endif

#if defined(UP_NO_EXTERN_TEMPLATES) && !defined(LIBUPCORE_NO_EXTERN_TEMPLATES)
#   define LIBUPCORE_NO_EXTERN_TEMPLATES
#endif

//
// platform integration
//

#if !defined(UP_STDC_EXTENSIONS) && (UP_PLATFORM == UP_PLATFORM_WINDOWS) && (UP_COMPILER != UP_COMPILER_GCC)
#   define UP_STDC_EXTENSIONS UP_STDC_EXTENSIONS_MSVC
#endif

#if !defined(UP_HAS_UTF8_FILESYSTEM) && (UP_BASESYSTEM == UP_BASESYSTEM_DARWIN)
#   define UP_HAS_UTF8_FILESYSTEM
#endif

#endif
