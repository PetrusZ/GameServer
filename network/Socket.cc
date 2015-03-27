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

Socket::Socket(SOCKET fd) : socket_(fd){

}

bool Socket::Create(int type) {
    socket_ = ::socket(AF_INET, type, 0);
}
