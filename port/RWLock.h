/*
 * =====================================================================================
 *
 *       Filename:  RWLock.h
 *
 *    Description:  RWLock
 *
 *        Version:  1.0
 *        Created:  04/23/2015 02:27:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef RWLOCK_H_86CAQHNY
#define RWLOCK_H_86CAQHNY

#include <pthread.h>

class RWLock {
    public:
        RWLock();
        virtual ~RWLock();

        RWLock(const RWLock&) = delete;
        RWLock& operator=(const RWLock&) = delete;

        bool ReadLock();
        bool WriteLock();
        bool UnLock();

        bool TryReadLock();
        bool TryWriteLock();

    private:
        pthread_rwlock_t rwlock_;
};

#endif /* end of include guard: RWLOCK_H_86CAQHNY */
