/*
 * =====================================================================================
 *
 *       Filename:  Logger.cc
 *
 *    Description:  Logger
 *
 *        Version:  1.0
 *        Created:  03/31/2015 03:02:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef LOGGER_CC_AZQLX2MN
#define LOGGER_CC_AZQLX2MN

#include "Logger.h"
#include "port/Env.h"
#include "port/FileSystem.h"
#include <cstdarg>
#include <cstdio>
#include <iostream>

Logger::~Logger() {
    if (error_log_) delete error_log_;
    if (info_log_)  delete info_log_;
    if (debug_log_) delete debug_log_;
    if (trace_log_) delete trace_log_;
}

bool Logger::AppendLog(LogLevel level, std::string &log) {
    std::string description;
    WritableFile** log_file;
    switch(level) {
        case kError:
            log_file = &error_log_;
            description = "Error";
            break;
        case kInfo:
            log_file = &info_log_;
            description = "Info";
            break;
        case kDebug:
            log_file = &debug_log_;
            description = "Debug";
            break;
        case kTrace:
            log_file = &trace_log_;
            description = "Trace";
            break;
        default:
            assert(false);
            return false;
            break;
    }

    if (log.size() <= 0) {
        return true;
    }

    if (is_file_log_) {
        if (!LogRotate(level)) {
            return false;
        }

        assert(*log_file);

        log += "\n";

        if (!(*log_file)->Append(log)) {
            return false;
        }
        (*log_file)->Flush();
    } else {
        std::string file_name = FormatLogFileName("Std", description, true);
        if (kFatal <= level && level <= kError) {
            std::cerr << file_name << ": " << log << std::endl;
        } else if (kInfo <= level && level <= kTrace) {
            std::cout << file_name << ": " << log << std::endl;
        }
    }

    return true;
}

std::string Logger::FormatLogFileName(const std::string &prefix, const std::string &description, bool useDate) {
    std::string file_name;
    file_name = prefix + "/" + process_name_ + "/";
    file_name += description;

    if (useDate) {
        std::string date = sEnv.GetDate();
        file_name = file_name + "-" + date;
    }
    file_name += ".log";

    return file_name;
}

bool Logger::LogRotate(LogLevel level) {
    std::string file_name;
    WritableFile** log_file;

    switch (level) {
        case kError:
            file_name = FormatLogFileName("Log", "Error", true);
            log_file = &error_log_;
            break;
        case kInfo:
            file_name = FormatLogFileName("Log", "Info", true);
            log_file = &info_log_;
            break;
        case kDebug:
            file_name = FormatLogFileName("Log", "Debug", true);
            log_file = &debug_log_;
            break;
        case kTrace:
            file_name = FormatLogFileName("Log", "Trace", true);
            log_file = &trace_log_;
            break;
        default:
            assert(false);
            return false;
            break;
    }

    if (!sFileSystem.MakeDirRecursive(file_name)) {
        return false;
    }

    if (!(*log_file)) {
        bool result = sFileSystem.NewWritableFile(file_name, log_file);
        if (!result) {
            return false;
        }
    } else {
        if ((*log_file)->GetName() != file_name || !sFileSystem.FileExists(file_name)) {
            delete *log_file;
            *log_file = NULL;
            bool result = sFileSystem.NewWritableFile(file_name, log_file);
            if (!result) {
                return false;
            }
        }
    }

    assert(*log_file);

    return true;
}

std::string Logger::ConstructLog(const char *msg, va_list &ap) {
    if (!msg) return "";

    int len;
    char prefix_buf[LOG_PREFIX_BUFF_SIZE];
    char log_buf[LOG_BUFF_SIZE];

    std::string time = sEnv.GetTime();
    len = snprintf(prefix_buf, LOG_PREFIX_BUFF_SIZE, "[%s] %s", time.c_str(), msg);

    assert(len <= LOG_PREFIX_BUFF_SIZE);

    len = vsnprintf(log_buf, LOG_BUFF_SIZE, prefix_buf, ap);

    assert(len <= LOG_BUFF_SIZE);

    return log_buf;
}

bool Logger::Fatal(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    return AppendLog(kFatal, log);
}

bool Logger::Error(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    return AppendLog(kError, log);
}

bool Logger::Info(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    return AppendLog(kInfo, log);
}

bool Logger::Debug(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    return AppendLog(kDebug, log);
}

bool Logger::Trace(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    return AppendLog(kTrace, log);
}

void Logger::StdOut(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    std::cout << log << std::endl;
}

void Logger::StdErr(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    std::string log = ConstructLog(msg, ap);
    va_end(ap);

    std::cerr << log << std::endl;
}

#endif /* end of include guard: LOGGER_CC_AZQLX2MN */
