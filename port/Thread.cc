/*
 * =====================================================================================
 *
 *       Filename:  Thread.cc
 *
 *    Description:  Thread
 *
 *        Version:  1.0
 *        Created:  04/24/2015 06:09:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "Thread.h"

volatile int ThreadController::thread_id_count_ = 0;
