/*
 * =====================================================================================
 *
 *       Filename:  SocketAddress.cc
 *
 *    Description:  SocketAddress
 *
 *        Version:  1.0
 *        Created:  2015年03月29日 00时13分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#include "SocketAddress.h"
#include <cstring>

SocketAddress::SocketAddress(const std::string hostname, const uint16_t port) : hostname_(hostname), port_(port) {

}

SocketAddress::SocketAddress(const uint32_t ip, const uint16_t port) : ip_(ip), port_(port) {

}

void SocketAddress::ToSockAddr(struct sockaddr *sockaddr) {
    memset(sockaddr, 0, sizeof(*sockaddr));
    sockaddr->sa_family = AF_INET;
}
