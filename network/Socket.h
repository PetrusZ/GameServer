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

class Socket {
    public:
        // Constructor. If fd = 0, it will be assigned
        Socket();

        // Destructor.
        virtual ~Socket();

        // Open a connection to another machine.
        bool Connect();

        // Disconnect the socket.
        void Disconnect();
}

#endif /* end of include guard: SOCKET_H_1UEDGFJN */

