/*
 * =====================================================================================
 *
 *       Filename:  PacketMgr.h
 *
 *    Description:  PacketMgr
 *
 *        Version:  1.0
 *        Created:  06/16/2015 04:50:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef PACKETMGR_H_TXWGO6KZ
#define PACKETMGR_H_TXWGO6KZ

#include <queue>
#include "Network.h"

class PacketMgr {
    public:
        PacketMgr();
        virtual ~PacketMgr();

    private:
        std::queue<Packet_t> recv_queue_;
};

#endif /* end of include guard: PACKETMGR_H_TXWGO6KZ */
