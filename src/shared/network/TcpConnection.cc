/*
 * =====================================================================================
 *
 *       Filename:  TcpConnection.cc
 *
 *    Description:  TcpConnection
 *
 *        Version:  1.0
 *        Created:  04/09/2015 10:58:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "TcpConnection.h"
#include "TcpServerBase.h"

TcpConnection::TcpConnection(Socket* socket, BufferEvent* buffer_event) : socket_(socket), buffer_event_(buffer_event) {

}

TcpConnection::~TcpConnection() {
    if (buffer_event_) {
        delete buffer_event_;
        buffer_event_ = nullptr;
    }
    if (socket_) {
        delete socket_;
        socket_ = nullptr;
    }
}

SOCKET TcpConnection::GetSocket() {
    return buffer_event_->GetFd();
}

size_t TcpConnection::ReadData(void *data, size_t data_len) {
    return buffer_event_->Read(data, data_len);

}

void TcpConnection::WriteData(const void *data, size_t data_len) {
    buffer_event_->Write(data, data_len);
}
