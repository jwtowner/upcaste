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

#include "device_enumerator.hpp"
#include <up/cstdatomic.hpp>
#include <up/cstdio.hpp>
#include <up/ctime.hpp>
#include <up/page.hpp>
#include <up/test.hpp>
#include <random>

namespace create_buffer
{
    using up::system::bench::device_enumerator;
    
    constexpr size_t buffer_size = 1024 * 1024;
    constexpr size_t buffer_count = 32;

    struct buffer_fixture
	{
        cl_context context;
        cl_mem_flags mem_flags;
        void* host_data;
        size_t host_data_length;

		explicit buffer_fixture(size_t n)
        : mem_flags(CL_MEM_READ_WRITE),
        host_data_length(n) {
            host_data = up::page_allocate(host_data_length, up::page_access_read | up::page_access_write);
        }

		~buffer_fixture() {
            up::page_deallocate(host_data, host_data_length);
        }

        void setup() { }
        void teardown() { }
	};

    UPNOINLINE
    void default_allocate(buffer_fixture& fixture) {
        cl_mem buffers[buffer_count];

        for (size_t i = 0; i < buffer_count; ++i) {
            buffers[i] = clCreateBuffer(fixture.context, fixture.mem_flags, buffer_size, nullptr, nullptr);
        }

        for (size_t i = 0; i < buffer_count; ++i) {
            clReleaseMemObject(buffers[i]);
        }
    }

    UPNOINLINE
    void host_allocate(buffer_fixture& fixture) {
        cl_mem buffers[buffer_count];

        for (size_t i = 0; i < buffer_count; ++i) {
            buffers[i] = clCreateBuffer(fixture.context, fixture.mem_flags | CL_MEM_ALLOC_HOST_PTR, buffer_size, nullptr, nullptr);
        }

        for (size_t i = 0; i < buffer_count; ++i) {
            clReleaseMemObject(buffers[i]);
        }
    }

    UPNOINLINE
    void use_host_allocate(buffer_fixture& fixture) {
        cl_mem buffers[buffer_count];

        for (size_t i = 0; i < buffer_count; ++i) {
            buffers[i] = clCreateBuffer(fixture.context, fixture.mem_flags | CL_MEM_USE_HOST_PTR, buffer_size, ((char*)fixture.host_data) + (i * buffer_size), nullptr);
        }

        for (size_t i = 0; i < buffer_count; ++i) {
            clReleaseMemObject(buffers[i]);
        }
    }

    UP_TEST_BENCHMARK(buffer_strategy) {
        unsigned int const iterations = 1000;
        unsigned int const size = buffer_count * buffer_size;
        char category[256];
        char const* name;

        buffer_fixture fixture(size);
        device_enumerator device;

        fixture.mem_flags = CL_MEM_READ_WRITE;

        while (device.next_device()) {
            fixture.context = device.context();
            name = device.device_name();

            up::snprintf(category, sizeof(category) - 1, "%s - default read/write", name);
            UP_TEST_RUN_BENCHMARK(category, size, iterations, &default_allocate, fixture);

            up::snprintf(category, sizeof(category) - 1, "%s - allochost read/write", name);
            UP_TEST_RUN_BENCHMARK(category, size, iterations, &host_allocate, fixture);

            up::snprintf(category, sizeof(category) - 1, "%s - usehost read/write", name);
            UP_TEST_RUN_BENCHMARK(category, size, iterations, &use_host_allocate, fixture);
        }
    }
}
