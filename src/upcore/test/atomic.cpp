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

#include <up/atomic.hpp>
#include <up/cthreads.hpp>
#include <up/cstring.hpp>
#include <up/ctime.hpp>
#include <up/log.hpp>
#include <up/test.hpp>

namespace atomic
{
    struct alignas(UP_MAX_CACHE_LINE_SIZE) context_data
    {
        up::atomic_flag flag;
        char cache_line_pad1[UP_MAX_CACHE_LINE_SIZE - sizeof(up::atomic_flag)];
        up::atomic<unsigned long> acquire_count;
        unsigned long max_acquire_count;
        unsigned long lock_count;
        char cache_line_pad2[UP_MAX_CACHE_LINE_SIZE - sizeof(unsigned long[3])];
    };

    constexpr size_t spin_lock_iterations = 100000;

    int spin_lock_worker(void* arg) {
        context_data* data = reinterpret_cast<context_data*>(arg);
        unsigned long acq_count;
                
        for (size_t i = 0; i < spin_lock_iterations; ++i) {
            up::atomic_flag_spin_lock_explicit(&data->flag, up::memory_order_acquire);
            ++data->lock_count;
            acq_count = 1 + up::atomic_fetch_add_explicit(&data->acquire_count, 1, up::memory_order_relaxed);
            if (acq_count > data->max_acquire_count) {
                data->max_acquire_count = acq_count;
            }
            up::atomic_fetch_sub_explicit(&data->acquire_count, 1, up::memory_order_relaxed);
            up::atomic_flag_clear_explicit(&data->flag, up::memory_order_release);
        }

        up::atomic_thread_fence(up::memory_order_seq_cst);
        return 0;
    }

    int spin_lock_backoff_worker(void* arg) {
        context_data* data = reinterpret_cast<context_data*>(arg);
        unsigned long acq_count;
                
        for (size_t i = 0; i < spin_lock_iterations; ++i) {
            up::atomic_flag_spin_lock_backoff_explicit(&data->flag, up::memory_order_acquire);
            ++data->lock_count;
            acq_count = 1 + up::atomic_fetch_add_explicit(&data->acquire_count, 1, up::memory_order_relaxed);
            if (acq_count > data->max_acquire_count) {
                data->max_acquire_count = acq_count;
            }
            up::atomic_fetch_sub_explicit(&data->acquire_count, 1, up::memory_order_relaxed);
            up::atomic_flag_clear_explicit(&data->flag, up::memory_order_release);
        }

        up::atomic_thread_fence(up::memory_order_seq_cst);
        return 0;
    }

    UP_TEST_CASE(spin_lock) {
        up::atomic_flag flag = ATOMIC_FLAG_INIT;
        up::atomic_flag_spin_lock(&flag);
        up::atomic_flag_clear(&flag);
        up::atomic_flag_spin_lock_explicit(&flag, up::memory_order_acquire);
        up::atomic_flag_clear_explicit(&flag, up::memory_order_release);

        context_data data;
        up::atomic_flag_clear_explicit(&data.flag, up::memory_order_relaxed);
        data.acquire_count.store(0, up::memory_order_relaxed);
        data.max_acquire_count = 0;
        data.lock_count = 0;

        up::timespec start, end;
        require(0 == up::clock_gettime(CLOCK_MONOTONIC, &start));

        up::thrd_t threads[32];
        up::memset(&threads, 0, sizeof(threads));

        for (size_t i = 0; i < 32; ++i) {
            up::thrd_create(&threads[i], &spin_lock_worker, &data);
        }

        for (size_t i = 0; i < 32; ++i) {
            int res = 0x8b8b8b8b;
            up::thrd_join(threads[i], &res);
            require(res == 0);
        }

        require(0 == up::clock_gettime(CLOCK_MONOTONIC, &end));
        up::log_eventf(up::log_level_info, "Spin Lock: %.6fs\n", up::timespec_diff(&start, &end));

        up::atomic_thread_fence(up::memory_order_seq_cst);
        require(data.max_acquire_count == 1);
        require(data.lock_count == (32 * spin_lock_iterations));
    }

    UP_TEST_CASE(spin_lock_backoff) {
        up::atomic_flag flag = ATOMIC_FLAG_INIT;
        up::atomic_flag_spin_lock_backoff(&flag);
        up::atomic_flag_clear(&flag);
        up::atomic_flag_spin_lock_backoff_explicit(&flag, up::memory_order_acquire);
        up::atomic_flag_clear_explicit(&flag, up::memory_order_release);

        context_data data;
        up::atomic_flag_clear_explicit(&data.flag, up::memory_order_relaxed);
        data.acquire_count.store(0, up::memory_order_relaxed);
        data.max_acquire_count = 0;
        data.lock_count = 0;

        up::timespec start, end;
        require(0 == up::clock_gettime(CLOCK_MONOTONIC, &start));

        up::thrd_t threads[32];
        up::memset(&threads, 0, sizeof(threads));

        for (size_t i = 0; i < 32; ++i) {
            up::thrd_create(&threads[i], &spin_lock_backoff_worker, &data);
        }

        for (size_t i = 0; i < 32; ++i) {
            int res = 0x8b8b8b8b;
            up::thrd_join(threads[i], &res);
            require(res == 0);
        }

        require(0 == up::clock_gettime(CLOCK_MONOTONIC, &end));
        up::log_eventf(up::log_level_info, "Spin Lock w/ Exponential Backoff: %.6fs\n", up::timespec_diff(&start, &end));

        up::atomic_thread_fence(up::memory_order_seq_cst);
        require(data.max_acquire_count == 1);
        require(data.lock_count == (32 * spin_lock_iterations));
    }

    struct pod
    {
        long long a, b, c, d;
    };

    UP_TEST_CASE(pod) {
        up::atomic<pod> p;
        pod x = { 1, 2, 3, 4 };
        pod y = { -1, -2, -3, -4 };
        pod z, w, a, b;

        require(!p.is_lock_free());

        p.store(x);
        z = p.load();
        
        p.store(y, up::memory_order_release);
        w = p.load(up::memory_order_consume);

        require(0 == up::memcmp(&x, &z, sizeof(pod)));
        require(0 == up::memcmp(&y, &w, sizeof(pod)));

        require(p.compare_exchange_strong(w, x));
        a = p.load();

        require(p.compare_exchange_strong(z, y, up::memory_order_acq_rel));
        b = p.load(up::memory_order_acquire);

        require(0 == up::memcmp(&x, &z, sizeof(pod)));
        require(0 == up::memcmp(&y, &w, sizeof(pod)));
        require(0 == up::memcmp(&x, &a, sizeof(pod)));
        require(0 == up::memcmp(&y, &b, sizeof(pod)));
    }
}
