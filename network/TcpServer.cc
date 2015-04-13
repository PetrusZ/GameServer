/*
 * =====================================================================================
 *
 *       Filename:  TcpServer.cc
 *
 *    Description:  TcpServer
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
#include "TcpServer.h"
#include <cstring>

TcpServer::TcpServer() {
    memset(tcp_connections_, 0, sizeof(TcpConnection*) * SOCKET_HOLDER_SIZE);
}

TcpServer::~TcpServer() {
    if (event_base_)  {
        delete event_base_;
        event_base_ = nullptr;
    }
    if (listen_event_) {
        delete listen_event_;
        listen_event_ = nullptr;
    }
    if (listen_socket_) {
        delete listen_socket_;
        listen_socket_ = nullptr;
    }
    for (int i = 0; i < SOCKET_HOLDER_SIZE; ++i) {
        RemoveTcpConnection(i);
    }
}

void TcpServer::StartLoop() {
    event_base_->DumpEvents();
    event_base_->Loop();
}

bool TcpServer::BindListenSocket(Socket* socket) {
    if (sLibEvent.NewEventBase(&event_base_, "select")) {
        listen_socket_ = socket;
        event_base_->NewEvent(socket->GetFd(), kEventRead | kEventPersist, TcpServer::AcceptCallback, this, &listen_event_);
        return true;
    }
    return false;
}

bool TcpServer::AddTcpConnection(EventSocket socket, TcpConnection* tcp_connection) {
    if (tcp_connections_[socket] == 0) {
        tcp_connections_[socket] = tcp_connection;
        bufevent_conn_map_[tcp_connection->buffer_event_->buffer_event_] = tcp_connection;
        LOG_TRACE("Add TcpConnection with socket(%d)", socket);
        return true;
    }
    ASSERT(0 != tcp_connections_[socket]);
    return false;
}

void TcpServer::RemoveTcpConnection(EventSocket socket) {
    if (tcp_connections_[socket] != 0) {
        auto it = bufevent_conn_map_.find(tcp_connections_[socket]->buffer_event_->buffer_event_);
        if (it != bufevent_conn_map_.end()) {
            bufevent_conn_map_.erase(tcp_connections_[socket]->buffer_event_->buffer_event_);
        }
        delete tcp_connections_[socket];
        tcp_connections_[socket] = 0;
    }
}

bool TcpServer::NewTcpConnection(Socket* socket) {
    BufferEvent* buffer_event = nullptr;
    SOCKET fd = socket->GetFd();
    if (event_base_->NewBufferEvent(fd, kThreadSafe, &buffer_event)) {
        TcpConnection* tcp_connection = new TcpConnection(socket, buffer_event);

        buffer_event->Enable(kEventRead | kEventWrite | kEventPersist);
        buffer_event->SetCallback(TcpServer::ReadCallback, NULL, TcpServer::EventCallback, this);

        int result = AddTcpConnection(fd, tcp_connection);
        if (result) {
            return true;
        }
    }

    if (buffer_event) {
        delete buffer_event;
    }
    return false;
}

TcpConnection* TcpServer::GetTcpConnection(BufferEventStruct* buffer_event_struct) {
    auto it = bufevent_conn_map_.find(buffer_event_struct);
    if (it != bufevent_conn_map_.end()) {
        return bufevent_conn_map_[buffer_event_struct];
    }
    return nullptr;
}

void TcpServer::AcceptCallback(EventSocket fd, EventFlagType what, void* arg) {
    TcpServer* tcp_server = static_cast<TcpServer*>(arg);
    BufferEvent* buffer_event = nullptr;
    SOCKET client_fd = ::accept(fd, NULL, NULL);
    Socket* client_socket = new Socket(client_fd);
    if (tcp_server->event_base_->NewBufferEvent(client_fd, kThreadSafe, &buffer_event)) {
        TcpConnection* tcp_connection = new TcpConnection(client_socket, buffer_event);

        buffer_event->Enable(kEventRead | kEventWrite | kEventPersist);
        buffer_event->SetCallback(TcpServer::ReadCallback, NULL, TcpServer::EventCallback, tcp_server);

        int result = tcp_server->AddTcpConnection(client_fd, tcp_connection);
        if (!result) {
            LOG_ERROR("Socket(%d) AddTcpConnection failed!", client_fd);
        }
    }

    LOG_ERROR("Socket(%d) accept failed!", fd);
}

void TcpServer::ReadCallback(BufferEventStruct* buffer_event_struct, void* arg) {
    ASSERT(buffer_event_struct);
    ASSERT(arg);

    TcpServer* tcp_server = static_cast<TcpServer*>(arg);
    TcpConnection* tcp_connection = tcp_server->GetTcpConnection(buffer_event_struct);

    ASSERT(tcp_connection);

    if (tcp_connection) {
        char *data_buff = new char[DATA_BUFF_SIZE];
        size_t data_len = tcp_connection->ReadData(data_buff, DATA_BUFF_SIZE);

        ASSERT(data_len < DATA_BUFF_SIZE);

        if (data_len < DATA_BUFF_SIZE) {
            TcpServer::ProcessDataFromClient(tcp_connection->GetSocket(), data_buff, data_len);
        }
        LOG_ERROR("Socket(%d) receive data length greater than buffer size(%d)", tcp_connection->GetSocket(), DATA_BUFF_SIZE);

        delete[] data_buff;
    }
    LOG_ERROR("Can't find TcpConnection in ReadCallback function.");
}

void TcpServer::EventCallback(BufferEventStruct* buffer_event_struct, BufferEventFlagType what, void* arg) {
    ASSERT(buffer_event_struct);
    ASSERT(arg);

    TcpServer* tcp_server = static_cast<TcpServer*>(arg);
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

void TcpServer::ProcessDataFromClient(SOCKET fd, const void *data, size_t data_len) {
    //TODO: 处理收到的数据。交给其他线程处理

}

void TcpServer::SendDataToClient(SOCKET fd, const void* data, size_t data_len) {
    //TODO: 逻辑线程处理完毕后，由此函数统一发送给客户端

}
