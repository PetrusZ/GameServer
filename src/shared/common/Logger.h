/*
 * =====================================================================================
 *
 *       Filename:  Logger.h
 *
 *    Description:  Logger
 *
 *        Version:  1.0
 *        Created:  03/31/2015 11:24:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef LOGGER_H_9ROTCNVM
#define LOGGER_H_9ROTCNVM

#include "port/WritableFile.h"
#include "base/Singleton.hpp"
#include <cassert>
#include <string>

#define LOG_PREFIX_BUFF_SIZE 1024
#define LOG_BUFF_SIZE 32768

class Logger : public Singleton<Logger> {
    public:
        Logger() = default;
        virtual ~Logger();

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        void Init(const char* process_name, bool is_file_log = true) { process_name_ = process_name; is_file_log_ = is_file_log; }

        bool Fatal(const char* msg, ...);
        bool Error(const char* msg, ...);
        bool Info(const char* msg, ...);
        bool Debug(const char* msg, ...);
        bool Trace(const char* msg, ...);

        void StdErr(const char* msg, ...);
        void StdOut(const char* msg, ...);

    private:
        enum LogLevel {
            kNull  = 0,
            kFatal = 1,
            kError = 2,
            kInfo  = 3,
            kDebug = 4,
            kTrace = 5,
            kMax   = 6
        };

        std::string ConstructLog(const char* msg, va_list& ap);

        bool AppendLog(LogLevel level, std::string &log);

        bool LogRotate(LogLevel level);

        std::string FormatLogFileName(const std::string& prefix, const std::string& description, bool useDate);

        WritableFile *error_log_ = NULL;
        WritableFile *info_log_ = NULL;
        WritableFile *debug_log_ = NULL;
        WritableFile *trace_log_ = NULL;

        bool is_file_log_ = true;
        std::string process_name_;
};

#define sLogger Logger::getSingleton()

#define LOG_FATAL(msg, ...) sLogger.Fatal(msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) sLogger.Error(msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) sLogger.Info(msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) sLogger.Debug(msg, ##__VA_ARGS__)
#define LOG_TRACE(msg, ...) sLogger.Trace(msg, ##__VA_ARGS__)
#define LOG_STDOUT(msg, ...) sLogger.StdOut(msg, ##__VA_ARGS__)
#define LOG_STDERR(msg, ...) sLogger.StdErr(msg, ##__VA_ARGS__)

#define LOG_KFATAL(key, msg, ...) sLogger.Fatal("%s, " msg, key, ##__VA_ARGS__)
#define LOG_KERROR(key, msg, ...) sLogger.Error("%s, " msg, key, ##__VA_ARGS__)
#define LOG_KINFO(key, msg, ...) sLogger.Info("%s, " msg, key, ##__VA_ARGS__)
#define LOG_KDEBUG(key, msg, ...) sLogger.Debug("%s, " msg, key, ##__VA_ARGS__)
#define LOG_KTRACE(key, msg, ...) sLogger.Trace("%s, " msg, key, ##__VA_ARGS__)
#define LOG_KSTDOUT(key, msg, ...) sLogger.StdOut("%s, " msg, key, ##__VA_ARGS__)
#define LOG_KSTDERR(key, msg, ...) sLogger.StdErr("%s, " msg, key, ##__VA_ARGS__)

#endif /* end of include guard: LOGGER_H_9ROTCNVM */
