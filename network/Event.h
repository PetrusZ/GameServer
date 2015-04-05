/*
 * =====================================================================================
 *
 *       Filename:  Event.h
 *
 *    Description:  Event
 *
 *        Version:  1.0
 *        Created:  04/03/2015 03:55:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef EVENT_H_7YKFXRLG
#define EVENT_H_7YKFXRLG

#include "event2/event.h"
#include "common/Common.h"

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02010500
#error "Libevent not found or libevent version too old to supporte. Please get 2.0.22-stable or later"
#endif

typedef evutil_socket_t EventSocket;
typedef event_callback_fn EventCallback;
typedef int EventFlagType;

class Event {
    public:
        enum EventFlag {
            kEventTimeout   = EV_TIMEOUT,
            kEventRead      = EV_READ,
            kEventWrite     = EV_WRITE,
            kEventSignal    = EV_SIGNAL,
            kEventPersist   = EV_PERSIST,
            kEventEv        = EV_ET
        };

        Event(struct event* event);
        virtual ~Event();

        bool Add(const struct timeval *tv);
        bool Del();
        bool PrioritySet(int priority);
        bool Pending(EventFlagType what, struct timeval *tv_out);
        void Active(EventFlagType what);

        EventSocket GetFd();
        EventFlagType GetEvents();
        EventCallback GetCallback();
        void* GetCallbackArg();
        bool Initialized();

    private:
        // TODO
        // struct event_base* GetBase();

        struct event* event_;
};

#endif /* end of include guard: EVENT_H_7YKFXRLG */
