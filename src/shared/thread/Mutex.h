/*
 * =====================================================================================
 *
 *       Filename:  Mutex.h
 *
 *    Description:  Mutex
 *
 *        Version:  1.0
 *        Created:  04/23/2015 11:34:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef MUTEX_H_WMU51HFT
#define MUTEX_H_WMU51HFT

#include <pthread.h>

class Mutex {
    public:
        Mutex();
        ~Mutex();

        bool Lock();
        bool TryLock();
        bool UnLock();

    private:
        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;

        pthread_mutex_t mutex_;

        static bool attr_initalized_;
        static pthread_mutexattr_t attr_;
};

#endif /* end of include guard: MUTEX_H_WMU51HFT */
