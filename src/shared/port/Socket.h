/*
 * =====================================================================================
 *
 *       Filename:  Socket.h
 *
 *    Description:  socket wrapper
 *
 *        Version:  1.0
 *        Created:  2015年03月25日 22时05分54秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */
#ifndef SOCKET_H_1UEDGFJN
#define SOCKET_H_1UEDGFJN

#include <sys/types.h>
#include <sys/socket.h>

#include "SocketAddress.h"

#define INVALID_SOCKET (-1)

typedef int SOCKET;

class Socket {

    public:
        // Constructor. If fd = 0, it will be assigned
        Socket(SOCKET fd = INVALID_SOCKET);

        // Destructor.
        virtual ~Socket();

        SOCKET GetFd() const ;

        // create socket fd.
        bool Create(const int type);

        int Bind(const SocketAddress& addr);

        int Listen(const int backlog);

        // Open a connection to another machine.
        bool Connect(const SocketAddress& addr);

        Socket* Accept(SocketAddress* paddr);

        bool Close();

    private:
        SOCKET socket_;
};

#endif /* end of include guard: SOCKET_H_1UEDGFJN */

