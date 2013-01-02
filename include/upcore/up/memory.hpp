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

#ifndef UP_MEMORY_HPP
#define UP_MEMORY_HPP

#include <up/cstdalign.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/utility.hpp>
#include <new>

namespace up { namespace detail
{
    template
    <
        class U1,
        class U2,
        bool Same = ::std::is_same<U1, U2>::value,
        bool Empty1 = ::std::is_empty<U1>::value,
        bool Empty2 = ::std::is_empty<U2>::value
    >
    class compressed_pair_wrapper
    {
        U1 u1;
        U2 u2;

    public:

        UPDEFAULTCTOR(compressed_pair_wrapper);
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x, int = 0) : u1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x, int = 0) : u1(::up::move(x)) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y, int* = 0) : u2(y) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y, int* = 0) : u2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : u1(x), u2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : u1(::up::forward<U1>(x)), u2(::up::forward<U2>(y)) { }
        UPALWAYSINLINE U1& first() { return u1; }
        UPALWAYSINLINE U1 const& first() const { return u1; }
        UPALWAYSINLINE void first(U1 const& x) { u1 = x; }
        UPALWAYSINLINE void first(U1&& x) { u1 = ::up::move(x); }
        UPALWAYSINLINE U2& second() { return u2; }
        UPALWAYSINLINE U2 const& second() const { return u2; }
        UPALWAYSINLINE void second(U2 const& x) { u2 = x; }
        UPALWAYSINLINE void second(U2&& x) { u2 = ::up::move(x); }
    };

    template <class U1, class U2, bool Same>
    class compressed_pair_wrapper<U1, U2, Same, false, true> : public U2
    {
        U1 u1;
            
    public:

        UPDEFAULTCTOR(compressed_pair_wrapper);
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x, int = 0) : u1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x, int = 0) : u1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y, int* = 0) : U2(y) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y, int* = 0) : U2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U2(y), u1(x) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U2(::up::forward<U2>(y)), u1(::up::forward<U1>(x)) { }
        UPALWAYSINLINE U1& first() { return u1; }
        UPALWAYSINLINE U1 const& first() const { return u1; }
        UPALWAYSINLINE void first(U1 const& x) { u1 = x; }
        UPALWAYSINLINE void first(U1&& x) { u1 = ::up::move(x); }
        UPALWAYSINLINE U2& second() { return *static_cast<U2*>(this); }
        UPALWAYSINLINE U2 const& second() const { return *static_cast<U2 const*>(this); }
        UPALWAYSINLINE void second(U2 const& x) { *static_cast<U2*>(this) = x; }
        UPALWAYSINLINE void second(U2&& x) { *static_cast<U2*>(this) = ::up::move(x); }
    };

    template <class U1, class U2, bool Same>
    class compressed_pair_wrapper<U1, U2, Same, true, false> : public U1
    {
        U2 u2;

    public:

        UPDEFAULTCTOR(compressed_pair_wrapper);
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x, int = 0) : U1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x, int = 0) : U1(::up::move(x)) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y, int* = 0) : u2(y) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y, int* = 0) : u2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U1(x), u2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U1(::up::forward<U1>(x)), u2(::up::forward<U2>(y)) { }
        UPALWAYSINLINE U1& first() { return *static_cast<U1*>(this); }
        UPALWAYSINLINE U1 const& first() const { return *static_cast<U1 const*>(this); }
        UPALWAYSINLINE void first(U1 const& x) { *static_cast<U1*>(this) = x; }
        UPALWAYSINLINE void first(U1&& x) { *static_cast<U1*>(this) = ::up::move(x); }
        UPALWAYSINLINE U2& second() { return u2; }
        UPALWAYSINLINE U2 const& second() const { return u2; }
        UPALWAYSINLINE void second(U2 const& x) { u2 = x; }
        UPALWAYSINLINE void second(U2&& x) { u2 = ::up::move(x); }
    };

    template <class U1, class U2>
    class compressed_pair_wrapper<U1, U2, false, true, true> : public U1, public U2
    {
    public:

        UPDEFAULTCTOR(compressed_pair_wrapper);
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x) : U1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x) : U1(::up::move(x)) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y) : U2(y) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y) : U2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U1(x), U2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U1(::up::forward<U1>(x)), U2(::up::forward<U2>(y)) { }
        UPALWAYSINLINE U1& first() { return *static_cast<U1*>(this); }
        UPALWAYSINLINE U1 const& first() const { return *static_cast<U1 const*>(this); }
        UPALWAYSINLINE void first(U1 const& x) { *static_cast<U1*>(this) = x; }
        UPALWAYSINLINE void first(U1&& x) { *static_cast<U1*>(this) = ::up::move(x); }
        UPALWAYSINLINE U2& second() { return *static_cast<U2*>(this); }
        UPALWAYSINLINE U2 const& second() const { return *static_cast<U2 const*>(this); }
        UPALWAYSINLINE void second(U2 const& x) { *static_cast<U2*>(this) = x; }
        UPALWAYSINLINE void second(U2&& x) { *static_cast<U2*>(this) = ::up::move(x); }
    };

    template <class U1, class U2>
    class compressed_pair_wrapper<U1, U2, true, true, true> : public U1
    {
    public:

        UPALWAYSINLINE compressed_pair_wrapper() { ::new(static_cast<U2*>(this)) U2; }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x) : U1(x) { ::new(static_cast<U2*>(this)) U2; }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x) : U1(::up::move(x)) { ::new(static_cast<U2*>(this)) U2; }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U1(x) { ::new(static_cast<U2*>(this)) U2(y); }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U1(::up::forward<U1>(x)) { ::new(static_cast<U2*>(this)) U2(::up::forward<U2>(y)); }
        UPALWAYSINLINE ~compressed_pair_wrapper() { static_cast<U2*>(this)->~U2(); }
        UPALWAYSINLINE U1& first() { return *static_cast<U1*>(this); }
        UPALWAYSINLINE U1 const& first() const { return *static_cast<U1 const*>(this); }
        UPALWAYSINLINE void first(U1 const& x) { *static_cast<U1*>(this) = x; }
        UPALWAYSINLINE void first(U1&& x) { *static_cast<U1*>(this) = ::up::move(x); }
        UPALWAYSINLINE U2& second() { return *static_cast<U2*>(this); }
        UPALWAYSINLINE U2 const& second() const { return *static_cast<U2 const*>(this); }
        UPALWAYSINLINE void second(U2 const& x) { *static_cast<U2*>(this) = x; }
        UPALWAYSINLINE void second(U2&& x) { *static_cast<U2*>(this) = ::up::move(x); }
    };
}}

namespace up
{
    template <class T1, class T2>
    class UPVISIBLE compressed_pair : public detail::compressed_pair_wrapper<T1, T2>
    {
    public:

        typedef T1 first_type;
        typedef T2 second_type;

        UPDEFAULTCTOR(compressed_pair);

        UPALWAYSINLINE
        explicit compressed_pair(T1 const& x, int = 0)
        : detail::compressed_pair_wrapper<T1, T2>(x) {
        }

        UPALWAYSINLINE
        explicit compressed_pair(T2 const& y, int* = 0)
        : detail::compressed_pair_wrapper<T1, T2>(y) {
        }

        UPALWAYSINLINE
        compressed_pair(T1 const& x, T2 const& y)
        : detail::compressed_pair_wrapper<T1, T2>(x, y) {
        }

        UPALWAYSINLINE
        compressed_pair(compressed_pair const& p)
        : detail::compressed_pair_wrapper<T1, T2>(p.first(), p.second()) {
        }

        template <class U, class V>
        UPALWAYSINLINE
        compressed_pair(compressed_pair<U, V> const& p)
        : detail::compressed_pair_wrapper<T1, T2>(p.first(), p.second()) {
        }

        UPALWAYSINLINE
        compressed_pair& operator=(compressed_pair const& p) {
            this->first() = p.first();
            this->second() = p.second();
            return *this;
        }

        UPALWAYSINLINE
        explicit compressed_pair(T1&& x, int = 0)
        : detail::compressed_pair_wrapper<T1, T2>(::up::forward<T1>(x)) {
        }
        
        UPALWAYSINLINE
        explicit compressed_pair(T2&& y, int* = 0)
        : detail::compressed_pair_wrapper<T1, T2>(::up::forward<T2>(y)) {
        }

        UPALWAYSINLINE
        compressed_pair(T1&& x, T2&& y)
        : detail::compressed_pair_wrapper<T1, T2>(::up::forward<T1>(x), ::up::forward<T2>(y)) {
        }

        UPALWAYSINLINE
        compressed_pair(compressed_pair&& p)
        : detail::compressed_pair_wrapper<T1, T2>(::up::move(p.first()), ::up::move(p.second())) {
        }

        template <class U, class V>
        UPALWAYSINLINE
        compressed_pair(compressed_pair<U, V>&& p)
        : detail::compressed_pair_wrapper<T1, T2>(::up::move(p.first()), ::up::move(p.second())) {
        }

        UPALWAYSINLINE
        compressed_pair& operator=(compressed_pair&& p) {
            this->first() = ::up::move(p.first());
            this->second() = ::up::move(p.second());
            return *this;
        }

        template <class U, class V>
        UPALWAYSINLINE
        compressed_pair& operator=(compressed_pair<U, V>&& p) {
            this->first() = ::up::move(p.first());
            this->second() = ::up::move(p.second());
            return *this;
        }

        UPALWAYSINLINE
        void swap(compressed_pair& x) {
            ::up::iter_swap(&this->first(), &x.first());
            ::up::iter_swap(&this->second(), &x.second());
        }
    };

    template <class T1, class T2>
    inline UPALWAYSINLINE
    compressed_pair<T1, T2> make_compressed_pair(T1&& x, T2&& y) {
        return compressed_pair<T1, T2>(::up::forward<T1>(x), ::up::forward<T2>(y));
    }

    template <class T1, class T2>
    inline UPALWAYSINLINE
    void swap(compressed_pair<T1, T2>& x, compressed_pair<T1, T2>& y) {
        x.swap(y);
    }

    template <class T1, class T2>
    inline UPALWAYSINLINE
    bool operator==(compressed_pair<T1, T2> const& x, compressed_pair<T1, T2> const& y) {
        return (x.first() == y.first()) && (x.second() == y.second());
    }

    template <class T1, class T2>
    inline UPALWAYSINLINE
    bool operator!=(compressed_pair<T1, T2> const& x, compressed_pair<T1, T2> const& y) {
        return (x.first() != y.first()) || (x.second() != y.second());
    }
}

namespace up { namespace detail
{
    template <class Iterator> struct iterator_value_type { typedef typename Iterator::value_type type; };
    template <class T> struct iterator_value_type<T*> { typedef typename std::remove_cv<T>::type type; };
    template <class T> struct is_destroyable : std::integral_constant<bool, !std::is_trivially_destructible<T>::value && !std::is_void<T>::value> { };
    template <class T, class R> struct enable_if_destroyable : std::enable_if<is_destroyable<T>::value, R> { };
    template <class T, class R> struct enable_if_not_destroyable : std::enable_if<!is_destroyable<T>::value, R> { };

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void destruct(ForwardIterator, std::false_type) noexcept {
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void destruct(ForwardIterator position, std::true_type) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        position->~value_type();
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator destruct(ForwardIterator, ForwardIterator last, std::false_type) noexcept {
        return last;
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator destruct(ForwardIterator first, ForwardIterator last, std::true_type) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        for ( ; first != last; ++first) {
            first->~value_type();
        }
        return first;
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator destruct_n(ForwardIterator first, Size n, std::false_type) noexcept {
        return first + n;
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator destruct_n(ForwardIterator first, Size n, std::true_type) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        for ( ; n > 0; ++first, --n) {
            first->~value_type();
        }
        return first;
    }
}}

namespace up
{
    template <class T>
    inline UPALWAYSINLINE UPPURE
    T* addressof(T& value) noexcept {
        return (T*)(&((char&)value));
    }
    
    template <class T>
    inline UPALWAYSINLINE UPPURE
    T** addressof(T*& value) noexcept {
        return (T**)((char*)(&value));
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location) {
        ::new(::up::addressof(*location)) typename detail::iterator_value_type<ForwardIterator>::type;
    }

#ifndef UP_NO_VARIADIC_TEMPLATES

    template <class ForwardIterator, class... Args>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, Args&&... args) {
        ::new(::up::addressof(*location)) typename detail::iterator_value_type<ForwardIterator>::type(::up::forward<Args>(args)...);
    }

#else

    template <class ForwardIterator, class A1>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, A1&& a1) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type(::up::forward<A1>(a1));
    }

    template <class ForwardIterator, class A1, class A2>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, A1&& a1, A2&& a2) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type(
            ::up::forward<A1>(a1),
            ::up::forward<A2>(a2)
        );
    }

    template <class ForwardIterator, class A1, class A2, class A3>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, A1&& a1, A2&& a2, A3&& a3) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type(
            ::up::forward<A1>(a1),
            ::up::forward<A2>(a2),
            ::up::forward<A3>(a3)
        );
    }

    template <class ForwardIterator, class A1, class A2, class A3, class A4>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, A1&& a1, A2&& a2, A3&& a3, A4&& a4) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type(
            ::up::forward<A1>(a1),
            ::up::forward<A2>(a2),
            ::up::forward<A3>(a3),
            ::up::forward<A4>(a4)
        );
    }

    template <class ForwardIterator, class A1, class A2, class A3, class A4, class A5>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type(
            ::up::forward<A1>(a1),
            ::up::forward<A2>(a2),
            ::up::forward<A3>(a3),
            ::up::forward<A4>(a4),
            ::up::forward<A5>(a5)
        );
    }

#endif

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void destruct(ForwardIterator location) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::up::detail::destruct(location, detail::is_destroyable<value_type>());
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator destruct(ForwardIterator first, ForwardIterator last) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        return ::up::detail::destruct(first, last, detail::is_destroyable<value_type>());
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator destruct_n(ForwardIterator first, Size n) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        return ::up::detail::destruct_n(first, n, detail::is_destroyable<value_type>());
    }
}

namespace up { namespace detail
{
    template <class OutputIterator>
    struct is_contiguously_constructible_impl
    {
        typedef typename std::conditional
        <
            std::is_pointer<OutputIterator>::value
            && std::is_trivial<typename std::remove_pointer<OutputIterator>::type>::value,
            std::true_type,
            std::false_type
        >
        ::type result;
    };

    template <class OutputIterator>
    struct is_contiguously_constructible : std::integral_constant<bool, is_contiguously_constructible_impl<OutputIterator>::result::value> { };

    template <class InputIterator, class OutputIterator>
    struct is_contiguously_copyable_impl
    {
        typedef typename std::remove_pointer<InputIterator>::type input_type;
        typedef typename std::remove_pointer<OutputIterator>::type output_type;
        typedef typename std::conditional
        <
            std::is_pointer<InputIterator>::value
            && std::is_pointer<OutputIterator>::value
            && std::is_same<input_type, output_type>::value
            && std::is_trivially_copyable<input_type>::value,
            std::true_type,
            std::false_type
        >
        ::type result;
    };

    template <class InputIterator, class OutputIterator>
    struct is_contiguously_copyable : std::integral_constant<bool, is_contiguously_copyable_impl<InputIterator, OutputIterator>::result::value> { };

    template <class OutputIterator, class InputType>
    struct is_contiguously_fillable_impl
    {
        typedef typename std::conditional
        <
            std::is_pointer<OutputIterator>::value
            && std::is_same<InputType, typename std::remove_pointer<OutputIterator>::type>::value
            && std::is_trivially_copyable<InputType>::value,
            std::true_type,
            std::false_type
        >
        ::type result;
    };

    template <class OutputIterator, class InputType>
    struct is_contiguously_fillable : std::integral_constant<bool, is_contiguously_fillable_impl<OutputIterator, InputType>::result::value> { };

    template <class ContiguousIterator>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_construct(ContiguousIterator first, ContiguousIterator last, std::true_type) noexcept {
        ::up::memset(first, 0, last - first * sizeof(*first));
        return last;
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_construct(ForwardIterator first, ForwardIterator last, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(first);
            
        UPTRY {        
            for ( ; cur < last; ++cur) {
                ::new(::up::addressof(*cur)) value_type();
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(first, cur));

        return cur;
    }

    template <class ContiguousIterator, class Size>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_construct_n(ContiguousIterator first, Size n, std::true_type) noexcept {
        ::up::memset(first, 0, n * sizeof(*first));
        return first + n;
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_construct_n(ForwardIterator first, Size n, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(first);
            
        UPTRY {
            for ( ; n > 0; ++cur, --n) {
                ::new(::up::addressof(*cur)) value_type();
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(first, cur));

        return cur;
    }

    template <class ContiguousIterator1, class ContiguousIterator2>
    inline UPALWAYSINLINE
    ContiguousIterator2
    uninitialized_copy(ContiguousIterator1 first, ContiguousIterator1 last, ContiguousIterator2 result, std::true_type) noexcept {
        ptrdiff_t const n = last - first;
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(result);
            
        UPTRY {
            for ( ; first != last; ++cur, ++first) {
                ::new(::up::addressof(*cur)) value_type(*first);
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(result, cur));

        return cur;
    }

    template <class ContiguousIterator1, class ContiguousIterator2>
    inline UPALWAYSINLINE
    ContiguousIterator2
    uninitialized_copy_backward(
        ContiguousIterator1 first,
        ContiguousIterator1 last,
        ContiguousIterator2 result,
        std::true_type
    )
    noexcept {
        ptrdiff_t const n = last - first;
        ContiguousIterator2 const result0 = result - n;
        ::up::memmove(result0, first, n * sizeof(*result0));
        return result0;
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline UPALWAYSINLINE
    BidirectionalIterator2
    uninitialized_copy_backward(
        BidirectionalIterator1 first,
        BidirectionalIterator1 last,
        BidirectionalIterator2 result,
        std::false_type
    ) {
        typedef typename iterator_value_type<BidirectionalIterator2>::type value_type;
        BidirectionalIterator2 cur(result);
            
        UPTRY {
            while (first != last) {
                ::new(::up::addressof(*(--cur))) value_type(*(--last));
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(++cur, result));
            
        return cur;
    }

    template <class ContiguousIterator1, class Size, class ContiguousIterator2>
    inline UPALWAYSINLINE
    ContiguousIterator2
    uninitialized_copy_n(ContiguousIterator1 first, Size n, ContiguousIterator2 result, std::true_type) noexcept {
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class Size, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_copy_n(InputIterator first, Size n, ForwardIterator result, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(result);
            
        UPTRY {
            for ( ; n > 0; ++cur, ++first, --n) {
                ::new(::up::addressof(*cur)) value_type(*first);
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(result, cur));
            
        return cur;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, std::true_type, std::integral_constant<size_t, 1>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        union { value_type value; uint8_t byte; } u = { static_cast<value_type>(value) };
        ::up::memset(first, u.byte, n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, std::true_type, std::integral_constant<size_t, 2>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        value_type actual_value = static_cast<value_type>(value);
        ::up::memset_pattern2(first, ::up::addressof(actual_value), n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, std::true_type, std::integral_constant<size_t, 4>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        value_type actual_value = static_cast<value_type>(value);
        ::up::memset_pattern4(first, ::up::addressof(actual_value), n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, std::true_type, std::integral_constant<size_t, 8>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        value_type actual_value = static_cast<value_type>(value);
        ::up::memset_pattern8(first, ::up::addressof(actual_value), n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T, class Length, class Aligned>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T const& value, Length, Aligned) {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        ContiguousIterator cur(first);
            
        UPTRY {
            for ( ; n > 0; ++cur, --n) {
                ::new(::up::addressof(*cur)) value_type(value);
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(first, cur));

        return cur;
    }

    template <class ContiguousIterator, class T>
    inline UPALWAYSINLINE
    void uninitialized_fill(ContiguousIterator first, ContiguousIterator last, T const& value, std::true_type) {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        typedef std::integral_constant<size_t, sizeof(value_type)> value_size;
        typedef std::integral_constant<bool, !(alignof(value_type) % sizeof(value_type))> is_aligned;
        ::up::detail::uninitialized_fill_n(first, last - first, value, is_aligned(), value_size());
    }

    template <class ForwardIterator, class T>
    inline UPALWAYSINLINE
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, T const& value, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(first);
        
        UPTRY {
            for ( ; cur != last; ++cur) {
                ::new(::up::addressof(*cur)) value_type(value);
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(first, cur));
    }
        
    template <class ContiguousIterator1, class ContiguousIterator2>
    inline UPALWAYSINLINE
    ContiguousIterator2
    uninitialized_move(ContiguousIterator1 first, ContiguousIterator1 last, ContiguousIterator2 result, std::true_type) noexcept {
        ptrdiff_t const n = last - first;
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(result);
        UPTRY {
            for ( ; first != last; ++cur, ++first) {
                ::new(::up::addressof(*cur)) value_type(::up::move(*first));
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(result, cur));
        return cur;
    }
        
    template <class ContiguousIterator1, class ContiguousIterator2>
    inline UPALWAYSINLINE
    ContiguousIterator2
    uninitialized_move_backward(
        ContiguousIterator1 first,
        ContiguousIterator1 last,
        ContiguousIterator2 result,
        std::true_type
    )
    noexcept {
        ptrdiff_t const n = last - first;
        ContiguousIterator2 const result0 = result - n;
        ::up::memmove(result0, first, n * sizeof(*result0));
        return result0;
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline UPALWAYSINLINE
    BidirectionalIterator2
    uninitialized_move_backward(
        BidirectionalIterator1 first,
        BidirectionalIterator1 last,
        BidirectionalIterator2 result,
        std::false_type
    ) {
        typedef typename iterator_value_type<BidirectionalIterator2>::type value_type;
        BidirectionalIterator2 cur(result);
        UPTRY {
            while (first != last) {
                ::new(::up::addressof(*(--cur))) value_type(::up::move(*(--last)));
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(++cur, result));
        return cur;
    }
        
    template <class ContiguousIterator1, class Size, class ContiguousIterator2>
    inline UPALWAYSINLINE
    ContiguousIterator2
    uninitialized_move_n(ContiguousIterator1 first, Size n, ContiguousIterator2 result, std::true_type) noexcept {
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class Size, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_move_n(InputIterator first, Size n, ForwardIterator result, std::false_type) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        ForwardIterator cur(result);
        UPTRY {        
            for ( ; n > 0; ++cur, ++first, --n) {
                ::new(::up::addressof(*cur)) value_type(::up::move(*first));
            }
        }
        UPCATCHANY_RETHROW(::up::destruct(result, cur));
        return cur;
    }
}}

namespace up
{
    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_construct(ForwardIterator first, ForwardIterator last) {
        typedef detail::is_contiguously_constructible<ForwardIterator> contiguously_constructible;
        return ::up::detail::uninitialized_construct(first, last, contiguously_constructible());
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_construct_n(ForwardIterator first, Size n) {
        typedef detail::is_contiguously_constructible<ForwardIterator> contiguously_constructible;
        return ::up::detail::uninitialized_construct_n(first, n, contiguously_constructible());
    }

    template <class InputIterator, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        typedef detail::is_contiguously_copyable<InputIterator, ForwardIterator> contiguously_copyable;
        return ::up::detail::uninitialized_copy(first, last, result, contiguously_copyable());
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline UPALWAYSINLINE
    BidirectionalIterator2
    uninitialized_copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        typedef detail::is_contiguously_copyable<BidirectionalIterator1, BidirectionalIterator2> contiguously_copyable;
        return ::up::detail::uninitialized_copy_backward(first, last, result, contiguously_copyable());
    }

    template <class InputIterator, class Size, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_copy_n(InputIterator first, Size n, ForwardIterator result) {
        typedef detail::is_contiguously_copyable<InputIterator, ForwardIterator> contiguously_copyable;
        return ::up::detail::uninitialized_copy_n(first, n, result, contiguously_copyable());
    }

    template <class ForwardIterator, class T>
    inline UPALWAYSINLINE
    void
    uninitialized_fill(ForwardIterator first, ForwardIterator last, T const& value) {
        typedef detail::is_contiguously_fillable<ForwardIterator, T> contiguously_fillable;
        ::up::detail::uninitialized_fill(first, last, value, contiguously_fillable());
    }
    
    template <class ForwardIterator, class Size, class T>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_fill_n(ForwardIterator first, Size n, T const& value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        typedef std::integral_constant<size_t, sizeof(value_type)> value_size;
        typedef detail::is_contiguously_fillable<ForwardIterator, T> contiguously_fillable;
        typedef std::integral_constant<bool, (contiguously_fillable::value && !(alignof(value_type) % sizeof(value_type)))> contiguously_fillable_and_aligned;
        return ::up::detail::uninitialized_fill_n(first, n, value, contiguously_fillable_and_aligned(), value_size());
    }
    
    template <class InputIterator, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result) {
        typedef detail::is_contiguously_copyable<InputIterator, ForwardIterator> contiguously_copyable;
        return ::up::detail::uninitialized_move(first, last, result, contiguously_copyable());
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline UPALWAYSINLINE
    BidirectionalIterator2
    uninitialized_move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        typedef detail::is_contiguously_copyable<BidirectionalIterator1, BidirectionalIterator2> contiguously_copyable;
        return ::up::detail::uninitialized_move_backward(first, last, result, contiguously_copyable());
    }
    
    template <class InputIterator, class Size, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_move_n(InputIterator first, Size n, ForwardIterator result) {
        typedef detail::is_contiguously_copyable<InputIterator, ForwardIterator> contiguously_copyable;
        return ::detail::uninitialized_move_n(first, n, result, contiguously_copyable());
    }
}

namespace up { namespace detail
{
    template <class T> struct deallocate_sizeof : std::integral_constant<size_t, sizeof(T)> { };
    template <> struct deallocate_sizeof<void> : std::integral_constant<size_t, 0> { };
    template <> struct deallocate_sizeof<void const> : std::integral_constant<size_t, 0> { };
    template <> struct deallocate_sizeof<void volatile> : std::integral_constant<size_t, 0> { };
    template <> struct deallocate_sizeof<void const volatile> : std::integral_constant<size_t, 0> { };
}}

namespace up
{
    template <class T> struct is_allocator : std::false_type { };
    template <class T> struct is_allocator<T const> : std::integral_constant<bool, is_allocator<T>::value> { };
    template <class T> struct is_allocator<T volatile> : std::integral_constant<bool, is_allocator<T>::value> { };
    template <class T> struct is_allocator<T const volatile> : std::integral_constant<bool, is_allocator<T>::value> { };
    template <class T> struct is_allocator<T&> : std::integral_constant<bool, is_allocator<T>::value> { };
    template <class T> struct is_allocator<T&&> : std::integral_constant<bool, is_allocator<T>::value> { };
    template <class T> struct is_allocator<T*> : std::integral_constant<bool, is_allocator<T>::value> { };
    template <> struct is_allocator<allocator> : std::true_type { };
    template <> struct is_allocator<page_allocator> : std::true_type { };
    template <> struct is_allocator<malloc_allocator> : std::true_type { };

    template <class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    void* allocate(Alloc* a, size_t n) noexcept {
#ifdef UP_NO_EXCEPTIONS
        return a->allocate(n);
#else
        return a->allocate_throw(n);
#endif
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate(Alloc* a) {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(a->allocate(sizeof(T)));
#else
        return static_cast<T*>(a->allocate_throw(sizeof(T)));
#endif
    }

    template <class Alloc>
    inline UPALWAYSINLINE
    void deallocate(Alloc* a, void* p, size_t n) noexcept {
        a->deallocate(p, n);
    }

    template <class Alloc>
    inline UPALWAYSINLINE
    void deallocate(Alloc* a, void const* p, size_t n) noexcept {
        a->deallocate(const_cast<void*>(p), n);
    }

    template <class Alloc>
    inline UPALWAYSINLINE
    void deallocate(Alloc* a, void volatile* p, size_t n) noexcept {
        a->deallocate(const_cast<void*>(p), n);
    }

    template <class Alloc>
    inline UPALWAYSINLINE
    void deallocate(Alloc* a, void const volatile* p, size_t n) noexcept {
        a->deallocate(const_cast<void*>(p), n);
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void deallocate(Alloc* a, T* p) noexcept {
        a->deallocate(const_cast<typename std::remove_cv<T>::type*>(p), ::up::detail::deallocate_sizeof<T>::value);
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, typename std::enable_if<std::is_trivial<T>::value, nat_t>::type = nat_t()) {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(a->allocate_zero(1, sizeof(T)));
#else
        return static_cast<T*>(a->allocate_zero_throw(1, sizeof(T)));
#endif
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, typename std::enable_if<!std::is_trivial<T>::value, nat_t>::type = nat_t()) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T;
    }

#ifndef UP_NO_VARIADIC_TEMPLATES

    template <class T, class Alloc, class... Args>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, Args&&... args) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<Args>(args)...);
    }

#else

    template <class T, class Alloc, class A1>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, A1&& a1) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1));
    }

    template <class T, class Alloc, class A1, class A2>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, A1&& a1, A2&& a2) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2));
    }

    template <class T, class Alloc, class A1, class A2, class A3>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, A1&& a1, A2&& a2, A3&& a3) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3));
    }

    template <class T, class Alloc, class A1, class A2, class A3, class A4>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, A1&& a1, A2&& a2, A3&& a3, A4&& a4) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3), ::up::forward<A4>(a4));
    }

    template <class T, class Alloc, class A1, class A2, class A3, class A4, class A5>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = a->allocate_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3), ::up::forward<A4>(a4), ::up::forward<A5>(a5));
    }

#endif

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void destruct_deallocate(Alloc* a, T* p, typename detail::enable_if_not_destroyable<T, nat_t>::type = nat_t()) noexcept {
        a->deallocate(const_cast<typename std::remove_cv<T>::type*>(p), sizeof(T));
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void destruct_deallocate(Alloc* a, T* p, typename detail::enable_if_destroyable<T, nat_t>::type = nat_t())
    UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        if (p) {
            typename std::remove_cv<T>::type* q = const_cast<typename std::remove_cv<T>::type*>(p);
            q->~T();
            a->deallocate(q, sizeof(T));
        }
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_n(Alloc* a, size_t n) {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(a->allocate(n * sizeof(T)));
#else
        return static_cast<T*>(a->allocate_throw(n * sizeof(T)));
#endif
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void deallocate_n(Alloc* a, T* p, size_t n) noexcept {
        a->deallocate(const_cast<typename std::remove_cv<T>::type*>(p), n * sizeof(T));
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct_n(Alloc* a, typename std::enable_if<std::is_trivial<T>::value, size_t>::type n) noexcept {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(a->allocate_zero(n, sizeof(T)));
#else
        return static_cast<T*>(a->allocate_zero_throw(n, sizeof(T)));
#endif
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct_n(Alloc* a, typename std::enable_if<!std::is_trivial<T>::value, size_t>::type n) {
#ifdef UP_NO_EXCEPTIONS
        T* const p = static_cast<T*>(a->allocate(n * sizeof(T)));
        if (!p) {
            return nullptr;
        }
#else
        T* const p = static_cast<T*>(a->allocate_throw(n * sizeof(T)));
#endif
        ::up::uninitialized_construct_n(p, n);
        return p;
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void destruct_deallocate_n(Alloc* a, T* p, size_t n) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typename std::remove_cv<T>::type* q = const_cast<typename std::remove_cv<T>::type*>(p);
        ::up::destruct_n(q, n);
        a->deallocate(q, n * sizeof(T));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc() {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(::up::malloc(sizeof(T)));
#else
        return static_cast<T*>(::up::malloc_throw(sizeof(T)));
#endif
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(typename std::enable_if<std::is_trivial<T>::value, nat_t>::type = nat_t()) {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(::up::calloc(1, sizeof(T)));
#else
        return static_cast<T*>(::up::calloc_throw(1, sizeof(T)));
#endif
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(typename std::enable_if<!std::is_trivial<T>::value, nat_t>::type = nat_t()) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = ::up::malloc_throw(sizeof(T));
#endif
        return ::new(p) T;
    }

#ifndef UP_NO_VARIADIC_TEMPLATES

    template <class T, class... Args>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(Args&&... args) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = ::up::malloc_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<Args>(args)...);
    }

#else

    template <class T, class A1>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(A1&& a1) {
#ifdef UP_NO_EXCEPTIONS
        void* const p  = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p  = ::up::malloc_throw(sizeof(T));

#endif
        return ::new(p) T(::up::forward<A1>(a1));
    }

    template <class T, class A1, class A2>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(A1&& a1, A2&& a2) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = ::up::malloc_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2));
    }

    template <class T, class A1, class A2, class A3>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(A1&& a1, A2&& a2, A3&& a3) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = ::up::malloc_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3));
    }

    template <class T, class A1, class A2, class A3, class A4>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(A1&& a1, A2&& a2, A3&& a3, A4&& a4) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = ::up::malloc_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3), ::up::forward<A4>(a4));
    }

    template <class T, class A1, class A2, class A3, class A4, class A5>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5) {
#ifdef UP_NO_EXCEPTIONS
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
#else
        void* const p = ::up::malloc_throw(sizeof(T));
#endif
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3), ::up::forward<A4>(a4), ::up::forward<A5>(a5));
    }

#endif

    template <class T>
    inline UPALWAYSINLINE
    void destruct_free(T* p, typename detail::enable_if_not_destroyable<T, nat_t>::type = nat_t()) noexcept {
        ::up::free(const_cast<typename std::remove_cv<T>::type*>(p));
    }

    template <class T>
    inline UPALWAYSINLINE
    void destruct_free(T* p, typename detail::enable_if_destroyable<T, nat_t>::type = nat_t())
    UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        if (p) {
            typename std::remove_cv<T>::type* q = const_cast<typename std::remove_cv<T>::type*>(p);
            q->~T();
            ::up::free(q);
        }
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_n(size_t n) {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(::up::malloc(n * sizeof(T)));
#else
        return static_cast<T*>(::up::malloc_throw(n * sizeof(T)));
#endif
    }

    template <class T>
    inline UPALWAYSINLINE
    void free_n(T* p, size_t) noexcept {
        ::up::free(const_cast<typename std::remove_cv<T>::type*>(p));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct_n(typename std::enable_if<std::is_trivial<T>::value, size_t>::type n) noexcept {
#ifdef UP_NO_EXCEPTIONS
        return static_cast<T*>(::up::calloc(n, sizeof(T)));
#else
        return static_cast<T*>(::up::calloc_throw(n, sizeof(T)));
#endif
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct_n(typename std::enable_if<!std::is_trivial<T>::value, size_t>::type n) {
#ifdef UP_NO_EXCEPTIONS
        T* const p = static_cast<T*>(::up::malloc(n * sizeof(T)));
        if (!p) {
            return nullptr;
        }
#else
        T* const p = static_cast<T*>(::up::malloc_throw(n * sizeof(T)));
#endif
        ::up::uninitialized_construct_n(p, n);
        return p;
    }

    template <class T>
    inline UPALWAYSINLINE
    void destruct_free_n(T* p, size_t n) UPNOEXCEPT(std::is_nothrow_destructible<T>::value) {
        typename std::remove_cv<T>::type* q = const_cast<typename std::remove_cv<T>::type*>(p);
        ::up::destruct_n(q, n);
        ::up::free(q);
    }
}

#endif
