/*
 * =====================================================================================
 *
 *       Filename:  BufferEvent.cc
 *
 *    Description:  BufferEvent
 *
 *        Version:  1.0
 *        Created:  04/07/2015 02:46:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "BufferEvent.h"

BufferEvent::BufferEvent(struct bufferevent* buffer_event) : buffer_event_(buffer_event) {

}

BufferEvent::~BufferEvent() {
    if (buffer_event_) {
        bufferevent_free(buffer_event_);
    }
}

void BufferEvent::SetCallback(BufferEventDataCallBack read_callback, BufferEventDataCallBack write_callback, BufferEventEventCallBack event_callback, void *callback_arg) {
    bufferevent_setcb(buffer_event_, read_callback, write_callback, event_callback, callback_arg);
}

void BufferEvent::Enable(EventFlagType flags) {
    bufferevent_enable(buffer_event_, flags);
}

void BufferEvent::Disable(EventFlagType flags) {
    bufferevent_disable(buffer_event_, flags);
}

EventFlagType BufferEvent::GetEnabled() {
    return bufferevent_get_enabled(buffer_event_);
}

bool BufferEvent::Write(const void *data, size_t size) {
    int result = bufferevent_write(buffer_event_, data, size);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

size_t BufferEvent::Read(void *data, size_t size) {
    return bufferevent_read(buffer_event_, data, size);
}

void BufferEvent::SetTimeouts(const struct timeval *timeout_read, const struct timeval *timeout_write) {
    bufferevent_set_timeouts(buffer_event_, timeout_read, timeout_write);
}

bool BufferEvent::SetPriority(int priority) {
    int result = bufferevent_priority_set(buffer_event_, priority);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool BufferEvent::SetFd(EventSocket fd) {
    int result = bufferevent_setfd(buffer_event_, fd);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

EventSocket BufferEvent::GetFd() {
    return bufferevent_getfd(buffer_event_);
}

void BufferEvent::Lock() {
    bufferevent_lock(buffer_event_);
}

void BufferEvent::UnLock() {
    bufferevent_unlock(buffer_event_);
}
