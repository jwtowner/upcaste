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

#ifndef UP_UTILITY_HPP
#define UP_UTILITY_HPP

#include <up/type_traits.hpp>

namespace up
{
#if (UP_COMPILER == UP_COMPILER_GCC) && (__GNUC__ == 4) && (__GNUC_MINOR__ <= 4)
    namespace detail
    {
        template <class T> struct UPHIDDEN identity { typedef T type; };
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    T&& forward(typename detail::identity<T>::type&& t) noexcept {
        return t;
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    typename remove_reference<T>::type&& move(T&& t) noexcept {
        return t;
    }
#else
    template <class T>
    inline UPALWAYSINLINE UPPURE
    T&& forward(typename remove_reference<T>::type& t) noexcept {
        return static_cast<T&&>(t);
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    T&& forward(typename remove_reference<T>::type&& t) noexcept {
        return static_cast<T&&>(t);
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    typename remove_reference<T>::type&& move(T&& t) noexcept {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }
#endif

    namespace detail
    {
        template <class T>
        struct UPHIDDEN move_if_noexcept_result
        {
            typedef typename conditional
            <
                !is_nothrow_move_constructible<T>::value
                && is_copy_constructible<T>::value,
                T const&,
                T&&
            >
            ::type type;
        };
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    typename detail::move_if_noexcept_result<T>::type move_if_noexcept(T& x) noexcept {
        return static_cast<typename detail::move_if_noexcept_result<T>::type>(x);
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    typename decay<T>::type decay_copy(T&& v) noexcept {
        return ::up::forward<T>(v);
    }

    template <class T>
    UPHIDDEN
    typename add_rvalue_reference<T>::type declval() noexcept;

    template <class T>
    inline UPALWAYSINLINE
    void swap(T& a, T& b)
    UPNOEXCEPT(is_nothrow_move_constructible<T>::value && is_nothrow_move_assignable<T>::value) {
        T t(::up::move(a));
        a = ::up::move(b);
        b = ::up::move(t);
    }
    
    template <class ForwardIterator1, class ForwardIterator2>
    inline UPALWAYSINLINE
    void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
    UPNOEXCEPT(noexcept(swap(*::up::declval<ForwardIterator1>(), *::up::declval<ForwardIterator2>()))) {
        swap(*a, *b);
    }

    template <class T, size_t N>
    inline UPALWAYSINLINE
    void swap(T (&a)[N], T (&b)[N])
    UPNOEXCEPT(noexcept(::up::iter_swap(&::up::declval<T&>(), &::up::declval<T&>()))) {
        for (size_t i = 0; i < N; ++i) {
            ::up::iter_swap(&a[i], &b[i]);
        }
    }

    namespace detail
    {
        namespace is_swappable_impl
        {
            using ::up::swap;
            nat_t swap(any_t, any_t);

            template <class T, bool = is_const<T>::value || is_reference<T>::value>
            struct UPHIDDEN result
            {
                typedef decltype(swap(::up::declval<T&>(), ::up::declval<T&>())) type;
                static constexpr bool value = !is_same<type, nat_t>::value;
            };

            template <class T>
            struct UPHIDDEN result<T, true> : false_type { };
        }
        
        template <class T, bool>
        struct UPHIDDEN is_nothrow_swappable_impl : false_type { };

#ifndef UP_NO_NOEXCEPT
        template <class T>
        struct UPHIDDEN is_nothrow_swappable_impl<T, true> : integral_constant<bool, noexcept(swap(::up::declval<T&>(), ::up::declval<T&>()))> { };
#endif
    }
    
    template <class T>
    struct UPVISIBLE is_swappable : integral_constant<bool, detail::is_swappable_impl::result<T>::value> { };

    template <class T>
    struct UPVISIBLE is_nothrow_swappable : integral_constant<bool, detail::is_nothrow_swappable_impl<T, is_swappable<T>::value>::value> { };

    template <class T>
    inline UPALWAYSINLINE UPPURE
    T const& min(T const& x, T const& y) noexcept {
        return (y < x) ? y : x;
    }

    template <class T, class Compare>
    inline UPALWAYSINLINE UPPURE
    T const& min(T const& x, T const& y, Compare comp) noexcept {
        return comp(y, x) ? y : x;
    }

    template <class T>
    inline UPALWAYSINLINE UPPURE
    T const& max(T const& x, T const& y) noexcept {
        return (x < y) ? y : x;
    }

    template <class T, class Compare>
    inline UPALWAYSINLINE UPPURE
    T const& max(T const& x, T const& y, Compare comp) noexcept {
        return comp(x, y) ? y : x;
    }
}

#endif
