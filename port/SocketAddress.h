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
#include <sys/socket.h>

class SocketAddress {
    public:
        SocketAddress(const std::string hostname, const uint16_t port);

        SocketAddress(const uint32_t ip, const uint16_t port);

        void ToSockAddr(struct sockaddr_in* sockaddr) const;
        void FromSockAddr(const struct sockaddr_in& saddr);

        void SetIP(const uint32_t ip);
        void SetIP(const std::string& hostname);
        void SetPort(const int port);

        bool StringToIP(const std::string& hostname, uint32_t* ip);
        uint32_t StringToIP(const std::string& hostname);

        std::string IPAsString() const;
        std::string IPToString() const;

        uint32_t ip() const { return ip_; }
        uint16_t port() const { return port_; }
        std::string hostname() const { return hostname_; }

    private:
        std::string hostname_;
        uint32_t ip_;
        uint16_t port_;
};

#endif /* end of include guard: SOCKETADDRESS_H_WEGUF5SN */

