/*
 * =====================================================================================
 *
 *       Filename:  TcpServerBase.h
 *
 *    Description:  TcpServerBase
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
#ifndef TcpServerBase_H_WIP2ZBBJ
#define TcpServerBase_H_WIP2ZBBJ

#include "NetWork.h"
#include "LibEvent.h"
#include "EventBase.h"
#include "port/Socket.h"
#include "TcpConnection.h"
#include "thread/Mutex.h"
#include "base/Singleton.hpp"
#include "base/FQueue.hpp"
#include <map>
#include <unordered_map>
#include <queue>

#define SOCKET_HOLDER_SIZE 30000

class TcpServerBase : public Singleton <TcpServerBase> {
    public:
        TcpServerBase();
        virtual ~TcpServerBase();

        bool AddListenSocket(Socket* socket);
        bool NewTcpConnection(Socket* socket);

        void StartLoop();

        Packet_t* PopClientPacket();
        Packet_t* PopServerPacket();

        virtual void SendData(SOCKET fd, const void* data, size_t data_len);

    private:
        TcpServerBase(const TcpServerBase&) = delete;
        TcpServerBase& operator=(const TcpServerBase&) = delete;

        TcpConnection* GetTcpConnection(BufferEventStruct* buffer_event_struct);
        bool AddTcpConnection(EventSocket socket, TcpConnection* tcp_connection);
        void RemoveTcpConnection(EventSocket socket);

        void RemoveListenSocket(SOCKET fd);

        static void AcceptCallback(EventSocket socket, EventFlagType what, void* arg);

        //XXX: 因为libevent回调函数参数要求，不得不把BufferEventStruct暴露到接口中
        static Packet_t* ReadCallback(BufferEventStruct* buffer_event_struct, void* arg);
        static void ClientReadCallback(BufferEventStruct* buffer_event_struct, void* arg);
        static void ServerReadCallback(BufferEventStruct* buffer_event_struct, void* arg);

        static void EventCallback(BufferEventStruct* buffer_event_struct, BufferEventFlagType what, void* arg);

        void PushClientPacket(Packet_t* packet);
        void PushServerPacket(Packet_t* packet);


        EventBase* event_base_ = nullptr;

        std::unordered_map<SOCKET, Event*> listenfd_event_map_;
        std::unordered_map<SOCKET, Socket*> listenfd_socket_map_;

        TcpConnection* tcp_connections_[SOCKET_HOLDER_SIZE];
        std::map<BufferEventStruct*, TcpConnection*> bufevent_conn_map_;

        //TODO: 多线程加锁
        FQueue<Packet_t*> client_recv_queue_;
        FQueue<Packet_t*> server_recv_queue_;
};

#define sTcpServer TcpServerBase::getSingleton()

#endif /* end of include guard: TcpServerBase_H_WIP2ZBBJ */
