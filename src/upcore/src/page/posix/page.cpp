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
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>

namespace up
{
    namespace
    {
        inline UPALWAYSINLINE
        int get_protection_flags(unsigned int flags) noexcept {
            int prot = PROT_NONE;

            if (flags & page_access_read) {
                prot |= PROT_READ;
            }

            if (flags & page_access_write) {
                prot |= PROT_WRITE;
            }

            if (flags & page_access_execute) {
                prot |= PROT_EXEC;
            }

            return prot;    
        }

        inline UPALWAYSINLINE
        int page_size_compare(void const* p1, void const* p2) noexcept {
            return (*static_cast<size_t const*>(p1)) < (*static_cast<size_t const*>(p2));
        }

        int UPCDECL compare_sizes(void const* x, void const* y) noexcept {
            if (*static_cast<size_t const*>(x) < *static_cast<size_t const*>(y)) {
                return -1;
            }
            else if (*static_cast<size_t const*>(x) > *static_cast<size_t const*>(y)) {
                return 1;
            }
            return 0;
        }
    }

    LIBUPCOREAPI
    int page_get_metrics(page_metrics*) noexcept {
        errno = ENOSYS;
        return -1;
    }

    LIBUPCOREAPI
    int page_get_default_size(size_t* page_size) noexcept {
        if (!page_size) {
            errno = EINVAL;
            return -1;
        }

#if UP_BASESYSTEM == UP_BASESYSTEM_LINUX
        long default_page_size = ::sysconf(_SC_PAGESIZE);
        if (default_page_size <= 0) {
            return -1;
        }
#else
#   error "Platform not yet supported."
#endif

        *page_size = static_cast<size_t>(default_page_size);
        return 0;
    }

    LIBUPCOREAPI
    ssize_t page_get_sizes(size_t* page_sizes, size_t max_page_sizes) noexcept {
#if UP_BASESYSTEM == UP_BASESYSTEM_LINUX
        size_t count = 1;
        long default_page_size;
        unsigned long hugetlb_size;
        struct ::dirent entry_buffer, * entry;
        DIR* hugetlb_dir;
        char* unitptr;
        int result;

        // get regular page size
        default_page_size = ::sysconf(_SC_PAGESIZE);
        if (default_page_size <= 0) {
            return -1;
        }
        
        if (page_sizes && (max_page_sizes > 0)) {
            page_sizes[0] = static_cast<size_t>(default_page_size);
        }

        // read the hugepage sizes from sysfs
        hugetlb_dir = ::opendir("/sys/kernel/mm/hugepages");
        if (hugetlb_dir) {
            for (;;) {
                result = ::readdir_r(hugetlb_dir, &entry_buffer, &entry);
                if (!entry || (result != 0)) {
                    break;
                }

                // parse the page size for directory name
                if (strncmp(entry->d_name, "hugepages-", 10) != 0) {
                    continue;
                }

                hugetlb_size = fast_strtoul(&entry->d_name[10], &unitptr, 10);

                // ensure page size is power of two
                if (!hugetlb_size || ((hugetlb_size & (hugetlb_size - 1)) != 0)) {
                    continue;
                }

                // ensure entry name is suffixed with "kB"
                if (!unitptr || (&entry->d_name[10] == unitptr) || (strcmp(unitptr, "kB") != 0)) {
                    continue;
                }

                // record page size
                if (page_sizes && (count < max_page_sizes)) {
                    page_sizes[count] = static_cast<size_t>(hugetlb_size * 1024);
                }

                ++count;
            }

            result = ::closedir(hugetlb_dir);
            if (!result) {
                return -1;
            }
        }

        // sort and remove duplicates
        if (page_sizes) {
            count = count < max_page_sizes ? count : max_page_sizes;
            qsort(page_sizes, count, sizeof(size_t), &compare_sizes);
        }

        return count;
#else
#   error "Platform not yet supported."
#endif
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* page_allocate(size_t n, unsigned int flags) noexcept {
        int const prot = get_protection_flags(flags);

        if (flags & (page_option_huge_relaxed | page_option_huge_strict)) {
            void* result = ::mmap(nullptr, n, prot, MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, 0, 0);
            if (result || !(flags & page_option_huge_relaxed)) {
                return result;
            }
        }

        return ::mmap(nullptr, n, prot, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    }

    LIBUPCOREAPI
    int page_deallocate(void* p, size_t n) noexcept {
        if (!p || !n) {
            return 0;
        }
        return ::munmap(p, n);
    }

    LIBUPCOREAPI
    int page_protect(void* p, size_t n, unsigned int flags) noexcept {
        int const prot = get_protection_flags(flags);
        return ::mprotect(p, n, prot);
    }
}
