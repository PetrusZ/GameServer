/*
 * =====================================================================================
 *
 *       Filename:  Common.cc
 *
 *    Description:  Common
 *
 *        Version:  1.0
 *        Created:  04/02/2015 11:20:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "Common.h"
#include <execinfo.h>

namespace Common {
    int BackTrace(std::vector<std::string>& stack_trace) {
        int stack_num = 0;
        void *buff[STACK_SIZE];
        char **stack_trace_ptr = NULL;

        stack_num = backtrace(buff, STACK_SIZE);
        stack_trace_ptr = backtrace_symbols(buff, stack_num);

        if (stack_trace_ptr) {
            for (int i = 2; i < stack_num; ++i) {
                stack_trace.emplace_back(stack_trace_ptr[i]);
            }

            free(stack_trace_ptr);

            return stack_num - 2;
        }

        return 0;
    }

    void AssertFailed(const char* file, const int line, const char* expr) {
        int stack_num;
        std::vector<std::string> stack_trace;

        stack_num = BackTrace(stack_trace);

        LOG_ERROR("Assert Failed: %s", expr);
        LOG_ERROR("Location: %s(%d)", file, line);

        if (stack_trace.size() > 0) {
            LOG_ERROR("Stack Trace:");
            int i = 1;
            for (auto stack : stack_trace) {
                if (i < stack_num) {
                    LOG_ERROR("#%d: %s", i, stack.data());
                } else {
                    LOG_ERROR("#%d: %s\n", i, stack.data());
                }
                ++i;
            }
        } else {
            LOG_ERROR("No Stack Trace Info!\n");
        }
    }
}

