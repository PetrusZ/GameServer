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
    // memset(fd_event_, 0, sizeof(Event*) * SOCKET_HOLDER_SIZE);
    // memset(fd_bufferevent_, 0, sizeof(BufferEvent*) * SOCKET_HOLDER_SIZE);
}

EventBase::~EventBase() {
    if (event_base_) {
        // DeleteAllEvent();

        // 注意：不会释放当前与event_base关联的任何事件，或者关闭他们的套接字，或者释放任何指针。
         event_base_free(event_base_);
    }
}

bool EventBase::NewEvent(EventSocket fd, EventFlagType what, EventCallback callback, void* arg, Event** event) {
    /*
    if (fd_event_[fd] != NULL) {
        return false;
    }
    */

    struct event* event_struct;
    event_struct = event_new(event_base_, fd, what, callback, arg);

    if (event_struct) {
        *event = new Event(event_struct);
        (*event)->Add();
        // fd_event_[fd] = *event;
        LOG_TRACE("New event register with socket(%d), EventFlag(%d)", fd, what);
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
    bool result = NewEvent(sig_num, kEventSignal | kEventPersist, callback, arg, event);
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

bool EventBase::NewBufferEvent(EventSocket fd, BufferEventOptionType buffer_event_option, BufferEvent** buffer_event) {
    /*
    if (fd_bufferevent_[fd] != NULL) {
        return false;
    }
    */

    struct bufferevent* buffer_event_struct = bufferevent_socket_new(event_base_, fd, buffer_event_option);
    if (buffer_event_struct) {
        *buffer_event = new BufferEvent(buffer_event_struct);
        LOG_TRACE("New buffer_event register with socket(%d), BufferEventOption(%d)", fd, buffer_event_option);
        // fd_bufferevent_[fd] = *buffer_event;
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
        LOG_TRACE("Event base start dispatch");
        int result = event_base_dispatch(event_base_);
        LOG_TRACE("Event base end dispatch");
        return 0 == result;
    } else {
        LOG_TRACE("Event base start loop with flags(%d)", flags);
        int result = event_base_loop(event_base_, flags);
        LOG_TRACE("Event base end loop with flags(%d)", flags);
        return 0 == result;
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

EventLoopExitType EventBase::GotExitType() {
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

void EventBase::DumpEvents(const std::string& dump_name) {
    FILE* fp = fopen(dump_name.c_str(), "a");
    event_base_dump_events(event_base_, fp);
    fclose(fp);
}

/*
void EventBase::DeleteEvent(EventSocket fd) {
    if (fd_event_[fd]) {
        //XXX: 只在TcpServer中管理event，而不在event_base中管理？
        // delete fd_event_[fd];
        fd_event_[fd] = NULL;
    }
}

void EventBase::DeleteBufferEvent(EventSocket fd) {
    if (fd_bufferevent_[fd]) {
        //XXX: 只在TcpConnection中管理bufferevent，而不在event_base中管理？
        // delete fd_bufferevent_[fd];
        fd_bufferevent_[fd] = NULL;
    }
}

void EventBase::DeleteAllEvent() {
    for (uint32_t i = 0; i < SOCKET_HOLDER_SIZE; ++i) {
        DeleteEvent(i);
    }
}

void EventBase::DeleteAllBufferEvent() {
    for (uint32_t i = 0; i < SOCKET_HOLDER_SIZE; ++i) {
        DeleteBufferEvent(i);
    }
}
*/
