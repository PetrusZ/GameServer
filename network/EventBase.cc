/*
 * =====================================================================================
 *
 *       Filename:  EventBase.cc
 *
 *    Description:  EventBase
 *
 *        Version:  1.0
 *        Created:  04/03/2015 04:29:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#include "EventBase.h"
#include <string.h>

EventBase::EventBase(struct event_base* base) : event_base_(base) {
    memset(fd_event_, 0, sizeof(Event*) * SOCKET_HOLDER_SIZE);
}

EventBase::~EventBase() {
    if (event_base_) {
        DeleteAllEvent();
        // 注意：不会释放当前与event_base关联的任何事件，或者关闭他们的套接字，或者释放任何指针。
         event_base_free(event_base_);
    }
}

bool EventBase::NewEvent(EventSocket fd, EventFlagType what, EventCallback callback, void* arg, Event** event) {
    if (fd_event_[fd] != NULL) {
        return false;
    }

    struct event* event_struct;
    event_struct = event_new(event_base_, fd, what, callback, arg);

    if (event_struct) {
        *event = new Event(event_struct);
        fd_event_[fd] = *event;
        return true;
    } else {
        return false;
    }
}

bool EventBase::NewTimerEvent(EventCallback callback, void *arg, Event **event) {
    bool result = NewEvent(-1, 0, callback, arg, event);
    if (result) {
        return true;
    } else {
        return false;
    }
}

bool EventBase::NewSignalEvent(EventSocket sig_num, EventCallback callback, void *arg, Event **event) {
    bool result = NewEvent(sig_num, Event::kEventSignal | Event::kEventPersist, callback, arg, event);
    if (result) {
        return true;
    } else {
        return false;
    }
}

bool EventBase::NewEventOnce(EventSocket fd, EventFlagType what, EventCallback callback, void *arg, const struct timeval *tv) {
    bool result = event_base_once(event_base_, fd, what, callback, arg, tv);
    if (result) {
        return true;
    } else {
        return false;
    }
}

bool EventBase::Reinit() {
    int result;
    result = event_reinit(event_base_);

    if (-1 == result) {
        return false;
    } else {
        return true;
    }
}

std::string EventBase::GetMethod() {
    return event_base_get_method(event_base_);
}

int EventBase::GetFeatures() {
    return event_base_get_features(event_base_);
}

bool EventBase::Loop(int flags) {
    if (flags == kEventLoopFlagNull) {
        return 0 == event_base_dispatch(event_base_);
    } else {
        return 0 == event_base_loop(event_base_, flags);
    }
}

bool EventBase::LoopExit(const struct timeval *tv, EventLoopExitType exit_type) {
    if (!tv && exit_type == kBreak) {
        return 0 == event_base_loopbreak(event_base_);
    } else if (!tv && exit_type == kExit) {
        return 0 == event_base_loopexit(event_base_, tv);
    } else if (tv) {
        return 0 == event_base_loopexit(event_base_, tv);
    }
    return false;
}

EventBase::EventLoopExitType EventBase::GotExitType() {
    if (event_base_got_exit(event_base_)) {
        return kExit;
    } else if (event_base_got_break(event_base_)) {
        return kBreak;
    } else {
        return kNomal;
    }

}

bool EventBase::GettimeofdayCached(struct timeval *tv) {
    if (0 == event_base_gettimeofday_cached(event_base_, tv)) {
        return true;
    }
    return false;
}

void EventBase::DumpEvents(FILE *fp) {
    event_base_dump_events(event_base_, fp);
}

void EventBase::DeleteAllEvent() {
    for (uint32_t i = 0; i < SOCKET_HOLDER_SIZE; ++i) {
        if (!fd_event_[i]) {
            delete fd_event_[i];
        }
    }
}
