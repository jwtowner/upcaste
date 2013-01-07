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

#ifndef UP_CSTDIO_HPP
#define UP_CSTDIO_HPP

#include <up/cstddef.hpp>
#include <stdio.h>

namespace up
{
    using ::FILE;
    using ::fpos_t;
    using ::clearerr;
    using ::fclose;
    using ::feof;
    using ::ferror;
    using ::fflush;
    using ::fgetc;
    using ::fgetpos;
    using ::fgets;
    using ::fopen;
    using ::fputc;
    using ::fputs;
    using ::fread;
    using ::freopen;
    using ::fseek;
    using ::fsetpos;
    using ::fwrite;
    using ::getc;
    using ::getchar;
    using ::gets;
    using ::perror;
    using ::putc;
    using ::putchar;
    using ::puts;
    using ::remove;
    using ::rename;
    using ::rewind;
    using ::setbuf;
    using ::setvbuf;
    using ::tmpfile;
    using ::tmpnam;
    using ::ungetc;
    using ::printf;
    using ::fprintf;
    using ::sprintf;
    using ::vprintf;
    using ::fprintf;
    using ::vsprintf;
    using ::scanf;
    using ::fscanf;
    using ::sscanf;

#ifndef UP_HAS_POSIX_GETDELIM
    extern LIBUPCOREAPI UPNONNULL(4)
    ssize_t getdelim(char** UPRESTRICT lineptr, size_t* UPRESTRICT n, int delimiter, FILE* UPRESTRICT stream);
#endif

#ifndef UP_HAS_POSIX_GETLINE
    extern LIBUPCOREAPI UPNONNULL(3)
    ssize_t getline(char** UPRESTRICT lineptr, size_t* UPRESTRICT n, FILE* UPRESTRICT stream);
#endif

#ifndef UP_HAS_POSIX_ASPRINTF
    extern LIBUPCOREAPI UPPRINTF(2,3) UPWARNRESULT
    int asprintf(char** UPRESTRICT strp, char const* UPRESTRICT format, ...) noexcept;
#else
    using ::asprintf;
#endif

#ifndef UP_HAS_POSIX_VASPRINTF
    extern LIBUPCOREAPI UPPRINTF(2,0) UPWARNRESULT
    int vasprintf(char** UPRESTRICT strp, char const* UPRESTRICT format, va_list args) noexcept;
#else
    using ::vasprintf;
#endif

#ifndef UP_HAS_STDC_SNPRINTF
    extern LIBUPCOREAPI UPPRINTF(3,4)
    int snprintf(char* UPRESTRICT stream, size_t n, char const* UPRESTRICT format, ...) noexcept;
#else
    using ::snprintf;
#endif

#ifndef UP_HAS_STDC_VSNPRINTF
    extern LIBUPCOREAPI UPPRINTF(3,0)
    int vsnprintf(char* UPRESTRICT s, size_t n, char const* UPRESTRICT format, va_list args) noexcept;
#else
    using ::vsnprintf;
#endif

#ifndef UP_HAS_STDC_VSCANF
    extern LIBUPCOREAPI UPSCANF(1,0)
    int vscanf(char const* UPRESTRICT format, va_list args) noexcept;
#else
    using ::vscanf;
#endif

#ifndef UP_HAS_STDC_VFSCANF
    extern LIBUPCOREAPI UPSCANF(2,0)
    int vfscanf(FILE* UPRESTRICT f, char const* UPRESTRICT format, va_list args) noexcept;
#else
    using ::vfscanf;
#endif

#ifndef UP_HAS_STDC_VSSCANF
    extern LIBUPCOREAPI UPSCANF(2,0)
    int vsscanf(char const* UPRESTRICT s, char const* UPRESTRICT format, va_list args) noexcept;
#else
    using ::vsscanf;
#endif

    extern LIBUPCOREAPI UPPRINTF(1,2)
    int fast_printf(char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPPRINTF(2,3)
    int fast_fprintf(FILE* UPRESTRICT stream, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPPRINTF(2,3)
    int fast_sprintf(char* UPRESTRICT s, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPPRINTF(3,4)
    int fast_snprintf(char* UPRESTRICT s, size_t n, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPPRINTF(2,3) UPWARNRESULT
    int fast_asprintf(char** UPRESTRICT strp, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPPRINTF(1,0)
    int fast_vprintf(char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPPRINTF(2,0)
    int fast_vfprintf(FILE* UPRESTRICT stream, char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPPRINTF(2,0)
    int fast_vsprintf(char* UPRESTRICT s, char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPPRINTF(3,0)
    int fast_vsnprintf(char* UPRESTRICT s, size_t n, char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPPRINTF(2,0) UPWARNRESULT
    int fast_vasprintf(char** UPRESTRICT strp, char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPSCANF(1,2)
    int fast_scanf(char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPSCANF(2,3)
    int fast_fscanf(FILE* UPRESTRICT stream, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPSCANF(2,3)
    int fast_sscanf(char const* UPRESTRICT s, char const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPSCANF(1,0)
    int fast_vscanf(char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPSCANF(2,0)
    int fast_vfscanf(FILE* UPRESTRICT stream, char const* UPRESTRICT format, va_list args) noexcept;

    extern LIBUPCOREAPI UPSCANF(2,0)
    int fast_vsscanf(char const* UPRESTRICT s, char const* UPRESTRICT format, va_list args) noexcept;
}

#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
#   include <up/detail/cstdio_msvc.inl>
#elif UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_POSIX
#   include <up/detail/cstdio_posix.inl>
#else
#   error "No stdio extensions compatability layer for target platform."
#endif

#endif
