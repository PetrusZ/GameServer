/*
 * =====================================================================================
 *
 *       Filename:  Master.cc
 *
 *    Description:  Master cpp file
 *
 *        Version:  1.0
 *        Created:  03/26/2015 01:58:20 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#include "Master.h"
#include <iostream>
#include <signal.h>

Master::Master() {

}

Master::~Master() {

}

bool Master::Run(int argc, char** argv) {
    HookSignal();
    std::cout << "Hello Game Server From Master" << std::endl;
    UnHookSignal();
    return true;
}

void Master::OnSignal(int signal) {
    switch (signal) {
        case SIGHUP:
            break;
        default:
            break;
    }
	::signal(signal, OnSignal);
}

void Master::HookSignal() {
    signal(SIGINT, OnSignal);
	signal(SIGTERM, OnSignal);
	signal(SIGABRT, OnSignal);
	signal(SIGHUP, OnSignal);
	signal(SIGUSR1, OnSignal);
}

void Master::UnHookSignal() {
    signal(SIGINT, 0);
	signal(SIGTERM, 0);
	signal(SIGABRT, 0);
	signal(SIGHUP, 0);
	signal(SIGUSR1, 0);
}
