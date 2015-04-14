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
#include "TcpConnection.h"
#include "common/Common.h"
#include <map>

#define SOCKET_HOLDER_SIZE 30000

class TcpServer : public Singleton <TcpServer> {
    public:
        TcpServer();
        virtual ~TcpServer();

        TcpServer(const TcpServer&);
        TcpServer& operator=(const TcpServer&);

        bool BindListenSocket(Socket* socket);
        bool NewTcpConnection(Socket* socket);

        void StartLoop();

        virtual void ProcessDataFromClient(SOCKET fd, const void* data, size_t data_len);
        virtual void SendDataToClient(SOCKET fd, const void* data, size_t data_len);

    private:
        TcpConnection* GetTcpConnection(BufferEventStruct* buffer_event_struct);
        bool AddTcpConnection(EventSocket socket, TcpConnection* tcp_connection);
        void RemoveTcpConnection(EventSocket socket);

        static void AcceptCallback(EventSocket socket, EventFlagType what, void* arg);

        //XXX: 因为libevent回调函数参数要求，不得不把BufferEventStruct暴露到接口中
        static void ReadCallback(BufferEventStruct* buffer_event_struct, void* arg);
        static void EventCallback(BufferEventStruct* buffer_event_struct, BufferEventFlagType what, void* arg);

        EventBase* event_base_ = nullptr;
        Event* listen_event_ = nullptr;
        Socket* listen_socket_ = nullptr;
        TcpConnection* tcp_connections_[SOCKET_HOLDER_SIZE];
        std::map<BufferEventStruct*, TcpConnection*> bufevent_conn_map_;
};

#define sTcpServer TcpServer::getSingleton()

#endif /* end of include guard: TCPSERVER_H_WIP2ZBBJ */
