/*
 * =====================================================================================
 *
 *       Filename:  TcpConnection.h
 *
 *    Description:  TcpConnection
 *
 *        Version:  1.0
 *        Created:  04/09/2015 10:46:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef TCPCONNECTION_H_DX9TE0VQ
#define TCPCONNECTION_H_DX9TE0VQ

#include "LibEventDefine.h"
#include "BufferEvent.h"
#include "port/Socket.h"
#include "common/Common.h"

#define DATA_BUFF_SIZE 65536

class TcpConnection {
    public:
        TcpConnection(BufferEvent* buffer_event);
        virtual ~TcpConnection();

        virtual size_t ReadData(void* data, size_t data_len);
        virtual void WriteData(const void* data, size_t data_len);

    private:
        friend class TcpServer;

        SOCKET GetSocket();

        BufferEvent* buffer_event_ = nullptr;
};

#endif /* end of include guard: TCPCONNECTION_H_DX9TE0VQ */
