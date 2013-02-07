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

#ifndef UP_TYPE_TRAITS_GENERIC_INL
#define UP_TYPE_TRAITS_GENERIC_INL

namespace up
{
    template <class T, T Value>
    struct UPVISIBLE integral_constant
    {
        typedef integral_constant<T, Value> type;
        typedef T value_type;
        static constexpr T value = Value;
        UPCONSTEXPR operator T() const { return Value; }
    };
    
    template <class T, T Value>
    constexpr T integral_constant<T, Value>::value;
    
    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;
    
    template <bool Value, class T, class U> struct UPVISIBLE conditional { typedef T type; };
    template <class T, class U> struct UPVISIBLE conditional<false, T, U> { typedef U type; };  

    template <bool Value, class T = void> struct UPVISIBLE enable_if { typedef T type; };
    template <class T> struct UPVISIBLE enable_if<false, T> { };
}

namespace up { namespace detail
{
    template <class T> struct is_integral_impl : false_type { };
    template <> struct is_integral_impl<bool> : true_type { };
    template <> struct is_integral_impl<signed char> : true_type { };
    template <> struct is_integral_impl<short> : true_type { };
    template <> struct is_integral_impl<int> : true_type { };
    template <> struct is_integral_impl<long> : true_type { };
    template <> struct is_integral_impl<long long> : true_type { };
    template <> struct is_integral_impl<unsigned char> : true_type { };
    template <> struct is_integral_impl<unsigned short> : true_type { };
    template <> struct is_integral_impl<unsigned int> : true_type { };
    template <> struct is_integral_impl<unsigned long> : true_type { };
    template <> struct is_integral_impl<unsigned long long> : true_type { };
    template <> struct is_integral_impl<char> : true_type { };
#ifndef UP_NO_NATIVE_WCHAR_T
    template <> struct is_integral_impl<wchar_t> : true_type { };
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    template <> struct is_integral_impl<char16_t> : true_type { };
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    template <> struct is_integral_impl<char32_t> : true_type { };
#endif

    template <class T> struct is_floating_point_impl : false_type { };
    template <> struct is_floating_point_impl<float> : true_type { };
    template <> struct is_floating_point_impl<double> : true_type { };
    template <> struct is_floating_point_impl<long double> : true_type { };

    template <class T>
    struct is_function_impl
    {
        typedef char yes;
        typedef char (&no)[2];
        template <class U> static U& declval() noexcept;
        template <class U> static yes sfinae(U*) noexcept;
        template <class U> static no sfinae(...) noexcept;
        typedef integral_constant<bool, sizeof(sfinae<T>(declval<T>())) == sizeof(yes)> result;
    };

    template <class T> struct is_member_function_pointer_impl
        : false_type { };
    template <class T, class U> struct is_member_function_pointer_impl<T U::*>
        : integral_constant<bool, is_function_impl<T>::result::value> { };

    template <class T> struct is_member_object_pointer_impl
        : false_type { };
    template <class T, class U> struct is_member_object_pointer_impl<T U::*>
        : integral_constant<bool, !is_function_impl<T>::result::value> { };

    template <class T> struct is_pointer_impl : false_type { };
    template <class T> struct is_pointer_impl<T*> : true_type { };

#ifdef UP_TT_IS_ENUM
    template <class T> struct is_enum_impl : integral_constant<bool, UP_TT_IS_ENUM(T)> { };
#else
    template <class T> struct is_enum_impl : false_type { };
#endif

#ifdef UP_TT_IS_UNION
    template <class T> struct is_union_impl : integral_constant<bool, UP_TT_IS_UNION(T)> { };
#else
    template <class T> struct is_union_impl : false_type { };
#endif
}}

namespace up
{
    template <class T> struct UPVISIBLE is_void : false_type { };
    template <> struct UPVISIBLE is_void<void> : true_type { };
    template <> struct UPVISIBLE is_void<void const> : true_type { };
    template <> struct UPVISIBLE is_void<void volatile> : true_type { };
    template <> struct UPVISIBLE is_void<void const volatile> : true_type { };

    template <class T> struct UPVISIBLE is_integral : integral_constant<bool, detail::is_integral_impl<T>::value> { };
    template <class T> struct UPVISIBLE is_integral<T const> : is_integral<T> { };
    template <class T> struct UPVISIBLE is_integral<T volatile> : is_integral<T> { };
    template <class T> struct UPVISIBLE is_integral<T const volatile> : is_integral<T> { };

    template <class T> struct UPVISIBLE is_floating_point : integral_constant<bool, detail::is_floating_point_impl<T>::value> { };
    template <class T> struct UPVISIBLE is_floating_point<T const> : is_floating_point<T> { };
    template <class T> struct UPVISIBLE is_floating_point<T volatile> : is_floating_point<T> { };
    template <class T> struct UPVISIBLE is_floating_point<T const volatile> : is_floating_point<T> { };
    
    template <class T> struct UPVISIBLE is_array : false_type { };
    template <class T, size_t Size> struct UPVISIBLE is_array<T[Size]> : true_type { };
    template <class T, size_t Size> struct UPVISIBLE is_array<T const[Size]> : true_type { };
    template <class T, size_t Size> struct UPVISIBLE is_array<T volatile[Size]> : true_type { };
    template <class T, size_t Size> struct UPVISIBLE is_array<T const volatile[Size]> : true_type { };
    template <class T> struct UPVISIBLE is_array<T[]> : true_type { };
    template <class T> struct UPVISIBLE is_array<T const[]> : true_type { };
    template <class T> struct UPVISIBLE is_array<T volatile[]> : true_type { };
    template <class T> struct UPVISIBLE is_array<T const volatile[]> : true_type { };
    
    template <class T> struct UPVISIBLE is_function : integral_constant<bool, detail::is_function_impl<T>::result::value> { };
    template <>        struct UPVISIBLE is_function<void> : false_type { };
    template <class T> struct UPVISIBLE is_function<T&> : false_type { };
#ifndef UP_NO_RVALUE_REFERENCES    
    template <class T> struct UPVISIBLE is_function<T&&> : false_type { };
#endif
    template <class T> struct UPVISIBLE is_function<T const> : is_function<T> { };
    template <class T> struct UPVISIBLE is_function<T volatile> : is_function<T> { };
    template <class T> struct UPVISIBLE is_function<T const volatile> : is_function<T> { };

    template <class T> struct UPVISIBLE is_member_function_pointer
        : integral_constant<bool, detail::is_member_function_pointer_impl<T>::value> { };
    template <class T> struct UPVISIBLE is_member_function_pointer<T const>
        : is_member_function_pointer<T> { };
    template <class T> struct UPVISIBLE is_member_function_pointer<T volatile>
        : is_member_function_pointer<T> { };
    template <class T> struct UPVISIBLE is_member_function_pointer<T const volatile>
        : is_member_function_pointer<T> { };
    
    template <class T> struct UPVISIBLE is_member_object_pointer
        : integral_constant<bool, detail::is_member_object_pointer_impl<T>::value> { };
    template <class T> struct UPVISIBLE is_member_object_pointer<T const> : is_member_object_pointer<T> { };
    template <class T> struct UPVISIBLE is_member_object_pointer<T volatile> : is_member_object_pointer<T> { };
    template <class T> struct UPVISIBLE is_member_object_pointer<T const volatile> : is_member_object_pointer<T> { };

    template <class T> struct UPVISIBLE is_pointer
        : integral_constant<bool, detail::is_pointer_impl<T>::value
          && !is_member_function_pointer<T>::value && !is_member_object_pointer<T>::value> { };
    template <class T> struct UPVISIBLE is_pointer<T const> : is_pointer<T> { };
    template <class T> struct UPVISIBLE is_pointer<T volatile> : is_pointer<T> { };
    template <class T> struct UPVISIBLE is_pointer<T const volatile> : is_pointer<T> { };
    
    template <class T> struct UPVISIBLE is_lvalue_reference : false_type { };
    template <class T> struct UPVISIBLE is_lvalue_reference<T&> : true_type { };

    template <class T> struct UPVISIBLE is_rvalue_reference : false_type { };
#ifndef UP_NO_RVALUE_REFERENCES
    template <class T> struct UPVISIBLE is_rvalue_reference<T&&> : true_type { };
#endif

    template <class T> struct UPVISIBLE is_enum : integral_constant<bool, detail::is_enum_impl<T>::value> { };
    template <class T> struct UPVISIBLE is_enum<T const> : is_enum<T> { };
    template <class T> struct UPVISIBLE is_enum<T volatile> : is_enum<T> { };
    template <class T> struct UPVISIBLE is_enum<T const volatile> : is_enum<T> { };
    
    template <class T> struct UPVISIBLE is_union : integral_constant<bool, detail::is_union_impl<T>::value> { };
    template <class T> struct UPVISIBLE is_union<T const> : is_union<T> { };
    template <class T> struct UPVISIBLE is_union<T volatile> : is_union<T> { };
    template <class T> struct UPVISIBLE is_union<T const volatile> : is_union<T> { };
}

namespace up { namespace detail
{
    template <class T>
    struct is_class_impl
    {
        typedef integral_constant
        <
            bool,
#ifdef UP_TT_IS_CLASS
            UP_TT_IS_CLASS(T)
#else
            !is_void<T>::value
            && !is_integral<T>::value
            && !is_floating_point<T>::value
            && !is_array<T>::value
            && !is_member_function_pointer<T>::value
            && !is_member_object_pointer<T>::value
            && !is_pointer<T>::value
            && !is_lvalue_reference<T>::value
            && !is_rvalue_reference<T>::value
            && !is_enum<T>::value
            && !is_union<T>::value
            && !is_function<T>::value
#endif
        >
        result;
    };
}}

namespace up
{
    template <class T> struct UPVISIBLE is_class : integral_constant<bool, detail::is_class_impl<T>::result::value> { };
    template <class T> struct UPVISIBLE is_class<T const> : is_class<T> { };
    template <class T> struct UPVISIBLE is_class<T volatile> : is_class<T> { };
    template <class T> struct UPVISIBLE is_class<T const volatile> : is_class<T> { };

    template <class T>
    struct UPVISIBLE is_reference :
        integral_constant<bool, is_lvalue_reference<T>::value
            || is_rvalue_reference<T>::value> { };
    
    template <class T>
    struct UPVISIBLE is_arithmetic :
        integral_constant<bool, is_integral<T>::value
            || is_floating_point<T>::value> { };
    
    template <class T>
    struct UPVISIBLE is_fundamental :
        integral_constant<bool, is_arithmetic<T>::value || is_void<T>::value> { };
    
    template <class T>
    struct UPVISIBLE is_member_pointer :
        integral_constant<bool, is_member_function_pointer<T>::value
            || is_member_object_pointer<T>::value> { };
    
    template <class T>
    struct UPVISIBLE is_scalar :
        integral_constant<bool, is_arithmetic<T>::value || is_enum<T>::value
            || is_member_pointer<T>::value || is_pointer<T>::value> { };
    
    template <>
    struct UPVISIBLE is_scalar<nullptr_t> : true_type { };
    
    template <class T>
    struct UPVISIBLE is_object :
        integral_constant<bool, is_scalar<T>::value || is_array<T>::value
            || is_class<T>::value || is_union<T>::value> { };
    
    template <class T>
    struct UPVISIBLE is_compound :
        integral_constant<bool, !is_fundamental<T>::value> { };
}

namespace up { namespace detail
{
    template <class Head, class Tail>
    struct type_list
    {
        typedef Head head_type;
        typedef Tail tail_type;
    };

    typedef type_list
    <
        signed char,
        type_list<signed short,
        type_list<signed int,
        type_list<signed long,
        type_list<signed long long,
        nat_t > > > >
    >
    all_signed_types;
        
    typedef type_list
    <
        unsigned char,
        type_list<unsigned short,
        type_list<unsigned int,
        type_list<unsigned long,
        type_list<unsigned long long,
        nat_t > > > >
    >
    all_unsigned_types;
        
    template <class T, class List>
    struct sign_search;
        
    template <class T, class Head>
    struct sign_search<T, type_list<Head, nat_t> >
    {
        static_assert(sizeof(T) <= sizeof(Head), "no type found for type T");
        typedef Head type;
    };
        
    template <class T, class Head, class Tail>
    struct sign_search<T, type_list<Head, Tail> >
    {
        typedef typename conditional<sizeof(T) <= sizeof(Head), Head, typename sign_search<T, Tail>::type>::type type;
    };

    template <class T, bool = is_enum<T>::value || is_integral<T>::value>
    struct make_signed_impl;
        
    template <class T>
    struct make_signed_impl<T, true>
    {
        typedef typename sign_search<T, all_signed_types>::type type;
    };
        
    template <> struct make_signed_impl<bool, true> { };
    template <> struct make_signed_impl<signed short, true> { typedef short type; };
    template <> struct make_signed_impl<signed int, true> { typedef int type; };
    template <> struct make_signed_impl<signed long, true> { typedef long type; };
    template <> struct make_signed_impl<signed long long, true> { typedef long long type; };
    template <> struct make_signed_impl<unsigned short, true> { typedef short type; };
    template <> struct make_signed_impl<unsigned int, true> { typedef int type; };
    template <> struct make_signed_impl<unsigned long, true> { typedef long type; };
    template <> struct make_signed_impl<unsigned long long, true> {  typedef long long type; };
        
    template <class T, bool = is_enum<T>::value || is_integral<T>::value>
    struct make_unsigned_impl;
        
    template <class T>
    struct make_unsigned_impl<T, true>
    {
        typedef typename sign_search<T, all_unsigned_types>::type type;
    };
                
    template <> struct make_unsigned_impl<bool, true> { };
    template <> struct make_unsigned_impl<signed short, true> { typedef unsigned short type; };
    template <> struct make_unsigned_impl<signed int, true> { typedef unsigned int type; };
    template <> struct make_unsigned_impl<signed long, true> { typedef unsigned long type; };
    template <> struct make_unsigned_impl<signed long long, true> { typedef unsigned long long type; };
    template <> struct make_unsigned_impl<unsigned short, true> { typedef unsigned short type; };
    template <> struct make_unsigned_impl<unsigned int, true> { typedef unsigned int type; };
    template <> struct make_unsigned_impl<unsigned long, true> { typedef unsigned long type; };
    template <> struct make_unsigned_impl<unsigned long long, true> {  typedef unsigned long long type; };
}}

namespace up
{
    template <class T> struct UPVISIBLE remove_const { typedef T type; };
    template <class T> struct UPVISIBLE remove_const<T const> { typedef T type; };
    template <class T> struct UPVISIBLE remove_const<T volatile> { typedef T volatile type; };
    template <class T> struct UPVISIBLE remove_const<T const volatile> { typedef T volatile type; };
    
    template <class T> struct UPVISIBLE remove_volatile { typedef T type; };
    template <class T> struct UPVISIBLE remove_volatile<T const> { typedef T const type; };
    template <class T> struct UPVISIBLE remove_volatile<T volatile> { typedef T type; };
    template <class T> struct UPVISIBLE remove_volatile<T const volatile> { typedef T const type; };
    
    template <class T> struct UPVISIBLE remove_cv { typedef T type; };
    template <class T> struct UPVISIBLE remove_cv<T const> { typedef T type; };
    template <class T> struct UPVISIBLE remove_cv<T volatile> { typedef T type; };
    template <class T> struct UPVISIBLE remove_cv<T const volatile> { typedef T type; };
    
    template <class T> struct UPVISIBLE add_const { typedef T const type; };
    template <class T> struct UPVISIBLE add_const<T const> { typedef T const type; };
    template <class T> struct UPVISIBLE add_const<T volatile> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_const<T const volatile> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_const<T&> { typedef T& type; };
    
    template <class T> struct UPVISIBLE add_volatile { typedef T volatile type; };
    template <class T> struct UPVISIBLE add_volatile<T const> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_volatile<T volatile> { typedef T volatile type; };
    template <class T> struct UPVISIBLE add_volatile<T const volatile> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_volatile<T&> { typedef T& type; };
    
    template <class T> struct UPVISIBLE add_cv { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_cv<T const> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_cv<T volatile> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_cv<T const volatile> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE add_cv<T&> { typedef T& type; };
    
    template <class T> struct UPVISIBLE remove_reference { typedef T type; };
    template <class T> struct UPVISIBLE remove_reference<T&> { typedef T type; };
#ifndef UP_NO_RVALUE_REFERENCES
    template <class T> struct UPVISIBLE remove_reference<T&&> { typedef T type; };
#endif

    template <class T> struct UPVISIBLE add_lvalue_reference { typedef typename remove_reference<T>::type& type; };
    template <> struct UPVISIBLE add_lvalue_reference<void> { typedef void type; };
    template <> struct UPVISIBLE add_lvalue_reference<void const> { typedef void const type; };
    template <> struct UPVISIBLE add_lvalue_reference<void volatile> { typedef void volatile type; };
    template <> struct UPVISIBLE add_lvalue_reference<void const volatile> { typedef void const volatile type; };

#ifndef UP_NO_RVALUE_REFERENCES
    template <class T> struct UPVISIBLE add_rvalue_reference { typedef typename remove_reference<T>::type&& type; };
#else
    template <class T> struct UPVISIBLE add_rvalue_reference { typedef typename remove_reference<T>::type type; };
#endif
    template <class T> struct UPVISIBLE add_rvalue_reference<T&> { typedef T& type; };
    template <> struct UPVISIBLE add_rvalue_reference<void> { typedef void type; };
    template <> struct UPVISIBLE add_rvalue_reference<void const> { typedef void const type; };
    template <> struct UPVISIBLE add_rvalue_reference<void volatile> { typedef void volatile type; };
    template <> struct UPVISIBLE add_rvalue_reference<void const volatile> { typedef void const volatile type; };

    template <class T> struct UPVISIBLE make_signed { typedef typename detail::make_signed_impl<T>::type type; };
    template <class T> struct UPVISIBLE make_signed<T const> { typedef typename add_const<typename make_signed<T>::type>::type type; };
    template <class T> struct UPVISIBLE make_signed<T volatile> { typedef typename add_volatile<typename make_signed<T>::type>::type type; };
    template <class T> struct UPVISIBLE make_signed<T const volatile> { typedef typename add_cv<typename make_signed<T>::type>::type type; };
    template <class T> struct UPVISIBLE make_unsigned { typedef typename detail::make_unsigned_impl<T>::type type; };
    template <class T> struct UPVISIBLE make_unsigned<T const> { typedef typename add_const<typename make_unsigned<T>::type>::type type; };
    template <class T> struct UPVISIBLE make_unsigned<T volatile> { typedef typename add_volatile<typename make_unsigned<T>::type>::type type; };
    template <class T> struct UPVISIBLE make_unsigned<T const volatile> { typedef typename add_cv<typename make_unsigned<T>::type>::type type; };

    template <class T> struct UPVISIBLE remove_extent { typedef T type; };
    template <class T, size_t Size> struct UPVISIBLE remove_extent<T[Size]> { typedef T type; };
    template <class T, size_t Size> struct UPVISIBLE remove_extent<T const[Size]> { typedef T const type; };
    template <class T, size_t Size> struct UPVISIBLE remove_extent<T volatile[Size]> { typedef T volatile type; };
    template <class T, size_t Size> struct UPVISIBLE remove_extent<T const volatile[Size]> { typedef T const volatile type; };
    template <class T> struct UPVISIBLE remove_extent<T[]> { typedef T type; };
    template <class T> struct UPVISIBLE remove_extent<T const[]> { typedef T const type; };
    template <class T> struct UPVISIBLE remove_extent<T volatile[]> { typedef T volatile type; };
    template <class T> struct UPVISIBLE remove_extent<T const volatile[]> { typedef T const volatile type; };
    
    template <class T> struct UPVISIBLE remove_all_extents
        { typedef T type; };
    template <class T, size_t Size> struct UPVISIBLE remove_all_extents<T[Size]>
        { typedef typename remove_all_extents<T>::type type; };
    template <class T, size_t Size> struct UPVISIBLE remove_all_extents<T const[Size]>
        { typedef typename remove_all_extents<T const>::type type; };
    template <class T, size_t Size> struct UPVISIBLE remove_all_extents<T volatile[Size]>
        { typedef typename remove_all_extents<T volatile>::type type; };
    template <class T, size_t Size> struct UPVISIBLE remove_all_extents<T const volatile[Size]>
        { typedef typename remove_all_extents<T const volatile>::type type; };
    template <class T> struct UPVISIBLE remove_all_extents<T[]>
        { typedef typename remove_all_extents<T>::type type; };
    template <class T> struct UPVISIBLE remove_all_extents<T const[]>
        { typedef typename remove_all_extents<T const>::type const type; };
    template <class T> struct UPVISIBLE remove_all_extents<T volatile[]>
        { typedef typename remove_all_extents<T volatile>::type volatile type; };
    template <class T> struct UPVISIBLE remove_all_extents<T const volatile[]>
        { typedef typename remove_all_extents<T const volatile>::type const volatile type; };    
    
    template <class T> struct UPVISIBLE remove_pointer { typedef T type; };
    template <class T> struct UPVISIBLE remove_pointer<T*> { typedef T type; };
    template <class T> struct UPVISIBLE remove_pointer<T* const> { typedef T type; };
    template <class T> struct UPVISIBLE remove_pointer<T* volatile> { typedef T type; };
    template <class T> struct UPVISIBLE remove_pointer<T* const volatile> { typedef T type; };
    
    template <class T> struct UPVISIBLE add_pointer { typedef T* type; };
    template <class T> struct UPVISIBLE add_pointer<T&> { typedef T* type; };
#ifndef UP_NO_RVALUE_REFERENCES
    template <class T> struct UPVISIBLE add_pointer<T&&> { typedef T* type; };
#endif

    template <class T, class U> struct UPVISIBLE is_same : false_type { };
    template <class T> struct UPVISIBLE is_same<T, T> : true_type { };
}

namespace up { namespace detail
{
    template <class T>
    UPHIDDEN
    typename conditional<is_fundamental<T>::value, T, typename add_rvalue_reference<T>::type>::type
    declval() noexcept;    

#ifndef UP_IS_BASE_OF
    template <class Base, class Derived>
    struct is_base_of_checker
    {
        struct convertor
        {
            operator Base const volatile*() const;
            operator Derived const volatile*();
        };
            
        typedef char (&failure)[2];
        template <class U> static char sfinae(Derived const volatile*, U*) noexcept;
        static failure sfinae(Base const volatile*, int) noexcept;
        static constexpr bool value = sizeof(sfinae(convertor(), 0)) != sizeof(failure);
    };

    template
    <
        class Base,
        class Derived,
        bool = is_class<Base>::value,
        bool = is_class<Derived>::value,
        bool = is_same<Base, Derived>::value
    >
    struct is_base_of_impl : false_type { };
        
    template <class Base, class Derived>
    struct is_base_of_impl<Base, Derived, true, true, true> : true_type { };
        
    template <class Base, class Derived>
    struct is_base_of_impl<Base, Derived, true, true, false> : is_base_of_checker<Base, Derived> { };
#endif

#ifndef UP_IS_CONVERTIBLE_TO
    template
    <
        class T,
        bool = is_array<T>::value, 
        bool = is_function<T>::value,
        bool = is_void<T>::value
    >
    struct is_convertible_array_function_or_void
        : integral_constant<unsigned int, 0> { };
        
    template <class T>
    struct is_convertible_array_function_or_void<T, true, false, false>
        : integral_constant<unsigned int, 1> { };
        
    template <class T>
    struct is_convertible_array_function_or_void<T, false, true, false>
        : integral_constant<unsigned int, 2> { };
        
    template <class T>
    struct is_convertible_array_function_or_void<T, false, false, true>
        : integral_constant<unsigned int, 3> { };

    template
    <
        class T,
        class U,
        unsigned int = is_convertible_array_function_or_void<T>::value,
        unsigned int = is_convertible_array_function_or_void<U>::value
    >
    struct is_convertible_impl
    {
        typedef char (&failure)[2];
#ifndef UP_NO_RVALUE_REFERENCES
        template <class V> static char sfinae(typename remove_reference<V>::type const volatile&&) noexcept;
#else
        template <class V> static char sfinae(V) noexcept;
#endif
        template <class V> static failure sfinae(...) noexcept;
        static constexpr bool value = sizeof(sfinae<U>(::up::detail::declval<T>())) == sizeof(char);
    };

    template <class T, class U> struct is_convertible_impl<T, U, 1, 0> : false_type { };

    template <class T> struct is_convertible_impl<T, const T&, 1, 0> : true_type { };
#ifndef UP_NO_RVALUE_REFERENCES
    template <class T> struct is_convertible_impl<T, T&&, 1, 0> : true_type { };
    template <class T> struct is_convertible_impl<T, const T&&, 1, 0> : true_type { };
    template <class T> struct is_convertible_impl<T, volatile T&&, 1, 0> : true_type { };
    template <class T> struct is_convertible_impl<T, const volatile T&&, 1, 0> : true_type { };
#endif

    template <class T, class U> struct is_convertible_impl<T, U*, 1, 0>
        : integral_constant<bool, is_convertible_impl<typename remove_all_extents<T>::type*, U*>::value> { };

    template <class T, class U> struct is_convertible_impl<T, U* const, 1, 0>
        : integral_constant<bool, is_convertible_impl<typename remove_all_extents<T>::type*, U*const>::value> { };

    template <class T, class U> struct is_convertible_impl<T, U* volatile, 1, 0>
        : integral_constant<bool, is_convertible_impl<typename remove_all_extents<T>::type*, U*volatile>::value> { };

    template <class T, class U> struct is_convertible_impl<T, U* const volatile, 1, 0>
        : integral_constant<bool, is_convertible_impl<typename remove_all_extents<T>::type*, U*const volatile>::value> { };

    template <class T, class U> struct is_convertible_impl<T, U, 2, 0> : false_type { };
#ifndef UP_NO_RVALUE_REFERENCES
    template <class T> struct is_convertible_impl<T, T&&, 2, 0> : true_type { };
#endif
    template <class T> struct is_convertible_impl<T, T*, 2, 0> : true_type { };
    template <class T> struct is_convertible_impl<T, T*const, 2, 0> : true_type { };
    template <class T> struct is_convertible_impl<T, T*volatile, 2, 0> : true_type { };
    template <class T> struct is_convertible_impl<T, T*const volatile, 2, 0> : true_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 3, 0> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 0, 1> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 1, 1> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 2, 1> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 3, 1> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 0, 2> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 1, 2> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 2, 2> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 3, 2> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 0, 3> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 1, 3> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 2, 3> : false_type { };
    template <class T, class U> struct is_convertible_impl<T, U, 3, 3> : true_type { };
#endif

#ifndef UP_TT_IS_ABSTRACT
    template <class T, bool = is_class<T>::value>
    struct is_abstract_impl
    {
        typedef char yes;
        typedef char (&no)[2];
        template <class U> static no sfinae(T (*)[1]) noexcept;
        template <class U> static yes sfinae(...) noexcept;
        static constexpr bool value = sizeof(sfinae<T>(0)) == sizeof(yes);
    };

    template <class T> struct is_abstract_impl<T, false> : false_type {};
#endif
}}

namespace up
{
#ifdef UP_TT_IS_BASE_OF
    template <class Base, class Derived>
    struct UPVISIBLE is_base_of :
        integral_constant<bool, UP_TT_IS_BASE_OF(Base, Derived)> { };
#else
    template <class Base, class Derived>
    struct UPVISIBLE is_base_of :
        integral_constant<bool, detail::is_base_of_impl<typename remove_cv<Base>::type,
            typename remove_cv<Derived>::type>::value> { };
#endif
    
#ifdef UP_TT_IS_CONVERTIBLE_TO
    template <class From, class To>
    struct UPVISIBLE is_convertible :
        integral_constant<bool, UP_TT_IS_CONVERTIBLE_TO(From, To)> { };
#else
    template <class T, class U>
    struct UPVISIBLE is_convertible
        : integral_constant<bool, detail::is_convertible_impl<T, U>::value> { };
#endif

#ifdef UP_TT_IS_ABSTRACT
    template <class T>
    struct UPVISIBLE is_abstract
        : integral_constant<bool, UP_TT_IS_ABSTRACT(T)> { };
#else
    template <class T>
    struct UPVISIBLE is_abstract
        : integral_constant
        <
            bool,
            detail::is_abstract_impl
            <
                typename remove_cv<typename remove_all_extents<T>::type>::type
            >::value
        > {};
#endif
}

namespace up { namespace detail
{
    template <class T> struct is_signed_impl : false_type { };
    template <> struct is_signed_impl<float> : true_type { };
    template <> struct is_signed_impl<double> : true_type { };
    template <> struct is_signed_impl<long double> : true_type { };
    template <> struct is_signed_impl<signed char> : true_type { };
    template <> struct is_signed_impl<short> : true_type { };
    template <> struct is_signed_impl<int> : true_type { };
    template <> struct is_signed_impl<long> : true_type { };
    template <> struct is_signed_impl<long long> : true_type { };
#if CHAR_MIN < 0
    template <> struct is_signed_impl<char> : true_type { };
#endif
#if !defined(UP_NO_NATIVE_WCHAR_T) && (WCHAR_MIN < 0)
    template <> struct is_signed_impl<wchar_t> : true_type { };
#endif
#if !defined(UP_NO_NATIVE_CHAR16_T) && (CHAR16_MIN < 0)
    template <> struct is_signed_impl<char16_t> : true_type { };
#endif
#if !defined(UP_NO_NATIVE_CHAR32_T) && (CHAR32_MIN < 0)
    template <> struct is_signed_impl<char32_t> : true_type { };
#endif

#ifndef UP_TT_IS_EMPTY
    template <class T, bool = is_class<T>::value>
    struct is_empty_impl
    {
        class expected_type { double x; };

        class actual_type : public T
        {
            double x;
            actual_type();
            actual_type(actual_type const&);
            actual_type& operator=(actual_type const&);
        };

        static constexpr bool value = sizeof(actual_type) == sizeof(expected_type);
    };
        
    template <class T> struct is_empty_impl<T, false> : false_type { };
#endif

#ifndef UP_TT_IS_POLYMORPHIC
    template <class T, bool = is_class<T>::value>
    struct is_polymorphic_impl
    {
        struct derived_type : T { };
        struct virtual_derived_type : T { virtual ~virtual_derived_type() noexcept; };
        static constexpr bool value = sizeof(derived_type) == sizeof(virtual_derived_type);
    };
        
    template <class T> struct is_polymorphic_impl<T, false> : false_type { };
#endif

    template <class T, bool = is_void<T>::value || is_abstract<T>::value>
    struct is_destructible_impl
    {
        template <class U>
        struct test { T t; };
            
        template <class U>
        static decltype((::up::detail::declval<test<U> >().~test<U>(), true_type()))
#ifndef UP_NO_RVALUE_REFERENCES            
        sfinae(U&&) noexcept;
#else
        sfinae(U&) noexcept;
#endif            
            
        template <class U>
        static false_type sfinae(any_t) noexcept;
            
        typedef decltype(sfinae<T>(::up::detail::declval<T>())) result;
    };
        
    template <class T>
    struct is_destructible_impl<T, true> : false_type { };
}}

namespace up
{
    template <class T> struct UPVISIBLE is_const : false_type { };
    template <class T> struct UPVISIBLE is_const<T const> : true_type { };
    template <class T> struct UPVISIBLE is_const<T volatile> : false_type { };
    template <class T> struct UPVISIBLE is_const<T const volatile> : true_type { };
    
    template <class T> struct UPVISIBLE is_volatile : false_type { };
    template <class T> struct UPVISIBLE is_volatile<T const> : false_type { };
    template <class T> struct UPVISIBLE is_volatile<T volatile> : true_type { };
    template <class T> struct UPVISIBLE is_volatile<T const volatile> : true_type { };

    template <class T> struct UPVISIBLE is_signed
        : integral_constant<bool, detail::is_signed_impl<typename remove_reference<T>::type>::value> { };

    template <class T> struct UPVISIBLE is_unsigned
        : integral_constant<bool, is_integral<T>::value && !is_signed<T>::value> { };

#ifdef UP_TT_IS_EMPTY
    template <class T>
    struct UPVISIBLE is_empty : integral_constant<bool, UP_TT_IS_EMPTY(T)> { };
#else
    template <class T>
    struct UPVISIBLE is_empty
        : integral_constant<bool, detail::is_empty_impl<typename remove_cv<T>::type>::value> { };
#endif
    
#ifdef UP_TT_IS_POLYMORPHIC
    template <class T>
    struct UPVISIBLE is_polymorphic : integral_constant<bool, UP_TT_IS_POLYMORPHIC(T)> { };
#else
    template <class T>
    struct UPVISIBLE is_polymorphic
        : integral_constant
        <
            bool,
            detail::is_polymorphic_impl<typename remove_cv<typename remove_all_extents<T>::type>::type>::value
        >
        { };
#endif

    template <class T>
    struct UPVISIBLE has_virtual_destructor 
#ifdef UP_TT_HAS_VIRTUAL_DESTRUCTOR
        : integral_constant<bool, UP_TT_HAS_VIRTUAL_DESTRUCTOR(T)> { };
#else
        : integral_constant<bool, is_polymorphic<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_destructible
        : integral_constant<bool, detail::is_destructible_impl<typename remove_all_extents<T>::type>::result::value> { };
}

namespace up { namespace detail
{
#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_DECLTYPE)
    template <bool, class T, class... Args>
    struct is_constructible_impl
    {
        template <class V, size_t N, bool = is_scalar<V>::value || is_reference<V>::value>
        struct tester
        {
            template <class U, class... A>
            static decltype((U(::up::detail::declval<A>()...), true_type())) sfinae(U*) noexcept;
            template <class U, class... A>
            static false_type sfinae(...) noexcept;
        };

        template <class V, size_t N>
        struct tester<V, N, true>
        {
            template <class U, class... A>
            static false_type sfinae(...) noexcept;
        };
            
        template <class V>
        struct tester<V, 0, true>
        {
            template <class U, class... A>
            static typename enable_if<is_scalar<U>::value, true_type>::type sfinae(U*) noexcept;
            template <class U, class... A>
            static false_type sfinae(...) noexcept;
        };
            
        template <class V>
        struct tester<V, 1, true>
        {
            template <class U, class A, class... B>
            static typename enable_if<is_convertible<U, A>::value, true_type>::type sfinae(U*) noexcept;
            template <class U, class A, class... B>
            static false_type sfinae(...) noexcept;
        };

        typedef decltype(tester<T, sizeof...(Args)>::template sfinae<T, Args...>(0)) result;
    };

    template <class A, size_t N>
    struct is_constructible_impl<false, A[N]>
        : is_constructible_impl<false, typename remove_all_extents<A>::type> { };
        
    template <class A, size_t N, class... Args>
    struct is_constructible_impl<false, A[N], Args...>
    {
        typedef false_type result;
    };
        
    template <class A>
    struct is_constructible_impl<false, A[]>
    {
        typedef false_type result;
    };
        
    template <class T, class... Args>
    struct is_constructible_impl<true, T, Args...>
    {
        typedef false_type result;
    };
#else
    typedef char (&is_constructible_t)[1];
    typedef char (&is_not_constructible_t)[2];

    template <bool, class T, class A1 = nat_t, class A2 = nat_t, class A3 = nat_t, class A4 = nat_t>
    struct is_constructible_impl
    {
        template <class V, bool = is_scalar<V>::value || is_reference<V>::value>
        struct tester
        {
            template <class U, class B1, class B2, class B3, class B4>
            static is_constructible_t sfinae(
                decltype((U(::up::detail::declval<B1>(), 
                            ::up::detail::declval<B2>(),
                            ::up::detail::declval<B3>(),
                            ::up::detail::declval<B4>())))*) noexcept;

            template <class U, class B1, class B2, class B3, class B4>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        template <class V>
        struct tester<V, true>
        {
            template <class U, class B1, class B2, class B3, class B4>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        typedef integral_constant<bool, (sizeof(tester<T>::template sfinae<T,A1,A2,A3,A4>(0)) == sizeof(is_constructible_t))> result;
    };

    template <class T>
    struct is_constructible_impl<false, T, nat_t, nat_t, nat_t, nat_t>
    {
        template <class V, bool = is_scalar<V>::value || is_reference<V>::value>
        struct tester
        {
            template <class U>
            static is_constructible_t sfinae(decltype((U()))*);
            template <class U>
            static is_not_constructible_t sfinae(...);
        };
            
        template <class V>
        struct tester<V, true>
        {
            template <class U>
            static typename enable_if<is_scalar<U>::value, is_constructible_t>::type sfinae(int) noexcept;
            template <class U>
            static is_not_constructible_t sfinae(...) noexcept;
        };

        typedef integral_constant<bool, (sizeof(tester<T>::template sfinae<T>(0)) == sizeof(is_constructible_t))> result;
    };

    template <class T, class A1 >
    struct is_constructible_impl<false, T, A1, nat_t, nat_t, nat_t>
    {
        template <class V, bool = is_scalar<V>::value || is_reference<V>::value>
        struct tester
        {
            template <class U, class B1>
            static is_constructible_t sfinae(decltype((U(::up::detail::declval<B1>())))*) noexcept;
            template <class U, class B1>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        template <class V>
        struct tester<V, true>
        {
            template <class U, class B1>
            static typename enable_if<is_convertible<U, B1>::value, is_constructible_t>::type sfinae(int) noexcept;
            template <class U, class B1>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        typedef integral_constant<bool, (sizeof(tester<T>::template sfinae<T,A1>(0)) == sizeof(is_constructible_t))> result;
    };

    template <class T, class A1, class A2 >
    struct is_constructible_impl<false, T, A1, A2, nat_t, nat_t>
    {
        template <class V, bool = is_scalar<V>::value || is_reference<V>::value>
        struct tester
        {
            template <class U, class B1, class B2>
            static is_constructible_t sfinae(decltype((U(::up::detail::declval<B1>(), ::up::detail::declval<B2>())))*) noexcept;
            template <class U, class B1, class B2>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        template <class V>
        struct tester<V, true>
        {
            template <class U, class B1, class B2>
            static is_not_constructible_t sfinae(...) noexcept;
        };

        typedef integral_constant<bool, (sizeof(tester<T>::template sfinae<T,A1,A2>(0)) == sizeof(is_constructible_t))> result;
    };

    template <class T, class A1, class A2, class A3>
    struct is_constructible_impl<false, T, A1, A2, A3, nat_t>
    {
        template <class V, bool = is_scalar<V>::value || is_reference<V>::value>
        struct tester
        {
            template <class U, class B1, class B2, class B3>
            static is_constructible_t sfinae(
                decltype((U(::up::detail::declval<B1>(),
                            ::up::detail::declval<B2>(),
                            ::up::detail::declval<B3>())))*) noexcept;

            template <class U, class B1, class B2, class B3>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        template <class V>
        struct tester<V, true>
        {
            template <class U, class B1, class B2, class B3>
            static is_not_constructible_t sfinae(...) noexcept;
        };
            
        typedef integral_constant<bool, (sizeof(tester<T>::template sfinae<T,A1,A2,A3>(0)) == sizeof(is_constructible_t))> result;
    };
        
    template <class A, size_t N>
    struct is_constructible_impl<false, A[N], nat_t, nat_t, nat_t, nat_t>
        : is_constructible_impl<false, typename remove_all_extents<A>::type, nat_t, nat_t, nat_t, nat_t> { };
        
    template <class A, size_t N, class A1, class A2, class A3, class A4>
    struct is_constructible_impl<false, A[N], A1, A2, A3, A4>
    {
        typedef false_type result;
    };
        
    template <class A, class A1, class A2, class A3, class A4>
    struct is_constructible_impl<false, A[], A1, A2, A3, A4>
    {
        typedef false_type result;
    };
        
    template <class T, class A1, class A2, class A3, class A4>
    struct is_constructible_impl<true, T, A1, A2, A3, A4>
    {
        typedef false_type result;
    };
#endif

    template
    <
        class T,
        class U,
        class X = typename remove_reference<T>::type,
        bool = is_scalar<X>::value,
        bool = is_void<T>::value || is_void<U>::value || is_const<X>::value
    >
    struct is_assignable_impl
    {
        template <class V, class W> static decltype((::up::detail::declval<V>() = ::up::detail::declval<W>(), true_type())) sfinae(int) noexcept;
        template <class V, class W> static false_type sfinae(...) noexcept;
        typedef decltype(sfinae<T, U>(0)) result;
    };
        
    template <class T, class U, class X>
    struct is_assignable_impl<T, U, X, true, false> { typedef is_convertible<typename remove_reference<U>::type, X> result; };
        
    template <class T, class U, class X>
    struct is_assignable_impl<T, U, X, false, true> { typedef false_type result; };
        
    template <class T, class U, class X>
    struct is_assignable_impl<T, U, X, true, true> { typedef false_type result; };

#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_NOEXCEPT)
    template <bool, class T, class... Args>
    struct is_nothrow_constructible_impl;

    template <class T, class... Args>
    struct is_nothrow_constructible_impl<true, T, Args...> : integral_constant<bool, noexcept(T(::up::detail::declval<Args>()...))> { };

    template <class T, class... Args>
    struct is_nothrow_constructible_impl<false, T, Args...> : false_type { };
#endif

#ifndef UP_NO_NOEXCEPT
    template <
        class T,
        class U,
        bool = is_assignable_impl<T, U>::result::value,
        bool = is_scalar<typename remove_reference<T>::type>::value
            && is_scalar<typename remove_reference<U>::type>::value
    >
    struct is_nothrow_assignable_impl
        : false_type { };

    template <class T, class U>
    struct is_nothrow_assignable_impl<T, U, true, false>
        : integral_constant<bool, noexcept((::up::detail::declval<T>() = ::up::detail::declval<U>()))> { };
            
    template <class T, class U>
    struct is_nothrow_assignable_impl<T, U, true, true>
        : true_type { };
#endif

    template <class T, bool = is_destructible<T>::value>
    struct is_nothrow_destructible_impl : false_type { };
        
#ifndef UP_NO_NOEXCEPT
    template <class T>
    struct is_nothrow_destructible_impl<T, true> : integral_constant<bool, noexcept(::up::detail::declval<T>().~T())> { };
#else
    template <class T>
    struct is_nothrow_destructible_impl<T, true> : integral_constant<bool, is_scalar<T>::value || is_reference<T>::value> { };
#endif
}}

namespace up
{
#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_DECLTYPE)
    template <class T, class... Args>
    struct UPVISIBLE is_constructible :
        integral_constant<bool, detail::is_constructible_impl<is_abstract<T>::value, T, Args...>::result::value> { };
#else
    template <class T, class A1 = nat_t, class A2 = nat_t, class A3 = nat_t, class A4 = nat_t>
    struct UPVISIBLE is_constructible :
        integral_constant<bool, detail::is_constructible_impl<is_abstract<T>::value, T, A1, A2, A3, A4>::result::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_default_constructible
        : is_constructible<T> { };
    
    template <class T>
    struct UPVISIBLE is_copy_constructible
        : is_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> { };

    template <class T>
    struct UPVISIBLE is_move_constructible
        : is_constructible<T, typename add_rvalue_reference<T>::type> { };
    
    template <class T, class U>
    struct UPVISIBLE is_assignable
        : integral_constant<bool, detail::is_assignable_impl<T, U>::result::value> { };
    
    template <class T>
    struct UPVISIBLE is_copy_assignable
        : is_assignable<typename add_lvalue_reference<T>::type, typename add_lvalue_reference<typename add_const<T>::type>::type> { };

    template <class T>
    struct UPVISIBLE is_move_assignable
        : is_assignable<typename add_lvalue_reference<T>::type, typename add_rvalue_reference<T>::type> { };
    
#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_NOEXCEPT)

    template <class T, class... Args>
    struct UPVISIBLE is_nothrow_constructible
        : integral_constant<bool, detail::is_nothrow_constructible_impl<is_constructible<T, Args...>::value, T, Args...>::value> { };

    template <class T, size_t N>
    struct UPVISIBLE is_nothrow_constructible<T[N]>
        : integral_constant<bool, detail::is_nothrow_constructible_impl<is_constructible<T>::value, T>::value> { };

#elif !defined(UP_NO_VARIADIC_TEMPLATES)

    template <class T, class... Args>
    struct UPVISIBLE is_nothrow_constructible : false_type { };

    template <class T>
    struct UPVISIBLE is_nothrow_constructible<T>
#ifdef UP_TT_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_nothrow_constructible<T, T&>
#ifdef UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_nothrow_constructible<T, const T&>
#ifdef UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
#if !defined(UP_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE) && !defined(UP_NO_RVALUE_REFERENCES )
    struct UPVISIBLE is_nothrow_constructible<T, T&&>
        : integral_constant<bool, UP_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)> { };
#else
    struct UPVISIBLE is_nothrow_constructible<T, T>
        : integral_constant<bool, is_scalar<T>::value || is_reference<T>::value> { };
#endif

#else
    
    template <class T, class A1 = nat_t, class A2 = nat_t, class A3 = nat_t, class A4 = nat_t>
    struct UPVISIBLE is_nothrow_constructible : false_type { };

    template <class T>
    struct UPVISIBLE is_nothrow_constructible<T, nat_t, nat_t, nat_t, nat_t>
#ifdef UP_TT_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_nothrow_constructible<T, T&, nat_t, nat_t, nat_t>
#ifdef UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_nothrow_constructible<T, const T&, nat_t, nat_t, nat_t>
#ifdef UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
#if !defined(UP_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE) && !defined(UP_NO_RVALUE_REFERENCES )
    struct UPVISIBLE is_nothrow_constructible<T, T&&, nat_t, nat_t, nat_t>
        : integral_constant<bool, UP_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)> { };
#else
    struct UPVISIBLE is_nothrow_constructible<T, T, nat_t, nat_t, nat_t>
        : integral_constant<bool, is_scalar<T>::value || is_reference<T>::value> { };
#endif

#endif

    template <class T>
    struct UPVISIBLE is_nothrow_default_constructible
        : is_nothrow_constructible<T> { };

    template <class T>
    struct UPVISIBLE is_nothrow_copy_constructible
        : is_nothrow_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> { };
    
    template <class T>
    struct UPVISIBLE is_nothrow_move_constructible
        : is_nothrow_constructible<T, typename add_rvalue_reference<T>::type> { };

#ifndef UP_NO_NOEXCEPT
    template <class T, class U>
    struct UPVISIBLE is_nothrow_assignable
        : integral_constant<bool, detail::is_nothrow_assignable_impl<T, U>::value> { };
#else
    template <class T, class U>
    struct UPVISIBLE is_nothrow_assignable : false_type { };

    template <class T>
    struct UPVISIBLE is_nothrow_assignable<T, T&>
#ifdef UP_TT_IS_NOTHROW_COPY_ASSIGNABLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_COPY_ASSIGNABLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif
    
    template <class T>
    struct UPVISIBLE is_nothrow_assignable<T, T const&>
#ifdef UP_TT_IS_NOTHROW_COPY_ASSIGNABLE
        : integral_constant<bool, UP_TT_IS_NOTHROW_COPY_ASSIGNABLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
#if defined(UP_TT_IS_NOTHROW_MOVE_ASSIGNABLE) && !defined(UP_NO_RVALUE_REFERENCES)
    struct UPVISIBLE is_nothrow_assignable<T, T&&>
        : integral_constant<bool, UP_TT_IS_NOTHROW_MOVE_ASSIGNABLE(T)> { };
#else
    struct UPVISIBLE is_nothrow_assignable<T, T>
        : integral_constant<bool, is_scalar<T>::value> { };
#endif
#endif

    template <class T>
    struct UPVISIBLE is_nothrow_copy_assignable
        : is_nothrow_assignable<T, typename add_lvalue_reference<typename add_const<T>::type>::type> { };

    template <class T>
    struct UPVISIBLE is_nothrow_move_assignable
        : is_nothrow_assignable<T, typename add_rvalue_reference<T>::type> { };

    template <class T>
    struct UPVISIBLE is_nothrow_destructible
        : integral_constant<bool, detail::is_nothrow_destructible_impl<typename remove_all_extents<T>::type>::value> { };

    template <class T>
    struct UPVISIBLE is_nothrow_destructible<T&> : true_type { };

#ifndef UP_NO_RVALUE_REFERENCES
    template <class T>
    struct UPVISIBLE is_nothrow_destructible<T&&> : true_type { };
#endif
  
#ifndef UP_NO_VARIADIC_TEMPLATES

    template <class T, class... Args>
    struct UPVISIBLE is_trivially_constructible : false_type { };

    template <class T>
    struct UPVISIBLE is_trivially_constructible<T>
#ifdef UP_TT_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_trivially_constructible<T, T&>
#ifdef UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_trivially_constructible<T, T const&>
#ifdef UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
#if defined(UP_TT_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE) && !defined(UP_NO_RVALUE_REFERENCES)
    struct UPVISIBLE is_trivially_constructible<T, T&&>
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)> { };
#else
    struct UPVISIBLE is_trivially_constructible<T, T>
        : integral_constant<bool, is_scalar<T>::value || is_reference<T>::value> { };
#endif

#else

    template <class T, class A1 = nat_t, class A2 = nat_t, class A3 = nat_t, class A4 = nat_t>
    struct UPVISIBLE is_trivially_constructible : false_type { };

    template <class T>
    struct UPVISIBLE is_trivially_constructible<T, nat_t, nat_t, nat_t, nat_t>
#ifdef UP_TT_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif    

    template <class T>
    struct UPVISIBLE is_trivially_constructible<T, T&, nat_t, nat_t, nat_t>
#ifdef UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif
    
    template <class T>
    struct UPVISIBLE is_trivially_constructible<T, T const&, nat_t, nat_t, nat_t>
#ifdef UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
#if defined(UP_TT_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE) && !defined(UP_NO_RVALUE_REFERENCES)
    struct UPVISIBLE is_trivially_constructible<T, T&&, nat_t, nat_t, nat_t>
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)> { };
#else
    struct UPVISIBLE is_trivially_constructible<T, T, nat_t, nat_t, nat_t>
        : integral_constant<bool, is_scalar<T>::value || is_reference<T>::value> { };
#endif

#endif

    template <class T>
    struct UPVISIBLE is_trivially_default_constructible :
        is_trivially_constructible<T> { };

    template <class T>
    struct UPVISIBLE is_trivially_copy_constructible :
        is_trivially_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> { };
    
    template <class T>
    struct UPVISIBLE is_trivially_move_constructible :
        is_trivially_constructible<T, typename add_rvalue_reference<T>::type> { };

    template <class T, class U>
    struct UPVISIBLE is_trivially_assignable : false_type { };

    template <class T>
    struct UPVISIBLE is_trivially_assignable<T, T&>
#ifdef UP_TT_IS_TRIVIALLY_COPY_ASSIGNABLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPY_ASSIGNABLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif
    
    template <class T>
    struct UPVISIBLE is_trivially_assignable<T, T const&>
#ifdef UP_TT_IS_TRIVIALLY_COPY_ASSIGNABLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPY_ASSIGNABLE(T)> { };
#else
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
#if defined(UP_TT_IS_TRIVIALLY_MOVE_ASSIGNABLE) && !defined(UP_NO_RVALUE_REFERENCES)
    struct UPVISIBLE is_trivially_assignable<T, T&&>
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)> { };
#else
    struct UPVISIBLE is_trivially_assignable<T, T>
        : integral_constant<bool, is_scalar<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_trivially_copy_assignable
        : is_trivially_assignable<T, typename add_lvalue_reference<typename add_const<T>::type>::type> { };

    template <class T>
    struct UPVISIBLE is_trivially_move_assignable
        : is_trivially_assignable<T, typename add_rvalue_reference<T>::type> { };

    template <class T>
    struct UPVISIBLE is_trivially_destructible
#ifdef UP_TT_IS_TRIVIALLY_DESTRUCTIBLE    
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_DESTRUCTIBLE(T)> { };
#else
        : integral_constant<bool, is_scalar<typename remove_all_extents<T>::type>::value
            || is_reference<typename remove_all_extents<T>::type>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_trivially_copyable
#ifdef UP_TT_IS_TRIVIALLY_COPYABLE
        : integral_constant<bool, UP_TT_IS_TRIVIALLY_COPYABLE(T)> { };
#else    
        : integral_constant<bool,
            is_trivially_destructible<typename remove_const<typename remove_all_extents<T>::type>::type>::value
            && is_trivially_copy_constructible<typename remove_const<typename remove_all_extents<T>::type>::type>::value
            && is_trivially_move_constructible<typename remove_const<typename remove_all_extents<T>::type>::type>::value
            && is_trivially_copy_assignable<typename remove_const<typename remove_all_extents<T>::type>::type>::value
            && is_trivially_move_assignable<typename remove_const<typename remove_all_extents<T>::type>::type>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_trivial
#ifdef UP_TT_IS_TRIVIAL
        : integral_constant<bool, UP_TT_IS_TRIVIAL(T)> { };
#else
        : integral_constant<bool,
            is_trivially_copyable<T>::value
            && is_trivially_default_constructible<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_pod
#ifdef UP_TT_IS_POD
        : integral_constant<bool, UP_TT_IS_POD(T)> { };
#else    
        : integral_constant<bool, is_trivial<T>::value && !is_polymorphic<T>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_standard_layout
#ifdef UP_TT_IS_STANDARD_LAYOUT
        : integral_constant<bool, UP_TT_IS_STANDARD_LAYOUT(T)> { };
#else
        : integral_constant<bool, is_pod<typename remove_all_extents<T>::type>::value
            && !is_reference<typename remove_all_extents<T>::type>::value> { };
#endif

    template <class T>
    struct UPVISIBLE is_literal_type
#ifdef UP_TT_IS_LITERAL_TYPE
        : integral_constant<bool, UP_TT_IS_LITERAL_TYPE(T)> { };
#else
        : integral_constant<bool, is_pod<typename remove_all_extents<T>::type>::value
            && !is_reference<typename remove_all_extents<T>::type>::value> { };
#endif
}

namespace up { namespace detail
{
#ifdef UP_NO_ALIGNOF
    template <class T> struct alignment_of_impl { T _1; char _2; T _3; };
#endif
}}

namespace up
{
#ifndef UP_NO_ALIGNOF
    template <class T> struct UPVISIBLE alignment_of : integral_constant<size_t, alignof(T)> { };
#else
    template <class T> struct UPVISIBLE alignment_of : integral_constant<size_t, sizeof(detail::alignment_of_impl<T>) - (2 * sizeof(T))> { };
    template <class T> struct UPVISIBLE alignment_of<T&> : alignment_of<T*> { };
#endif

    template <class T> struct UPVISIBLE rank : integral_constant<size_t, 0> { };
    template <class T> struct UPVISIBLE rank<T[]> : integral_constant<size_t, rank<T>::value + 1> { };
    template <class T, size_t N> struct UPVISIBLE rank<T[N]> : integral_constant<size_t, rank<T>::value + 1> { };

    template <class T, unsigned I = 0> struct UPVISIBLE extent : integral_constant<size_t, 0> { };
    template <class T> struct UPVISIBLE extent<T[], 0> : integral_constant<size_t, 0> { };
    template <class T, unsigned I> struct UPVISIBLE extent<T[], I> : integral_constant<size_t, extent<T, I - 1>::value> { };
    template <class T, size_t N> struct UPVISIBLE extent<T[N], 0> : integral_constant<size_t, N> { };
    template <class T, size_t N, unsigned I> struct UPVISIBLE extent<T[N], I> : integral_constant<size_t, extent<T, I - 1>::value> { };
}

namespace up { namespace detail
{
#ifdef UP_NO_ALIGNAS
    template <class T>
    struct aligned_node : alignment_of<T>
    {
        typedef T type;
    };
        
    template <size_t Align, class T>
    struct aligned_storage_impl;
        
    template <size_t Align, class Head>
    struct aligned_storage_impl<Align, type_list<Head, nat_t> >
    {
        typedef typename conditional<Align == Head::value, typename Head::type, char>::type type;
    };
        
    template <size_t Align, class Head, class Tail>
    struct aligned_storage_impl<Align, type_list<Head, Tail> >
    {
        typedef typename conditional<Align == Head::value, typename Head::type, typename aligned_storage_impl<Align, Tail>::type>::type type;
    };
        
    typedef type_list
    <
        aligned_node<char>,
        type_list<aligned_node<short>,
        type_list<aligned_node<long>,
        type_list<aligned_node<long long>,
        type_list<aligned_node<double>,
        type_list<aligned_node<long double>,
        type_list<aligned_node<char>,
        type_list<aligned_node<int nat_t::*>,
        type_list<aligned_node<int (nat_t::*)()>,
        type_list<aligned_node<max_align_t>,
        nat_t> > > > > > > > >
    >
    all_aligned_types;
#endif

    template <class T>
    struct decay_impl
    {
        typedef typename conditional
        <
            is_array<T>::value,
            typename remove_extent<T>::type*,
            typename conditional
            <
                is_function<T>::value,
                typename add_pointer<T>::type,
                typename remove_cv<T>::type
            >
            ::type
        >
        ::type type;
    };

    template <class T, bool = is_function<T>::value, bool = is_member_function_pointer<T>::value>
    struct result_of_impl;

#ifndef UP_NO_VARIADIC_TEMPLATES    
    template <class F, class... Args> struct result_of_impl<F(Args...), true, false>
        { typedef decltype(::up::detail::declval<F>()(::up::detail::declval<Args>()...)) type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...), false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) const, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) volatile, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) const volatile, false, true> { typedef R type; };
#ifndef UP_NO_REFERENCE_QUALIFIED_FUNCTIONS        
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...)&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) const&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) volatile&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) const volatile&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...)&&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) const&&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) volatile&&, false, true> { typedef R type; };
    template <class R, class T, class... Args> struct result_of_impl<R(T::*)(Args...) const volatile&&, false, true> { typedef R type; };
#endif
#else
    template <class F>
    struct result_of_impl<F(), true, false>
    { typedef decltype(::up::detail::declval<F>()()) type; };

    template <class F, class A1>
    struct result_of_impl<F(A1), true, false>
    { typedef decltype(::up::detail::declval<F>()(::up::detail::declval<A1>())) type; };
        
    template <class F, class A1, class A2>
    struct result_of_impl<F(A1,A2), true, false>
    { typedef decltype(::up::detail::declval<F>()(::up::detail::declval<A1>(), ::up::detail::declval<A2>())) type; };
        
    template <class F, class A1, class A2, class A3>
    struct result_of_impl<F(A1,A2,A3), true, false>
    { typedef decltype(::up::detail::declval<F>()(::up::detail::declval<A1>(), ::up::detail::declval<A2>(), ::up::detail::declval<A3>())) type; };

    template <class F, class A1, class A2, class A3, class A4>
    struct result_of_impl<F(A1,A2,A3,A4), true, false>
    { typedef decltype(::up::detail::declval<F>()(::up::detail::declval<A1>(), ::up::detail::declval<A2>(), ::up::detail::declval<A3>(), ::up::detail::declval<A4>())) type; };

    template <class R, class T>
    struct result_of_impl<R(T::*)(), false, true>
    { typedef R type; };
        
    template <class R, class T, class A1>
    struct result_of_impl<R(T::*)(A1), false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2>
    struct result_of_impl<R(T::*)(A1,A2), false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2, class A3>
    struct result_of_impl<R(T::*)(A1,A2,A3), false, true>
    { typedef R type; };

    template <class R, class T, class A1, class A2, class A3, class A4>
    struct result_of_impl<R(T::*)(A1,A2,A3,A4), false, true>
    { typedef R type; };
        
    template <class R, class T>
    struct result_of_impl<R(T::*)() const, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1>
    struct result_of_impl<R(T::*)(A1) const, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2>
    struct result_of_impl<R(T::*)(A1,A2) const, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2, class A3>
    struct result_of_impl<R(T::*)(A1,A2,A3) const, false, true>
    { typedef R type; };

    template <class R, class T, class A1, class A2, class A3, class A4>
    struct result_of_impl<R(T::*)(A1,A2,A3,A4) const, false, true>
    { typedef R type; };

    template <class R, class T>
    struct result_of_impl<R(T::*)() volatile, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1>
    struct result_of_impl<R(T::*)(A1) volatile, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2>
    struct result_of_impl<R(T::*)(A1,A2) volatile, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2, class A3>
    struct result_of_impl<R(T::*)(A1,A2,A3) volatile, false, true>
    { typedef R type; };

    template <class R, class T, class A1, class A2, class A3, class A4>
    struct result_of_impl<R(T::*)(A1,A2,A3,A4) volatile, false, true>
    { typedef R type; };
        
    template <class R, class T>
    struct result_of_impl<R(T::*)() const volatile, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1>
    struct result_of_impl<R(T::*)(A1) const volatile, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2>
    struct result_of_impl<R(T::*)(A1,A2) const volatile, false, true>
    { typedef R type; };
        
    template <class R, class T, class A1, class A2, class A3>
    struct result_of_impl<R(T::*)(A1,A2,A3) const volatile, false, true>
    { typedef R type; };

    template <class R, class T, class A1, class A2, class A3, class A4>
    struct result_of_impl<R(T::*)(A1,A2,A3,A4) const volatile, false, true>
    { typedef R type; };               
#endif
}}

namespace up
{
#ifndef UP_NO_ALIGNAS
    template <size_t Len, size_t Align>
    struct UPVISIBLE aligned_storage
    {
        struct type { alignas(Align) char storage_[Len]; };
    };
#else
    template <size_t Len, size_t Align>
    struct UPVISIBLE aligned_storage
    {
        union type
        {
            char storage_[Len];
            typename detail::aligned_storage_impl<Align, detail::all_aligned_types>::type aligned_;
        };
    };
#endif

    template <class T>
    struct UPVISIBLE decay
    { 
        typedef typename detail::decay_impl<typename remove_reference<T>::type>::type type;
    };

#ifndef UP_NO_VARIADIC_TEMPLATES
    template <class... T>
    struct UPVISIBLE common_type;
    
    template <class T, class U, class... V>
    struct UPVISIBLE common_type<T, U, V...>
    {
        typedef typename common_type<typename common_type<T, U>::type, V...>::type type;
    };
#else
    template <class T1, class T2 = T1, class T3 = T2, class T4 = T3, class T5 = T4, class T6 = T5, class T7 = T6, class T8 = T7>
    struct UPVISIBLE common_type
    {
        typedef typename common_type
        <
            decltype(true ? ::up::detail::declval<T1>() : ::up::detail::declval<T2>()),
            T3, T4, T5, T6, T7, T8, T8
        >
        ::type type;
    };
#endif
    
    template <class T>
    struct UPVISIBLE common_type<T>
    {
        typedef T type;
    };
    
    template <class T, class U>
    struct UPVISIBLE common_type<T, U>
    {
        typedef decltype(true ? ::up::detail::declval<T>() : ::up::detail::declval<U>()) type;
    };

    template <class T>
    struct UPVISIBLE result_of
    {
        typedef typename detail::result_of_impl<typename remove_reference<T>::type>::type type;
    };
}

#endif
