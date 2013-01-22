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

#ifndef UP_DETAIL_HASH_INL
#define UP_DETAIL_HASH_INL

#if !defined(UP_HASHMAP_HPP) && !defined(UP_HASHSET_HPP)
#   error "Do not include this header directly. Instead include <up/hashmap.hpp> or <up/hashset.hpp>"
#endif

#include <up/bitwise.hpp>
#include <up/memory.hpp>

namespace up
{
    constexpr int hash_success = 0;
    constexpr int hash_badallocator = -1;
    constexpr int hash_badlength = -2;
    constexpr int hash_nomem = -3;
    constexpr int hash_nospace = -4;

    template <class Key>
    struct UPVISIBLE hashtraits
    {
        static_assert(is_unsigned<Key>::value, "Key parameter must be an unsigned integer type");
        
        typedef Key key_type;
        static constexpr Key vacant = static_cast<Key>(~0);
        static constexpr Key deleted = vacant - 1;

        inline UPALWAYSINLINE UPCONSTEXPR
        Key resolve(Key hashcode) noexcept {
            return (hashcode >= deleted) ? (hashcode + 2) : hashcode;
        }
    };

#ifdef UP_NO_CONSTEXPR
    template <class Key>
    constexpr Key hashtraits<Key>::vacant;

    template <class Key>
    constexpr Key hashtraits<Key>::deleted;
#endif

    template <class Key, class Value = nat_t>
    struct UPVISIBLE hashrecord
    {
        static_assert(is_unsigned<Key>::value, "Key parameter must be an unsigned integer type");
        
        typedef Key key_type;
        typedef Value value_type;
        Key const* key_ptr;
        Value* value_ptr;
        
        UPALWAYSINLINE bool valid() const noexcept { return key_ptr != nullptr; }
        UPALWAYSINLINE Key key() const noexcept { return *key_ptr; }
        UPALWAYSINLINE Value& value() noexcept { return *value_ptr; }
        UPALWAYSINLINE Value const& value() const noexcept { return *value_ptr; }
        UPALWAYSINLINE void value(Value const& v) noexcept { *value_ptr = v; }
        UPALWAYSINLINE void value(Value&& v) noexcept { *value_ptr = ::up::move(v); }
    };

    template <class Key>
    struct UPVISIBLE hashrecord<Key, nat_t>
    {
        static_assert(is_unsigned<Key>::value, "Key parameter must be an unsigned integer type");
        
        typedef Key key_type;
        Key const* key_ptr;
        size_t key_index;
        
        UPALWAYSINLINE bool valid() const noexcept { return key_ptr != nullptr; }
        UPALWAYSINLINE Key key() const noexcept { return *key_ptr; }
        UPALWAYSINLINE size_t index() const noexcept { return key_index; }
    };

    template <class Key, class Value = nat_t>
    struct UPVISIBLE hashresult
    {
        hashrecord<Key, Value> record;
        bool success;
    };

    template <class Key>
    struct UPVISIBLE hashresult<Key, nat_t>
    {
        hashrecord<Key> record;
        bool success;
    };
}

#endif
