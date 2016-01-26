/*
 * =====================================================================================
 *
 *       Filename:  LibEventDefine.h
 *
 *    Description:  LibEventDefine
 *
 *        Version:  1.0
 *        Created:  04/07/2015 02:50:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef LIBEVENTDEFINE_H_4PGJX386
#define LIBEVENTDEFINE_H_4PGJX386

#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/thread.h"

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02010500
#error "Libevent not found or libevent version too old to supporte. Please get 2.0.22-stable or later"
#endif

/*
 * Event
 */
typedef evutil_socket_t EventSocket;
typedef event_callback_fn EventCallback;
typedef short EventFlagType;

enum EventFlag {
    kEventTimeout   = EV_TIMEOUT,
    kEventRead      = EV_READ,
    kEventWrite     = EV_WRITE,
    kEventSignal    = EV_SIGNAL,
    kEventPersist   = EV_PERSIST,
    kEventEv        = EV_ET
};

/*
 * BufferEvent
 */
typedef int BufferEventOptionType;
typedef bufferevent_data_cb BufferEventDataCallBack;
typedef bufferevent_event_cb BufferEventEventCallBack;

typedef short BufferEventFlagType;
typedef struct bufferevent BufferEventStruct;

enum BufferEventOption {
    kCloseOnFree        = BEV_OPT_CLOSE_ON_FREE,
    kThreadSafe         = BEV_OPT_THREADSAFE,
    kDeferCallbacks     = BEV_OPT_DEFER_CALLBACKS,
    kUnlockCallbacks    = BEV_OPT_UNLOCK_CALLBACKS
};

enum BufferEventFlag {
    kReading        = BEV_EVENT_READING,
    kWriting        = BEV_EVENT_WRITING,
    kError          = BEV_EVENT_ERROR,
    kTimeout        = BEV_EVENT_TIMEOUT,
    kEof            = BEV_EVENT_EOF,
    kConnected      = BEV_EVENT_CONNECTED
};

/*
 * EventBase
 */
typedef int EventFeatureType;
typedef int EventBaseFlagType;
typedef int EventLoopFlagType;

enum EventFeature {
    kEventFeatureNull   = 0,
    kEventFeatureEt     = EV_FEATURE_ET,
    kEventFeatureO1     = EV_FEATURE_O1,
    kEventFeatureFds    = EV_FEATURE_FDS
};

enum EventBaseFlag {
    kEventBaseFlagNull                  = 0,
    kEventBaseFlagNoLock                = EVENT_BASE_FLAG_NOLOCK,
    kEventBaseFlagIgnoreEnv             = EVENT_BASE_FLAG_IGNORE_ENV,
    kEventBaseFlagStartupIocp           = EVENT_BASE_FLAG_STARTUP_IOCP,
    kEventBaseFlagNoCacheTime           = EVENT_BASE_FLAG_NO_CACHE_TIME,
    kEventBaseFlagEpollUseChangelist    = EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST
};

enum EventLoopFlag {
    kEventLoopFlagNull              = 0,
    kEventLoopFlagOnce              = EVLOOP_ONCE,
    kEventLoopFlagNonBlock          = EVLOOP_NONBLOCK
};

enum EventLoopExitType {
    kNomal  = 0,
    kExit   = 1,
    kBreak  = 2
};

#endif /* end of include guard: LIBEVENTDEFINE_H_4PGJX386 */
