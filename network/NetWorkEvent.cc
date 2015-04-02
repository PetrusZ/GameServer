/*
 * =====================================================================================
 *
 *       Filename:  NetWorkEvent.cc
 *
 *    Description:  NetWorkEvent
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
#include "NetWorkEvent.h"
#include "event2/thread.h"

bool NetWorkEvent::Init(std::string log_file_name) {
    if (!sFileSystem.MakeDirRecursive(log_file_name) || !sFileSystem.NewWritableFile(log_file_name, &log_file_)) {
        return false;
    }

    event_set_log_callback(LogCallback);
    event_set_fatal_callback(FatalCallback);

    evthread_enable_lock_debuging();
    event_enable_debug_mode();
    return true;
}

NetWorkEvent::~NetWorkEvent() {
    if (log_file_) {
        delete log_file_;
    }
}

bool NetWorkEvent::AppendLog(const std::string& status, const char* msg, ...) {
    int len;
    char log_buf[LIBEVENT_LOG_BUFF_SIZE];
    std::string time = sEnv.GetTime();

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

void NetWorkEvent::LogCallback(int severity, const char *msg) {
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

void NetWorkEvent::FatalCallback(int err) {
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
