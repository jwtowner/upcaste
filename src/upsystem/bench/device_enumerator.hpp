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

#ifndef UP_SYSTEM_BENCH_DEVICE_ENUMERATOR_HPP
#define UP_SYSTEM_BENCH_DEVICE_ENUMERATOR_HPP

#include <up/system.hpp>

namespace up { namespace system { namespace bench
{
    class device_enumerator
	{
    public:

		device_enumerator();
		~device_enumerator();
        cl_context context() const;
        cl_device_id device() const;
        char const* device_name() const;
        bool next_device();
        void reset();

    private:

		cl_uint platform_index_;
        cl_uint device_index_;
        cl_uint num_platforms_;
        cl_uint num_devices_;
        cl_platform_id* platforms_;
        cl_device_id* devices_;
        cl_context context_;
        mutable char device_name_[256];
    };

    inline
    cl_context device_enumerator::context() const {
        return ((device_index_ >= 0) && (device_index_ < num_devices_)) ? context_ : nullptr;
    }

    inline
    cl_device_id device_enumerator::device() const {
        return ((device_index_ >= 0) && (device_index_ < num_devices_)) ? devices_[device_index_] : nullptr;
    }
}}}

#endif
