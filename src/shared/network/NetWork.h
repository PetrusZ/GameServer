/*
 * =====================================================================================
 *
 *       Filename:  Network.h
 *
 *    Description:  Network
 *
 *        Version:  1.0
 *        Created:  06/16/2015 04:36:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef NETWORK_H_MQXWXPUD
#define NETWORK_H_MQXWXPUD

#include "port/Socket.h"

struct Packet_t {
    Packet_t(SOCKET fd, size_t data_len, const void* data) : fd_(fd), data_len_(data_len), data_(data) { }
    ~Packet_t() {
        delete[] static_cast<char*>(const_cast<void*>(data_));
    }

    SOCKET fd_;
    size_t data_len_;
    const void* data_;
};

#endif /* end of include guard: NETWORK_H_MQXWXPUD */
