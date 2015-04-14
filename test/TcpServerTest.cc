/*
 * =====================================================================================
 *
 *       Filename:  TcpServerTest.cc
 *
 *    Description:  TcpServerTest
 *
 *        Version:  1.0
 *        Created:  04/13/2015 06:01:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "port/Socket.h"
#include "port/SocketAddress.h"
#include "network/TcpServer.h"
#include <string>

#define BUFF_SIZE 4096

int main(int argc, char *argv[])
{
    SocketAddress server_addr("127.0.0.1", 19191);

    Socket* client_socket = new Socket();
    client_socket->Create(SOCK_STREAM);
    client_socket->Connect(server_addr);

    sTcpServer.NewTcpConnection(client_socket);

    std::string buff = "Hello server, this is client.";
    sTcpServer.SendDataToClient(client_socket->GetFd(), buff.c_str(), buff.size());

    sTcpServer.StartLoop();

    return 0;
}
