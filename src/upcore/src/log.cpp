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

#include <up/log.hpp>
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdio.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/cthreads.hpp>
#include <up/list.hpp>

#if (UP_BASESYSTEM == UP_BASESYSTEM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif

namespace up { namespace
{
    constexpr size_t max_category_length = 196;

    struct log_sink_node
    {
        list_node node;
        log_handler handler;
        void* user_data;
        unsigned int min_level;
        unsigned int max_level;
        char category[max_category_length];
    };

    struct log_event_dispatcher
    {
        log_record* record;

        UPALWAYSINLINE
        log_event_dispatcher(log_record* r) noexcept
        : record(r) {
        }

        UPALWAYSINLINE
        void operator()(log_sink_node const* sink) const noexcept {
            if ( (sink->category[0] == '\0' || !strcmp(sink->category, record->category))
                && (sink->min_level <= record->level) && (record->level <= sink->max_level)
            ) {
                sink->handler(record, sink->user_data);
            }
        }
    };

    once_flag log_once = ONCE_FLAG_INIT;
    mtx_t log_monitor;
    list_node log_sinks_head;
    unsigned long log_sequence;

    void UPCDECL log_term() noexcept {
        list_clear<log_sink_node, &log_sink_node::node>(&log_sinks_head, malloc_allocator::instance());
        mtx_destroy(&log_monitor);
    }

    void UPCDECL log_init() noexcept {
        mtx_init(&log_monitor, mtx_plain);
        list_construct(&log_sinks_head);
        log_sequence = 0;
        verify(!atexit(&log_term));
    }

    inline char const* canonical_category_name(char const* category_name) noexcept {
        return (category_name && (category_name[0] != '\0')) ? category_name : "";
    }

    void log_event_record(char const* file, long line, char const* category_name, unsigned int level, char const* message) {
        log_record record;
        record.file = file;
        record.line = line;
        record.category = canonical_category_name(category_name);
        record.level = level;
        record.message = message;
        verify(!clock_gettime(CLOCK_REALTIME, &record.time_stamp));
        call_once(&log_once, &log_init);
        verify(thrd_success == mtx_lock(&log_monitor));
        record.sequence = log_sequence++;
        list_foreach<log_sink_node, &log_sink_node::node>(&log_sinks_head, log_event_dispatcher(&record));
        verify(thrd_success == mtx_unlock(&log_monitor));
    }
}}

namespace up
{
    LIBUPCOREAPI
    void UPCDECL debugger_log_handler(log_record const* record, void*) {
#if (UP_BASESYSTEM == UP_BASESYSTEM_WINDOWS)
        ::OutputDebugStringA(record->message);
#else
        // TODO: use syslog
        fputs(record->message, stderr);
        fputc('\n', stderr);
        fflush(stderr);
#endif
    }

    LIBUPCOREAPI
    void UPCDECL stdout_log_handler(log_record const* record, void*) {
        fputs(record->message, stdout);
        fputc('\n', stdout);
        fflush(stdout);
    }

    LIBUPCOREAPI
    void UPCDECL stderr_log_handler(log_record const* record, void*) {
        fputs(record->message, stderr);
        fputc('\n', stderr);
        fflush(stderr);
    }

    LIBUPCOREAPI
    void clear_log_sinks() {
        call_once(&log_once, &log_init);
        verify(thrd_success == mtx_lock(&log_monitor));
        list_clear<log_sink_node, &log_sink_node::node>(&log_sinks_head, malloc_allocator::instance());
        verify(thrd_success == mtx_unlock(&log_monitor));
    }

    LIBUPCOREAPI UPNONNULL(1)
    log_sink add_log_sink(log_handler handler, void* user_data, unsigned int min_level, unsigned int max_level) {
        return add_log_sink(handler, user_data, nullptr, min_level, max_level);
    }

    LIBUPCOREAPI UPNONNULL(1)
    log_sink add_log_sink(log_handler handler, void* user_data, char const* category_name, unsigned int min_level, unsigned int max_level) {
        char const* const name = canonical_category_name(category_name);
        size_t const name_length = strlen(name);
        if (((name_length + 1) >= max_category_length) || !handler || (min_level > max_level)) {
            errno = EINVAL;
            return nullptr;
        }

        log_sink_node* new_sink = static_cast<log_sink_node*>(malloc(sizeof(log_sink_node)));
        if (!new_sink) {
            return nullptr;
        }

        new_sink->handler = handler;
        new_sink->user_data = user_data;
        new_sink->min_level = min_level;
        new_sink->max_level = max_level;
        *static_cast<char*>(mempcpy(new_sink->category, name, name_length)) = '\0';

        call_once(&log_once, &log_init);
        verify(thrd_success == mtx_lock(&log_monitor));
        list_push_back(&log_sinks_head, &new_sink->node);
        verify(thrd_success == mtx_unlock(&log_monitor));
        return static_cast<log_sink>(new_sink);
    }

    LIBUPCOREAPI UPNONNULL(1)
    int remove_log_sink(log_sink s) {
        log_sink_node* sink = static_cast<log_sink_node*>(s);
        if (!sink || !list_is_linked(&sink->node)) {
            errno = EINVAL;
            return -1;
        }
        
        call_once(&log_once, &log_init);
        verify(thrd_success == mtx_lock(&log_monitor));
        list_unlink(&sink->node);
        verify(thrd_success == mtx_unlock(&log_monitor));
        free(sink);
        return 0;
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event(unsigned int level, char const* message) {
        log_event_record(nullptr, -1, nullptr, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event(unsigned int level, char const* format, ...) {
        assert(format);

        char message[4096];
        va_list args;

        va_start(args, format);
        fast_vsnprintf(message, sizeof(message), format, args);
        va_end(args);
        
        log_event_record(nullptr, -1, nullptr, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event(char const* category_name, unsigned int level, char const* message) {
        log_event_record(nullptr, -1, category_name, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event(char const* category_name, unsigned int level, char const* format, ...) {
        assert(format);

        char message[4096];
        va_list args;

        va_start(args, format);
        fast_vsnprintf(message, sizeof(message), format, args);
        va_end(args);
        
        log_event_record(nullptr, -1, category_name, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, unsigned int level, char const* message) {
        log_event_record(file, line, nullptr, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, unsigned int level, char const* format, ...) {
        assert(format);

        char message[4096];
        va_list args;
        
        va_start(args, format);
        fast_vsnprintf(message, sizeof(message), format, args);
        va_end(args);

        log_event_record(file, line, nullptr, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, char const* category_name, unsigned int level, char const* message) {
        log_event_record(file, line, category_name, level, message);
    }

    LIBUPCOREAPI UPNONNULLALL
    void log_event_with_call_site(char const* file, long line, char const* category_name, unsigned int level, char const* format, ...) {
        assert(format);

        char message[4096];
        va_list args;
        
        va_start(args, format);
        fast_vsnprintf(message, sizeof(message), format, args);
        va_end(args);

        log_event_record(file, line, category_name, level, message);
    }
}
