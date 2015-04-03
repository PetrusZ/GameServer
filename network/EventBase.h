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

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02010500
#error "Libevent not found or libevent version too old to supporte. Please get 2.0.22-stable or later"
#endif

class EventBase {
    public:
        EventBase();
        virtual ~EventBase();

    private:
        enum EventFeature {
            kEventFeatureEt     = 1,
            kEventFeatureO1     = 2,
            kEventFeatureFds    = 3
        };

        enum EventFlag {
            kEventBaseFlagNoLock                = 1,
            kEventBaseFlagIgnoreEnv             = 2,
            kEventBaseFlagStartupIocp           = 3,
            kEventBaseFlagNoCacheTime           = 4,
            kEventBaseFlagEpollUseChangelist    = 5
        };

        bool NewEvent(evutil_socket_t fd, short what, event_callback_fn callback, void* arg, Event** event);

        struct event_base* event_base_;
};

#endif /* end of include guard: EVENTBASE_H_YRLNXQR4 */
