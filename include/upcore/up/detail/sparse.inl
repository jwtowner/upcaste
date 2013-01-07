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

#ifndef UP_DETAIL_SPARSE_INL
#define UP_DETAIL_SPARSE_INL

#if !defined(UP_SPARSEMAP_HPP) && !defined(UP_SPARSESET_HPP)
#   error "Do not include this header directly. Instead include <up/sparsemap.hpp> or <up/sparseset.hpp>"
#endif

#include <up/bitwise.hpp>
#include <up/memory.hpp>
#include <up/slist.hpp>

namespace up
{
    constexpr int sparse_success = 0;
    constexpr int sparse_badallocator = -1;
    constexpr int sparse_badlength = -2;
    constexpr int sparse_nomem = -3;

    template <class Key, class Value>
    struct UPVISIBLE sparserecord
    {
        UPNONCOPYABLE(sparserecord);
    public:
        typedef Key key_type;
        typedef Value value_type;
        slist_node node;
        Key const key;
        Value value;
        inline UPALWAYSINLINE sparserecord(Key const& k, Value const& v) : key(k), value(v) { ::up::slist_construct(&node); }
        inline UPALWAYSINLINE sparserecord(Key&& k, Value const& v) : key(::up::move(k)), value(v) { ::up::slist_construct(&node); }
        inline UPALWAYSINLINE sparserecord(Key const& k, Value&& v) : key(k), value(::up::move(v)) { ::up::slist_construct(&node); }
        inline UPALWAYSINLINE sparserecord(Key&& k, Value&& v) : key(::up::move(k)), value(::up::move(v)) { ::up::slist_construct(&node); }
    };

    template <class Key, class Value = nat_t>
    struct UPVISIBLE sparseresult
    {
        sparserecord<Key, Value>* record;
        bool success;
    };

    template <class Record>
    struct UPVISIBLE sparseresult<Record, nat_t>
    {
        Record* record;
        bool success;
    };

    extern LIBUPCOREAPI UPPURE
    ssize_t sparse_memory_footprint(size_t size, size_t num_buckets, size_t sizeof_record) noexcept;

    inline UPALWAYSINLINE UPPURE
    bool sparse_is_overloaded(size_t size, size_t num_buckets, double load_factor) noexcept {
        return (num_buckets > 0) ? ((static_cast<double>(size) / static_cast<double>(num_buckets)) > load_factor) : true;
    }
}

#endif
