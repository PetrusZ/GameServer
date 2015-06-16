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

#include "common/Common.h"
#include "LibEventDefine.h"

class Event {
    public:
        Event(struct event* event);
        virtual ~Event();

        bool Add(const struct timeval *tv = nullptr);
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
        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;

        // TODO
        // struct event_base* GetBase();

        struct event* event_;
};

#endif /* end of include guard: EVENT_H_7YKFXRLG */
