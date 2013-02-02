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

#ifndef UP_FILESYSTEM_HPP
#define UP_FILESYSTEM_HPP

#include <up/cstdint.hpp>
#include <up/cstdio.hpp>
#include <up/ctime.hpp>
#ifdef UP_HAS_STDC_LOCALE
#   include <up/clocale.hpp>
#endif

#if UP_FILESYSTEM == UP_FILESYSTEM_NONE
#   error "Target platform does not have a file system!"
#endif

namespace up { namespace filesystem
{
    enum copy_action
    {
        copy_continue,
        copy_skip,
        copy_quit
    };

    enum copy_option
    {
        copy_no_overwrite,
        copy_overwrite
    };

    enum status_type
    {
        status_invalid,
        status_regular,
        status_directory,
        status_symlink,
        status_block,
        status_character,
        status_fifo,
        status_socket,
        status_unknown
    };

    enum symlink_option
    {
        symlink_no_recurse,
        symlink_recurse
    };

    struct LIBUPCOREAPI space_info
    {
        uintmax_t capacity;
        uintmax_t free;
        uintmax_t available;
    };

    template <class T>
    struct UPVISIBLE split_result
    {
        T const* dirname;
        size_t dirname_length;
        T const* basename;
        size_t basename_length;
    };

    struct LIBUPCOREAPI status_info
    {
        status_type type;
        uintmax_t file_size;
        uintmax_t hard_link_count;
        timespec last_access_time;
        timespec last_write_time;

        UPALWAYSINLINE
        bool exists() noexcept {
            return type != status_invalid;
        }

        UPALWAYSINLINE
        bool is_directory() noexcept {
            return type == status_directory;
        }
        
        UPALWAYSINLINE
        bool is_other() noexcept {
            return (type != status_invalid) && (type != status_directory)
                && (type != status_regular) && (type != status_symlink);
        }

        UPALWAYSINLINE
        bool is_regular() noexcept {
            return type == status_regular;
        }

        UPALWAYSINLINE
        bool is_symlink() noexcept {
            return type == status_symlink;
        }
    };
}}

namespace up { namespace filesystem { namespace posix
{
    template <class T>
    struct path;

    template <>
    struct LIBUPCOREAPI path<char>
    {
        static constexpr size_t empty_length = 0;
        static constexpr size_t curdir_length = 1;
        static constexpr size_t pardir_length = 2;
        static constexpr size_t sep_length = 1;
        static constexpr size_t altsep_length = 0;
        static constexpr size_t sepset_length = 1;
        static constexpr size_t extsep_length = 1;
        static constexpr size_t pathsep_length = 1;
        static char const empty[1];
        static char const curdir[2];
        static char const pardir[3];
        static char const sep[2];
        static char const altsep[1];
        static char const sepset[2];
        static char const extsep[2];
        static char const pathsep[2];
    };

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* basename(char const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* basename(char const* p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* dirname(char const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* dirname(char const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int is_absolute_path(char const* UPRESTRICT p, bool* UPRESTRICT result) noexcept;    
    extern LIBUPCOREAPI int is_absolute_path(char const* UPRESTRICT p, size_t n, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(char* UPRESTRICT d, size_t dsz, char const* UPRESTRICT first, ...) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(char* UPRESTRICT d, size_t dsz, char const* const* UPRESTRICT args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* join_path(char const* first, ...) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* join_path(char const* const* args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(char* UPRESTRICT d, size_t dsz, char const* UPRESTRICT p) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(char* UPRESTRICT d, size_t dsz, char const* UPRESTRICT p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* normalize_path(char const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* normalize_path(char const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int split_path(char const* UPRESTRICT p, split_result<char>* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI int split_path(char const* UPRESTRICT p, size_t n, split_result<char>* UPRESTRICT result) noexcept;

#ifdef UP_HAS_STDC_WCHAR
    template <>
    struct LIBUPCOREAPI path<wchar_t>
    {
        static constexpr size_t empty_length = 0;
        static constexpr size_t curdir_length = 1;
        static constexpr size_t pardir_length = 2;
        static constexpr size_t sep_length = 1;
        static constexpr size_t altsep_length = 0;
        static constexpr size_t sepset_length = 1;
        static constexpr size_t extsep_length = 1;
        static constexpr size_t pathsep_length = 1;
        static wchar_t const empty[1];
        static wchar_t const curdir[2];
        static wchar_t const pardir[3];
        static wchar_t const sep[2];
        static wchar_t const altsep[1];
        static wchar_t const sepset[2];
        static wchar_t const extsep[2];
        static wchar_t const pathsep[2];
    };

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* basename(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* basename(wchar_t const* p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* dirname(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* dirname(wchar_t const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int is_absolute_path(wchar_t const* UPRESTRICT p, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI int is_absolute_path(wchar_t const* UPRESTRICT p, size_t n, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT first, ...) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* const* UPRESTRICT args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* join_path(wchar_t const* first, ...) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* join_path(wchar_t const* const* args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT p) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* normalize_path(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* normalize_path(wchar_t const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int split_path(wchar_t const* UPRESTRICT p, split_result<wchar_t>* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI int split_path(wchar_t const* UPRESTRICT p, size_t n, split_result<wchar_t>* UPRESTRICT result) noexcept;
#endif
}}}

namespace up { namespace filesystem { namespace windows
{
    template <class T>
    struct path;

    template <>
    struct LIBUPCOREAPI path<char>
    {
        static constexpr size_t empty_length = 0;
        static constexpr size_t curdir_length = 1;
        static constexpr size_t pardir_length = 2;
        static constexpr size_t sep_length = 1;
        static constexpr size_t altsep_length = 1;
        static constexpr size_t sepset_length = 2;
        static constexpr size_t extsep_length = 1;
        static constexpr size_t pathsep_length = 1;
        static char const empty[1];
        static char const curdir[2];
        static char const pardir[3];
        static char const sep[2];
        static char const altsep[2];
        static char const sepset[3];
        static char const extsep[2];
        static char const pathsep[2];
    };

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* basename(char const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* basename(char const* p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* dirname(char const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* dirname(char const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int is_absolute_path(char const* UPRESTRICT p, bool* UPRESTRICT result) noexcept;    
    extern LIBUPCOREAPI int is_absolute_path(char const* UPRESTRICT p, size_t n, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(char* UPRESTRICT d, size_t dsz, char const* UPRESTRICT first, ...) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(char* UPRESTRICT d, size_t dsz, char const* const* UPRESTRICT args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* join_path(char const* first, ...) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* join_path(char const* const* args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(char* UPRESTRICT d, size_t dsz, char const* UPRESTRICT p) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(char* UPRESTRICT d, size_t dsz, char const* UPRESTRICT p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* normalize_path(char const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* normalize_path(char const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int split_path(char const* UPRESTRICT p, split_result<char>* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI int split_path(char const* UPRESTRICT p, size_t n, split_result<char>* UPRESTRICT result) noexcept;

#ifdef UP_HAS_STDC_WCHAR
    template <>
    struct LIBUPCOREAPI path<wchar_t>
    {
        static constexpr size_t empty_length = 0;
        static constexpr size_t curdir_length = 1;
        static constexpr size_t pardir_length = 2;
        static constexpr size_t sep_length = 1;
        static constexpr size_t altsep_length = 1;
        static constexpr size_t sepset_length = 2;
        static constexpr size_t extsep_length = 1;
        static constexpr size_t pathsep_length = 1;
        static wchar_t const empty[1];
        static wchar_t const curdir[2];
        static wchar_t const pardir[3];
        static wchar_t const sep[2];
        static wchar_t const altsep[2];
        static wchar_t const sepset[3];
        static wchar_t const extsep[2];
        static wchar_t const pathsep[2];
    };

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* basename(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* basename(wchar_t const* p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* dirname(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* dirname(wchar_t const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int is_absolute_path(wchar_t const* UPRESTRICT p, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI int is_absolute_path(wchar_t const* UPRESTRICT p, size_t n, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT first, ...) noexcept;
    extern LIBUPCOREAPI ssize_t join_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* const* UPRESTRICT args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* join_path(wchar_t const* first, ...) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* join_path(wchar_t const* const* args, size_t arg_count) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT p) noexcept;
    extern LIBUPCOREAPI ssize_t normalize_path(wchar_t* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT p, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* normalize_path(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* normalize_path(wchar_t const* p, size_t n) noexcept;
    extern LIBUPCOREAPI int split_path(wchar_t const* UPRESTRICT p, split_result<wchar_t>* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI int split_path(wchar_t const* UPRESTRICT p, size_t n, split_result<wchar_t>* UPRESTRICT result) noexcept;
#endif
}}}

namespace up { namespace filesystem { namespace detail
{
    template <class Path>
    struct observer_operations;

    template <>
    struct LIBUPCOREAPI observer_operations<char>
    {
        static UPALLOC UPWARNRESULT char* current_directory_path() noexcept;
        static UPALLOC UPWARNRESULT char* temp_directory_path() noexcept;
        static UPALLOC UPWARNRESULT char* user_directory_path() noexcept;
    };

#ifdef UP_HAS_STDC_WCHAR
    template <>
    struct LIBUPCOREAPI observer_operations<wchar_t>
    {
        static UPALLOC UPWARNRESULT wchar_t* current_directory_path() noexcept;
        static UPALLOC UPWARNRESULT wchar_t* temp_directory_path() noexcept;
        static UPALLOC UPWARNRESULT wchar_t* user_directory_path() noexcept;
    };
#endif
}}}

namespace up { namespace filesystem
{
#if UP_FILESYSTEM == UP_FILESYSTEM_POSIX
    using namespace ::up::filesystem::posix;
#elif UP_FILESYSTEM == UP_FILESYSTEM_WINDOWS
    using namespace ::up::filesystem::windows;
#endif

    using ::remove;
    using ::rename;
    using ::tmpnam;

#ifdef UP_HAS_STDC_LOCALE
    extern LIBUPCOREAPI UPWARNRESULT locale_t get_transcode_locale() noexcept;
#endif

#ifdef UP_HAS_STDC_WCHAR
    extern LIBUPCOREAPI ssize_t transcode(char* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT s) noexcept;
    extern LIBUPCOREAPI ssize_t transcode(char* UPRESTRICT d, size_t dsz, wchar_t const* UPRESTRICT s, size_t n) noexcept;
    extern LIBUPCOREAPI ssize_t transcode(wchar_t* UPRESTRICT d, size_t dsz, char const* UPRESTRICT s) noexcept;
    extern LIBUPCOREAPI ssize_t transcode(wchar_t* UPRESTRICT d, size_t dsz, char const* UPRESTRICT s, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* transcode(wchar_t const* s) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* transcode(wchar_t const* s, size_t n) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* transcode(char const* s) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* transcode(char const* s, size_t n) noexcept;
#endif

    template <class Path>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    Path* current_directory_path() noexcept {
        return ::up::filesystem::detail::observer_operations<Path>::current_directory_path();
    }

    template <class Path>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    Path* temp_directory_path() noexcept {
        return ::up::filesystem::detail::observer_operations<Path>::temp_directory_path();
    }

    template <class Path>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    Path* unique_path() noexcept {
        return ::up::filesystem::detail::observer_operations<Path>::unique_path();
    }

    template <class Path>
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    Path* user_directory_path() noexcept {
        return ::up::filesystem::detail::observer_operations<Path>::user_directory_path();
    }

    extern LIBUPCOREAPI ssize_t absolute_path(char* d, size_t dsz, char const* p, char const* base = nullptr) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* absolute_path(char const* p, char const* base = nullptr) noexcept;
    /*extern LIBUPCOREAPI int copy(char const* from, char const* to) noexcept;
    extern LIBUPCOREAPI int copy_directory(char const* from, char const* to) noexcept;*/
    extern LIBUPCOREAPI int copy_file(char const* from, char const* to, copy_option option = copy_no_overwrite) noexcept;
    extern LIBUPCOREAPI int copy_symlink(char const* existing_symlink, char const* new_symlink) noexcept;
    extern LIBUPCOREAPI int create_directories(char const* p) noexcept;
    extern LIBUPCOREAPI int create_directory(char const* p) noexcept;
    extern LIBUPCOREAPI int create_directory_symlink(char const* to, char const* new_symlink) noexcept;
    extern LIBUPCOREAPI int create_hard_link(char const* to, char const* new_hard_link) noexcept;
    extern LIBUPCOREAPI int create_symlink(char const* to, char const* new_symlink) noexcept;
    extern LIBUPCOREAPI int current_directory_path(char const* p) noexcept;
    extern LIBUPCOREAPI int equivalent(char const* UPRESTRICT p1, char const* UPRESTRICT p2) noexcept;
    extern LIBUPCOREAPI int exists(char const* UPRESTRICT p, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT char* read_symlink(char const* p) noexcept;
    extern LIBUPCOREAPI uintmax_t remove_all(char const* p) noexcept;
    extern LIBUPCOREAPI int resize(char const* p, uintmax_t new_size) noexcept;
    extern LIBUPCOREAPI int space(char const* UPRESTRICT p, space_info* UPRESTRICT info) noexcept;
    extern LIBUPCOREAPI int status(char const* UPRESTRICT p, status_info* UPRESTRICT info) noexcept;
    extern LIBUPCOREAPI int symlink_status(char const* UPRESTRICT p, status_info* UPRESTRICT info) noexcept;
    extern LIBUPCOREAPI int touch(char const* UPRESTRICT p, timespec const* UPRESTRICT ts = nullptr) noexcept;

#ifdef UP_HAS_STDC_WCHAR
    extern LIBUPCOREAPI ssize_t absolute_path(wchar_t* d, size_t dsz, wchar_t const* p, wchar_t const* base = nullptr) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* absolute_path(wchar_t const* p, wchar_t const* base = nullptr) noexcept;
    extern LIBUPCOREAPI int copy_file(wchar_t const* from, wchar_t const* to, copy_option option = copy_no_overwrite) noexcept;
    extern LIBUPCOREAPI int copy_symlink(wchar_t const* existing_symlink, wchar_t const* new_symlink) noexcept;
    extern LIBUPCOREAPI int create_directories(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI int create_directory(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI int create_directory_symlink(wchar_t const* to, wchar_t const* new_symlink) noexcept;
    extern LIBUPCOREAPI int create_hard_link(wchar_t const* to, wchar_t const* new_hard_link) noexcept;
    extern LIBUPCOREAPI int create_symlink(wchar_t const* to, wchar_t const* new_symlink) noexcept;
    extern LIBUPCOREAPI int current_directory_path(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI int equivalent(wchar_t const* UPRESTRICT p1, wchar_t const* UPRESTRICT p2) noexcept;
    extern LIBUPCOREAPI int exists(wchar_t const* UPRESTRICT p, bool* UPRESTRICT result) noexcept;
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT wchar_t* read_symlink(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI uintmax_t remove_all(wchar_t const* p) noexcept;
    extern LIBUPCOREAPI int resize(wchar_t const* p, uintmax_t new_size) noexcept;
    extern LIBUPCOREAPI int space(wchar_t const* UPRESTRICT p, space_info* UPRESTRICT info) noexcept;
    extern LIBUPCOREAPI int status(wchar_t const* UPRESTRICT p, status_info* UPRESTRICT info) noexcept;
    extern LIBUPCOREAPI int symlink_status(wchar_t const* UPRESTRICT p, status_info* UPRESTRICT info) noexcept;
    extern LIBUPCOREAPI int touch(wchar_t const* UPRESTRICT p, timespec const* UPRESTRICT ts = nullptr) noexcept;
#endif
}}

#endif
