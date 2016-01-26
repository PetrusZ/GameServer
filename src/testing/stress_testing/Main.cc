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
#include "thread/ThreadPool.h"
#include "port/SocketAddress.h"
#include "network/TcpServerBase.h"
#include "network/TcpServerBaseThread.h"
#include "common/Common.h"

#include "StressSocketMgr.h"

#define BUFF_SIZE 4096

/*
 *  i/o线程1个
 *  逻辑线程1个
 *  主线程1个
 */
int main(int argc, char *argv[])
{
    sLogger.Init("StressTesting", false);
    sThreadPool.Startup();
    sThreadPool.ExecuteTask(new TcpServerBaseThread());

    SocketAddress server_addr("10.1.1.220", 19191);
    for (int i = 0; i < 1000; ++i) {
        Socket* socket = new Socket();
        socket->Create(SOCK_STREAM);
        socket->Connect(server_addr);

        sStressSocketMgr.AddSocket(socket);
        sTcpServer.NewTcpConnection(socket);
    }

    uint32_t i = 0;
    while(true) {
        
        /*
         * main loop
         */
        
        sStressSocketMgr.Reset();
        const Socket* socket;
        char buff[BUFF_SIZE];
        while(nullptr != (socket = sStressSocketMgr.Next())) {
            SOCKET fd = socket->GetFd();
            sprintf(buff, "No.%d, StressTesting write from socket(%d)\n", i, fd);
            int buff_len = strlen(buff);
            sTcpServer.SendData(fd, buff, buff_len);
            ++i;
        }
        sEnv.Sleep(1000 * 5);
    }

    return 0;
}
