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
#include <string>

#define LOG_PREFIX_BUFF_SIZE 1024
#define LOG_BUFF_SIZE 32768

class Logger : public Singleton<Logger> {
    public:
        Logger() = default;
        virtual ~Logger();

        bool Error(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Info(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Debug(const char* file, const int line, const char* func_name, const char* msg, ...);
        bool Trace(const char* file, const int line, const char* func_name, const char* msg, ...);

    private:
        enum LogLevel {
            kNull  = 0,
            kError = 1,
            kInfo  = 2,
            kDebug = 3,
            kTrace = 4,
            kMax   = 5
        };

        bool AppendLog(LogLevel level, std::string &log);

        void LogRotate();

        std::string FormatLogFileName(const std::string& prefix, const std::string& description, bool useDate);

        WritableFile *error_log_;
        WritableFile *info_log_;
        WritableFile *debug_log_;
        WritableFile *trace_log_;
};

#define sLogger Logger::getSingleton()

#endif /* end of include guard: LOGGER_H_9ROTCNVM */
