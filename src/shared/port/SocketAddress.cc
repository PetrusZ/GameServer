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
#include "util/ByteOrder.hpp"
#include <cstring>

SocketAddress::SocketAddress(const std::string hostname, const uint16_t port) {
    SetIP(hostname);
    SetPort(port);
}

SocketAddress::SocketAddress(const uint32_t ip, const uint16_t port) {
    SetIP(ip);
    SetPort(port);
}

void SocketAddress::ToSockAddr(struct sockaddr_in *sockaddr) const {
    memset(sockaddr, 0, sizeof(*sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = util::HostToNetwork16(port_);
    if (0 == ip_) {
        sockaddr->sin_addr.s_addr = INADDR_ANY;
    } else {
        sockaddr->sin_addr.s_addr = util::HostToNetwork32(ip_);
    }
}

void SocketAddress::FromSockAddr(const struct sockaddr_in &saddr) {
    SetIP(util::NetworkToHost32(saddr.sin_addr.s_addr));
    SetPort(util::NetworkToHost16(saddr.sin_port));
}

void SocketAddress::SetIP(const uint32_t ip) {
    hostname_.clear();
    ip_ = ip;
}

void SocketAddress::SetIP(const std::string &hostname) {
    hostname_= hostname;
    ip_ = StringToIP(hostname);
}

void SocketAddress::SetPort(const int port) {
    port_ = port;
}

bool SocketAddress::StringToIP(const std::string &hostname, uint32_t *ip) {
    in_addr addr;
    if (inet_aton(hostname.c_str(), &addr) == 0) {
        return false;
    }
    *ip = util::NetworkToHost32(addr.s_addr);
    return true;
}

uint32_t SocketAddress::StringToIP(const std::string &hostname) {
    uint32_t ip;
    StringToIP(hostname, &ip);
    return ip;
}

std::string SocketAddress::IPAsString() const {
    if (!hostname_.empty()) {
        return hostname_;
    }
    return IPToString();
}

std::string SocketAddress::IPToString() const {
    struct in_addr paddr;
    paddr.s_addr = util::HostToNetwork32(ip_);
    return std::string(inet_ntoa(paddr));
}
