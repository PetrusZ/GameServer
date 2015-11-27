/*
 * =====================================================================================
 *
 *       Filename:  CThread.cc
 *
 *    Description:  CThread
 *
 *        Version:  1.0
 *        Created:  11/06/2015 03:53:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "CThread.h"

CThread::CThread() : ThreadBase() {
    thread_state_.SetVal(THREADSTATE_AWAITING);
    start_time_ = 0;
}

CThread::~CThread() {

}

bool CThread::Run() {
    return false;
}

void CThread::OnShutdown() {
    running_ = false;
    thread_state_.SetVal(THREADSTATE_TERMINATE);
}
