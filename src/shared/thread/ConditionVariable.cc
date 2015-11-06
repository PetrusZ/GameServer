/*
 * =====================================================================================
 *
 *       Filename:  ConditionVariable.cc
 *
 *    Description:  ConditionVariable
 *
 *        Version:  1.0
 *        Created:  04/23/2015 03:13:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "ConditionVariable.h"

ConditionVariable::ConditionVariable() {
    int result1 = pthread_cond_init(&cond_, NULL);
    int result2 = pthread_mutex_init(&mutex_, NULL);
    if (0 != result1 || 0 != result2) {
        //error handler
    }
}

ConditionVariable::~ConditionVariable() {
    int result1 = pthread_cond_destroy(&cond_);
    int result2 = pthread_mutex_destroy(&mutex_);
    if (0 != result1 || 0 != result2) {
        //error handler
    }
}

bool ConditionVariable::Wait(const struct timespec* timeout) {
    int result;
    if (timeout != nullptr) {
        pthread_mutex_lock(&mutex_);
        result = pthread_cond_timedwait(&cond_, &mutex_, timeout);
        pthread_mutex_unlock(&mutex_);
    } else {
        pthread_mutex_lock(&mutex_);
        result = pthread_cond_wait(&cond_, &mutex_);
        pthread_mutex_unlock(&mutex_);
    }
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool ConditionVariable::Signal() {
    int result = pthread_cond_signal(&cond_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool ConditionVariable::BroadCast() {
    int result = pthread_cond_broadcast(&cond_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}
