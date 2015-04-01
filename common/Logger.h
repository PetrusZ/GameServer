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
#include <assert.h>
#include <string>

#define LOG_PREFIX_BUFF_SIZE 1024
#define LOG_BUFF_SIZE 32768

class Logger : public Singleton<Logger> {
    public:
        Logger() = default;
        virtual ~Logger();

        bool Fatal(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Error(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Info(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Debug(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Trace(const char* file, const int line, const char* func_name, const char* msg, ...);

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

        bool AppendLog(LogLevel level, std::string &log);

        bool LogRotate(LogLevel level);

        std::string FormatLogFileName(const std::string& prefix, const std::string& description, bool useDate);

        WritableFile *error_log_ = NULL;
        WritableFile *info_log_ = NULL;
        WritableFile *debug_log_ = NULL;
        WritableFile *trace_log_ = NULL;
};

#define sLogger Logger::getSingleton()

#define LOG_FATAL(msg, ...) sLogger.Fatal(__FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) sLogger.Error(__FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) sLogger.Info(__FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) sLogger.Debug(__FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_TRACE(msg, ...) sLogger.Trace(__FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)

#endif /* end of include guard: LOGGER_H_9ROTCNVM */
