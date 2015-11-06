/*
 * =====================================================================================
 *
 *       Filename:  CThread.h
 *
 *    Description:  CThread 
 *
 *        Version:  1.0
 *        Created:  11/05/2015 05:25:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef CTHREAD_H_TPAOH4SK
#define CTHREAD_H_TPAOH4SK

#include "Thread.h"
#include "AtomicULong.h"

enum CThreadState {
    THREADSTATE_TERMINATE   = 0,
    THREADSTATE_PAUSED,
    THREADSTATE_SLEEPING,
    THREADSTATE_BUSY,
    THREADSTATE_AWAITING
};

class CThread : public ThreadBase {
    public:
        CThread();
        virtual ~CThread();

        void SetThreadState(CThreadState thread_state) { thread_state_.SetVal(thread_state); }
        CThreadState GetThreadState() {
            unsigned long val = thread_state_.GetVal();
            return static_cast<CThreadState>(val);
        }

        time_t GetStartTime() { return start_time_; };

        virtual bool Run();
        virtual void OnShutdown();

    protected:
        bool        running_;
        AtomicULong thread_state_;
        time_t      start_time_;

    private:
        CThread(const CThread&) = delete;
        CThread& operator=(CThread&) = delete;

};

#endif /* end of include guard: CTHREAD_H_TPAOH4SK */
