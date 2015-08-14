/*
 * =====================================================================================
 *
 *       Filename:  World.cc
 *
 *    Description:  World
 *
 *        Version:  1.0
 *        Created:  06/01/2015 06:28:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "World.h"
#include "network/TcpServerBase.h"

void World::Update(uint64_t diff) {
    Packet_t* packet;
    while((packet = sTcpServer.PopClientPacket()) != nullptr) {
        LOG_KINFO("World Update", "Receive fd(%d) length(%d) data: %s", packet->fd_, packet->data_len_, packet->data_);
    }
}
