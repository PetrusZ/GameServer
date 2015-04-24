/*
 * =====================================================================================
 *
 *       Filename:  ConditionVariable.h
 *
 *    Description:  ConditionVariable
 *
 *        Version:  1.0
 *        Created:  04/23/2015 03:10:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef CONDITIONVARIABLE_H_JPXQ47PB
#define CONDITIONVARIABLE_H_JPXQ47PB

#include <pthread.h>

class ConditionVariable {
    public:
        ConditionVariable();
        virtual ~ConditionVariable();

        bool Wait(const struct timespec* timeout = nullptr);
        bool Signal();
        bool BroadCast();

    private:
        ConditionVariable(const ConditionVariable&) = delete;
        ConditionVariable& operator=(const ConditionVariable&) = delete;

        pthread_cond_t cond_;
        pthread_mutex_t mutex_;
};

#endif /* end of include guard: CONDITIONVARIABLE_H_JPXQ47PB */
