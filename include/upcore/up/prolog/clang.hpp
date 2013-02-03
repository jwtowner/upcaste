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

#ifndef UP_PROLOG_CLANG_HPP
#define UP_PROLOG_CLANG_HPP

//
// versions check
//
#if (__clang_major__ <= 2)
    #error "Clang versions prior to 3.0 are not supported!"
#endif

#define UP_COMPILER UP_COMPILER_CLANG

//
// C++11 feature detection
//
#if !__has_feature(__cxx_auto_types__)
#   define UP_NO_AUTO_DECLARATIONS
#   define UP_NO_AUTO_MULTIDECLARATIONS
#endif
#if !__has_feature(__cxx_alias_templates__)
#   define UP_NO_TEMPLATE_ALIASES
#endif
#if !__has_feature(__cxx_alignas__)
#   define UP_NO_ALIGNAS
#   define UP_NO_ALIGNOF
#endif
#if !__has_feature(__cxx_constexpr__)
#   define UP_NO_CONSTEXPR
#endif
#if !__has_feature(__cxx_decltype__)
#   define UP_NO_DECLTYPE
#endif
#if !__has_feature(__cxx_default_function_template_args__)
#   define UP_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#endif
#if !__has_feature(__cxx_defaulted_functions__)
#   define UP_NO_DEFAULTED_FUNCTIONS
#endif
#if !__has_feature(__cxx_delegating_constructors__)
#   define UP_NO_DELEGATING_CONSTRUCTORS
#endif
#if !__has_feature(__cxx_deleted_functions__)
#   define UP_NO_DELETED_FUNCTIONS
#endif
#if !__has_feature(__cxx_explicit_conversions__)
#   define UP_NO_EXPLICIT_CONVERSION_OPERATORS
#endif
#if !__has_feature(__cxx_generalized_initializers__)
#   define UP_NO_INITIALIZER_LISTS
#endif
#if !__has_feature(__cxx_inheriting_constructors__)
#   define UP_NO_INHERITING_CONSTRUCTORS
#endif
#if !__has_feature(__cxx_lambdas__)
#   define UP_NO_LAMBDAS
#endif
#if !__has_feature(__cxx_noexcept__)
#   define UP_NO_NOEXCEPT
#endif
#if !__has_feature(__cxx_nullptr__)
#   define UP_NO_NULLPTR
#endif
#if !__has_feature(__cxx_range_for__)
#   define UP_NO_RANGE_BASED_FOR
#endif
#if !__has_feature(__cxx_raw_string_literals__)
#   define UP_NO_RAW_LITERALS
#endif
#if !__has_feature(__cxx_reference_qualified_functions__)
#   define UP_NO_REFERENCE_QUALIFIED_FUNCTIONS
#endif
#if !__has_feature(__cxx_rvalue_references__)
#   define UP_NO_RVALUE_REFERENCES
#endif
#if !__has_feature(__cxx_access_control_sfinae__)
#   define UP_NO_SFINAE_EXPR
#endif
#if !__has_feature(__cxx_static_assert__)
#   define UP_NO_STATIC_ASSERT
#endif
#if !__has_feature(__cxx_strong_enums__)
#   define UP_NO_SCOPED_ENUMS
#endif
#if !__has_feature(__cxx_thread_local__)
#   define UP_NO_THREAD_LOCAL
#endif
#if !__has_feature(__cxx_unicode_literals__)
#   define UP_NO_NATIVE_CHAR16_T
#   define UP_NO_NATIVE_CHAR32_T
#   define UP_NO_UNICODE_LITERALS
#endif
#if !__has_feature(__cxx_variadic_templates__)
#   define UP_NO_VARIADIC_TEMPLATES
#endif

#ifdef UP_NO_ALIGNAS
#   define alignas(expression) __attribute__((aligned(expression)))
#endif
#ifdef UP_NO_ALIGNOF
#   define alignof __alignof__
#endif
#ifdef UP_NO_CONSTEXPR
#   define constexpr const
#endif
#ifdef UP_NO_DECLTYPE
#   define decltype __typeof__
#endif
#ifdef UP_NO_NOEXCEPT
#   define noexcept
#endif
#ifdef UP_NO_THREAD_LOCAL
#   define thread_local __thread
#endif

//
// C++98 feature detection
//
#if !__has_feature(__cxx_exceptions__)
#   define UP_NO_EXCEPTIONS
#endif
#if !__has_feature(__cxx_rtti__)
#   define UP_NO_RTTI
#endif

//
// compiler type-traits
//
#if !__has_extension(__has_trivial_constructor__)
#   define UP_TT_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T) __has_trivial_constructor(T)
#endif
#if !__has_extension(__has_trivial_copy__)
#   define UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T) __has_trivial_copy(T)
#endif
#if !__has_extension(__has_trivial_assign__)
#   define UP_TT_IS_TRIVIALLY_COPY_ASSIGNABLE(T) __has_trivial_assign(T)
#endif
#if !__has_extension(__has_trivial_copy__)
#   define UP_TT_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T) __has_trivial_copy(T)
#endif
#if !__has_extension(__has_trivial_assign__)
#   define UP_TT_IS_TRIVIALLY_MOVE_ASSIGNABLE(T) __has_trivial_assign(T)
#endif
#if !__has_extension(__has_trivial_destructor__)
#   define UP_TT_IS_TRIVIALLY_DESTRUCTIBLE(T) __has_trivial_destructor(T)
#endif
#if !__has_extension(__has_nothrow_constructor__)
#   define UP_TT_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T) __has_nothrow_constructor(T)
#endif
#if !__has_extension(__has_nothrow_copy__)
#   define UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE(T) __has_nothrow_copy(T)
#endif
#if !__has_extension(__has_nothrow_assign__)
#   define UP_TT_IS_NOTHROW_COPY_ASSIGNABLE(T) __has_nothrow_assign(T)
#endif
#if !__has_extension(__has_nothrow_copy__)
#   define UP_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T) __has_nothrow_copy(T)
#endif
#if !__has_extension(__has_nothrow_assign__)
#   define UP_TT_IS_NOTHROW_MOVE_ASSIGNABLE(T) __has_nothrow_assign(T)
#endif
#if !__has_extension(__has_virtual_destructor__)
#   define UP_TT_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)
#endif
#if !__has_extension(__is_abstract__)
#   define UP_TT_IS_ABSTRACT(T) __is_abstract(T)
#endif
#if !__has_extension(__is_base_of__)
#   define UP_TT_IS_BASE_OF(T,U) __is_base_of(T,U)
#endif
#if !__has_extension(__is_class__)
#   define UP_TT_IS_CLASS(T) __is_class(T)
#endif
#if !__has_extension(__is_convertible_to__)
#   define UP_TT_IS_CONVERTIBLE_TO(T, U) __is_convertible_to(T, U)
#endif
#if !__has_extension(__is_empty__)
#   define UP_TT_IS_EMPTY(T) __is_empty(T)
#endif
#if !__has_extension(__is_enum__)
#   define UP_TT_IS_ENUM(T) __is_enum(T)
#endif
#if !__has_extension(__is_literal__)
#   define UP_TT_IS_LITERAL_TYPE(T) __is_literal(T)
#endif
#if !__has_extension(__is_pod__)
#   define UP_TT_IS_POD(T) __is_pod(T)
#endif
#if !__has_extension(__is_polymorphic__)
#   define UP_TT_IS_POLYMORPHIC(T) __is_polymorphic(T)
#endif
#if !__has_extension(__is_standard_layout__)
#   define UP_TT_IS_STANDARD_LAYOUT(T) __is_standard_layout(T)
#endif
#if !__has_extension(__is_union__)
#   define UP_TT_IS_UNION(T) __is_union(T)
#endif

//
// compiler language pragmas
//
#if (__clang_major__ == 3) && (__clang_minor__ <= 2)
#   define UP_DETAIL_STDC_FENV_ACCESS_ON
#   define UP_DETAIL_STDC_FENV_ACCESS_OFF
#   define UP_DETAIL_STDC_FP_CONTRACT_ON
#   define UP_DETAIL_STDC_FP_CONTRACT_OFF
#else
#   define UP_DETAIL_STDC_FENV_ACCESS_ON
#   define UP_DETAIL_STDC_FENV_ACCESS_OFF
#   define UP_DETAIL_STDC_FP_CONTRACT_ON \
        _Pragma("STDC FP_CONTRACT ON")
#   define UP_DETAIL_STDC_FP_CONTRACT_OFF \
        _Pragma("STDC FP_CONTRACT OFF")
#endif

#define UP_STDC_FENV_ACCESS(arg) UP_DETAIL_STDC_FENV_ACCESS_##arg
#define UP_STDC_FP_CONTRACT(arg) UP_DETAIL_STDC_FP_CONTRACT_##arg

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
#define UPALLOC __attribute__((__malloc__))
#define UPCOLD __attribute__((__cold__))
#define UPDEPRECATED(msg) __attribute__((__deprecated__(msg)))
#define UPALWAYSINLINE __attribute__((__always_inline__,__visibility__("hidden")))
#define UPHIDDENINLINE __attribute__((__visibility__("hidden")))
#define UPHOT __attribute__((__hot__))
#define UPNOINLINE __attribute__((__noinline__))
#define UPNONNULLALL __attribute__((__nonnull__))
#define UPNONNULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#define UPNORETURN __attribute__((__noreturn__))
#define UPPRINTF(fmt, args) 
#define UPPURE __attribute__((__pure__))
#define UPRESTRICT __restrict__
#define UPSCANF(fmt, args)
#define UPUSED
#define UPWARNRESULT __attribute__((__warn_unused_result__))

#ifndef UP_NO_NOEXCEPT
#   define UPEXCEPTNOTHROW noexcept
#else
#   define UPEXCEPTNOTHROW throw()
#endif

//
// calling conventions
//
#if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86)
#   define UPCDECL __attribute__((__cdecl__))
#   define UPFASTCALL __attribute__((__fastcall__))
#   define UPSTDCALL __attribute__((__stdcall__))
#   define UPTHISCALL __attribute__((__thiscall__))
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
#   define UPEXPORT __attribute__((__visibility__("default")))
#   define UPIMPORT __attribute__((__visibility__("default")))
#   define UPVISIBLE __attribute__((__visibility__("default")))
#   define UPHIDDEN __attribute__((__visibility__("hidden")))
#   define UPEXPORTEXTERN extern
#   define UPIMPORTEXTERN extern
#   define UPEXPORTEXCEPT __attribute__((__visibility__("default")))
#   define UPIMPORTEXCEPT __attribute__((__visibility__("default")))
#else
#   define UPEXPORT __attribute__((__dllexport__))
#   define UPIMPORT __attribute__((__dllimport__))
#   define UPVISIBLE
#   define UPHIDDEN
#   define UPEXPORTEXTERN
#   define UPIMPORTEXTERN extern
#   define UPEXPORTEXCEPT
#   define UPIMPORTEXCEPT
#endif

#if UP_BASESYSTEM == UP_BASESYSTEM_LINUX
#   define __extern_inline extern __inline __attribute((__gnu_inline__))
#   define __extern_always_inline extern __always_inline __attribute((__gnu_inline__, __artificial__))
    struct UPHIDDEN __float128 { unsigned char x[16]; };
#endif

#endif

