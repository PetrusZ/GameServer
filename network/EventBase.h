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

#include "common/Common.h"
#include "LibEventDefine.h"
#include "Event.h"
#include "BufferEvent.h"
#include <string>

#define SOCKET_HOLDER_SIZE 30000

class EventBase {
    public:
        EventBase(struct event_base* base);
        virtual ~EventBase();

        EventBase(const EventBase&) = delete;
        EventBase& operator=(const EventBase&) = delete;

        bool Reinit();
        std::string GetMethod();
        int GetFeatures();

        bool Loop(EventLoopFlagType flags = kEventLoopFlagNull);
        bool LoopExit(const struct timeval *tv = NULL, EventLoopExitType exit_type = kExit);
        EventLoopExitType GotExitType();
        bool GettimeofdayCached(struct timeval *tv);
        void DumpEvents(const std::string& dump_name = "Log/Eventbase.dump");

        /*
         * fd: 监听的fd
         * what: EventFlag的合集
         * callback: 与what对应的callback函数
         */
        bool NewEvent(EventSocket fd, EventFlagType what, EventCallback callback, void* arg, Event** event);
        bool NewTimerEvent(EventCallback callback, void* arg, Event** event);
        bool NewSignalEvent(EventSocket sig_num, EventCallback callback, void* arg, Event** event);
        bool NewEventOnce(EventSocket fd, EventFlagType what, EventCallback callback, void* arg, const struct timeval* tv);
        bool NewBufferEvent(EventSocket fd, BufferEventOptionType buffer_event_option, BufferEvent** buffer_event);

        // void DeleteEvent(EventSocket fd);
        // void DeleteBufferEvent(EventSocket fd);

    private:
        // void DeleteAllEvent();
        // void DeleteAllBufferEvent();

        struct event_base* event_base_;

        // XXX: 一个fd只能对应一个event，但是会有一个fd同时监听读和写事件的情况
        // Event* fd_event_[SOCKET_HOLDER_SIZE];
        // BufferEvent* fd_bufferevent_[SOCKET_HOLDER_SIZE];
};

#endif /* end of include guard: EVENTBASE_H_YRLNXQR4 */
