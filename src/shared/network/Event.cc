/*
 * =====================================================================================
 *
 *       Filename:  Event.cc
 *
 *    Description:  Event
 *
 *        Version:  1.0
 *        Created:  04/03/2015 04:33:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "Event.h"

Event::Event(struct event* event) : event_(event) {
}

Event::~Event() {
    event_free(event_);
}

bool Event::Add(const struct timeval *tv) {
    int result = event_add(event_, tv);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool Event::Del() {
    int result = event_del(event_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool Event::PrioritySet(int priority) {
    int result = event_priority_set(event_, priority);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool Event::Pending(EventFlagType what, struct timeval *tv_out) {
    return event_pending(event_, what, tv_out);
}

void Event::Active(EventFlagType what) {
    event_active(event_, what, 0);
}

EventSocket Event::GetFd() {
    return event_get_fd(event_);
}

EventFlagType Event::GetEvents() {
    return event_get_events(event_);
}

EventCallback Event::GetCallback() {
    return event_get_callback(event_);
}

void* Event::GetCallbackArg() {
    return event_get_callback_arg(event_);
}

bool Event::Initialized() {
    return event_initialized(event_);
}
