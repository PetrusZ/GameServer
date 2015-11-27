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
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

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

    Sigfunc* Signal(int signo, Sigfunc* func) {
        struct sigaction act, oact;

        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;
#endif
        } else {
#ifdef SA_RESTART
            act.sa_flags |= SA_RESTART;
#endif
        }

        if (sigaction(signo, &act, &oact) < 0) {
            return SIG_ERR;
        } else {
            return oact.sa_handler;
        }
    }

    uint32_t WritePID(const char* process_name) {
        std::string pid_file = process_name;
        pid_file += ".pid";
        int fd = open(pid_file.c_str(), O_CREAT | O_WRONLY, S_IRWXU);

        if (-1 != fd) {
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 1;
            if (fcntl(fd, F_SETLK, &lock) < 0){
                close(fd);
                return 0;
            }

            ftruncate(fd, 0);

            uint32_t pid = getpid();
            FILE* fp = fdopen(fd, "w");
            fprintf(fp, "%u", pid);
            fflush(fp);

            return pid;
        }

        return 0;
    }
}
