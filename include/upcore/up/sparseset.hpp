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

#ifndef UP_SPARSESET_HPP
#define UP_SPARSESET_HPP

#include <up/detail/sparse.inl>

namespace up
{   
    template <class Record, slist_node Record::* NodePtr, class Hasher, class Equals>
    struct UPVISIBLE sparseset
    {
        typedef Record record_type;
        typedef Hasher hasher_type;
        typedef Equals equals_type;
        compressed_pair<slist_node*, Hasher> buckets_and_hasher;
        compressed_pair<size_t, Equals> num_buckets_and_equals;
        size_t size;
        inline UPALWAYSINLINE slist_node* buckets() const noexcept { return buckets_and_hasher.first(); }
        inline UPALWAYSINLINE size_t num_buckets() const noexcept { return num_buckets_and_equals.first(); }
        inline UPALWAYSINLINE Hasher const& hasher() const noexcept { return buckets_and_hasher.second(); }
        inline UPALWAYSINLINE Equals const& equals() const noexcept { return num_buckets_and_equals.second(); }
    };

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    bool sparseset_validate(sparseset<R, N, H, E> const& set) noexcept {
        H const& hasher = set.hasher();
        slist_node const* const buckets = set.buckets();
        size_t const num_buckets = set.num_buckets();
        size_t const size = set.size;
        size_t const mask = num_buckets - 1;
        size_t index, count;
        slist_node const* node;
        R const* record;

        if ( (num_buckets && !buckets)
            || (!num_buckets && buckets)
            || (!num_buckets && (size > 0))
            || ((num_buckets & (num_buckets - 1)) != 0)
        ) {
            return false;
        }

        for (index = 0, count = 0; index < num_buckets; ++index) {
            for (node = buckets[index].next; node; ++count, node = node->next) {
                record = ::up::slist_cast<R const*>(node, N);
                if ((hasher(*record) & mask) != index) {
                    return false;
                }
            }
        }

        if (count != size) {
            return false;
        }

        return true;
    }

    template <class R>
    inline UPALWAYSINLINE
    ssize_t sparseset_memory_footprint(size_t size, size_t num_buckets) noexcept {
        return ::up::sparse_memory_footprint(size, num_buckets, sizeof(R));
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPALWAYSINLINE
    ssize_t sparseset_memory_footprint(sparseset<R, N, H, E> const& set) noexcept {
        return ::up::sparse_memory_footprint(set.size, set.num_buckets(), sizeof(R));
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPALWAYSINLINE UPPURE
    bool sparseset_is_overloaded(sparseset<R, N, H, E> const& set, double load_factor) noexcept {
        ::up::sparse_is_overloaded(set.size, set.num_buckets(), load_factor);
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPALWAYSINLINE
    bool sparseset_is_empty(sparseset<R, N, H, E> const& set) noexcept {
        return !set.size;
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPALWAYSINLINE
    size_t sparseset_size(sparseset<R, N, H, E> const& set) noexcept {
        return set.size;
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPALWAYSINLINE
    size_t sparseset_num_buckets(sparseset<R, N, H, E> const& set) noexcept {
        return set.num_buckets();
    }

    template <class R, slist_node R::* N, class H, class E, class Recycle>
    UPVISIBLE
    void sparseset_clear(sparseset<R, N, H, E>& set, Recycle recycle) noexcept {
        slist_node* bucket = set.buckets();
        slist_node* const end_bucket = bucket + set.num_buckets();
        slist_node* curr, * next;

        if (set.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    curr->next = nullptr;
                    recycle(::up::slist_cast<R*>(curr, N));
                }
                bucket->next = nullptr;
            }
            set.size = 0;
        }
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    int sparseset_clear(sparseset<R, N, H, E>& set, allocator* record_alloc) noexcept {
        if (!node_alloc) {
            return sparse_badallocator;
        }
        ::up::sparseset_clear(set, [=] (R* record) {
            ::up::destruct_deallocate(record_alloc, record);
        });
        return sparse_success;
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    void sparseset_swap(sparseset<R, N, H, E>& set1, sparseset<R, N, H, E>& set2) noexcept {
        if (&set1 != &set2) {
            ::up::swap(set1.buckets_and_hasher, set2.buckets_and_hasher);
            ::up::swap(set1.num_buckets_and_equals, set2.num_buckets_and_equals);
            ::up::swap(set1.size, set2.size);
        }
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    void sparseset_construct(sparseset<R, N, H, E>& set, H const& hasher = H(), E const& equals = E()) noexcept {
        set.buckets_and_hasher.first(nullptr);
        set.buckets_and_hasher.second(hasher);
        set.num_buckets_and_equals.first(0);
        set.num_buckets_and_equals.second(equals);
        set.size = 0;
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    int sparseset_construct(sparseset<R, N, H, E>& set, allocator* bucket_alloc, size_t num_buckets, H const& hasher = H(), E const& equals = E()) {
        if (!bucket_alloc) {
            return sparse_badallocator;
        }

        size_t const num_buckets2 = ::up::ceil2(num_buckets);
        if (num_buckets2 > (SIZE_MAX / sizeof(slist_node))) {
            return sparse_badlength;
        }

        slist_node* buckets = nullptr;
        if (num_buckets2 > 0) {
            buckets = ::up::allocate_construct_n<slist_node>(bucket_alloc, num_buckets2);
            if (!buckets) {
                return sparse_nomem;
            }
        }
        
        set.buckets_and_hasher.first(buckets);
        set.buckets_and_hasher.second(hasher);
        set.num_buckets_and_equals.first(num_buckets2);
        set.num_buckets_and_equals.second(equals);
        set.size = 0;
        return sparse_success;
    }

    template <class R, slist_node R::* N, class H, class E, class Recycle>
    UPVISIBLE
    int sparseset_destruct(sparseset<R, N, H, E>& set, allocator* bucket_alloc, Recycle recycle) {
        if (!bucket_alloc) {
            return sparse_badallocator;
        }

        ::up::sparseset_clear(set, recycle);

        size_t const num_buckets = set.num_buckets();
        if (num_buckets) {
            ::up::deallocate_n(bucket_alloc, set.buckets(), num_buckets);
            set.buckets_and_hasher.first(nullptr);
            set.num_buckets_and_equals.first(0);
            set.size = 0;
        }

        return sparse_success;
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    int sparseset_destruct(sparseset<R, N, H, E>& set, allocator* bucket_alloc, allocator* record_alloc) {
        if (!record_alloc) {
            return sparse_badallocator;
        }
        return ::up::sparseset_destruct(set, bucket_alloc, [=] (R* record) {
            ::up::destruct_deallocate(record_alloc, record);
        });
    }

    template <class R, slist_node R::* N, class H, class E, class Function>
    inline UPHIDDENINLINE
    void sparseset_foreach(sparseset<R, N, H, E> const& set, Function func) noexcept {
        slist_node* bucket = set.buckets(), * curr;
        slist_node* const end_bucket = bucket + set.num_buckets();
        R* record;

        for ( ; bucket < end_bucket; ++bucket) {
            for (curr = bucket->next; curr; curr = curr->next) {
                func(::up::slist_cast<R*>(curr, N));
            }
        }
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    R* sparseset_find(sparseset<R, N, H, E> const& set, Key const& key, Hasher const& hasher, Equals const& equals) {
        if (!set.size) {
            return nullptr;
        }

        H const& record_hasher = set.hasher();
        size_t const mask = set.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = set.buckets();
        slist_node* curr;
        R* record;

        for (curr = buckets[hashcode & mask].next; curr; curr = curr->next) {
            record = ::up::slist_cast<R*>(curr, N);
            if ((record_hasher(*record) == hashcode) && equals(*record, key)) {
                return record;
            }
        }

        return nullptr;
    }

    template <class R, slist_node R::* N, class H, class E, class Key>
    inline UPHIDDENINLINE
    R* sparseset_find(sparseset<R, N, H, E> const& set, Key const& key) {
        return ::up::sparseset_find(set, key, set.hasher(), set.equals());
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    R* sparseset_erase(sparseset<R, N, H, E>& set, R* record) {
        if (!set.size || !record) {
            return nullptr;
        }

        H const& hasher = set.hasher();
        size_t const mask = set.num_buckets() - 1;
        size_t const hashcode = hasher(*record);
        slist_node* const buckets = set.buckets();
        slist_node* const record_node = ::up::slist_cast<slist_node*>(record, N);
        slist_node* prev, * curr;

        for (prev = &buckets[hashcode & mask], curr = prev->next; curr; curr = curr->next) {
            if (curr == record_node) {
                prev->next = curr->next;
                curr->next = nullptr;
                --set.size;
                return record;
            }
            else {
                prev = curr;
            }
        }

        return nullptr;
    }

    template <class R, slist_node R::* N, class H, class E, class Recycle>
    inline UPHIDDENINLINE
    size_t sparseset_erase(sparseset<R, N, H, E>& set, R* record, Recycle recycle) {
        R* result = ::up::sparseset_erase(set, record);
        if (result) {
            recycle(result);
            return 1;
        }
        return 0;
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    size_t sparseset_erase(sparseset<R, N, H, E>& set, allocator* record_alloc, R* record) {
        return ::up::sparseset_erase(set, record, [=] (R* r) {
            ::up::destruct_deallocate(record_alloc, r);
        });
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    R* sparseset_erase(sparseset<R, N, H, E>& set, Key const& key, Hasher const& hasher, Equals const& equals) {
        if (!set.size) {
            return nullptr;
        }

        H const& record_hasher = set.hasher();
        size_t const mask = set.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = set.buckets();
        slist_node* prev, * curr;
        R* record;

        for (prev = &buckets[hashcode & mask], curr = prev->next; curr; curr = curr->next) {
            record = ::up::slist_cast<R*>(curr, N);
            if ((record_hasher(*record) == hashcode) && equals(*record, key)) {
                ::up::slist_unlink(prev, curr);
                --set.size;
                return record;
            }
            else {
                prev = curr;
            }
        }

        return nullptr;
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Hasher, class Equals, class Recycle>
    inline UPHIDDENINLINE
    size_t sparseset_erase(sparseset<R, N, H, E>& set, Key const& key, Hasher const& hasher, Equals const& equals, Recycle recycle) {
        R* result = ::up::sparseset_erase(set, key, hasher, equals);
        if (result) {
            recycle(result);
            return 1;
        }
        return 0;
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    size_t sparseset_erase(sparseset<R, N, H, E>& set, allocator* record_alloc, Key const& key, Hasher const& hasher, Equals const& equals) {
        return ::up::sparseset_erase(set, key, hasher, equals, [=] (R* record) {
            ::up::destruct_deallocate(record_alloc, record);
        });
    }

    template <class R, slist_node R::* N, class H, class E, class Key>
    inline UPHIDDENINLINE
    R* sparseset_erase(sparseset<R, N, H, E>& set, Key const& key) {
        return ::up::sparseset_erase(set, key, set.hasher(), set.equals());
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Recycle>
    inline UPHIDDENINLINE
    size_t sparseset_erase(sparseset<R, N, H, E>& set, Key const& key, Recycle recycle) {
        R* record = ::up::sparseset_erase(set, key, set.hasher(), set.equals());
        if (record) {
            recycle(record);
            return 1;
        }
        return 0;
    }

    template <class R, slist_node R::* N, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t sparseset_erase(sparseset<R, N, H, E>& set, allocator* record_alloc, Key const& key) {
        return ::up::sparseset_erase(set, key, set.hasher(), set.equals(), [=] (R* record) {
            ::up::destruct_deallocate(record_alloc, record);
        });
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Hasher, class Equals, class Recycle>
    UPVISIBLE
    size_t sparseset_multi_erase(sparseset<R, N, H, E>& set, Key const& key, Hasher const& hasher, Equals const& equals, Recycle recycle) {
        if (!set.size) {
            return 0;
        }

        H const& record_hasher = set.hasher();
        size_t const mask = set.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = set.buckets();
        slist_node* prev, * curr, * next;
        size_t count = 0;
        R* record;

        for (prev = &buckets[hashcode & mask], curr = prev->next; curr; curr = next) {
            next = curr->next;
            record = ::up::slist_cast<R*>(curr, N);
            if ((record_hasher(*record) == hashcode) && equals(*record, key)) {
                ::up::slist_unlink(prev, curr);
                recycle(record);
                ++count;
            }
            else {
                prev = curr;
            }
        }

        set.size -= count;
        return count;
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    size_t sparseset_multi_erase(sparseset<R, N, H, E>& set, allocator* record_alloc, Key const& key, Hasher const& hasher, Equals const& equals) {
        return ::up::sparseset_multi_erase(set, key, hasher, equals, [=] (R* record) {
            ::up::destruct_deallocate(record_alloc, record);
        });
    }

    template <class R, slist_node R::* N, class H, class E, class Key, class Recycle>
    inline UPHIDDENINLINE
    size_t sparseset_multi_erase(sparseset<R, N, H, E>& set, Key const& key, Recycle recycle) {
        return ::up::sparseset_multi_erase(set, key, set.hasher(), set.equals(), recycle);
    }

    template <class R, slist_node R::* N, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t sparseset_multi_erase(sparseset<R, N, H, E>& set, allocator* record_alloc, Key const& key) {
        return ::up::sparseset_multi_erase(set, key, set.hasher(), set.equals(), [=] (R* record) {
            ::up::destruct_deallocate(record_alloc, record);
        });
    }

    template <class R, slist_node R::* N, class H, class E, class Key>
    UPVISIBLE
    sparseresult<R> sparseset_prepare(sparseset<R, N, H, E>& set, Key const& key, slist_node** bucket_ptr) {
        sparseresult<R> result;
        if (!set.num_buckets()) {
            result.record = nullptr;
            result.success = false;
            return result;
        }

        H const& hasher = set.hasher();
        E const& equals = set.equals();
        size_t const mask = set.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = set.buckets();
        slist_node* bucket, * node;
        R* record;

        bucket = &buckets[hashcode & mask];
        *bucket_ptr = bucket;

        for (node = bucket->next; node; node = node->next) {
            record = ::up::slist_cast<R*>(node, N);
            if ((hasher(*record) == hashcode) && equals(*record, key)) {
                result.record = record;
                result.success = false;
                return result;
            }
        }

        result.record = nullptr;
        result.success = true;
        return result;
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    sparseresult<R> sparseset_insert(sparseset<R, N, H, E>& set, R* record) {
        sparseresult<R> result;
        if (!record) {
            result.record = nullptr;
            result.success = false;
            return result;
        }
                
        slist_node* bucket = nullptr;
        result = ::up::sparseset_prepare(set, *record, &bucket);
        if (result.success) {
            result.record = record;
            ::up::slist_link_before(bucket, ::up::slist_cast<slist_node*>(record, N));
            ++set.size;
        }

        return result;
    }

    template <class R, slist_node R::* N, class H, class E, class Data>
    UPVISIBLE
    sparseresult<R> sparseset_insert(sparseset<R, N, H, E>& set, allocator* record_alloc, Data const& data) {
        sparseresult<R> result;
        if (!record_alloc) {
            result.record = nullptr;
            result.success = false;
            return result;
        }

        slist_node* bucket = nullptr;
        result = ::up::sparseset_prepare(set, data, &bucket);
        if (result.success) {
            result.record = ::up::allocate_construct<R>(record_alloc, data);
            if (result.record) {
                ::up::slist_link_before(bucket, ::up::slist_cast<slist_node*>(result.record, N));
                ++set.size;
            }
            else {
                result.success = false;
            }
        }

        return result;
    }

    template <class R, slist_node R::* N, class H, class E, class Data>
    UPVISIBLE
    sparseresult<R> sparseset_insert(sparseset<R, N, H, E>& set, allocator* record_alloc, Data&& data) {
        sparseresult<R> result;
        if (!record_alloc) {
            result.record = nullptr;
            result.success = false;
            return result;
        }

        slist_node* bucket = nullptr;
        result = ::up::sparseset_prepare(set, data, &bucket);
        if (result.success) {
            result.record = ::up::allocate_construct<R>(record_alloc, ::up::move(data));
            if (result.record) {
                ::up::slist_link_before(bucket, ::up::slist_cast<slist_node*>(result.record, N));
                ++set.size;
            }
            else {
                result.success = false;
            }
        }

        return result;
    }

    template <class R, slist_node R::* N, class H, class E, class Key>
    inline UPHIDDENINLINE
    slist_node* sparseset_multi_prepare(sparseset<R, N, H, E>& set, Key const& key) {
        if (!set.num_buckets()) {
            return nullptr;
        }

        H const& hasher = set.hasher();
        size_t const mask = set.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = set.buckets();
        return &buckets[hashcode & mask];
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    R* sparseset_multi_insert(sparseset<R, N, H, E>& set, R* record) {
        if (!record) {
            return nullptr;
        }

        slist_node* bucket = ::up::sparseset_multi_prepare(set, *record);
        if (!bucket) {
            return nullptr;
        }

        ::up::slist_link_before(bucket, ::up::slist_cast<slist_node*>(record, N));
        ++set.size;
        return record;
    }

    template <class R, slist_node R::* N, class H, class E, class Data>
    UPVISIBLE
    R* sparseset_multi_insert(sparseset<R, N, H, E>& set, allocator* record_alloc, Data const& data) {
        if (!record) {
            return nullptr;
        }

        slist_node* bucket = ::up::sparseset_multi_prepare(set, data);
        if (!bucket) {
            return nullptr;
        }

        R* record = ::up::allocate_construct<R>(record_alloc, data);
        if (!record) {
            return nullptr;
        }

        ::up::slist_link_before(bucket, ::up::slist_cast<slist_node*>(record, N));
        ++set.size;
        return record;
    }

    template <class R, slist_node R::* N, class H, class E, class Data>
    UPVISIBLE
    R* sparseset_multi_insert(sparseset<R, N, H, E>& set, allocator* record_alloc, Data&& data) {
        if (!record) {
            return nullptr;
        }

        slist_node* bucket = ::up::sparseset_multi_prepare(set, data);
        if (!bucket) {
            return nullptr;
        }

        R* record = ::up::allocate_construct<R>(record_alloc, ::up::move(data));
        if (!record) {
            return nullptr;
        }

        ::up::slist_link_before(bucket, ::up::slist_cast<slist_node*>(record, N));
        ++set.size;
        return record;
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    void sparseset_move(sparseset<R, N, H, E>& dst, sparseset<R, N, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.num_buckets()) {
            ::up::sparseset_swap(dst, src);
            return;
        }
        
        slist_node* bucket = src.buckets(), * curr, * next;
        slist_node* const end_bucket = bucket + src.num_buckets();
        sparseresult<R> result;
        R* record;

        if (src.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    record = ::up::slist_cast<R*>(curr, N);
                    result = ::up::sparseset_insert(dst, record);
                    UP_ASSERT(result.record);
                }
                bucket->next = nullptr;
            }
            src.size = 0;
        }
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    void sparseset_multi_move(sparseset<R, N, H, E>& dst, sparseset<R, N, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.num_buckets()) {
            ::up::sparseset_swap(dst, src);
            return;
        }

        slist_node* bucket = src.buckets(), * curr, * next;
        slist_node* const end_bucket = bucket + src.num_buckets();
        sparseresult<R> result;
        R* record;

        if (src.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    record = ::up::slist_cast<R*>(curr, N);
                    result = ::up::sparseset_multi_insert(dst, record);
                    UP_ASSERT(result.record);
                }
                bucket->next = nullptr;
            }
            src.size = 0;
        }
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    int sparseset_reserve(sparseset<R, N, H, E>& set, allocator* bucket_alloc, size_t num_buckets) {
        num_buckets = (num_buckets && (num_buckets < 8)) ? 8 : ceil2(num_buckets);
        if ((num_buckets < set.size) || (num_buckets == set.num_buckets())) {
            return sparse_success;
        }

        sparseset<R, N, H, E> temp;
        int retval = ::up::sparseset_construct(temp, bucket_alloc, num_buckets, set.hasher(), set.equals());
        if (retval != sparse_success) {
            return retval;
        }

        ::up::sparseset_move(temp, set);
        ::up::sparseset_swap(temp, set);
        return ::up::sparseset_destruct(temp, bucket_alloc, bucket_alloc);
    }

    template <class R, slist_node R::* N, class H, class E>
    UPVISIBLE
    int sparseset_multi_reserve(sparseset<R, N, H, E>& set, allocator* bucket_alloc, size_t num_buckets) {
        num_buckets = (num_buckets && (num_buckets < 8)) ? 8 : ceil2(num_buckets);
        if ((num_buckets < set.size) || (num_buckets == set.num_buckets())) {
            return sparse_success;
        }

        sparseset<R, N, H, E> temp;
        int retval = ::up::sparseset_construct(temp, bucket_alloc, num_buckets, set.hasher(), set.equals());
        if (retval != sparse_success) {
            return retval;
        }

        ::up::sparseset_multi_move(temp, set);
        ::up::sparseset_swap(temp, set);
        return ::up::sparseset_destruct(temp, bucket_alloc, bucket_alloc);
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    int sparseset_grow(sparseset<R, N, H, E>& set, allocator* bucket_alloc) noexcept {
        return ::up::sparseset_reserve(set, bucket_alloc, set.size ? (set.size << 1) : 8);
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    int sparseset_multi_grow(sparseset<R, N, H, E>& set, allocator* bucket_alloc) noexcept {
        return ::up::sparseset_multi_reserve(set, bucket_alloc, set.size ? (set.size << 1) : 8);
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    int sparseset_shrink_to_fit(sparseset<R, N, H, E>& set, allocator* bucket_alloc) noexcept {
        return ::up::sparseset_reserve(set, bucket_alloc, set.size);
    }

    template <class R, slist_node R::* N, class H, class E>
    inline UPHIDDENINLINE
    int sparseset_multi_shrink_to_fit(sparseset<R, N, H, E>& set, allocator* bucket_alloc) noexcept {
        return ::up::sparseset_multi_reserve(set, bucket_alloc, set.size);
    }
}

#endif
