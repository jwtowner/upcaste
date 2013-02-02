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

#ifndef UP_PROLOG_GCC_HPP
#define UP_PROLOG_GCC_HPP

//
// versions check
//
#if (__GNUC__ == 4) && (__GNUC_MINOR__ < 2)
    #error "GCC versions prior to 4.2 are not supported"
#elif (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 7))
    #warning "Untested GCC compiler version"
#endif

#define UP_COMPILER UP_COMPILER_GCC

//
// C++11 feature detection
//
#if (__GNUC__ == 4)
#   if UP_BASESYSTEM == UP_BASESYSTEM_DARWIN
#       define UP_NO_THREAD_LOCAL
#   endif
#   if (__GNUC_MINOR__ < 3) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#       define UP_NO_DECLTYPE
#       define UP_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#       define UP_NO_STATIC_ASSERT
#   endif
#   if (__GNUC_MINOR__ < 4) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#       define UP_NO_AUTO_DECLARATIONS
#       define UP_NO_AUTO_MULTIDECLARATIONS
#       define UP_NO_DEFAULTED_FUNCTIONS
#       define UP_NO_DELETED_FUNCTIONS
#       define UP_NO_EXCEPTION_PTR
#       define UP_NO_INITIALIZER_LISTS
#       define UP_NO_NATIVE_CHAR16_T
#       define UP_NO_NATIVE_CHAR32_T
#       define UP_NO_NESTED_EXCEPTION
#       define UP_NO_RVALUE_REFERENCES
#       define UP_NO_SCOPED_ENUMS
#       define UP_NO_SFINAE_EXPR
#       define UP_NO_VARIADIC_TEMPLATES
#   endif
#   if (__GNUC_MINOR__ < 5) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#       define UP_NO_ALIGNOF
#       define UP_NO_EXPLICIT_CONVERSION_OPERATORS
#       define UP_NO_LAMBDAS
#       define UP_NO_RAW_LITERALS
#       define UP_NO_UNICODE_LITERALS
#   endif
#   if (__GNUC_MINOR__ < 6) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#       define UP_NO_CONSTEXPR
#       define UP_NO_MAKE_EXCEPTION_PTR
#       define UP_NO_NOEXCEPT
#       define UP_NO_NULLPTR
#   endif
#   if (__GNUC_MINOR__ < 7) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#       define UP_NO_TEMPLATE_ALIASES
#       define UP_NO_NOEXCEPT_FUNCTION_PTR
#   endif
#   define UP_NO_ALIGNAS
#   define UP_NO_REFERENCE_QUALIFIED_FUNCTIONS
#   define UP_NO_TYPE_ALIASED_REINTERPRET_CAST
#   ifdef UP_NO_ALIGNAS
#       define alignas(expression) __attribute__((aligned(expression)))
#   endif
#   ifdef UP_NO_ALIGNOF
#       define alignof __alignof__
#   endif
#   ifdef UP_NO_CONSTEXPR
#       define constexpr const
#   endif
#   ifdef UP_NO_DECLTYPE
#       define decltype __typeof__
#   endif
#   ifdef UP_NO_NOEXCEPT
#       define noexcept
#   endif
#   ifdef UP_NO_THREAD_LOCAL
#       define thread_local _Pragma( "error Platform does not support thread_local!" )
#   else
#       define thread_local __thread
#   endif
#endif

//
// C++98 feature detection
//
#ifndef __EXCEPTIONS
#   define UP_NO_EXCEPTIONS
#endif
#ifndef __GXX_RTTI
#   define UP_NO_RTTI
#endif
#ifndef __WCHAR_TYPE__
#   define UP_NO_NATIVE_WCHAR_T
#endif

//
// compiler type-traits
//
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 2))
#   define UP_TT_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T) __has_trivial_constructor(T)
#   define UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T) __has_trivial_copy(T)
#   define UP_TT_IS_TRIVIALLY_COPY_ASSIGNABLE(T) __has_trivial_assign(T)
#   define UP_TT_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T) __has_trivial_copy(T)
#   define UP_TT_IS_TRIVIALLY_MOVE_ASSIGNABLE(T) __has_trivial_assign(T)
#   define UP_TT_IS_TRIVIALLY_DESTRUCTIBLE(T) __has_trivial_destructor(T)
#   define UP_TT_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T) __has_nothrow_constructor(T)
#   define UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE(T) __has_nothrow_copy(T)
#   define UP_TT_IS_NOTHROW_COPY_ASSIGNABLE(T) __has_nothrow_assign(T)
#   define UP_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T) __has_nothrow_copy(T)
#   define UP_TT_IS_NOTHROW_MOVE_ASSIGNABLE(T) __has_nothrow_assign(T)
#   define UP_TT_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)
#   define UP_TT_IS_ABSTRACT(T) __is_abstract(T)
#   define UP_TT_IS_BASE_OF(T,U) __is_base_of(T,U)
#   define UP_TT_IS_CLASS(T) __is_class(T)
#   define UP_TT_IS_EMPTY(T) __is_empty(T)
#   define UP_TT_IS_ENUM(T) __is_enum(T)
#   define UP_TT_IS_POD(T) __is_pod(T)
#   define UP_TT_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   define UP_TT_IS_UNION(T) __is_union(T)
#endif
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))
#   define UP_TT_IS_LITERAL_TYPE(T) __is_literal_type(T)
#   define UP_TT_IS_STANDARD_LAYOUT(T) __is_standard_layout(T)
#   define UP_TT_IS_TRIVIAL(T) __is_trivial(T)
#endif
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 7))
#   define UP_TT_IS_CONVERTIBLE_TO(T, U) __is_convertible_to(T, U)
#endif

//
// compiler language pragmas
//
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 4))
#   define UP_DETAIL_STDC_FENV_ACCESS_ON \
        _Pragma("GCC push_options") \
        _Pragma("GCC optimize(\"-fno-fast-math\")") \
        _Pragma("GCC optimize(\"-fsignaling-nans\")") \
        _Pragma("GCC optimize(\"-frounding-math\")")
#   define UP_DETAIL_STDC_FENV_ACCESS_OFF \
        _Pragma("GCC pop_options")
#elif (__GNUC__ == 4) && (__GNUC_MINOR__ == 4)
#   define UP_DETAIL_STDC_FENV_ACCESS_ON \
        _Pragma("GCC push_options") \
        _Pragma("GCC optimize(\"-fno-fast-math\")")
#   define UP_DETAIL_STDC_FENV_ACCESS_OFF \
        _Pragma("GCC pop_options")
#else
#   define UP_DETAIL_STDC_FENV_ACCESS_ON
#   define UP_DETAIL_STDC_FENV_ACCESS_OFF
#endif

#define UP_STDC_FENV_ACCESS(arg) UP_DETAIL_STDC_FENV_ACCESS_##arg
#define UP_STDC_FP_CONTRACT(arg)

//
// compiler built-ins
//
#define UPABORT __builtin_trap
#define UPASSUME(expr) if (!__builtin_expect((expr),1)) { }
#define UPLIKELY(expr) __builtin_expect((expr),1)
#define UPUNLIKELY(expr) __builtin_expect((expr),0)
#define UPPREFETCH(address, rw, locality) __builtin_prefetch(address,rw,locality)
#define UPIGNORE(expr) ((void)sizeof((expr)))

//
// function and parameter attributes
//
#define UPALLOC __attribute__((malloc))
#define UPCOLD __attribute__((cold))
#define UPDEPRECATED(msg) __attribute__((deprecated(msg)))
#define UPALWAYSINLINE __attribute__((always_inline,visibility("hidden")))
#define UPHIDDENINLINE __attribute__((visibility("hidden")))
#define UPHOT __attribute__((hot))
#define UPNOINLINE __attribute__((noinline))
#define UPNONNULLALL __attribute__((nonnull))
#define UPNONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#define UPNORETURN __attribute__((noreturn))
#define UPPRINTF(fmt, args) __attribute__((format(gnu_printf, fmt, args)))
#define UPPURE __attribute__((pure))
#define UPRESTRICT __restrict__
#define UPSCANF(fmt, args) __attribute__((format(gnu_scanf, fmt, args)))
#define UPUSED __attribute__((used))
#define UPWARNRESULT __attribute__((warn_unused_result))

#ifndef UP_NO_NOEXCEPT
#   define UPEXCEPTNOTHROW noexcept
#else
#   define UPEXCEPTNOTHROW throw()
#endif

//
// calling conventions
//
#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
#   define UPCDECL __attribute__((cdecl))
#   define UPFASTCALL __attribute__((fastcall))
#   define UPSTDCALL __attribute__((stdcall))
#   define UPTHISCALL __attribute__((thiscall))
#else
#   define UPCDECL
#   define UPFASTCALL
#   define UPSTDCALL
#   define UPTHISCALL
#endif

//
// shared/static library visibility
//
#if (UP_PLATFORM != UP_PLATFORM_WINDOWS)
#   define UPEXPORT __attribute__((visibility("default")))
#   define UPIMPORT __attribute__((visibility("default")))
#   define UPVISIBLE __attribute__((visibility("default")))
#   define UPHIDDEN __attribute__((visibility("hidden")))
#   define UPEXPORTEXTERN extern
#   define UPIMPORTEXTERN extern
#   define UPEXPORTEXCEPT __attribute__((visibility("default")))
#   define UPIMPORTEXCEPT __attribute__((visibility("default")))
#else
#   define UPEXPORT __attribute__((dllexport))
#   define UPIMPORT __attribute__((dllimport))
#   define UPVISIBLE
#   define UPHIDDEN
#   define UPEXPORTEXTERN
#   define UPIMPORTEXTERN extern
#   define UPEXPORTEXCEPT
#   define UPIMPORTEXCEPT
#endif

#endif
