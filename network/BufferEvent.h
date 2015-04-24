/*
 * =====================================================================================
 *
 *       Filename:  BufferEvent.h
 *
 *    Description:  BufferEvent
 *
 *        Version:  1.0
 *        Created:  04/07/2015 02:43:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef BUFFEREVENT_H_F7VDYOKR
#define BUFFEREVENT_H_F7VDYOKR

#include "LibEventDefine.h"

class BufferEvent {
    public:
        BufferEvent(struct bufferevent* buffer_event_);
        virtual ~BufferEvent();

        void SetCallback(BufferEventDataCallBack read_callback, BufferEventDataCallBack write_callback, BufferEventEventCallBack event_callback, void* callback_arg);

        void Enable(EventFlagType flags);
        void Disable(EventFlagType flags);
        EventFlagType GetEnabled();

        // TODO: 添加buffer接口
        bool Write(const void* data, size_t size);
        size_t Read(void* data, size_t size);

        void SetTimeouts(const struct timeval *timeout_read, const struct timeval *timeout_write);
        bool SetPriority(int priority);
        bool SetFd(EventSocket fd);
        EventSocket GetFd();

        void Lock();
        void UnLock();

    private:
        BufferEvent(const BufferEvent&) = delete;
        BufferEvent& operator=(const BufferEvent&) = delete;

        friend class TcpServer;
        struct bufferevent* buffer_event_ = nullptr;
};

#endif /* end of include guard: BUFFEREVENT_H_F7VDYOKR*/
