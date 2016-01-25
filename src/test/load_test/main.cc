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
#include "port/Socket.h"
#include "port/SocketAddress.h"
#include "network/TcpServerBase.h"
#include <string>

#define BUFF_SIZE 4096

/*
 *  i/o线程1个
 *  逻辑线程1个
 *  主线程1个
 */
int main(int argc, char *argv[])
{

    while(true) {
        /*
         *  main loop
         */
        
    }

    return 0;
}
