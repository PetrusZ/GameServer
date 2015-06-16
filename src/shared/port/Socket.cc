/*
 * =====================================================================================
 *
 *       Filename:  Socket.cc
 *
 *    Description:  socket wrapper cpp file
 *
 *        Version:  1.0
 *        Created:  2015年03月26日 15时38分54秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */
#include "Socket.h"
#include "SocketAddress.h"
#include "common/Common.h"
#include <netinet/in.h>
#include <unistd.h>

Socket::Socket(SOCKET fd) : socket_(fd){

}

Socket::~Socket() {
    Close();
}

bool Socket::Create(int type) {
    socket_ = ::socket(AF_INET, type, 0);
    LOG_TRACE("Create socket(%d)", socket_);
    return socket_ != INVALID_SOCKET;
}

int Socket::Bind(const SocketAddress &addr) {
    sockaddr_in saddr;
    addr.ToSockAddr(&saddr);
    int err = ::bind(socket_, (sockaddr*)&saddr, sizeof(saddr));
    LOG_TRACE("Bind socket(%d) with ip(%s), port(%d)", socket_, addr.IPAsString().c_str(), addr.port());
    return err;
}

int Socket::Listen(const int backlog) {
    int err = ::listen(socket_, backlog);
    LOG_TRACE("Listen socket(%d) with backlog(%d)", socket_, backlog);
    return err;
}

bool Socket::Connect(const SocketAddress& addr) {
    if (socket_ == INVALID_SOCKET && !Create(SOCK_STREAM)) {
        return false;
    }
    sockaddr_in saddr;
    addr.ToSockAddr(&saddr);
    if (::connect(socket_, (sockaddr*)&saddr, sizeof(saddr)) == -1) {
        return false;
    }
    return true;
}

Socket* Socket::Accept(SocketAddress *paddr) {
    sockaddr_in saddr;
    socklen_t addr_len = sizeof(saddr);
    SOCKET client_socket = ::accept(socket_, (sockaddr*)&saddr, &addr_len);
    if (paddr) {
        paddr->FromSockAddr(saddr);
    }
    return new Socket(client_socket);
}

SOCKET Socket::GetFd() {
    return socket_;
}

bool Socket::Close() {
    if (socket_ != INVALID_SOCKET) {
        int err = ::close(socket_);
        if (0 == err) {
            return true;
        }
    }
    return false;
}
