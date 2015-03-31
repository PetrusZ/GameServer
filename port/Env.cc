/*
 * =====================================================================================
 *
 *       Filename:  Env.cc
 *
 *    Description:  Env
 *
 *        Version:  1.0
 *        Created:  03/31/2015 11:54:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#include "Env.h"
#include <pthread.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <stdio.h>

#define DATE_LONGTH (100)
#define DATE_FORMAT "%Y-%m-%d"

#define TIME_LONGTH (100)
#define TIME_FORMAT "%H-%m-%S"

uint64_t Env::GetTid() {
    pthread_t tid = pthread_self();
    uint64_t thread_id;
    memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
    return thread_id;
}

std::string Env::GetDate() {
    time_t now = time(NULL);
    char date[DATE_LONGTH];
    strftime(date, DATE_LONGTH, DATE_FORMAT, gmtime(&now));

    return date;
}

std::string Env::GetTime() {
    time_t now = time(NULL);
    char time[TIME_LONGTH];
    strftime(time, TIME_LONGTH, TIME_FORMAT, gmtime(&now));

    return time;
}
