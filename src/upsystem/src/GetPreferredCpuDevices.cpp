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

#include <up/system.hpp>
#include <up/cstdlib.hpp>

namespace up { namespace system
{
    LIBUPSYSTEMAPI
    cl_int GetPreferredCpuDevices(cl_uint max_devices, cl_device_id* UPRESTRICT devices, cl_uint* UPRESTRICT num_devices) noexcept {
        cl_uint i, num_platforms, num_platform_devices, total_platform_devices;
        cl_platform_id* platforms = nullptr;
        cl_device_id* platform_devices = nullptr;
        cl_int retval;

        // load
        retval = clGetPlatformIDs(0, nullptr, &num_platforms);
        if (retval != CL_SUCCESS) {
            return retval;
        }

        platforms = static_cast<cl_platform_id*>(malloca(sizeof(cl_platform_id) * num_platforms));
        if (!platforms) {
            return CL_OUT_OF_HOST_MEMORY;
        }

        retval = clGetPlatformIDs(num_platforms, platforms, nullptr);
        if (retval != CL_SUCCESS) {
            goto cleanup;
        }

        // count number of CPU devices across all platforms
        for (i = 0, total_platform_devices = 0; i < num_platforms; ++i, total_platform_devices += num_platform_devices) {
            retval = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, 0, nullptr, &num_platform_devices);
            if (retval == CL_DEVICE_NOT_FOUND) {
                continue;
            }
            else if (retval != CL_SUCCESS) {
                goto cleanup;
            }
        }

        platform_devices = static_cast<cl_device_id*>(malloca(sizeof(cl_device_id) * total_platform_devices));
        if (!platform_devices) {
            return CL_OUT_OF_HOST_MEMORY;
        }

        UPIGNORE(num_devices);
        UPIGNORE(devices);
        UPIGNORE(max_devices);

    cleanup:

        freea(platform_devices);
        freea(platforms);
        return retval;
    }
}}
