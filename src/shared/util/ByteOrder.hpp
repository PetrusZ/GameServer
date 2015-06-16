/*
 * =====================================================================================
 *
 *       Filename:  ByteOrder.hpp
 *
 *    Description:  ByteOrder
 *
 *        Version:  1.0
 *        Created:  04/13/2015 11:42:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef BYTEORDER_HPP_GY7LNWF3
#define BYTEORDER_HPP_GY7LNWF3

#include <cstdint>
#include <arpa/inet.h>

namespace util {
    inline uint16_t NetworkToHost16(uint16_t net) {
        return ntohs(net);
    }

    inline uint32_t NetworkToHost32(uint32_t net) {
        return ntohl(net);
    }

    inline uint16_t HostToNetwork16(uint16_t host) {
        return htons(host);
    }

    inline uint32_t HostToNetwork32(uint32_t host) {
        return htonl(host);
    }
}

#endif /* end of include guard: BYTEORDER_HPP_GY7LNWF3 */
