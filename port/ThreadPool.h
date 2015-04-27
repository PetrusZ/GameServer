/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.h
 *
 *    Description:  ThreadPool
 *
 *        Version:  1.0
 *        Created:  2015年03月29日 10时32分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#ifndef THREADPOOL_H_O4CDDLR9
#define THREADPOOL_H_O4CDDLR9

#include <set>
#include "Thread.h"
#include "base/Singleton.hpp"

class ThreadPool : public Singleton <ThreadPool> {
    public:
        // ThreadPool();
        // virtual ~ThreadPool();

        void Startup();

        // shutdown all threads
        void Shutdown();

        // creates a thread, returns a handle to it.
        Thread* StartThread(ThreadBase* execution_target);

        // grabs/spawns a thread, and tells it to execute a task.
        void ExecuteTask(ThreadBase* execution_target);

        // return false - suspend ourselves, and wait for a future task.
        // return true - exit, we're shutting down or no longer needed.
        bool ThreadExit(Thread* thread);

        // prints some neat debug stats
        void ShowStatus();

        static void* ThreadProcess(void *arg);

    private:
        const int kThreadReserve = 10;

        uint32_t threads_requested_since_last_check_;
        uint32_t threads_freed_since_last_check_;
        uint32_t threads_exited_since_last_check_;
        uint32_t threads_to_exit_;
        int32_t threads_eaten_;

        std::set<Thread*> active_threads_;
        std::set<Thread*> idle_threads_;
        Mutex mutex_;
};

#define sThreadPool ThreadPool::getSingleton()

#endif /* end of include guard: THREADPOOL_H_O4CDDLR9 */
