/*
 * =====================================================================================
 *
 *       Filename:  TcpWatcherThread.cc
 *
 *    Description:  TcpWatcherThread
 *
 *        Version:  1.0
 *        Created:  04/29/2015 03:02:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "TcpWatcherThread.h"
#include "TcpServer.h"
#include "port/Socket.h"
#include "common/Common.h"

TcpWatcherThread::TcpWatcherThread(const std::string& hostname, const uint16_t port) {
    listen_addr_ = new SocketAddress(hostname, port);
}

TcpWatcherThread::TcpWatcherThread(const uint32_t ip, const uint16_t port) {
    listen_addr_ = new SocketAddress(ip, port);
}

TcpWatcherThread::~TcpWatcherThread() {
    if (listen_addr_) {
        delete listen_addr_;
        listen_addr_ = nullptr;
    }
}

bool TcpWatcherThread::Run() {
    running_ = true;

    LOG_TRACE("TcpWatcherThread, Running.");

    Socket* listen_socket = new Socket();
    listen_socket->Create(SOCK_STREAM);
    listen_socket->Bind(*listen_addr_);
    listen_socket->Listen(256);

    sTcpServer.AddListenSocket(listen_socket);
    sTcpServer.StartLoop();

    return true;
}

void TcpWatcherThread::OnShutdown() {

}
