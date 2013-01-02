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

#include <up/cerrno.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace up
{
    namespace
    {
        struct error_entry
        {
            unsigned long oserror;
            int error_code;
        };

        error_entry const errors[] =
        {
            { ERROR_SUCCESS,                0         },
            { ERROR_CALL_NOT_IMPLEMENTED,   ENOSYS    },
            { ERROR_INVALID_PARAMETER,      EINVAL    },
            { ERROR_NO_PROC_SLOTS,          EAGAIN    },
            { ERROR_DRIVE_LOCKED,           EACCES    },
            { ERROR_BROKEN_PIPE,            EPIPE     },
            { ERROR_INVALID_FUNCTION,       EINVAL    },
            { ERROR_FILE_NOT_FOUND,         ENOENT    },
            { ERROR_PATH_NOT_FOUND,         ENOENT    },
            { ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
            { ERROR_ACCESS_DENIED,          EACCES    },
            { ERROR_INVALID_HANDLE,         EBADF     },
            { ERROR_ARENA_TRASHED,          ENOMEM    },
            { ERROR_NOT_ENOUGH_MEMORY,      ENOMEM    },
            { ERROR_INVALID_BLOCK,          ENOMEM    },
            { ERROR_BAD_ENVIRONMENT,        E2BIG     },
            { ERROR_BAD_FORMAT,             ENOEXEC   },
            { ERROR_INVALID_ACCESS,         EINVAL    },
            { ERROR_INVALID_DATA,           EINVAL    },
            { ERROR_INVALID_DRIVE,          ENOENT    },
            { ERROR_CURRENT_DIRECTORY,      EACCES    },
            { ERROR_NOT_SAME_DEVICE,        EXDEV     },
            { ERROR_NO_MORE_FILES,          ENOENT    },
            { ERROR_LOCK_VIOLATION,         EACCES    },
            { ERROR_BAD_NETPATH,            ENOENT    },
            { ERROR_NETWORK_ACCESS_DENIED,  EACCES    },
            { ERROR_BAD_NET_NAME,           ENOENT    },
            { ERROR_FILE_EXISTS,            EEXIST    },
            { ERROR_CANNOT_MAKE,            EACCES    },
            { ERROR_FAIL_I24,               EACCES    },
            { ERROR_DISK_FULL,              ENOSPC    },
            { ERROR_INVALID_TARGET_HANDLE,  EBADF     },
            { ERROR_INVALID_HANDLE,         EINVAL    },
            { ERROR_WAIT_NO_CHILDREN,       ECHILD    },
            { ERROR_CHILD_NOT_COMPLETE,     ECHILD    },
            { ERROR_DIRECT_ACCESS_HANDLE,   EBADF     },
            { ERROR_NEGATIVE_SEEK,          EINVAL    },
            { ERROR_SEEK_ON_DEVICE,         EACCES    },
            { ERROR_DIR_NOT_EMPTY,          ENOTEMPTY },
            { ERROR_NOT_LOCKED,             EACCES    },
            { ERROR_BAD_PATHNAME,           ENOENT    },
            { ERROR_MAX_THRDS_REACHED,      EAGAIN    },
            { ERROR_LOCK_FAILED,            EACCES    },
            { ERROR_ALREADY_EXISTS,         EEXIST    },
            { ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
            { ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
            { ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
        };
    }

    LIBUPCOREAPI UPPURE
    int oserror_to_errno(unsigned long oserror) noexcept {
        for (size_t i = 0; i < (sizeof(errors) / sizeof(error_entry)); ++i) {
            if (oserror == errors[i].oserror) {
                return errors[i].error_code;
            }
        }
        
        if ((ERROR_INVALID_STARTING_CODESEG <= oserror) && (oserror <= ERROR_INFLOOP_IN_RELOC_CHAIN)) {
            return ENOEXEC;
        }
        else if ((ERROR_WRITE_PROTECT <= oserror) && (oserror <= ERROR_SHARING_BUFFER_EXCEEDED)) {
            return EACCES;
        }
        else {
            return EINVAL;
        }
    }
}
