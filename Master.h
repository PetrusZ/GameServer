/*
 * =====================================================================================
 *
 *       Filename:  Master.cc
 *
 *    Description:  Game Server Master
 *
 *        Version:  1.0
 *        Created:  2015年03月25日 23时10分55秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#ifndef MASTER_CC_XPRMPWDX
#define MASTER_CC_XPRMPWDX

#include "base/Singleton.hpp"

class Master : public Singleton<Master> {
    public:
        Master();
        ~Master();

        bool Run(int argc, char** argv);

    private:
        void Daemonize();
        void HookSignal();
        void UnHookSignal();

        static void OnSignal(int signal);
};

#define sMaster Master::getSingleton()

#endif /* end of include guard: MASTER_CC_XPRMPWDX */
