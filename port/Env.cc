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
#define TIME_FORMAT "%H:%M:%S"

uint64_t Env::GetTid() {
    pthread_t tid = pthread_self();
    uint64_t thread_id;
    memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
    return thread_id;
}

uint64_t Env::GetRealMSTime() {
    UpdateTime();
    return (tv_.tv_sec * 1000) + (tv_.tv_usec / 1000);
}

uint64_t Env::GetMSTime() {
    return (tv_.tv_sec * 1000) + (tv_.tv_usec / 1000);
}

uint32_t Env::GetNowStamp() {
    return tv_.tv_sec;
}

std::string Env::GetDate() {
    time_t now = GetNowStamp();
    char date[DATE_LONGTH];
    strftime(date, DATE_LONGTH, DATE_FORMAT, localtime(&now));

    return date;
}

std::string Env::GetTime() {
    time_t now = GetNowStamp();
    char time[TIME_LONGTH];
    strftime(time, TIME_LONGTH, TIME_FORMAT, localtime(&now));

    return time;
}

void Env::Sleep(unsigned long time_ms) {
    timespec tv;
    tv.tv_sec = time_ms / 1000;
    tv.tv_nsec = (time_ms % 1000) * 1000 * 1000;

    nanosleep(&tv, NULL);
}

void Env::UpdateTime() {
    gettimeofday(&tv_, NULL);
}
