/*
 * =====================================================================================
 *
 *       Filename:  EventBase.h
 *
 *    Description:  EventBase
 *
 *        Version:  1.0
 *        Created:  04/03/2015 03:54:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef EVENTBASE_H_YRLNXQR4
#define EVENTBASE_H_YRLNXQR4

#include "event2/event.h"
#include "common/Common.h"
#include "Event.h"
#include <string>

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02010500
#error "Libevent not found or libevent version too old to supporte. Please get 2.0.22-stable or later"
#endif

class EventBase {
    public:
        enum EventFeature {
            kEventFeatureNull   = 0,
            kEventFeatureEt     = EV_FEATURE_ET,
            kEventFeatureO1     = EV_FEATURE_O1,
            kEventFeatureFds    = EV_FEATURE_FDS
        };

        enum EventBaseFlag {
            kEventBaseFlagNull                  = 0,
            kEventBaseFlagNoLock                = EVENT_BASE_FLAG_NOLOCK,
            kEventBaseFlagIgnoreEnv             = EVENT_BASE_FLAG_IGNORE_ENV,
            kEventBaseFlagStartupIocp           = EVENT_BASE_FLAG_STARTUP_IOCP,
            kEventBaseFlagNoCacheTime           = EVENT_BASE_FLAG_NO_CACHE_TIME,
            kEventBaseFlagEpollUseChangelist    = EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST
        };

        enum EventLoopFlag {
            kEventLoopFlagNull      = 0,
            kEventLoopFlagOnce      = EVLOOP_ONCE,
            kEventLoopFlagNonBlock  = EVLOOP_NONBLOCK
        };

        enum EventLoopExitType {
            kNomal  = 0,
            kExit   = 1,
            kBreak  = 2
        };

        EventBase(struct event_base* base);
        virtual ~EventBase();

        bool Reinit();
        std::string GetMethod();
        int GetFeatures();

        bool Loop(int flags = kEventLoopFlagNull);
        bool LoopExit(const struct timeval *tv = NULL, EventLoopExitType exit_type = kExit);
        EventLoopExitType GotExitType();
        bool GettimeofdayCached(struct timeval *tv);
        void DumpEvents(FILE* fp);

    private:
        bool NewEvent(evutil_socket_t fd, short what, event_callback_fn callback, void* arg, Event** event);

        struct event_base* event_base_;
};

#endif /* end of include guard: EVENTBASE_H_YRLNXQR4 */
