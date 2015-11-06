/*
 * =====================================================================================
 *
 *       Filename:  RWLock.cc
 *
 *    Description:  RWLock
 *
 *        Version:  1.0
 *        Created:  04/23/2015 02:29:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "RWLock.h"

RWLock::RWLock() {
    int result = pthread_rwlock_init(&rwlock_, NULL);
    if (0 != result) {
        //error handler
    }
}

RWLock::~RWLock() {
    int result = pthread_rwlock_destroy(&rwlock_);
    if (0 != result) {
        //error handler
    }
}

bool RWLock::ReadLock() {
    int result = pthread_rwlock_rdlock(&rwlock_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool RWLock::WriteLock() {
    int result = pthread_rwlock_wrlock(&rwlock_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool RWLock::TryReadLock() {
    int result = pthread_rwlock_tryrdlock(&rwlock_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool RWLock::TryWriteLock() {
    int result = pthread_rwlock_trywrlock(&rwlock_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}
