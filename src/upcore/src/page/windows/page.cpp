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

#include <up/page.hpp>
#include <up/cassert.hpp>
#include <up/cerrno.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

namespace up
{
    namespace
    {
        unsigned long get_allocation_type(unsigned int flags) noexcept {
            unsigned long result = MEM_COMMIT | MEM_RESERVE;
            if (flags & (page_option_huge_relaxed | page_option_huge_strict)) {
                result |= MEM_LARGE_PAGES;
            }
            return result;
        }

        unsigned long get_protection_flags(unsigned int flags) noexcept {
            unsigned long result;

            switch (flags & 0x07) {
            case page_access_read: result = PAGE_READONLY; break;
            case page_access_write: result = PAGE_WRITECOPY; break;
            case page_access_read | page_access_write: result = PAGE_READWRITE; break;
            case page_access_execute: result = PAGE_EXECUTE; break;
            case page_access_execute | page_access_read: result = PAGE_EXECUTE_READ; break;
            case page_access_execute | page_access_write: result = PAGE_EXECUTE_WRITECOPY; break;
            case page_access_execute | page_access_read | page_access_write: result = PAGE_EXECUTE_READWRITE; break;
            default: result = PAGE_NOACCESS; break;
            }

            return result;    
        }
    }

    LIBUPCOREAPI
    int page_get_metrics(page_metrics* metrics) noexcept {
#if NTDDI_VERSION >= NTDDI_VISTA
        PROCESS_MEMORY_COUNTERS_EX counters;
#else
        PROCESS_MEMORY_COUNTERS counters;
#endif
        HANDLE process;

        if (!metrics) {
            errno = EINVAL;
            return -1;
        }
        
        process = ::GetCurrentProcess();
        if (!process) {
            set_errno_with_last_oserror();
            return -1;
        }
        
        memset(&counters, 0, sizeof(counters));
        counters.cb = sizeof(counters);
        if (!::GetProcessMemoryInfo(process, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters))) {
            set_errno_with_last_oserror();
            return -1;
        }

        metrics->resident_set_size = counters.WorkingSetSize;
        metrics->peak_resident_set_size = counters.PeakWorkingSetSize;
#if NTDDI_VERSION >= NTDDI_VISTA
        metrics->page_usage = counters.PrivateUsage;
#else
        metrics->page_usage = counters.PagefileUsage;
#endif
        metrics->peak_page_usage = counters.PeakPagefileUsage;
        return 0;
    }

    LIBUPCOREAPI
    int page_get_default_size(size_t* page_size) noexcept {
        SYSTEM_INFO info;

        if (!page_size) {
            errno = EINVAL;
            return -1;
        }

        ::GetSystemInfo(&info);
        *page_size = info.dwPageSize;
        return 0;
    }

    LIBUPCOREAPI
    ssize_t page_get_sizes(size_t* page_sizes, size_t max_page_sizes) noexcept {
        SYSTEM_INFO info;
        
        if (!page_sizes && max_page_sizes) {
            errno = EINVAL;
            return -1;
        }

        if (max_page_sizes > 0) {
            ::GetSystemInfo(&info);
            page_sizes[0] = info.dwPageSize;
        }

#if NTDDI_VERSION >= NTDDI_VISTA
        if (max_page_sizes > 1) {
            page_sizes[1] = ::GetLargePageMinimum();
        }

        return (page_sizes && (max_page_sizes <= 2)) ? max_page_sizes : 2;
#else
        return (page_sizes && (max_page_sizes <= 1)) ? max_page_sizes : 1;
#endif
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* page_allocate(size_t n, unsigned int flags) noexcept {
        unsigned long const alloc_type = get_allocation_type(flags);
        unsigned long const prot = get_protection_flags(flags);
        void* result;
        
        result = ::VirtualAlloc(nullptr, n, alloc_type, prot);
        if (result) {
            return result;
        }

        if ((flags & page_option_huge_strict) == page_option_huge_relaxed) {
            result = ::VirtualAlloc(nullptr, n, MEM_COMMIT | MEM_RESERVE, prot);
            if (result) {
                return result;
            }
        }
        
        set_errno_with_last_oserror();
        return nullptr;
    }

    LIBUPCOREAPI
    int page_deallocate(void* p, size_t n) noexcept {
        if (!p || !n) {
            return 0;
        }

        if (!::VirtualFree(p, 0, MEM_RELEASE)) {
            set_errno_with_last_oserror();
            return -1;
        }

        return 0;
    }

    LIBUPCOREAPI
    int page_protect(void* p, size_t n, unsigned int flags) noexcept {
        unsigned long const prot = get_protection_flags(flags);
        unsigned long old_prot = 0;

        if (!::VirtualProtect(p, n, prot, &old_prot)) {
            set_errno_with_last_oserror();
            return -1;
        }

        return 0;
    }
}
