/*
 * =====================================================================================
 *
 *       Filename:  LibEvent.h
 *
 *    Description:  LibEvent
 *
 *        Version:  1.0
 *        Created:  2015年03月29日 12时28分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#ifndef LIBEVENT_H_ZLTEOFP7
#define LIBEVENT_H_ZLTEOFP7

#include <string>
#include <vector>
#include "event2/event.h"
#include "base/Singleton.hpp"
#include "common/Common.h"
#include "port/FileSystem.h"
#include "port/WritableFile.h"
#include "port/Env.h"
#include "EventBase.h"

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02010500
#error "Libevent not found or libevent version too old to supporte. Please get 2.0.22-stable or later"
#endif

#define LIBEVENT_LOG_BUFF_SIZE 32768

class LibEvent : public Singleton<LibEvent> {
    public:
        LibEvent(std::string log_file_name = "Log/libevent.log");

        ~LibEvent();

        bool NewEventBase(EventBase** base,
                std::string avoid_method = "",
                int event_featrue = EventBase::kEventFeatureNull,
                int event_base_flag = EventBase::kEventBaseFlagNull,
                int priority = 0);

    private:
        static void LogCallback(int severity, const char* msg);
        static void FatalCallback(int err);
        static bool AppendLog(const std::string& status, const char* msg, ...);
        static std::string FormatLogFileName(const std::string &prefix, const std::string &description, bool useDate);
        void GetSupportedMethods(std::vector<std::string>& supported_methods);

        static std::string log_file_name_;
        static WritableFile* log_file_;
        EventBase* event_base_;
};

#define sLibEvent LibEvent::getSingleton()

#endif /* end of include guard: LIBEVENT_H_ZLTEOFP7 */
