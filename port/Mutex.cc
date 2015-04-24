/*
 * =====================================================================================
 *
 *       Filename:  Mutex.cc
 *
 *    Description:  Mutex
 *
 *        Version:  1.0
 *        Created:  04/23/2015 11:47:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "Mutex.h"

bool Mutex::attr_initalized_ = false;
pthread_mutexattr_t Mutex::attr_;

Mutex::Mutex() {
    if (!attr_initalized_) {
        pthread_mutexattr_init(&attr_);
        pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
        attr_initalized_ = true;
    }
    pthread_mutex_init(&mutex_, &attr_);
}

Mutex::~Mutex() {
    int result = pthread_mutex_destroy(&mutex_);
    if (0 != result) {
        //error handler
    }
}

bool Mutex::Lock() {
    int result = pthread_mutex_lock(&mutex_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool Mutex::TryLock() {
    int result = pthread_mutex_trylock(&mutex_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}

bool Mutex::UnLock() {
    int result = pthread_mutex_unlock(&mutex_);
    if (0 == result) {
        return true;
    } else {
        return false;
    }
}
