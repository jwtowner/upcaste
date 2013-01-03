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

#ifndef UP_SLIST_HPP
#define UP_SLIST_HPP

#include <up/cstdint.hpp>
#include <up/allocator.hpp>
#include <up/type_traits.hpp>

namespace up
{
    struct LIBUPCOREAPI slist_node
    {
        slist_node* next;
    };

    extern LIBUPCOREAPI
    bool slist_validate(slist_node const* node, ptrdiff_t expected_size = -1) noexcept;

    namespace detail
    {
        template <class T, class U, class V>
        struct UPHIDDEN slist_cast_impl
        {
            static UPALWAYSINLINE
            T cast(U* x, slist_node V::* p) noexcept {
                return static_cast<T>(&(x->*p));
            }
        };

        template <class T, class V>
        struct UPHIDDEN slist_cast_impl<T, slist_node, V>
        {
            static UPALWAYSINLINE
            T cast(slist_node* x, slist_node V::* p) noexcept {
                uintptr_t addr = reinterpret_cast<uintptr_t>(x) - reinterpret_cast<uintptr_t>(&(((V*)0)->*p));
                return static_cast<T>(reinterpret_cast<V*>(addr));
            }
        };

        template <class T, class V>
        struct UPHIDDEN slist_cast_impl<T, slist_node const, V>
        {
            static UPALWAYSINLINE
            T cast(slist_node const* x, slist_node V::* p) noexcept {
                uintptr_t addr = reinterpret_cast<uintptr_t>(x) - reinterpret_cast<uintptr_t>(&(((V const*)0)->*p));
                return static_cast<T>(reinterpret_cast<V const*>(addr));
            }
        };
    }

    template <class T, class U, class V>
    inline UPALWAYSINLINE
    T slist_cast(U* x, slist_node V::* p) noexcept {
        return ::up::detail::slist_cast_impl<T, U, V>::cast(x, p);
    }

    inline UPALWAYSINLINE
    void slist_construct(slist_node* node) noexcept {
        node->next = nullptr;
    }

    inline UPALWAYSINLINE
    void slist_move(slist_node* node1, slist_node* node2) noexcept {
        node1->next = node2->next;
    }

    inline UPALWAYSINLINE
    void slist_swap(slist_node* node1, slist_node* node2) noexcept {
        slist_node* t = node1->next;
        node1->next = node2->next;
        node2->next = t;
    }

    inline UPALWAYSINLINE
    bool slist_is_empty(slist_node const* node) noexcept {
        return node->next == nullptr;
    }

    inline UPALWAYSINLINE
    bool slist_is_linked(slist_node const* node) noexcept {
        return node->next != nullptr;
    }

    inline UPALWAYSINLINE
    size_t slist_size(slist_node const* first, slist_node const* last) noexcept {
        size_t count = 0;
        for ( ; first != last; first = first->next) {
            ++count;
        }
        return count;
    }

    inline UPALWAYSINLINE
    size_t slist_size(slist_node const* head) noexcept {
        return ::up::slist_size(head->next, nullptr);
    }

    template <class Node, slist_node Node::* NodePtr, class Function>
    inline UPALWAYSINLINE
    void slist_foreach(slist_node* first, slist_node* last, Function func) noexcept {
        for ( ; first != last; first = first->next) {
            func(::up::slist_cast<Node*>(first, NodePtr));
        }
    }

    template <class Node, slist_node Node::* NodePtr, class Function>
    inline UPALWAYSINLINE
    void slist_foreach(slist_node* head, Function func) noexcept {
        ::up::slist_foreach<Node, NodePtr>(head->next, nullptr, func);
    }
    
    inline UPALWAYSINLINE
    void slist_erase(slist_node* head, slist_node* first, slist_node* last) noexcept {
        for (slist_node* node = first, * next; node != last; node = next) {
            next = node->next;
            node->next = nullptr;
        }
        head->next = last;
    }

    template <class Node, slist_node Node::* NodePtr, class Recycle>
    inline UPALWAYSINLINE
    typename enable_if<!is_convertible<Recycle, allocator*>::value>::type
    slist_erase(slist_node* head, slist_node* first, slist_node* last, Recycle recycle) noexcept {
        for (slist_node* node = first, * next; node != last; node = next) {
            next = node->next;
            node->next = nullptr;
            recycle(::up::slist_cast<Node*>(node, NodePtr));
        }
        head->next = last;
    }

    template <class Node, slist_node Node::* NodePtr>
    inline UPALWAYSINLINE
    void slist_erase(slist_node* head, slist_node* first, slist_node* last, allocator* alloc) noexcept {
        for (slist_node* node = first, * next; node != last; node = next) {
            next = node->next;
            alloc->deallocate(::up::slist_cast<Node*>(node, NodePtr), sizeof(Node));
        }
        head->next = last;
    }

    inline UPALWAYSINLINE
    void slist_clear(slist_node* head) noexcept {
        ::up::slist_erase(head, head->next, nullptr);
    }

    template <class Node, slist_node Node::* NodePtr, class Recycle>
    inline UPALWAYSINLINE
    typename enable_if<!is_convertible<Recycle, allocator*>::value>::type
    slist_clear(slist_node* head, Recycle recycle) noexcept {
        ::up::slist_erase<Node, NodePtr>(head, head->next, nullptr, recycle);
    }

    template <class Node, slist_node Node::* NodePtr>
    inline UPALWAYSINLINE
    void slist_clear(slist_node* head, allocator* alloc) noexcept {
        ::up::slist_erase<Node, NodePtr>(head, head->next, nullptr, alloc);
    }
    
    inline UPALWAYSINLINE
    void slist_unlink(slist_node* head, slist_node* node) noexcept {
        head->next = node->next;
        node->next = nullptr;
    }

    template <class Node, slist_node Node::* NodePtr, class Recycle>
    inline UPALWAYSINLINE
    typename enable_if<!is_convertible<Recycle, allocator*>::value>::type
    slist_unlink(slist_node* head, slist_node* node, Recycle recycle) noexcept {
        head->next = node->next;
        node->next = nullptr;
        recycle(node);
    }

    template <class Node, slist_node Node::* NodePtr>
    inline UPALWAYSINLINE
    void slist_unlink(slist_node* head, slist_node* node, allocator* alloc) noexcept {
        head->next = node->next;
        alloc->deallocate(::up::slist_cast<Node*>(node, NodePtr), sizeof(Node));
    }

    inline UPALWAYSINLINE
    void slist_link_before(slist_node* head, slist_node* node) noexcept {
        node->next = head->next;
        head->next = node;
    }

    inline UPALWAYSINLINE
    void slist_link_after(slist_node* head, slist_node* node) noexcept {
        head = head->next ? head->next : head;
        node->next = head->next;
        head->next = node;
    }

    inline UPALWAYSINLINE
    void slist_splice_before(slist_node* head, slist_node* first, slist_node* prev_last) noexcept {
        prev_last->next = head->next;
        head->next = first;
    }

    inline UPALWAYSINLINE
    void slist_splice_after(slist_node* head, slist_node* first, slist_node* prev_last) noexcept {
        head = head->next ? head->next : head;
        prev_last->next = head->next;
        head->next = first;
    }

    inline UPALWAYSINLINE
    void slist_push_front(slist_node* head, slist_node* node) noexcept {
        node->next = head->next;
        head->next = node;
    }

    inline UPALWAYSINLINE
    slist_node* slist_pop_front(slist_node* head) noexcept {
        slist_node* const front = head->next;
        head->next = front->next;
        front->next = nullptr;
        return front;
    }

    inline UPALWAYSINLINE
    slist_node* slist_peek_front(slist_node const* head) noexcept {
        return head->next;
    }

    inline UPALWAYSINLINE
    void slist_reverse(slist_node* head, slist_node* first, slist_node* last) noexcept {
        slist_node* node = first, * prev = last, * next;
        while (node != last) {
            next = node->next;
            node->next = prev;
            prev = node;
            node = next;
        }
        head->next = prev;
    }

    inline UPALWAYSINLINE
    void slist_reverse(slist_node* head) noexcept {
        ::up::slist_reverse(head, head->next, nullptr);
    }

    template <class Node, slist_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    void slist_merge(slist_node* head1, slist_node* head2, Compare comp) noexcept {
        if (head1 != head2) {
            slist_node* prev1 = head1;
            slist_node* node1 = head1->next;
            slist_node* node2 = head2->next;
            while ((node1 != nullptr) && (node2 != nullptr)) {
                if (comp(::up::slist_cast<Node const*>(node1, NodePtr), ::up::slist_cast<Node const*>(node2, NodePtr))) {
                    prev1 = node1;
                    node1 = node1->next;
                }
                else {
                    slist_node* next2 = node2->next;
                    ::up::slist_link_before(prev1, node2);
                    node2 = next2;
                }
            }
            if (node2) {
                ::up::slist_link_before(prev1, node2);
            }
            head2->next = nullptr;
        }
    }
    
    template <class Node, slist_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    bool slist_is_sorted(slist_node const* first, slist_node const* last, Compare comp) noexcept {
        if (first != last) {
            for (slist_node const* second = first->next; second != last; first = second, second = second->next) {
                if (!comp(::up::slist_cast<Node const*>(first, NodePtr), ::up::slist_cast<Node const*>(second, NodePtr))) {
                    return false;
                }
            }
        }
        return true;
    }

    template <class Node, slist_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    bool slist_is_sorted(slist_node const* head, Compare comp) noexcept {
        return ::up::slist_is_sorted<Node, NodePtr>(head->next, nullptr, comp);
    }

    template <class Node, slist_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    void slist_sort(slist_node* head, slist_node* first, slist_node* last, Compare comp) noexcept {
        slist_node* node = first, * new_first = last;
        while (node != last) {
            slist_node* next = node->next, * tail = new_first, ** tail_ptr = &new_first;
            while ((tail != last) && !comp(::up::slist_cast<Node const*>(node, NodePtr), ::up::slist_cast<Node const*>(tail, NodePtr))) {
                tail_ptr = &tail->next;
                tail = tail->next;
            }
            node->next = tail;
            *tail_ptr = node;
            node = next;
        }
        head->next = new_first;
    }

    template <class Node, slist_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    void slist_sort(slist_node* head, Compare comp) noexcept {
        ::up::slist_sort<Node, NodePtr>(head, head->next, nullptr, comp);
    }
}

#endif
