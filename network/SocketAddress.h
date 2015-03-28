/*
 * =====================================================================================
 *
 *       Filename:  SocketAddress.h
 *
 *    Description:  SocketAddress
 *
 *        Version:  1.0
 *        Created:  2015年03月29日 00时13分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#ifndef SOCKETADDRESS_H_WEGUF5SN
#define SOCKETADDRESS_H_WEGUF5SN

#include <cstdint>
#include <string>

class SocketAddress {
    public:
        SocketAddress(const std::string hostname, const uint16_t port);

        SocketAddress(const uint32_t ip, const uint16_t port);

    private:
        std::string hostname_;
        uint32_t ip_;
        uint16_t port_;
};

#endif /* end of include guard: SOCKETADDRESS_H_WEGUF5SN */

