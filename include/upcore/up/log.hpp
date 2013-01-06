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

#ifndef UP_LOG_HPP
#define UP_LOG_HPP

#include <up/climits.hpp>
#include <up/ctime.hpp>

#define UP_LOG_LEVEL_DEBUG 100
#define UP_LOG_LEVEL_INFO 200
#define UP_LOG_LEVEL_WARNING 300
#define UP_LOG_LEVEL_ERROR 400
#define UP_LOG_LEVEL_CRITICAL 500

namespace up
{
	constexpr unsigned int log_level_debug = UP_LOG_LEVEL_DEBUG;
	constexpr unsigned int log_level_info = UP_LOG_LEVEL_INFO;
	constexpr unsigned int log_level_warning = UP_LOG_LEVEL_WARNING;
	constexpr unsigned int log_level_error = UP_LOG_LEVEL_ERROR;
	constexpr unsigned int log_level_critical = UP_LOG_LEVEL_CRITICAL;

    struct LIBUPCOREAPI log_record
    {
	    char const* file;
	    long line;
        char const* category;
        unsigned int level;
	    char const* message;
        unsigned long sequence;
        timespec time_stamp;
    };

    typedef void (UPCDECL *log_handler)(log_record const* record, void* user);
    typedef void* log_sink;
    
    extern LIBUPCOREAPI
    void UPCDECL debugger_log_handler(log_record const* record, void* user);

    extern LIBUPCOREAPI
    void UPCDECL stdout_log_handler(log_record const* record, void* user);

    extern LIBUPCOREAPI
    void UPCDECL stderr_log_handler(log_record const* record, void* user);

    extern LIBUPCOREAPI
    void clear_log_sinks();

    extern LIBUPCOREAPI UPNONNULL(1)
    log_sink add_log_sink(
        log_handler handler,
        void* user,
        unsigned int min_level = 0,
        unsigned int max_level = UINT_MAX
    );

    extern LIBUPCOREAPI UPNONNULL(1)
    log_sink add_log_sink(
        log_handler handler,
        void* user,
        char const* category,
        unsigned int min_level = 0,
        unsigned int max_level = UINT_MAX
    );

    extern LIBUPCOREAPI UPNONNULL(1)
    int remove_log_sink(log_sink sink);

    extern LIBUPCOREAPI UPNONNULLALL
    void log_event(unsigned int level, char const* message);

    extern LIBUPCOREAPI UPNONNULLALL
    void log_event(unsigned int level, char const* format, ...);

    extern LIBUPCOREAPI UPNONNULLALL
    void log_event(char const* category, unsigned int level, char const* message);

    extern LIBUPCOREAPI UPNONNULLALL
    void log_event(char const* category, unsigned int level, char const* format, ...);

    extern LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, unsigned int level, char const* message);

    extern LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, unsigned int level, char const* format, ...);
    
    extern LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, char const* category, unsigned int level, char const* message);
    
    extern LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, char const* category, unsigned int level, char const* format, ...);
}

#endif

#if !defined(UP_LOG_ENABLED)
#   if defined(UP_DEBUG) || defined(UP_RELEASE) || defined(UP_SILVER) || defined(UP_GOLD)
#       define UP_LOG_ENABLED 1
#   else 
#       define UP_LOG_ENABLED 0
#   endif
#endif

#if !defined(UP_LOG_LEVEL)
#   if defined(UP_DEBUG)
#       define UP_LOG_LEVEL UP_LOG_LEVEL_DEBUG
#   elif defined(UP_RELEASE)
#       define UP_LOG_LEVEL UP_LOG_LEVEL_INFO
#   elif defined(UP_SILVER)
#       define UP_LOG_LEVEL UP_LOG_LEVEL_WARNING
#   elif defined(UP_GOLD)
#       define UP_LOG_LEVEL UP_LOG_LEVEL_ERROR
#   elif defined(UP_PLATINUM)
#       define UP_LOG_LEVEL UP_LOG_LEVEL_CRITICAL
#   endif
#endif

#undef UP_LOG
#undef UP_LOG_DEBUG
#undef UP_LOG_INFO
#undef UP_LOG_WARNING
#undef UP_LOG_ERROR
#undef UP_LOG_CRITICAL

#ifdef UP_LOG_ENABLED
#   define UP_LOG(name, level, ...) ::up::log_event_with_call_site(__FILE__, __LINE__, name, level, __VA_ARGS__)
#else
#   define UP_LOG(name, level, ...) { UPIGNORE(name); UPIGNORE(level); }
#endif

#if UP_LOG_LEVEL <= UP_LOG_LEVEL_DEBUG
#   define UP_LOG_DEBUG(name, ...) UP_LOG(name, ::up::log_level_debug, __VA_ARGS__)
#else
#   define UP_LOG_DEBUG(name, ...) UPIGNORE(name)
#endif

#if UP_LOG_LEVEL <= UP_LOG_LEVEL_INFO
#   define UP_LOG_INFO(name, ...) UP_LOG(name, ::up::log_level_info, __VA_ARGS__)
#else
#   define UP_LOG_INFO(name, ...) UPIGNORE(name)
#endif

#if UP_LOG_LEVEL <= UP_LOG_LEVEL_WARNING
#   define UP_LOG_WARNING(name, ...) UP_LOG(name, ::up::log_level_warning, __VA_ARGS__)
#else
#   define UP_LOG_WARNING(name, ...) UPIGNORE(name)
#endif

#if UP_LOG_LEVEL <= UP_LOG_LEVEL_ERROR
#   define UP_LOG_ERROR(name, ...) UP_LOG(name, ::up::log_level_error, __VA_ARGS__)
#else
#   define UP_LOG_ERROR(name, ...) UPIGNORE(name)
#endif

#if UP_LOG_LEVEL <= UP_LOG_LEVEL_CRITICAL
#   define UP_LOG_CRITICAL(name, ...) UP_LOG(name, ::up::log_level_critical, __VA_ARGS__)
#else
#   define UP_LOG_CRITICAL(name, ...) UPIGNORE(name)
#endif
