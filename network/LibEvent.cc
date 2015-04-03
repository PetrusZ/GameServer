/*
 * =====================================================================================
 *
 *       Filename:  LibEvent.cc
 *
 *    Description:  LibEvent
 *
 *        Version:  1.0
 *        Created:  2015年03月29日 12时33分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */
#include <cassert>
#include "LibEvent.h"
#include "event2/thread.h"

std::string LibEvent::log_file_name_;
WritableFile* LibEvent::log_file_ = NULL;

LibEvent::LibEvent(std::string log_file_name) {
    log_file_name_ = log_file_name;
    event_set_log_callback(LogCallback);
    event_set_fatal_callback(FatalCallback);

    evthread_enable_lock_debuging();
    event_enable_debug_mode();
}

LibEvent::~LibEvent() {
    if (log_file_) {
        delete log_file_;
    }
}

std::string LibEvent::FormatLogFileName(const std::string &prefix, const std::string &description, bool useDate) {
    std::string file_name;
    file_name = prefix + "/";
    file_name += description;

    if (useDate) {
        std::string date = sEnv.GetDate();
        file_name = file_name + "-" + date;
    }
    file_name += ".log";

    return file_name;
}

bool LibEvent::AppendLog(const std::string& status, const char* msg, ...) {
    int len;
    char log_buf[LIBEVENT_LOG_BUFF_SIZE];
    std::string time = sEnv.GetTime();

    if (!sFileSystem.FileExists(log_file_name_)) {
        if (log_file_) {
            delete log_file_;
        }
        if (!sFileSystem.MakeDirRecursive(log_file_name_) || !sFileSystem.NewWritableFile(log_file_name_, &log_file_)) {
            return false;
        }
    }

    va_list ap;
    va_start(ap, msg);
    len = vsnprintf(log_buf, LIBEVENT_LOG_BUFF_SIZE, msg, ap);
    va_end(ap);

    assert(len <= LIBEVENT_LOG_BUFF_SIZE);

    std::string log = "[" + time + "] " + "[" + status + "] " + log_buf + "\n";
    if (!log_file_->Append(log)) {
        return false;
    }
    log_file_->Flush();
    return true;
}

void LibEvent::LogCallback(int severity, const char *msg) {
    if (!log_file_) {
        return ;
    }

    std::string status;

    switch (severity) {
        case _EVENT_LOG_DEBUG: status = "debug"; break;
        case _EVENT_LOG_MSG:   status = "msg";   break;
        case _EVENT_LOG_WARN:  status = "warn";  break;
        case _EVENT_LOG_ERR:   status = "error"; break;
        default:               status = "unkonw";     break; /*  never reached */
    }

    AppendLog(status, msg);
}

void LibEvent::FatalCallback(int err) {
    int stack_num;
    std::vector<std::string> stack_trace;

    stack_num = Common::BackTrace(stack_trace);

    AppendLog("fatal", "Error Code(%d)", err);

    if (stack_trace.size() > 0) {
        AppendLog("fatal", "Stack Trace:");
        int i = 1;
        for (auto stack : stack_trace) {
            if (i < stack_num) {
                AppendLog("fatal", "#%d: %s", i, stack.data());
            } else {
                AppendLog("fatal", "#%d: %s\n", i, stack.data());
            }
            ++i;
        }
    } else {
        AppendLog("fatal", "No Stack Trace Info!\n");
    }
}

void LibEvent::GetSupportedMethods(std::vector<std::string>& supported_methods) {
    const char** supported_methods_ptr;
    supported_methods_ptr = event_get_supported_methods();

    for (int i = 0; supported_methods_ptr[i] != NULL ; ++i) {
        supported_methods.emplace_back(supported_methods_ptr[i]);
    }

    free(supported_methods_ptr);
}
