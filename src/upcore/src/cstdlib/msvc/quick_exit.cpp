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

#include <up/prolog.hpp>

#ifndef UP_HAS_STDC_UNSAFE_EXIT

#include <up/atomic.hpp>
#include <up/cassert.hpp>
#include <up/cstdlib.hpp>
#include <up/cthreads.hpp>
#include <up/slist.hpp>
#include <up/utility.hpp>

namespace up
{
    namespace
    {
        struct quick_exit_entry
        {
            slist_node node;
            void (UPCDECL*func)();
        };

        once_flag quick_exit_once_flag = ONCE_FLAG_INIT;
        atomic_flag quick_exit_manual = ATOMIC_FLAG_INIT;
        atomic_flag quick_exit_finished = ATOMIC_FLAG_INIT;
        mtx_t quick_exit_mutex;
        slist_node quick_exit_entries;

        void UPCDECL quick_exit_term() noexcept {
            if (atomic_flag_test_and_set(&quick_exit_finished)) {
                slist_clear<quick_exit_entry, &quick_exit_entry::node>(&quick_exit_entries, [] (quick_exit_entry* entry) {
                    UPTRY { (*entry->func)(); } UPCATCHANY_IGNORE();
                    free(entry);
                });
#ifndef UP_HAS_MSVC_XTHREADS
                if (atomic_flag_test_and_set(&quick_exit_manual)) {
                    // thread system is shut-down, causes access exception
                    mtx_destroy(&quick_exit_mutex);
                }
#endif
            }
        }

        void UPCDECL quick_exit_init() noexcept {
            slist_init(&quick_exit_entries);
            verify(thrd_success == mtx_init(&quick_exit_mutex, mtx_plain));
            verify(atexit(&quick_exit_term));
        }
    }

    LIBUPCOREAPI UPNONNULLALL
    void at_quick_exit(void (UPCDECL* func)()) noexcept {
        assert(func);
        
        quick_exit_entry* entry = static_cast<quick_exit_entry*>(malloc(sizeof(quick_exit_entry)));
        require(entry);

        call_once(&quick_exit_once_flag, &quick_exit_init);
        verify(thrd_success == mtx_lock(&quick_exit_mutex));
        slist_push_front(&quick_exit_entries, slist_cast<slist_node*>(entry, &quick_exit_entry::node));
        entry->func = func;
        atomic_thread_fence(memory_order_seq_cst);
        verify(thrd_success == mtx_unlock(&quick_exit_mutex));
    }
    
    LIBUPCOREAPI UPNORETURN
    void quick_exit(int status) noexcept {
        call_once(&quick_exit_once_flag, &quick_exit_init);
        if (atomic_flag_test_and_set(&quick_exit_manual)) {
            verify(thrd_success == mtx_lock(&quick_exit_mutex));
            quick_exit_term();
            verify(thrd_success == mtx_unlock(&quick_exit_mutex));
            atomic_flag_clear(&quick_exit_manual);
        }
        force_exit(status);
    }
}

#endif
