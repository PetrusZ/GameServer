/*
 * =====================================================================================
 *
 *       Filename:  TcpWorkerThread.c
 *
 *    Description:  TcpWorkerThread
 *
 *        Version:  1.0
 *        Created:  04/29/2015 04:31:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "TcpWorkerThread.h"
#include "common/Common.h"

bool TcpWorkerThread::Run() {
    running_ = true;
    LOG_INFO("TcpWorkerThread, Receive fd(%d) length(%d) data: %s", fd_, data_len_, data_);
    return true;
}

void TcpWorkerThread::OnShutdown() {
    running_ = false;
}
