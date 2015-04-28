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
#include "port/Env.h"

void ThreadPool::Startup() {
    for (int i = 0; i < kThreadReserve; ++i) {
        StartThread(NULL);
    }

    LOG_TRACE("ThreadPool startup, launched %d threads.", kThreadReserve);
}

void ThreadPool::Shutdown() {
    mutex_.Lock();
    int threads_count = active_threads_.size() + idle_threads_.size();
    LOG_TRACE("ThreadPool, Shutting down %u threads", threads_count);
    KillIdleThreads(idle_threads_.size());
    threads_to_exit_ += active_threads_.size();

    for (auto it = active_threads_.begin(); it != active_threads_.end(); ++it) {
        Thread* thread = *it;

        if (thread->ExecutionTarget_) {
            thread->ExecutionTarget_->OnShutdown();
        } else {
            thread->ControlInterface_.Signal();
        }
    }
    mutex_.UnLock();

    for (int i = 0; ; ++i)  {
        mutex_.Lock();
        if (active_threads_.size() || idle_threads_.size()) {
            if (idle_threads_.size() != 0) {
                /* if we are here then a thread in the free pool checked if it was being shut down just before CThreadPool::Shutdown() was called
                   but called Suspend() just after KillFreeThreads(). All we need to do is to resume it.*/
                Thread* thread;
                for (auto it = idle_threads_.begin(); it != idle_threads_.end(); ++it) {
                    thread = *it;
                    thread->ControlInterface_.Signal();
                }
            }
            LOG_TRACE("ThreadPool Shutdown", "%u active threads and %u idle threads remaining...", active_threads_.size(), idle_threads_.size());
            mutex_.UnLock();
            sEnv.Sleep(1000);
            continue;
        }

        mutex_.UnLock();
        break;
    }
}

void ThreadPool::IntegrityCheck() {
    mutex_.Lock();
    int32_t gobbled = threads_eaten_;

    if (gobbled < 0) {
        // this means we requested more threads than we had in the pool last time.
        // spawn "gobbled" + THREAD_RESERVE extra threads.

        uint32_t new_threads_count = abs(gobbled) + kThreadReserve;
        threads_eaten_ = 0;
        for (uint32_t i = 0; i < new_threads_count; ++i) {
            StartThread(NULL);
        }

        LOG_TRACE("ThreadPool, IntegrityCheck: (gobbled(%d) < 0), spawning %u threads", gobbled, new_threads_count);
    } else if (gobbled < kThreadReserve) {
        // this means while we didn't run out of threads, we were getting damn low.
        // spawn enough threads to keep the reserve amount up.

        uint32_t new_threads_count = kThreadReserve - gobbled;
        for (uint32_t i = 0; i < new_threads_count; ++i) {
            StartThread(NULL);
        }

        LOG_TRACE("ThreadPool, IntegrityCheck: (gobbled(%d) < %u), spawning %u threads", gobbled, kThreadReserve, new_threads_count);
    } else if (gobbled > kThreadReserve) {
        // this means we had "excess" threads sitting around doing nothing.
        // lets kill some of them off.

        uint32_t kill_threads_count = gobbled - kThreadReserve;
        KillIdleThreads(kill_threads_count);
        threads_eaten_ -= kill_threads_count;

        LOG_TRACE("ThreadPool, IntegrityCheck: (gobbled(%d) > %u), killing %u threads", gobbled, kThreadReserve, kill_threads_count);
    } else {
        // perfect! we have the ideal number of idle threads.

        LOG_TRACE("ThreadPool, IntegrityCheck: Perfect!");
    }

    threads_requested_since_last_check_ = 0;
    threads_exited_since_last_check_ = 0;
    threads_freed_since_last_check_ = 0;

    mutex_.UnLock();
}

Thread* ThreadPool::StartThread(ThreadBase *execution_target) {
    pthread_t pthread_id;
    Thread* thread = new Thread;
    thread->ExecutionTarget_ = execution_target;
    thread->DeleteAfterExit_ = false;

    mutex_.Lock();
    pthread_create(&pthread_id, NULL, ThreadProcess, (void*)thread);
    thread->ControlInterface_.Setup(pthread_id);
    pthread_detach(pthread_id);
    mutex_.UnLock();

    return thread;
}

bool ThreadPool::ThreadExit(Thread *thread) {
    mutex_.Lock();

    active_threads_.erase(thread);

    if (threads_to_exit_ > 0) {
        --threads_to_exit_;
        ++threads_exited_since_last_check_;
        if (thread->DeleteAfterExit_) {
            idle_threads_.erase(thread);
        }
        mutex_.UnLock();

        LOG_TRACE("ThreadPool, thread %u exit.", thread->ControlInterface_.thread_id());
        delete thread;
        return true;
    }

    ++threads_exited_since_last_check_;
    ++threads_eaten_;

    auto it = idle_threads_.find(thread);
    ASSERT(it == idle_threads_.end());

    idle_threads_.insert(thread);
    LOG_TRACE("ThreadPool, thread %u entered the idle pool.", thread->ControlInterface_.thread_id());

    mutex_.UnLock();
    return false;
}

void ThreadPool::ExecuteTask(ThreadBase *execution_target) {
    Thread* thread;
    mutex_.Lock();
    ++threads_requested_since_last_check_;
    --threads_eaten_;

    if (idle_threads_.size()) {
        thread = *idle_threads_.begin();
        idle_threads_.erase(idle_threads_.begin());

        thread->ExecutionTarget_ = execution_target;
        thread->ControlInterface_.Signal();

        LOG_TRACE("ThreadPool, Thread %u left idle thread pool", thread->ControlInterface_.thread_id());
    } else {
        thread = StartThread(execution_target);
    }

    LOG_TRACE("ThreadPool, Thread %u is now executing task at %p", thread->ControlInterface_.thread_id(), execution_target);

    active_threads_.insert(thread);
    mutex_.UnLock();
}

void ThreadPool::KillIdleThreads(uint32_t count) {
    LOG_TRACE("ThreadPool, Killing %u idle threads", count);

    mutex_.Lock();
    Thread* thread;
    auto it = idle_threads_.begin();

    for (uint32_t i = 0; i < count && it != idle_threads_.end(); ++i) {
        thread = *it;
        thread->ExecutionTarget_ = NULL;
        thread->DeleteAfterExit_ = true;
        thread->ControlInterface_.Signal();
        ++it;
    }

    mutex_.UnLock();
}

void ThreadPool::ShowStatus() {
    mutex_.Lock();
    LOG_TRACE("ThreadPool, ============ ThreadPool Status =============");
    LOG_TRACE("ThreadPool, Active Threads: %u", active_threads_.size());
    LOG_TRACE("ThreadPool, Idle Threads: %u", idle_threads_.size());
    LOG_TRACE("ThreadPool, Requested-To-Freed Ratio: %.3f%% (%u/%u)", float(float(threads_requested_since_last_check_ + 1) / float(threads_exited_since_last_check_ + 1) * 100.0f), threads_requested_since_last_check_, threads_exited_since_last_check_);
    LOG_TRACE("ThreadPool, Eaten Count: %d (negative is bad!)", threads_eaten_);
    LOG_TRACE("ThreadPool, ============================================");
    mutex_.UnLock();
}

void* ThreadPool::ThreadProcess(void *arg) {
    Thread* thread = (Thread*)arg;
    thread->SetupMutex_.Lock();
    LOG_TRACE("ThreadPool, Thread %u started.", thread->ControlInterface_.thread_id());
    thread->SetupMutex_.UnLock();

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

    LOG_TRACE("ThreadPool, thread %u real exit.", thread->ControlInterface_.thread_id());
    pthread_exit(NULL);
}
