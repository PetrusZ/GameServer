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
#include <fstream>
#include "NetWorkEvent.h"

NetWorkEvent::NetWorkEvent(std::string log_file_name) {
    if (!sFileSystem.NewWritableFile(log_file_name, &log_file_)) {
        // TODO: error handler
    }
}

NetWorkEvent::~NetWorkEvent() {
    if (log_file_) {
        delete log_file_;
    }
}

void NetWorkEvent::LogCallback(int severity, const char *msg) {
    if (!log_file_) {
        return ;
    }

    std::string status;

    switch (severity) {
        case _EVENT_LOG_DEBUG: status = "deNetWorkEvent_t"; break;
        case _EVENT_LOG_MSG:   status = "msg";   break;
        case _EVENT_LOG_WARN:  status = "warn";  break;
        case _EVENT_LOG_ERR:   status = "error"; break;
        default:               status = "unkonw";     break; /*  never reached */
    }

    std::string log = "[" + status + "] " + msg + "\n";
    if (!log_file_->Append(log)) {
        // TODO: error handler
    }
}
