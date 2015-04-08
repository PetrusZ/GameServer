/*
 * =====================================================================================
 *
 *       Filename:  TcpServer.h
 *
 *    Description:  TcpServer
 *
 *        Version:  1.0
 *        Created:  04/08/2015 06:35:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef TCPSERVER_H_WIP2ZBBJ
#define TCPSERVER_H_WIP2ZBBJ

#include "LibEvent.h"
#include "EventBase.h"
#include "base/Singleton.hpp"
#include "port/Socket.h"

class TcpServer : public Singleton <TcpServer> {
    public:
        TcpServer() = default;
        virtual ~TcpServer();

        bool BindListenSocket(Socket &socket);

    private:
        EventBase* event_base_ = nullptr;
};

#endif /* end of include guard: TCPSERVER_H_WIP2ZBBJ */
