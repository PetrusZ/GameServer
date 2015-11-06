/*
 * =====================================================================================
 *
 *       Filename:  TcpServerBase.cc
 *
 *    Description:  TcpServerBase
 *
 *        Version:  1.0
 *        Created:  2015年04月08日 21时00分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */
#include <cstring>
#include "TcpServerBase.h"
#include "TcpWorkerThread.h"
#include "thread/ThreadPool.h"

TcpServerBase::TcpServerBase() {
    memset(tcp_connections_, 0, sizeof(TcpConnection*) * SOCKET_HOLDER_SIZE);
}

TcpServerBase::~TcpServerBase() {
    if (event_base_)  {
        delete event_base_;
        event_base_ = nullptr;
    }
    auto it = listenfd_event_map_.cbegin();
    for ( ; it != listenfd_event_map_.cend(); ++it) {
        RemoveListenSocket(it->first);
    }
    for (int i = 0; i < SOCKET_HOLDER_SIZE; ++i) {
        RemoveTcpConnection(i);
    }
}

void TcpServerBase::StartLoop() {
    event_base_->Loop();
}

bool TcpServerBase::AddListenSocket(Socket* socket) {
    if (nullptr == event_base_ && !sLibEvent.NewEventBase(&event_base_, "select")) {
        return false;
    }

    auto it = listenfd_event_map_.find(socket->GetFd());
    if (it == listenfd_event_map_.end()) {
        Event* listen_event;
        event_base_->NewEvent(socket->GetFd(), kEventRead | kEventPersist, TcpServerBase::AcceptCallback, this, &listen_event);
        listenfd_event_map_[socket->GetFd()] = listen_event;
        listenfd_socket_map_[socket->GetFd()] = socket;
        return true;
    }
    return false;
}

void TcpServerBase::RemoveListenSocket(SOCKET fd) {
    auto it = listenfd_event_map_.find(fd);
    if (it != listenfd_event_map_.end()) {
        delete it->second;
        listenfd_event_map_.erase(it);

        delete listenfd_event_map_[fd];
        listenfd_socket_map_.erase(fd);
    }
}

bool TcpServerBase::NewTcpConnection(Socket* socket) {
    if (nullptr == event_base_ && !sLibEvent.NewEventBase(&event_base_, "select")) {
        return false;
    }

    BufferEvent* buffer_event = nullptr;
    SOCKET fd = socket->GetFd();
    if (event_base_->NewBufferEvent(fd, kThreadSafe, &buffer_event)) {
        TcpConnection* tcp_connection = new TcpConnection(socket, buffer_event);

        buffer_event->Enable(kEventRead | kEventWrite | kEventPersist);
        buffer_event->SetCallback(TcpServerBase::ServerReadCallback, NULL, TcpServerBase::EventCallback, this);

        int result = AddTcpConnection(fd, tcp_connection);
        if (result) {
            return true;
        } else {
            delete tcp_connection;
        }
    }

    if (buffer_event) {
        delete buffer_event;
    }
    return false;
}

bool TcpServerBase::AddTcpConnection(EventSocket socket, TcpConnection* tcp_connection) {
    if (tcp_connections_[socket] == 0) {
        tcp_connections_[socket] = tcp_connection;
        bufevent_conn_map_[tcp_connection->buffer_event_->buffer_event_] = tcp_connection;
        LOG_TRACE("Add TcpConnection with socket(%d)", socket);
        return true;
    }
    ASSERT(0 != tcp_connections_[socket]);
    return false;
}

void TcpServerBase::RemoveTcpConnection(EventSocket socket) {
    if (tcp_connections_[socket] != 0) {
        auto it = bufevent_conn_map_.find(tcp_connections_[socket]->buffer_event_->buffer_event_);
        if (it != bufevent_conn_map_.end()) {
            bufevent_conn_map_.erase(tcp_connections_[socket]->buffer_event_->buffer_event_);
        }
        delete tcp_connections_[socket];
        tcp_connections_[socket] = 0;

        // event_base_->DeleteBufferEvent(socket);
    }
}

TcpConnection* TcpServerBase::GetTcpConnection(BufferEventStruct* buffer_event_struct) {
    auto it = bufevent_conn_map_.find(buffer_event_struct);
    if (it != bufevent_conn_map_.end()) {
        return bufevent_conn_map_[buffer_event_struct];
    }
    return nullptr;
}

void TcpServerBase::AcceptCallback(EventSocket fd, EventFlagType what, void* arg) {
    TcpServerBase* tcp_server = static_cast<TcpServerBase*>(arg);
    BufferEvent* buffer_event = nullptr;
    SOCKET client_fd = ::accept(fd, NULL, NULL);
    Socket* client_socket = new Socket(client_fd);
    if (tcp_server->event_base_->NewBufferEvent(client_fd, kThreadSafe, &buffer_event)) {
        TcpConnection* tcp_connection = new TcpConnection(client_socket, buffer_event);

        buffer_event->Enable(kEventRead | kEventWrite | kEventPersist);
        buffer_event->SetCallback(TcpServerBase::ClientReadCallback, NULL, TcpServerBase::EventCallback, tcp_server);

        int result = tcp_server->AddTcpConnection(client_fd, tcp_connection);
        if (!result) {
            LOG_ERROR("Socket(%d) AddTcpConnection failed!", client_fd);
        }
    } else {
        LOG_ERROR("Socket(%d) accept failed!", fd);
    }
}

Packet_t* TcpServerBase::ReadCallback(BufferEventStruct* buffer_event_struct, void* arg) {
    ASSERT(buffer_event_struct);
    ASSERT(arg);

    TcpServerBase* tcp_server = static_cast<TcpServerBase*>(arg);
    TcpConnection* tcp_connection = tcp_server->GetTcpConnection(buffer_event_struct);

    ASSERT(tcp_connection);

    if (tcp_connection) {
        char* data_buff = new char[DATA_BUFF_SIZE];
        size_t data_len = tcp_connection->ReadData(data_buff, DATA_BUFF_SIZE);

        ASSERT(data_len < DATA_BUFF_SIZE);

        if (data_len < DATA_BUFF_SIZE) {
            Packet_t* packet = new Packet_t(tcp_connection->GetSocket(), data_len, data_buff);
            return packet;
        } else {
            LOG_ERROR("Socket(%d) receive data length greater than buffer size(%d)", tcp_connection->GetSocket(), DATA_BUFF_SIZE);
            return nullptr;
        }

    } else {
        LOG_ERROR("Can't find TcpConnection in ReadCallback function.");
        return nullptr;
    }
}

void TcpServerBase::ClientReadCallback(BufferEventStruct* buffer_event_struct, void* arg) {
    Packet_t* packet = ReadCallback(buffer_event_struct, arg);
    TcpServerBase* tcp_server = static_cast<TcpServerBase*>(arg);
    tcp_server->PushClientPacket(packet);
}

void TcpServerBase::ServerReadCallback(BufferEventStruct* buffer_event_struct, void* arg) {
    Packet_t* packet = ReadCallback(buffer_event_struct, arg);
    TcpServerBase* tcp_server = static_cast<TcpServerBase*>(arg);
    tcp_server->PushServerPacket(packet);
}

void TcpServerBase::EventCallback(BufferEventStruct* buffer_event_struct, BufferEventFlagType what, void* arg) {
    ASSERT(buffer_event_struct);
    ASSERT(arg);

    TcpServerBase* tcp_server = static_cast<TcpServerBase*>(arg);
    TcpConnection* tcp_connection = tcp_server->GetTcpConnection(buffer_event_struct);

    ASSERT(tcp_connection);

    if (what & kError) {
       LOG_ERROR("Socket(%d) encounter an error.", tcp_connection->GetSocket());
       tcp_server->RemoveTcpConnection(tcp_connection->GetSocket());
    } else if (what & kEof) {
       LOG_TRACE("Socket(%d) encounter Eof.", tcp_connection->GetSocket());
       tcp_server->RemoveTcpConnection(tcp_connection->GetSocket());
    } else if (what & kConnected) {
       LOG_TRACE("Socket(%d) connected.", tcp_connection->GetSocket());
    } else if (what & kReading) {
       LOG_TRACE("Socket(%d) reading.", tcp_connection->GetSocket());
    } else if (what & kWriting) {
       LOG_TRACE("Socket(%d) writing.", tcp_connection->GetSocket());
    } else if (what & kTimeout) {
       LOG_TRACE("Socket(%d) timeout.", tcp_connection->GetSocket());
    } else {
       LOG_ERROR("Socket(%d) encounter unkonw event.", tcp_connection->GetSocket());
    }
}

void TcpServerBase::SendData(SOCKET fd, const void* data, size_t data_len) {
    //TODO: 逻辑线程处理完毕后，由此函数统一发送给客户端
    if (tcp_connections_[fd]) {
        tcp_connections_[fd]->WriteData(data, data_len);
        LOG_TRACE("Send data to socket(%d), length(%d)", fd, data_len);
        LOG_INFO("Send data: %s", data);
    } else {
        LOG_ERROR("TcpConnection(%d) not exist, send data failed.", fd);
    }
}

void TcpServerBase::PushClientPacket(Packet_t* packet) {
    client_recv_queue_.push(packet);
}

Packet_t* TcpServerBase::PopClientPacket() {
    if (client_recv_queue_.size() > 0) {
        Packet_t* packet =  client_recv_queue_.front();
        client_recv_queue_.pop();
        return packet;
    } else {
        return nullptr;
    }
}

void TcpServerBase::PushServerPacket(Packet_t* packet) {
    server_recv_queue_.push(packet);
}

Packet_t* TcpServerBase::PopServerPacket() {
    if (server_recv_queue_.size() > 0) {
        Packet_t* packet =  server_recv_queue_.front();
        server_recv_queue_.pop();
        return packet;
    } else {
        return nullptr;
    }
}
