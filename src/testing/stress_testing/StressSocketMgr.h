/*
 * =====================================================================================
 *
 *       Filename:  StressSocketMgr.h
 *
 *    Description:  StressSocketMgr
 *
 *        Version:  1.0
 *        Created:  01/25/2016 04:58:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef STRESSSOCKETMGR_H_AHQ0IM6U
#define STRESSSOCKETMGR_H_AHQ0IM6U

#include <set>

#include "port/Socket.h"
#include "base/Singleton.hpp"

class StressSocketMgr : public Singleton <StressSocketMgr> {
    public:
        void AddSocket(Socket* socket) { socket_set_.insert(socket); }

        void Reset() { it_ = socket_set_.begin(); }
        const Socket* Next() { return it_ != socket_set_.end() ? *(it_++) : nullptr; }

    private:
        std::set<Socket*>::const_iterator it_;
        std::set<Socket*> socket_set_;
};

#define sStressSocketMgr StressSocketMgr::getSingleton()

#endif /* end of include guard: STRESSSOCKETMGR_H_AHQ0IM6U */
