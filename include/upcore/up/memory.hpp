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
        bool Same = is_same<U1, U2>::value,
        bool Empty1 = is_empty<U1>::value,
        bool Empty2 = is_empty<U2>::value
    >
    class compressed_pair_wrapper
    {
        U1 u1;
        U2 u2;

    public:

        UPALWAYSINLINE compressed_pair_wrapper() : u1(), u2() { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x, int = 0) : u1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y, int* = 0) : u2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : u1(x), u2(y) { }
        UPALWAYSINLINE U1& first() { return u1; }
        UPALWAYSINLINE U1 const& first() const { return u1; }
        UPALWAYSINLINE void first(U1 const& x) { u1 = x; }
        UPALWAYSINLINE U2& second() { return u2; }
        UPALWAYSINLINE U2 const& second() const { return u2; }
        UPALWAYSINLINE void second(U2 const& x) { u2 = x; }
#ifndef UP_NO_RVALUE_REFERENCES
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x, int = 0) : u1(::up::move(x)) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y, int* = 0) : u2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : u1(::up::forward<U1>(x)), u2(::up::forward<U2>(y)) { }
        UPALWAYSINLINE void first(U1&& x) { u1 = ::up::move(x); }
        UPALWAYSINLINE void second(U2&& x) { u2 = ::up::move(x); }
#endif
    };

    template <class U1, class U2, bool Same>
    class compressed_pair_wrapper<U1, U2, Same, false, true> : public U2
    {
        U1 u1;
            
    public:

        UPALWAYSINLINE compressed_pair_wrapper() : U2(), u1() { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x, int = 0) : u1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y, int* = 0) : U2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U2(y), u1(x) { }
        UPALWAYSINLINE U1& first() { return u1; }
        UPALWAYSINLINE U1 const& first() const { return u1; }
        UPALWAYSINLINE void first(U1 const& x) { u1 = x; }
        UPALWAYSINLINE U2& second() { return *static_cast<U2*>(this); }
        UPALWAYSINLINE U2 const& second() const { return *static_cast<U2 const*>(this); }
        UPALWAYSINLINE void second(U2 const& x) { *static_cast<U2*>(this) = x; }
#ifndef UP_NO_RVALUE_REFERENCES
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x, int = 0) : u1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y, int* = 0) : U2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U2(::up::forward<U2>(y)), u1(::up::forward<U1>(x)) { }
        UPALWAYSINLINE void first(U1&& x) { u1 = ::up::move(x); }
        UPALWAYSINLINE void second(U2&& x) { *static_cast<U2*>(this) = ::up::move(x); }
#endif
    };

    template <class U1, class U2, bool Same>
    class compressed_pair_wrapper<U1, U2, Same, true, false> : public U1
    {
        U2 u2;

    public:

        UPALWAYSINLINE compressed_pair_wrapper() : U1(), u2() { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x, int = 0) : U1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y, int* = 0) : u2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U1(x), u2(y) { }
        UPALWAYSINLINE U1& first() { return *static_cast<U1*>(this); }
        UPALWAYSINLINE U1 const& first() const { return *static_cast<U1 const*>(this); }
        UPALWAYSINLINE void first(U1 const& x) { *static_cast<U1*>(this) = x; }
        UPALWAYSINLINE U2& second() { return u2; }
        UPALWAYSINLINE U2 const& second() const { return u2; }
        UPALWAYSINLINE void second(U2 const& x) { u2 = x; }
#ifndef UP_NO_RVALUE_REFERENCES
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x, int = 0) : U1(::up::move(x)) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y, int* = 0) : u2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U1(::up::forward<U1>(x)), u2(::up::forward<U2>(y)) { }
        UPALWAYSINLINE void first(U1&& x) { *static_cast<U1*>(this) = ::up::move(x); }
        UPALWAYSINLINE void second(U2&& x) { u2 = ::up::move(x); }
#endif
    };

    template <class U1, class U2>
    class compressed_pair_wrapper<U1, U2, false, true, true> : public U1, public U2
    {
    public:

        UPALWAYSINLINE compressed_pair_wrapper() : U1(), U2() { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x) : U1(x) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2 const& y) : U2(y) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U1(x), U2(y) { }
        UPALWAYSINLINE U1& first() { return *static_cast<U1*>(this); }
        UPALWAYSINLINE U1 const& first() const { return *static_cast<U1 const*>(this); }
        UPALWAYSINLINE void first(U1 const& x) { *static_cast<U1*>(this) = x; }
        UPALWAYSINLINE U2& second() { return *static_cast<U2*>(this); }
        UPALWAYSINLINE U2 const& second() const { return *static_cast<U2 const*>(this); }
        UPALWAYSINLINE void second(U2 const& x) { *static_cast<U2*>(this) = x; }
#ifndef UP_NO_RVALUE_REFERENCES
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x) : U1(::up::move(x)) { }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U2&& y) : U2(::up::move(y)) { }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y) : U1(::up::forward<U1>(x)), U2(::up::forward<U2>(y)) { }
        UPALWAYSINLINE void first(U1&& x) { *static_cast<U1*>(this) = ::up::move(x); }
        UPALWAYSINLINE void second(U2&& x) { *static_cast<U2*>(this) = ::up::move(x); }
#endif
    };

    template <class U1, class U2>
    class compressed_pair_wrapper<U1, U2, true, true, true> : public U1
    {
    public:

        UPALWAYSINLINE compressed_pair_wrapper() : U1() { ::new(static_cast<U2*>(this)) U2; }
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1 const& x) : U1(x) { ::new(static_cast<U2*>(this)) U2; }
        UPALWAYSINLINE compressed_pair_wrapper(U1 const& x, U2 const& y) : U1(x) { ::new(static_cast<U2*>(this)) U2(y); }
        UPALWAYSINLINE ~compressed_pair_wrapper() { static_cast<U2*>(this)->~U2(); }
        UPALWAYSINLINE U1& first() { return *static_cast<U1*>(this); }
        UPALWAYSINLINE U1 const& first() const { return *static_cast<U1 const*>(this); }
        UPALWAYSINLINE void first(U1 const& x) { *static_cast<U1*>(this) = x; }
        UPALWAYSINLINE U2& second() { return *static_cast<U2*>(this); }
        UPALWAYSINLINE U2 const& second() const { return *static_cast<U2 const*>(this); }
        UPALWAYSINLINE void second(U2 const& x) { *static_cast<U2*>(this) = x; }
#ifndef UP_NO_RVALUE_REFERENCES
        UPALWAYSINLINE explicit compressed_pair_wrapper(U1&& x) : U1(::up::move(x)) { ::new(static_cast<U2*>(this)) U2; }
        UPALWAYSINLINE compressed_pair_wrapper(U1&& x, U2&& y)
            : U1(::up::forward<U1>(x)) { ::new(static_cast<U2*>(this)) U2(::up::forward<U2>(y)); }
        UPALWAYSINLINE void first(U1&& x) { *static_cast<U1*>(this) = ::up::move(x); }
        UPALWAYSINLINE void second(U2&& x) { *static_cast<U2*>(this) = ::up::move(x); }
#endif
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

#ifndef UP_NO_RVALUE_REFERENCES

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

#endif

        UPALWAYSINLINE
        void swap(compressed_pair& x) {
            ::up::iter_swap(&this->first(), &x.first());
            ::up::iter_swap(&this->second(), &x.second());
        }
    };

    template <class T1, class T2>
    inline UPALWAYSINLINE
#ifndef UP_NO_RVALUE_REFERENCES
    compressed_pair<T1, T2> make_compressed_pair(T1&& x, T2&& y) {
#else
    compressed_pair<T1, T2> make_compressed_pair(T1 const& x, T2 const& y) {
#endif    
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
    template <class T> struct is_destroyable
        : integral_constant<bool, !is_trivially_destructible<T>::value && !is_void<T>::value> { };
    
    template <class T, class R> struct enable_if_destroyable : enable_if<is_destroyable<T>::value, R> { };
    template <class T, class R> struct enable_if_not_destroyable : enable_if<!is_destroyable<T>::value, R> { };
    template <class Iterator> struct iterator_value_type { typedef typename Iterator::value_type type; };
    template <class T> struct iterator_value_type<T*> { typedef typename remove_cv<T>::type type; };
 
    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void destruct(ForwardIterator, false_type) noexcept {
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void destruct(ForwardIterator position, true_type)
    UPNOEXCEPT(is_nothrow_destructible<typename iterator_value_type<ForwardIterator>::type>::value) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        position->~value_type();
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator destruct(ForwardIterator, ForwardIterator last, false_type) noexcept {
        return last;
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator destruct(ForwardIterator first, ForwardIterator last, true_type)
    UPNOEXCEPT(is_nothrow_destructible<typename iterator_value_type<ForwardIterator>::type>::value) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
        for ( ; first != last; ++first) {
            first->~value_type();
        }
        return first;
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator destruct_n(ForwardIterator first, Size n, false_type) noexcept {
        return first + n;
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator destruct_n(ForwardIterator first, Size n, true_type)
    UPNOEXCEPT(is_nothrow_destructible<typename iterator_value_type<ForwardIterator>::type>::value) {
        typedef typename iterator_value_type<ForwardIterator>::type value_type;
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
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type;
    }

#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_RVALUE_REFERENCES)
    template <class ForwardIterator, class... Args>
    inline UPALWAYSINLINE
    void construct(ForwardIterator location, Args&&... args) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::new(::up::addressof(*location)) value_type(::up::forward<Args>(args)...);
    }
#else
#   define UP_DETAIL_DEFINE_CONSTRUCT(RVREF) \
    template <class ForwardIterator, class A1> \
    inline UPALWAYSINLINE \
    void construct(ForwardIterator location, A1 RVREF a1) { \
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type; \
        ::new(::up::addressof(*location)) value_type(::up::forward<A1>(a1)); \
    } \
    template <class ForwardIterator, class A1, class A2> \
    inline UPALWAYSINLINE \
    void construct(ForwardIterator location, A1 RVREF a1, A2 RVREF a2) { \
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type; \
        ::new(::up::addressof(*location)) value_type( \
            ::up::forward<A1>(a1), \
            ::up::forward<A2>(a2) \
        ); \
    } \
    template <class ForwardIterator, class A1, class A2, class A3> \
    inline UPALWAYSINLINE \
    void construct(ForwardIterator location, A1 RVREF a1, A2 RVREF a2, A3 RVREF a3) { \
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type; \
        ::new(::up::addressof(*location)) value_type( \
            ::up::forward<A1>(a1), \
            ::up::forward<A2>(a2), \
            ::up::forward<A3>(a3) \
        ); \
    } \
    template <class ForwardIterator, class A1, class A2, class A3, class A4> \
    inline UPALWAYSINLINE \
    void construct(ForwardIterator location, A1 RVREF a1, A2 RVREF a2, A3 RVREF a3, A4 RVREF a4) { \
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type; \
        ::new(::up::addressof(*location)) value_type( \
            ::up::forward<A1>(a1), \
            ::up::forward<A2>(a2), \
            ::up::forward<A3>(a3), \
            ::up::forward<A4>(a4) \
        ); \
    } \
    template <class ForwardIterator, class A1, class A2, class A3, class A4, class A5> \
    inline UPALWAYSINLINE \
    void construct(ForwardIterator location, A1 RVREF a1, A2 RVREF a2, A3 RVREF a3, A4 RVREF a4, A5 RVREF a5) { \
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type; \
        ::new(::up::addressof(*location)) value_type( \
            ::up::forward<A1>(a1), \
            ::up::forward<A2>(a2), \
            ::up::forward<A3>(a3), \
            ::up::forward<A4>(a4), \
            ::up::forward<A5>(a5) \
        ); \
    }
#   ifndef UP_NO_RVALUE_REFERENCES
    UP_DETAIL_DEFINE_CONSTRUCT(&&)
#   else
    UP_DETAIL_DEFINE_CONSTRUCT(const&)
#   endif
#   undef UP_DETAIL_DEFINE_CONSTRUCT
#endif

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    void destruct(ForwardIterator location)
    UPNOEXCEPT(is_nothrow_destructible<typename detail::iterator_value_type<ForwardIterator>::type>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        ::up::detail::destruct(location, detail::is_destroyable<value_type>());
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator destruct(ForwardIterator first, ForwardIterator last)
    UPNOEXCEPT(is_nothrow_destructible<typename detail::iterator_value_type<ForwardIterator>::type>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        return ::up::detail::destruct(first, last, detail::is_destroyable<value_type>());
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator destruct_n(ForwardIterator first, Size n)
    UPNOEXCEPT(is_nothrow_destructible<typename detail::iterator_value_type<ForwardIterator>::type>::value) {
        typedef typename detail::iterator_value_type<ForwardIterator>::type value_type;
        return ::up::detail::destruct_n(first, n, detail::is_destroyable<value_type>());
    }
}

namespace up { namespace detail
{
    template <class OutputIterator>
    struct is_contiguously_constructible_impl
    {
        typedef typename conditional
        <
            is_pointer<OutputIterator>::value
            && is_trivial<typename remove_pointer<OutputIterator>::type>::value,
            true_type,
            false_type
        >
        ::type result;
    };

    template <class OutputIterator>
    struct is_contiguously_constructible
        : integral_constant<bool, is_contiguously_constructible_impl<OutputIterator>::result::value> { };

    template <class InputIterator, class OutputIterator>
    struct is_contiguously_copyable_impl
    {
        typedef typename remove_pointer<InputIterator>::type input_type;
        typedef typename remove_pointer<OutputIterator>::type output_type;
        typedef typename conditional
        <
            is_pointer<InputIterator>::value
            && is_pointer<OutputIterator>::value
            && is_same<input_type, output_type>::value
            && is_trivially_copyable<input_type>::value,
            true_type,
            false_type
        >
        ::type result;
    };

    template <class InputIterator, class OutputIterator>
    struct is_contiguously_copyable
        : integral_constant<bool, is_contiguously_copyable_impl<InputIterator, OutputIterator>::result::value> { };

    template <class OutputIterator, class InputType>
    struct is_contiguously_fillable_impl
    {
        typedef typename conditional
        <
            is_pointer<OutputIterator>::value
            && is_same<InputType, typename remove_pointer<OutputIterator>::type>::value
            && is_trivially_copyable<InputType>::value,
            true_type,
            false_type
        >
        ::type result;
    };

    template <class OutputIterator, class InputType>
    struct is_contiguously_fillable
        : integral_constant<bool, is_contiguously_fillable_impl<OutputIterator, InputType>::result::value> { };

    template <class ContiguousIterator>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_construct(ContiguousIterator first, ContiguousIterator last, true_type) noexcept {
        ::up::memset(first, 0, last - first * sizeof(*first));
        return last;
    }

    template <class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_construct(ForwardIterator first, ForwardIterator last, false_type) {
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
    uninitialized_construct_n(ContiguousIterator first, Size n, true_type) noexcept {
        ::up::memset(first, 0, n * sizeof(*first));
        return first + n;
    }

    template <class ForwardIterator, class Size>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_construct_n(ForwardIterator first, Size n, false_type) {
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
    uninitialized_copy(ContiguousIterator1 first, ContiguousIterator1 last, ContiguousIterator2 result, true_type) noexcept {
        ptrdiff_t const n = last - first;
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, false_type) {
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
        true_type
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
        false_type
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
    uninitialized_copy_n(ContiguousIterator1 first, Size n, ContiguousIterator2 result, true_type) noexcept {
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class Size, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_copy_n(InputIterator first, Size n, ForwardIterator result, false_type) {
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
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, true_type, integral_constant<size_t, 1>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        union { value_type value; uint8_t byte; } u = { static_cast<value_type>(value) };
        ::up::memset(first, u.byte, n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, true_type, integral_constant<size_t, 2>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        value_type actual_value = static_cast<value_type>(value);
        ::up::memset_pattern2(first, ::up::addressof(actual_value), n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, true_type, integral_constant<size_t, 4>) noexcept {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        value_type actual_value = static_cast<value_type>(value);
        ::up::memset_pattern4(first, ::up::addressof(actual_value), n);
        return first + n;
    }

    template <class ContiguousIterator, class Size, class T>
    inline UPALWAYSINLINE
    ContiguousIterator
    uninitialized_fill_n(ContiguousIterator first, Size n, T value, true_type, integral_constant<size_t, 8>) noexcept {
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
    void uninitialized_fill(ContiguousIterator first, ContiguousIterator last, T const& value, true_type) {
        typedef typename iterator_value_type<ContiguousIterator>::type value_type;
        typedef integral_constant<size_t, sizeof(value_type)> value_size;
        typedef integral_constant<bool, !(alignof(value_type) % sizeof(value_type))> is_aligned;
        ::up::detail::uninitialized_fill_n(first, last - first, value, is_aligned(), value_size());
    }

    template <class ForwardIterator, class T>
    inline UPALWAYSINLINE
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, T const& value, false_type) {
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
    uninitialized_move(ContiguousIterator1 first, ContiguousIterator1 last, ContiguousIterator2 result, true_type) noexcept {
        ptrdiff_t const n = last - first;
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result, false_type) {
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
        true_type
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
        false_type
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
    uninitialized_move_n(ContiguousIterator1 first, Size n, ContiguousIterator2 result, true_type) noexcept {
        ::up::memmove(result, first, n * sizeof(*result));
        return result + n;
    }

    template <class InputIterator, class Size, class ForwardIterator>
    inline UPALWAYSINLINE
    ForwardIterator
    uninitialized_move_n(InputIterator first, Size n, ForwardIterator result, false_type) {
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
        typedef integral_constant<size_t, sizeof(value_type)> value_size;
        typedef detail::is_contiguously_fillable<ForwardIterator, T> contiguously_fillable;
        typedef integral_constant
        <
            bool,
            (contiguously_fillable::value && !(alignof(value_type) % sizeof(value_type)))
        >
        contiguously_fillable_and_aligned;
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
        return ::up::detail::uninitialized_move_n(first, n, result, contiguously_copyable());
    }
}

namespace up { namespace detail
{
    template <class T> struct deallocate_sizeof : integral_constant<size_t, sizeof(T)> { };
    template <> struct deallocate_sizeof<void> : integral_constant<size_t, 0> { };
    template <> struct deallocate_sizeof<void const> : integral_constant<size_t, 0> { };
    template <> struct deallocate_sizeof<void volatile> : integral_constant<size_t, 0> { };
    template <> struct deallocate_sizeof<void const volatile> : integral_constant<size_t, 0> { };
}}

namespace up
{
    template <class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    void* allocate(Alloc* a, size_t n) noexcept {
        return a->allocate(n);
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate(Alloc* a) {
        return static_cast<T*>(a->allocate(sizeof(T)));
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
        a->deallocate(const_cast<typename remove_cv<T>::type*>(p), ::up::detail::deallocate_sizeof<T>::value);
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, typename enable_if<is_trivial<T>::value, nat_t>::type = nat_t()) {
        return static_cast<T*>(a->allocate_zero(1, sizeof(T)));
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, typename enable_if<!is_trivial<T>::value, nat_t>::type = nat_t()) {
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
        return ::new(p) T;
    }

#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_RVALUE_REFERENCES)
    template <class T, class Alloc, class... Args>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct(Alloc* a, Args&&... args) {
        void* const p = a->allocate(sizeof(T));
        if (!p) {
            return nullptr;
        }
        return ::new(p) T(::up::forward<Args>(args)...);
    }
#else
#   define UP_DETAIL_DEFINE_ALLOCATE_CONSTRUCT(RVREF) \
    template <class T, class Alloc, class A1> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* allocate_construct(Alloc* a, A1 RVREF a1) { \
        void* const p = a->allocate(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1)); \
    } \
    template <class T, class Alloc, class A1, class A2> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* allocate_construct(Alloc* a, A1 RVREF a1, A2 RVREF a2) { \
        void* const p = a->allocate(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2)); \
    } \
    template <class T, class Alloc, class A1, class A2, class A3> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* allocate_construct(Alloc* a, A1 RVREF a1, A2 RVREF a2, A3 RVREF a3) { \
        void* const p = a->allocate(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3)); \
    } \
    template <class T, class Alloc, class A1, class A2, class A3, class A4> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* allocate_construct(Alloc* a, A1 RVREF a1, A2 RVREF a2, A3 RVREF a3, A4 RVREF a4) { \
        void* const p = a->allocate(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3), ::up::forward<A4>(a4)); \
    } \
    template <class T, class Alloc, class A1, class A2, class A3, class A4, class A5> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* allocate_construct(Alloc* a, A1 RVREF a1, A2 RVREF a2, A3 RVREF a3, A4 RVREF a4, A5 RVREF a5) { \
        void* const p = a->allocate(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T( \
            ::up::forward<A1>(a1), \
            ::up::forward<A2>(a2), \
            ::up::forward<A3>(a3), \
            ::up::forward<A4>(a4), \
            ::up::forward<A5>(a5) \
        ); \
    }
#   ifndef UP_NO_RVALUE_REFERENCES
    UP_DETAIL_DEFINE_ALLOCATE_CONSTRUCT(&&)
#   else
    UP_DETAIL_DEFINE_ALLOCATE_CONSTRUCT(const&)
#   endif
#   undef UP_DETAIL_DEFINE_ALLOCATE_CONSTRUCT
#endif

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void destruct_deallocate(Alloc* a, T* p, typename detail::enable_if_not_destroyable<T, nat_t>::type = nat_t()) noexcept {
        a->deallocate(const_cast<typename remove_cv<T>::type*>(p), sizeof(T));
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void destruct_deallocate(Alloc* a, T* p, typename detail::enable_if_destroyable<T, nat_t>::type = nat_t())
    UPNOEXCEPT(is_nothrow_destructible<T>::value) {
        if (p) {
            typename remove_cv<T>::type* q = const_cast<typename remove_cv<T>::type*>(p);
            q->~T();
            a->deallocate(q, sizeof(T));
        }
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_n(Alloc* a, size_t n) {
        return static_cast<T*>(a->allocate(n * sizeof(T)));
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void deallocate_n(Alloc* a, T* p, size_t n) noexcept {
        a->deallocate(const_cast<typename remove_cv<T>::type*>(p), n * sizeof(T));
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct_n(Alloc* a, typename enable_if<is_trivial<T>::value, size_t>::type n) noexcept {
        return static_cast<T*>(a->allocate_zero(n, sizeof(T)));
    }

    template <class T, class Alloc>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* allocate_construct_n(Alloc* a, typename enable_if<!is_trivial<T>::value, size_t>::type n) {
        T* const p = static_cast<T*>(a->allocate(n * sizeof(T)));
        if (!p) {
            return nullptr;
        }
        ::up::uninitialized_construct_n(p, n);
        return p;
    }

    template <class Alloc, class T>
    inline UPALWAYSINLINE
    void destruct_deallocate_n(Alloc* a, T* p, size_t n) UPNOEXCEPT(is_nothrow_destructible<T>::value) {
        typename remove_cv<T>::type* q = const_cast<typename remove_cv<T>::type*>(p);
        ::up::destruct_n(q, n);
        a->deallocate(q, n * sizeof(T));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc() {
        return static_cast<T*>(::up::malloc(sizeof(T)));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(typename enable_if<is_trivial<T>::value, nat_t>::type = nat_t()) {
        return static_cast<T*>(::up::calloc(1, sizeof(T)));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(typename enable_if<!is_trivial<T>::value, nat_t>::type = nat_t()) {
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
        return ::new(p) T;
    }

#if !defined(UP_NO_VARIADIC_TEMPLATES) && !defined(UP_NO_RVALUE_REFERENCES)
    template <class T, class... Args>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct(Args&&... args) {
        void* const p = ::up::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }
        return ::new(p) T(::up::forward<Args>(args)...);
    }
#else
#   define UP_DETAIL_DEFINE_MALLOC_CONSTRUCT(RVREF) \
    template <class T, class A1> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* malloc_construct(A1 RVREF a1) { \
        void* const p  = ::up::malloc(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1)); \
    } \
    template <class T, class A1, class A2> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* malloc_construct(A1 RVREF a1, A2 RVREF a2) { \
        void* const p = ::up::malloc(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2)); \
    } \
    template <class T, class A1, class A2, class A3> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* malloc_construct(A1 RVREF a1, A2 RVREF a2, A3 RVREF a3) { \
        void* const p = ::up::malloc(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3)); \
    } \
    template <class T, class A1, class A2, class A3, class A4> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* malloc_construct(A1 RVREF a1, A2 RVREF a2, A3 RVREF a3, A4 RVREF a4) { \
        void* const p = ::up::malloc(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T(::up::forward<A1>(a1), ::up::forward<A2>(a2), ::up::forward<A3>(a3), ::up::forward<A4>(a4)); \
    } \
    template <class T, class A1, class A2, class A3, class A4, class A5> \
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT \
    T* malloc_construct(A1 RVREF a1, A2 RVREF a2, A3 RVREF a3, A4 RVREF a4, A5 RVREF a5) { \
        void* const p = ::up::malloc(sizeof(T)); \
        if (!p) { \
            return nullptr; \
        } \
        return ::new(p) T( \
            ::up::forward<A1>(a1), \
            ::up::forward<A2>(a2), \
            ::up::forward<A3>(a3), \
            ::up::forward<A4>(a4), \
            ::up::forward<A5>(a5) \
        ); \
    }
#   ifndef UP_NO_RVALUE_REFERENCES
    UP_DETAIL_DEFINE_MALLOC_CONSTRUCT(&&)
#   else
    UP_DETAIL_DEFINE_MALLOC_CONSTRUCT(const&)
#   endif
#   undef UP_DETAIL_DEFINE_MALLOC_CONSTRUCT
#endif

    template <class T>
    inline UPALWAYSINLINE
    void destruct_free(T* p, typename detail::enable_if_not_destroyable<T, nat_t>::type = nat_t()) noexcept {
        ::up::free(const_cast<typename remove_cv<T>::type*>(p));
    }

    template <class T>
    inline UPALWAYSINLINE
    void destruct_free(T* p, typename detail::enable_if_destroyable<T, nat_t>::type = nat_t())
    UPNOEXCEPT(is_nothrow_destructible<T>::value) {
        if (p) {
            typename remove_cv<T>::type* q = const_cast<typename remove_cv<T>::type*>(p);
            q->~T();
            ::up::free(q);
        }
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_n(size_t n) {
        return static_cast<T*>(::up::malloc(n * sizeof(T)));
    }

    template <class T>
    inline UPALWAYSINLINE
    void free_n(T* p, size_t) noexcept {
        ::up::free(const_cast<typename remove_cv<T>::type*>(p));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct_n(typename enable_if<is_trivial<T>::value, size_t>::type n) noexcept {
        return static_cast<T*>(::up::calloc(n, sizeof(T)));
    }

    template <class T>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    T* malloc_construct_n(typename enable_if<!is_trivial<T>::value, size_t>::type n) {
        T* const p = static_cast<T*>(::up::malloc(n * sizeof(T)));
        if (!p) {
            return nullptr;
        }
        ::up::uninitialized_construct_n(p, n);
        return p;
    }

    template <class T>
    inline UPALWAYSINLINE
    void destruct_free_n(T* p, size_t n) UPNOEXCEPT(is_nothrow_destructible<T>::value) {
        typename remove_cv<T>::type* q = const_cast<typename remove_cv<T>::type*>(p);
        ::up::destruct_n(q, n);
        ::up::free(q);
    }

    template <class T>
    struct UPVISIBLE allocator_deleter
    {
    private:
        allocator* alloc_;
    public:
        UPALWAYSINLINE explicit allocator_deleter(allocator* alloc) noexcept : alloc_(alloc) { }
        UPALWAYSINLINE void operator()(T* p) const noexcept { ::up::destruct_deallocate(alloc_, p); }
    };

    template <class T>
    struct UPVISIBLE free_deleter
    {
        UPALWAYSINLINE void operator()(T* p) const noexcept { ::up::destruct_free(p); }
    };
}

#endif
