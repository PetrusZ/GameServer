/*
 * =====================================================================================
 *
 *       Filename:  Master.cc
 *
 *    Description:  Master cpp file
 *
 *        Version:  1.0
 *        Created:  03/26/2015 01:58:20 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#include "Master.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include "port/ThreadPool.h"
#include "network/TcpServer.h"
#include "network/TcpWatcherThread.h"

void Master::Daemonize() {
    int fd0, fd1, fd2;
    pid_t   pid;
    struct rlimit   rl;
    struct sigaction sa;

    // 清除文件创建屏蔽字
    umask(0);

    //XXX: 获得最大文件描述符数量？
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        std::cerr << "can't get file limit" << std::endl;
        exit(-1);
    }

    // Become a session leader to lose controlling TTY.
    if ((pid = fork()) < 0) {
        std::cerr << "can't fork" << std::endl;
        exit(-1);
    } else if (pid != 0) {
        exit(0);
    }
    setsid();

    // Ensure future opens won't allocate controlling TTYs.
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        std::cerr << "can't sgnore SIGHUP" << std::endl;
        exit(-1);
    }
    if ((pid = fork()) < 0) {
        std::cerr << "can't fork" << std::endl;
        exit(-1);
    } else if (pid != 0) {
        exit(0);
    }

    /*
    if (chdir("/") < 0) {
        std::cerr << "can't change directory to /" << std::endl;
        exit(-1);
    }
    */

    // 关闭所有打开的文件描述符
    if (rl.rlim_cur == RLIM_INFINITY) {
        rl.rlim_cur = 1024;
    }
    for (size_t i = 0; i < rl.rlim_cur; ++i) {
        close(i);
    }

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    if (fd0 != 0 || fd1 != 1 || fd2 !=2 ) {
        std::cerr << "unexpected file descriptors " << fd0 << " " <<fd1 << " " << fd2 << " " << std::endl;
        exit(-1);
    }
}

bool Master::Run(int argc, char** argv) {
    std::cout << "Hello Game Server From Master" << std::endl;
    Daemonize();
    HookSignal();

    sThreadPool.Startup();
    sThreadPool.ExecuteTask(new TcpWatcherThread("127.0.0.1", 19191));

    while(true) {
    }

    UnHookSignal();
    return true;
}

void Master::OnSignal(int signal) {
    switch (signal) {
        case SIGHUP:
            // smooth restart
            LOG_TRACE("Received sigal SIGHUP(%d), server restarting.", SIGHUP);
            break;
        case SIGTERM:
            // progress terminated, quick quit
            LOG_TRACE("Received sigal SIGTERM(%d), server closing.", SIGTERM);
            exit(EXIT_SUCCESS);
            break;
            /*
        case SIGABRT:
            // ASSERT failed or catch SIGABRT from outside.
            LOG_TRACE("Received sigal SIGABRT(%d), server closing.", SIGABRT);
            exit(EXIT_SUCCESS);
            break;
            */
        case SIGCHLD:
            // child progress terminated.
            break;
        case SIGINT:
            // quick quit, almost cause Ctrl+C
            LOG_TRACE("Received sigal SIGINT(%d), server closing.", SIGINT);
            exit(EXIT_SUCCESS);
            break;
        case SIGQUIT:
            // easy quit, cause Ctrl+"\"
            LOG_TRACE("Received sigal SIGINT(%d), server closing.", SIGINT);
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
}

void Master::HookSignal() {
    Common::Signal(SIGINT, OnSignal);
    Common::Signal(SIGTERM, OnSignal);
	Common::Signal(SIGABRT, OnSignal);
	Common::Signal(SIGHUP, OnSignal);
	Common::Signal(SIGQUIT, OnSignal);
    Common::Signal(SIGCHLD, OnSignal);
}

void Master::UnHookSignal() {
    Common::Signal(SIGINT, SIG_DFL);
	Common::Signal(SIGTERM, SIG_DFL);
	Common::Signal(SIGABRT, SIG_DFL);
	Common::Signal(SIGHUP, SIG_DFL);
	Common::Signal(SIGQUIT, SIG_DFL);
	Common::Signal(SIGCHLD, SIG_DFL);
}
