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

#ifndef UP_DETAIL_DENSE_INL
#define UP_DETAIL_DENSE_INL

#if !defined(UP_DENSEMAP_HPP) && !defined(UP_DENSESET_HPP)
#   error "Do not include this header directly. Instead include <up/densemap.hpp> or <up/denseset.hpp>"
#endif

#include <up/bitwise.hpp>
#include <up/memory.hpp>

namespace up
{
    constexpr int dense_success = 0;
    constexpr int dense_badallocator = -1;
    constexpr int dense_badlength = -2;
    constexpr int dense_nomem = -3;
    constexpr int dense_nospace = -4;

    constexpr size_t dense_vacant = SIZE_MAX;
    constexpr size_t dense_deleted = SIZE_MAX - 1;

    inline UPALWAYSINLINE UPCONSTEXPR
    size_t dense_resolve(size_t hashcode) noexcept {
        return (hashcode >= dense_deleted) ? (hashcode + 2) : hashcode;
    }

    template <class Key, class Value = nat_t>
    struct UPVISIBLE denserecord
    {
        typedef Key key_type;
        typedef Value value_type;
        Key const* key_ptr;
        Value* value_ptr;
        UPALWAYSINLINE bool valid() const noexcept { return key_ptr != nullptr; }
        UPALWAYSINLINE Key const& key() const noexcept { return *key_ptr; }
        UPALWAYSINLINE Value& value() noexcept { return *value_ptr; }
        UPALWAYSINLINE Value const& value() const noexcept { return *value_ptr; }
        UPALWAYSINLINE void value(Value const& v) noexcept { *value_ptr = v; }
        UPALWAYSINLINE void value(Value&& v) noexcept { *value_ptr = ::up::move(v); }
    };

    template <class Key>
    struct UPVISIBLE denserecord<Key, nat_t>
    {
        typedef Key key_type;
        typedef nat_t value_type;
        Key const* key_ptr;
        UPALWAYSINLINE bool valid() const noexcept { return key_ptr != nullptr; }
        UPALWAYSINLINE Key const& key() const noexcept { return *key_ptr; }
    };

    template <class Key, class Value = nat_t>
    struct UPVISIBLE denseresult
    {
        typedef Key key_type;
        typedef Value value_type;
        denserecord<Key, Value> record;
        bool success;
    };
}

#endif
