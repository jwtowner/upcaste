//
//  Hurricane Game Development Kit
//  Copyright (C) 2008-2010 DS Media Labs, Inc.
//  All rights reserved.
//

#include <up/prolog.hpp>

#if !defined(UP_HAS_STDCXX_ATOMIC) && defined(UP_NO_EXTERN_TEMPLATES)

#ifdef UP_NO_INCONSISTENT_EXPLICIT_INSTANTIATIONS
#   if !defined(LIBUPCORE_EXPORT_SYMBOLS) && !defined(LIBUPCORE_NO_EXTERN_TEMPLATES)
#       define LIBUPCORE_NO_EXTERN_TEMPLATES
#   endif
#endif

#include <up/atomic.hpp>

#if UP_COMPILER == UP_COMPILER_GCC
#   pragma GCC diagnostic ignored "-Wattributes"
#endif

namespace up { namespace detail
{
    template struct LIBUPCOREAPI atomic_storage<bool>;
    template struct LIBUPCOREAPI atomic_storage<void*>;
    template struct LIBUPCOREAPI atomic_storage<void const*>;
    template struct LIBUPCOREAPI atomic_storage<void volatile*>;
    template struct LIBUPCOREAPI atomic_storage<void const volatile*>;
    template struct LIBUPCOREAPI atomic_storage<signed char>;
    template struct LIBUPCOREAPI atomic_storage<short>;
    template struct LIBUPCOREAPI atomic_storage<int>;
    template struct LIBUPCOREAPI atomic_storage<long>;
    template struct LIBUPCOREAPI atomic_storage<long long>;
    template struct LIBUPCOREAPI atomic_storage<unsigned char>;
    template struct LIBUPCOREAPI atomic_storage<unsigned short>;
    template struct LIBUPCOREAPI atomic_storage<unsigned int>;
    template struct LIBUPCOREAPI atomic_storage<unsigned long>;
    template struct LIBUPCOREAPI atomic_storage<unsigned long long>;
    template struct LIBUPCOREAPI atomic_storage<char>;
#ifndef UP_NO_NATIVE_WCHAR_T
    template struct LIBUPCOREAPI atomic_storage<wchar_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    template struct LIBUPCOREAPI atomic_storage<char16_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    template struct LIBUPCOREAPI atomic_storage<char32_t>;
#endif

    template struct LIBUPCOREAPI atomic_base<bool>;
    template struct LIBUPCOREAPI atomic_base<void*>;
    template struct LIBUPCOREAPI atomic_base<void const*>;
    template struct LIBUPCOREAPI atomic_base<void volatile*>;
    template struct LIBUPCOREAPI atomic_base<void const volatile*>;
    template struct LIBUPCOREAPI atomic_base<signed char>;
    template struct LIBUPCOREAPI atomic_base<short>;
    template struct LIBUPCOREAPI atomic_base<int>;
    template struct LIBUPCOREAPI atomic_base<long>;
    template struct LIBUPCOREAPI atomic_base<long long>;
    template struct LIBUPCOREAPI atomic_base<unsigned char>;
    template struct LIBUPCOREAPI atomic_base<unsigned short>;
    template struct LIBUPCOREAPI atomic_base<unsigned int>;
    template struct LIBUPCOREAPI atomic_base<unsigned long>;
    template struct LIBUPCOREAPI atomic_base<unsigned long long>;
    template struct LIBUPCOREAPI atomic_base<char>;
#ifndef UP_NO_NATIVE_WCHAR_T
    template struct LIBUPCOREAPI atomic_base<wchar_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    template struct LIBUPCOREAPI atomic_base<char16_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    template struct LIBUPCOREAPI atomic_base<char32_t>;
#endif

    template struct LIBUPCOREAPI atomic_address_base<void*, int8_t>;
    template struct LIBUPCOREAPI atomic_address_base<void const*, int8_t>;
    template struct LIBUPCOREAPI atomic_address_base<void volatile*, int8_t>;
    template struct LIBUPCOREAPI atomic_address_base<void const volatile*, int8_t>;

    template struct LIBUPCOREAPI atomic_integral_base<signed char>;
    template struct LIBUPCOREAPI atomic_integral_base<short>;
    template struct LIBUPCOREAPI atomic_integral_base<int>;
    template struct LIBUPCOREAPI atomic_integral_base<long>;
    template struct LIBUPCOREAPI atomic_integral_base<long long>;
    template struct LIBUPCOREAPI atomic_integral_base<unsigned char>;
    template struct LIBUPCOREAPI atomic_integral_base<unsigned short>;
    template struct LIBUPCOREAPI atomic_integral_base<unsigned int>;
    template struct LIBUPCOREAPI atomic_integral_base<unsigned long>;
    template struct LIBUPCOREAPI atomic_integral_base<unsigned long long>;
    template struct LIBUPCOREAPI atomic_integral_base<char>;
#ifndef UP_NO_NATIVE_WCHAR_T
    template struct LIBUPCOREAPI atomic_integral_base<wchar_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    template struct LIBUPCOREAPI atomic_integral_base<char16_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    template struct LIBUPCOREAPI atomic_integral_base<char32_t>;
#endif
}}

#endif
