/*
 * =====================================================================================
 *
 *       Filename:  lievent.cc
 *
 *    Description:  lievent
 *
 *        Version:  1.0
 *        Created:  04/03/2015 03:57:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include <stdlib.h>

//多线程版Libevent
//保存线程的结构体
struct LibeventThread
{
    LibEvtServer* that;                         //用作传参
    std::shared_ptr<std::thread>   spThread;    // 线程
    struct event_base * thread_base;            // 事件根基
    struct event   notify_event;
    evutil_socket_t  notfiy_recv_fd;            // socketpair 接收端fd（工作线程接收通知）
    evutil_socket_t  notfiy_send_fd;            // socketpair 发送端fd（监听线程发送通知）
#ifdef BOOST_LOCKFREE
    boost::lockfree::spsc_queue<conn_queue_item, boost::lockfree::capacity<1000> > conn_queue;
#else
    std::mutex conn_mtx;                        //维护连接队列的锁
    std::queue<conn_queue_item>  conn_queue;    //conn_queue 是一个管理conn_queue_item的队列
#endif
};

bool LibEvtServer::init(I_NetServerEvent* event, int start, int size)
{
    m_ids = new ChannelIDGenerator();
    m_ids->init(start, size);
    m_allChannels.resize(m_ids->getSize());

    m_event = event;


    //event支持windows下线程的函数
    int hr = evthread_use_windows_threads();
    m_base = event_base_new();
    if (!m_base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return false;
    }
#ifdef MUL_LIBEVENT_THREAD
    m_last_thread = -1; //注意初始化为-1
    //初始化线程
    init_threads(THREAD_NUMB);
#endif
    return true;
}

bool LibEvtServer::init_threads(int thread_numb)
{
    m_libevent_threads.resize(thread_numb);

    //为每个线程指定双向通道（类似于管道）
    for(int i = 0; i < thread_numb; ++i)
    {

        LibeventThread* plt = new LibeventThread();
#ifdef WIN32
        //创建一个socketpair即可与互相通信的两个socket，保存在fds里面
        evutil_socket_t fds[2];
        if(evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0)
        {
            std::cout << "创建socketpair失败\n";
            return false;
        }
        //设置成无阻赛的socket
        evutil_make_socket_nonblocking(fds[0]);
        evutil_make_socket_nonblocking(fds[1]);
#else
        int fds[2];
        if (pipe(fds)) {
            perror("Can't create notify pipe");
            exit(1);
        }
#endif
        plt->notfiy_recv_fd = fds[0];
        plt->notfiy_send_fd = fds[1];

        //安装libevent线程[创建base，注册通道事件（用于监听新链接）]
        setup_libevent_thread(plt);

        //线程放入容器中
        m_libevent_threads[i] = plt;
    }

    //开始创建并启动线程
    for(int i = 0; i < thread_numb; ++i)
    {
        m_libevent_threads[i]->spThread.reset(new std::thread([]
        (void* arg)
        {
            auto me = (LibeventThread*) arg;
            //  Wait for events to become active, and run their callbacks.
            //This is a more flexible version of event_base_dispatch().
            event_base_loop(me->thread_base, 0);
        }, m_libevent_threads[i]));
    }
    return true;
}

//设置线程信息
void LibEvtServer::setup_libevent_thread(LibeventThread * pLibeventThread)
{
    auto plt = pLibeventThread;
    plt->thread_base = event_base_new(); // 创建线程的event_base

    //给每个libevent线程设置连接通知回调函数。
    plt->that = this;
    //设置线程事件notify_event
    event_set(&plt->notify_event, plt->notfiy_recv_fd,//EV_READ表示只要这个socket可读就调用notify_cb函数
        EV_READ | EV_PERSIST, ::notify_cb, plt);
    //设置事件和event_base的关系
    event_base_set(plt->thread_base, &plt->notify_event); // 设置事件的从属关系（相当于指明事件属于哪个event_base）
    //添加事件
    event_add(&plt->notify_event, 0); // 正式添加事件
}


void LibEvtServer::listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
#ifdef MUL_LIBEVENT_THREAD
    int cur_thread = (m_last_thread + 1) %  THREAD_NUMB; // 轮循选择工作线程
    m_last_thread = cur_thread;

    conn_queue_item item;
    item.fd = fd;
    //item.ch2 = NULL;

    auto  plt = m_libevent_threads[cur_thread];
    {
        //向线程的队列中放入一个item，每个线程有个队列，保存连接的socketfd
#ifdef BOOST_LOCKFREE
        while(!plt->conn_queue.push(item))
        {
#ifndef _DEBUG
            boost::this_thread::interruptible_wait(1);
#else
            Sleep(1);
#endif
            Plug::PlugMessageBox("连接队列居然满了，超过1000的未处理数！");
        }
#else
        std::lock_guard<std::mutex> lock(plt->conn_mtx);
        plt->conn_queue.push(item);
#endif
    }
    //激活读线程的读事件
    send(plt->notfiy_send_fd, "c", 1, 0);

#else
    auto base = evconnlistener_get_base(listener);

    auto bev = bufferevent_socket_new(base, fd, BEV_OPT_THREADSAFE);//|BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return ;
    }

    auto c2 = CreateChannel(bev);

    bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, c2);
    bufferevent_enable(bev, EV_READ | EV_WRITE );

#endif
}



//侦听端口,-1表示向系统申请一个任意可用端口
bool LibEvtServer::listen(int* port)
{
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    if(-1 == *port)
        sin.sin_port = htons(10000);
    else
        sin.sin_port = htons(*port);

    m_listener = evconnlistener_new_bind(m_base, ::listener_cb, (void*)this,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));
    if (!m_listener)
    {
        return false;
    }

    if( -1 == *port)
        *port = ntohs(sin.sin_port);

    if (!m_listener) {
        fprintf(stderr, "Could not create a listener!\n");
        return false;
    }
    m_spListenThread.reset(new std::thread([this]//现在看这个线程只是收到连接，然后交给线程，然后通知线程
    {
        //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
        //event_base_loop(m_base, EVLOOP_ONCE);
        event_base_dispatch(m_base);
        if(WSAENOTSOCK == WSAGetLastError())
        {
            Plug::PlugMessageBox(L"操作无效套接字啊！");
        }

        Plug::PlugMessageBox(L"Libevent派发线程退出！");
    }));
    return true;
}

void LibEvtServer::notify_cb(evutil_socket_t fd, short which, LibeventThread *pLibeventThread)
{
    //首先将socketpair的1个字节通知信号读出（这是必须的，在水平触发模式下如果不处理该事件，则会循环通知，直到事件被处理）
    char  buf[1];
    recv(fd, buf, 1, 0);//从sockpair的另一端读数据

    auto plt = pLibeventThread;

    conn_queue_item  item;

    //从自己的连接队列中取出连接数
    {
        //取出队列中的第一个元素
#ifdef BOOST_LOCKFREE
        while(!plt->conn_queue.pop(item))//pop一个出来
        {
#ifndef _DEBUG
            boost::this_thread::interruptible_wait(1);
#else
            Sleep(1);
#endif
            Plug::PlugMessageBox("通知队列居然弹空了啊！");
        }
#else
        std::lock_guard<std::mutex>  lck(plt->conn_mtx);
        item = plt->conn_queue.front();
#endif
    }

    //创建每个socket的bufferevent
    auto bev = bufferevent_socket_new(plt->thread_base, item.fd, BEV_OPT_THREADSAFE);

    Channel2* c2 = CreateChannel(bev);

    //设置接收、状态改变 回调函数
    bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, c2);
    bufferevent_enable(bev, EV_READ | EV_WRITE );
}

//看了这个过程就是这个样子的，监听线程接收到连接之后把这个socket丢给Libevent线程，libevent创建bufferevent
//处理相关读和写事件，这个工程通过每个线程的连接队列，然后一个socketpair通知的。这样每个线程就很平均的处理所有的连接事件
//多线程比单线程的是复杂很多，只是这种模式不知道，但bufferevent还是一样的
