/*
 * =====================================================================================
 *
 *       Filename:  QueryWorkerThread.h
 *
 *    Description:  QueryWorkerThread
 *
 *        Version:  1.0
 *        Created:  11/26/2015 05:42:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef QUERYWORKERTHREAD_H_N6XTUP1I
#define QUERYWORKERTHREAD_H_N6XTUP1I

#include "MySQLDatabase.h"

class QueryWorkerThread : public CThread {
    public:
        QueryWorkerThread(void* packet) : CThread(), packet_(packet) {}
        virtual ~QueryWorkerThread() {}

        bool Run();

    private:
        void* packet_;
};

#endif /* end of include guard: QUERYWORKERTHREAD_H_N6XTUP1I */
