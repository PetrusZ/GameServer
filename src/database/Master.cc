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
#include <iostream>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>

#include "Master.h"
#include "thread/ThreadPool.h"
#include "network/TcpServerBase.h"
#include "network/TcpServerBaseThread.h"
#include "MySQLDatabase.h"
#include "QueryWorkerThread.h"

volatile bool Master::stop_event_ = false;

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
    LOG_STDOUT("Hello Database Server From Master");

    Daemonize();
    HookSignal();

    sLogger.Init("Database");

    uint32_t pid = Common::WritePID("Database");
    if (!pid) {
        LOG_ERROR("Encounter ERROR while starting, can't write pid!");
        return false;
    }
    LOG_TRACE("Process ID: %u", pid);

    sThreadPool.Startup();
    sThreadPool.ExecuteTask(new TcpServerBaseThread("127.0.0.1", 19191));

    // Start Mysql
    if (!sMySQLDatabase.Initialize("127.0.0.1", 3306, "root", "123@taomee.com", "GameServer", 8)) {
        return false;
    }

    // Start Redis
    if (!sRedisDatabase.Initialize("127.0.0.1", 6379, "root", "123@taomee.com", "GameServer", 8)) {
        return false;
    }

    sThreadPool.ExecuteTask(new QueryWorkerThread((void*)"Show Databases;"));

    uint64_t current_time = 0;
    uint64_t prev_time = sEnv.GetRealMSTime();
    uint64_t diff_time = 0;
    uint64_t prev_sleep = 0;

    uint32_t loop_counter = 0;

    while (!stop_event_) {
        current_time = sEnv.GetRealMSTime();
        diff_time = current_time - prev_time;
        ++loop_counter;

        /* tick running */

        /*
           tick complete, update time and sleep

           diff (D0) include time of previous sleep (d0) + tick time (t0)
           we want that next d1 + t1 == WORLD_SLEEP_CONST
           we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
           d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
        */
        prev_time = sEnv.GetRealMSTime();
        if (diff_time <= TICK_TIME + prev_sleep) {
            prev_sleep = TICK_TIME  + prev_sleep - diff_time;
            sEnv.Sleep(prev_sleep);
        } else {
            prev_sleep = 0;
        }
    }

    sMySQLDatabase.Shutdown();

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
