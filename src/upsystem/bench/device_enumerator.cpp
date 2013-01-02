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

#include "device_enumerator.hpp"
#include <up/memory.hpp>

namespace up { namespace system { namespace bench
{
	device_enumerator::device_enumerator()
    : platform_index_(0),
    device_index_(UINT32_MAX),
    context_(nullptr) {
        clGetPlatformIDs(0, nullptr, &num_platforms_);
        platforms_ = up::malloc_n<cl_platform_id>(num_platforms_);
        clGetPlatformIDs(num_platforms_, platforms_, nullptr);
        clGetDeviceIDs(platforms_[platform_index_], CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices_);
        devices_ = up::malloc_n<cl_device_id>(num_devices_);
        clGetDeviceIDs(platforms_[platform_index_], CL_DEVICE_TYPE_ALL, num_devices_, devices_, nullptr);
    }

	device_enumerator::~device_enumerator() {
        clReleaseContext(context_);
        up::free_n(devices_, num_devices_);
        up::free_n(platforms_, num_platforms_);
    }
    
    char const* device_enumerator::device_name() const {
        if ((device_index_ < 0) || (device_index_ >= num_devices_)) {
            return nullptr;
        }

        clGetDeviceInfo(devices_[device_index_], CL_DEVICE_NAME, sizeof(device_name_) - 1, &device_name_[0], nullptr);
        device_name_[sizeof(device_name_) - 1] = '\0';
        return device_name_;
    }

    bool device_enumerator::next_device() {
        clReleaseContext(context_);
        context_ = nullptr;

        if (++device_index_ < num_devices_) {
            context_ = clCreateContext(nullptr, 1, &devices_[device_index_], nullptr, nullptr, nullptr);
            return true;
        }

        up::free_n(devices_, num_devices_);
        devices_ = nullptr;
        num_devices_ = 0;
        device_index_ = 0;

        if (++platform_index_ >= num_platforms_) {
            return false;
        }

        clGetDeviceIDs(platforms_[platform_index_], CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices_);
        devices_ = up::malloc_n<cl_device_id>(num_devices_);
        clGetDeviceIDs(platforms_[platform_index_], CL_DEVICE_TYPE_ALL, num_devices_, devices_, nullptr);
        context_ = clCreateContext(nullptr, 1, &devices_[device_index_], nullptr, nullptr, nullptr);
        return true;
    }
}}}
