/*
 * =====================================================================================
 *
 *       Filename:  TcpWorkerThread.h
 *
 *    Description:  TcpWorkerThread
 *
 *        Version:  1.0
 *        Created:  04/29/2015 04:27:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef TCPWORKERTHREAD_H_WPR0YYNB
#define TCPWORKERTHREAD_H_WPR0YYNB

#include "port/Socket.h"
#include "thread/Thread.h"

class TcpWorkerThread : public ThreadBase {
    public:
        TcpWorkerThread(SOCKET fd, const void* data, size_t data_len) : fd_(fd), data_(data), data_len_(data_len) { }
        virtual ~TcpWorkerThread() { }

        bool Run();
        void OnShutdown();

    private:
        SOCKET fd_;
        const void* data_;
        const size_t data_len_;
        bool running_ = false;
};

#endif /* end of include guard: TCPWORKERTHREAD_H_WPR0YYNB */
