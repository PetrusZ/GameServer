/*
 * =====================================================================================
 *
 *       Filename:  Common.h
 *
 *    Description:  Common
 *
 *        Version:  1.0
 *        Created:  04/01/2015 12:37:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef COMMON_H_WN5MDJKE
#define COMMON_H_WN5MDJKE

#include "Logger.h"
#include "Status.h"

#include <cstdlib>
#include <vector>
#include <string>

#if !defined(DEBUG)
#define ASSERT(EXPR) if (!(EXPR)) { Common::AssertFailed(__FILE__, __LINE__, #EXPR); abort(); }
#else
#define ASSERT(EXPR)
#endif

#define STACK_SIZE 10

namespace Common {
    extern int BackTrace(std::vector<std::string>& stack_trace);
    extern void AssertFailed(const char* file, const int line, const char* expr);
}

#endif /* end of include guard: COMMON_H_WN5MDJKE */
