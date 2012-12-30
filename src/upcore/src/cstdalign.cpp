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

#include <up/cstdalign.hpp>
#include <up/cerrno.hpp>

namespace up
{
    LIBUPCOREAPI
    void* align(size_t alignment, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept {
        if (!ptr || !*ptr || !space || !alignment || (((alignment & (alignment - 1)) != 0))) {
            errno = EINVAL;
            return nullptr;
        }

        uintptr_t const address = reinterpret_cast<uintptr_t>(*ptr);
        uintptr_t const new_address = (address + (alignment - 1)) & ~(alignment - 1);
        size_t const used_bytes = new_address - address;
        
        if (*space < (size + used_bytes)) {
            errno = ENOMEM;
            return nullptr;
        }

        *ptr = reinterpret_cast<void*>(new_address);
        *space -= used_bytes;
        return *ptr;
    }

    LIBUPCOREAPI
    void* align(size_t alignment, size_t offset, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept {
        if (!ptr || !*ptr || !space || !alignment || (((alignment & (alignment - 1)) != 0))) {
            errno = EINVAL;
            return nullptr;
        }

        uintptr_t const address = reinterpret_cast<uintptr_t>(*ptr);
        uintptr_t const new_address = ((address + (alignment - 1) + offset) & ~(alignment - 1)) - offset;
        size_t const used_bytes = new_address - address;
        
        if (*space < (size + used_bytes)) {
            errno = ENOMEM;
            return nullptr;
        }

        *ptr = reinterpret_cast<void*>(new_address);
        *space -= used_bytes;
        return *ptr;
    }

    LIBUPCOREAPI
    void* align_bidirectional(size_t alignment, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept {
        if (!ptr || !*ptr || !space || !alignment || (((alignment & (alignment - 1)) != 0))) {
            errno = EINVAL;
            return nullptr;
        }

        uintptr_t const first_address = reinterpret_cast<uintptr_t>(*ptr);
        uintptr_t const head_address = (first_address + (alignment - 1)) & ~(alignment - 1);
        uintptr_t const last_address = head_address + *space;
        uintptr_t const tail_address = last_address & ~(alignment - 1);
        size_t const used_bytes = (head_address - first_address) + (last_address - tail_address);

        if (*space < (size + used_bytes)) {
            errno = ENOMEM;
            return nullptr;
        }

        *ptr = reinterpret_cast<void*>(head_address);
        *space -= used_bytes;
        return *ptr;
    }

    LIBUPCOREAPI
    void* align_bidirectional(size_t alignment, size_t offset, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept {
        if (!ptr || !*ptr || !space || !alignment || (((alignment & (alignment - 1)) != 0))) {
            errno = EINVAL;
            return nullptr;
        }

        uintptr_t const first_address = reinterpret_cast<uintptr_t>(*ptr);
        uintptr_t const head_address = ((first_address + (alignment - 1) + offset) & ~(alignment - 1)) - offset;
        uintptr_t const last_address = head_address + *space;
        uintptr_t const tail_address = (last_address & ~(alignment - 1)) - offset;
        size_t const used_bytes = (head_address - first_address) + (last_address - tail_address);

        if (*space < (size + used_bytes)) {
            errno = ENOMEM;
            return nullptr;
        }

        *ptr = reinterpret_cast<void*>(head_address);
        *space -= used_bytes;
        return *ptr;
    }
}
