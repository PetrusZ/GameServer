/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.cc
 *
 *    Description:  ThreadPool
 *
 *        Version:  1.0
 *        Created:  2015年03月29日 10时33分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#include "ThreadPool.h"
#include "common/Common.h"

void ThreadPool::Startup() {
    for (int i = 0; i < kThreadReserve; ++i) {
        StartThread(NULL);
    }

    LOG_DEBUG("ThreadPool startup, launched %d threads.", kThreadReserve);
}

void ThreadPool::Shutdown() {

}

Thread* ThreadPool::StartThread(ThreadBase *execution_target) {
    pthread_t pthread_id;
    Thread* thread = new Thread;
    thread->ExecutionTarget_ = execution_target;
    thread->DeleteAfterExit_ = false;

    mutex_.Lock();
    pthread_create(&pthread_id, NULL, ThreadProcess, (void*)thread);
    thread->ControlInterface_.Init(pthread_id);
    pthread_detach(pthread_id);
    mutex_.UnLock();

    return thread;
}

bool ThreadPool::ThreadExit(Thread *thread) {
    mutex_.Lock();

    active_threads_.erase(thread);

    if (thread->DeleteAfterExit_) {
        idle_threads_.erase(thread);
        mutex_.UnLock();

        LOG_DEBUG("ThreadPool, thread %u exit.", thread->ControlInterface_.thread_id());
        delete thread;
        return true;
    }

    auto it = idle_threads_.find(thread);
    ASSERT(it == idle_threads_.end());

    idle_threads_.insert(thread);

    mutex_.UnLock();
    return false;
}

void* ThreadPool::ThreadProcess(void *arg) {
    Thread* thread = (Thread*)arg;
    LOG_DEBUG("ThreadPool, Thread %u started.", thread->ControlInterface_.thread_id());

    while(true) {
        if (NULL != thread->ExecutionTarget_) {
            if (thread->ExecutionTarget_->Run()) {
                delete thread->ExecutionTarget_;
            }
            thread->ExecutionTarget_ = NULL;
        }

        if (sThreadPool.ThreadExit(thread)) {
            break;
        } else {
            thread->ControlInterface_.Wait();
        }
    }

    LOG_DEBUG("ThreadPool, thread %u real exit.", thread->ControlInterface_.thread_id());
    pthread_exit(NULL);
}
