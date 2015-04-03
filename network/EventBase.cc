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

EventBase::EventBase() {
    event_base_  = event_base_new();
}

EventBase::~EventBase() {
    if (event_base_) {
         event_base_free(event_base_);
    }
}

bool EventBase::NewEvent(int fd, short what, event_callback_fn callback, void* arg, Event** event) {
    struct event* event_struct;
    event_struct = event_new(event_base_, fd, what, callback, arg);

    if (event_struct) {
        *event = new Event(event_struct);
        return true;
    } else {
        return false;
    }
}
