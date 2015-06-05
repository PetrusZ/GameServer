/*
 * =====================================================================================
 *
 *       Filename:  TcpWatcherThread.h
 *
 *    Description:  TcpWatcherThread
 *
 *        Version:  1.0
 *        Created:  04/29/2015 02:48:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef TCPLISTENTHREAD_H_JQ5PSR1X
#define TCPLISTENTHREAD_H_JQ5PSR1X

#include <string>
#include "port/Thread.h"
#include "port/SocketAddress.h"

class TcpWatcherThread : public ThreadBase {
    public:
        TcpWatcherThread(const std::string& hostname, const uint16_t port);
        TcpWatcherThread(const uint32_t ip, const uint16_t port);
        virtual ~TcpWatcherThread();

        bool Run();
        void OnShutdown();

    private:
        bool running_ = false;
        SocketAddress* listen_addr_ = nullptr;
};

#endif /* end of include guard: TCPLISTENTHREAD_H_JQ5PSR1X */