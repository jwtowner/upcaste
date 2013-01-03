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

#ifndef UP_TYPE_TRAITS_HPP
#define UP_TYPE_TRAITS_HPP

#include <up/cstddef.hpp>

namespace up
{
    struct LIBUPCOREAPI nat_t { };
    extern LIBUPCOREAPI nat_t const nat;
}

namespace up { namespace detail
{
    struct UPHIDDEN any_t
    {
        template <class U> UPHIDDEN any_t(U&);
        template <class U> UPHIDDEN any_t(U const&);
        template <class U> UPHIDDEN any_t(U volatile&);                
        template <class U> UPHIDDEN any_t(U const volatile&);
#ifndef UP_NO_RVALUE_REFERENCES
        template <class U> UPHIDDEN any_t(U&&);
#endif
    };
}}

#ifdef UP_HAS_STDCXX_TYPE_TRAITS_CXX11
#   include <up/detail/type_traits_cxx11.inl>
#else
#   include <up/detail/type_traits_generic.inl>
#endif

#endif
