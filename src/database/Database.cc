/*
 * =====================================================================================
 *
 *       Filename:  Database.cc
 *
 *    Description:  Database
 *
 *        Version:  1.0
 *        Created:  11/04/2015 03:27:59 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include <cstdarg>
#include <cstdio>

#include "Database.h"
#include "thread/ThreadPool.h"
#include "port/Env.h"

Database::Database() : CThread() {
    connections_ = NULL;
    connection_count_ = -1;
    running_ = true;
}

Database::~Database() {
    // TODO
}

void Database::Initialize_() {
    // Spawn Database thread
    sThreadPool.ExecuteTask(this);

    // Launch the query thread for QueryBuffer
    query_thread_ = new QueryThread(this);
    sThreadPool.ExecuteTask(query_thread_);
}

DatabaseConnection* Database::GetFreeConnection() {
    uint32_t i = 0;

    while(true) {
        DatabaseConnection* conn = connections_[((i++) % connection_count_)];
        if (conn->Busy_.TryLock()) {
            return conn;
        }
    }

    // Shouldn't be here.
    return NULL;
}

// Use query function when we request data and wait for respone.
QueryResult* Database::Query(const char* query_string, ...) {
    char sql[QUERY_STRING_SIZE];
    va_list ap;
    va_start(ap, query_string);
    vsnprintf(sql, QUERY_STRING_SIZE, query_string, ap);
    va_end(ap);

    return QueryNA(sql);
}

QueryResult* Database::QueryNA(const char* query_string) {
    DatabaseConnection* conn = GetFreeConnection();

    return FQuery(query_string, conn);
}

QueryResult* Database::FQuery(const char* query_string, DatabaseConnection* conn) {
    // Send the query
    QueryResult* result = NULL;

    if (SendQuery(conn, query_string)) {
        result = StoreQueryResult(conn);
    }

    conn->Busy_.UnLock();
    return result;
}

// Use WaitExecute Function when we need wait for respone and don't have result.
bool Database::WaitExecute(const char* query_string, ...) {
    char sql[QUERY_STRING_SIZE];
    va_list ap;
    va_start(ap, query_string);
    vsnprintf(sql, QUERY_STRING_SIZE, query_string, ap);
    va_end(ap);

    return WaitExecuteNA(sql);
}

bool Database::WaitExecuteNA(const char* query_string) {
    DatabaseConnection* conn = GetFreeConnection();
    return FWaitExecute(query_string, conn);
}

bool Database::FWaitExecute(const char* query_string, DatabaseConnection* conn) {
    bool result = SendQuery(conn, query_string);
    conn->Busy_.UnLock();
    return result;
}

// Use Execute function when we can wait till other queries do first.
bool Database::Execute(const char* query_string, ...) {
    char sql[QUERY_STRING_SIZE];
    va_list ap;
    va_start(ap, query_string);
    vsnprintf(sql, QUERY_STRING_SIZE, query_string, ap);
    va_end(ap);

    return ExecuteNA(sql);
}

bool Database::ExecuteNA(const char* query_string) {
    if (!running_)
        return WaitExecuteNA(query_string);

    size_t len = strlen(query_string);
    char* buffer = new char[len + 1];
    memcpy(buffer, query_string, len + 1);

    queries_queue_.push(buffer);
    return true;
}

void Database::QueueAsyncQuery(AsyncQuery* query) {
    query->SetDB(this);
    query->Perform();
}

void Database::AddQueryBuffer(QueryBuffer* buffer) {
    if (NULL == query_thread_) {
        PerformQueryBuffer(buffer, NULL);
        delete buffer;
    } else {
        query_buffer_.push(buffer);
    }
}

void Database::PerformQueryBuffer(QueryBuffer* buffer, DatabaseConnection* conn) {
    if (!buffer->queries_.size()) {
        return;
    }

    DatabaseConnection* conn2 = conn;
    if (NULL == conn) {
        conn2 = GetFreeConnection();
    }

    BeginTransaction(conn2);

    for (std::vector<char*>::iterator itr = buffer->queries_.begin(); itr != buffer->queries_.end(); ++itr) {
        SendQuery(conn2, *itr);
        delete[](*itr);
    }

    EndTransaction(conn2);

    if (NULL == conn) {
        conn2->Busy_.UnLock();
    }
}

// Process queries by thread
bool Database::Run() {
    SetThreadState(THREADSTATE_BUSY);
    running_ = true;

    char* query = queries_queue_.pop();
    DatabaseConnection* conn = GetFreeConnection();

    while(true) {
        if (NULL != query) {
            if (NULL == conn) {
                conn = GetFreeConnection();
            }
            SendQuery(conn, query);
            delete[] query;
        }

        if (GetThreadState() == THREADSTATE_TERMINATE)
            break;

        query = queries_queue_.pop();
        if (NULL == query) {
            if (NULL != conn) {
                conn->Busy_.UnLock();
                conn = NULL;
            }
            sEnv.Sleep(10);
        }
    }

    // execute all remaining queries
    query = queries_queue_.pop();
    while(query) {
        if (NULL == conn) {
            conn = GetFreeConnection();
        }
        SendQuery(conn, query);
        delete[] query;
        query = queries_queue_.pop();
    }

    if (NULL != conn)
        conn->Busy_.UnLock();

    running_ = false;
    return false;
}

// Process QueryBuffer by QueryThread
void Database::ThreadProcessQuery() {
    DatabaseConnection* conn = GetFreeConnection();
    QueryBuffer* buffer = query_buffer_.pop();

    while(true) {
        if (NULL != buffer) {
            PerformQueryBuffer(buffer, conn);
            delete buffer;
        }

        if (GetThreadState() == THREADSTATE_TERMINATE) {
            break;
        }

        buffer = query_buffer_.pop();
        if (NULL == buffer) {
            sEnv.Sleep(10);
        }
    }

    conn->Busy_.UnLock();

    // perform all remaining buffer
    while(NULL != buffer) {
        PerformQueryBuffer(buffer, NULL);
        delete buffer;

        buffer = query_buffer_.pop();
    }
}

void Database::EndThreads() {
    // these 2 loops spin untill there's nothing left.
    // Aka delete all staff in query_buffer_ and queries_queue_.
    while(true) {
        QueryBuffer* buffer = query_buffer_.pop();
        if (NULL == buffer) {
            break;
        }
        query_buffer_.push(buffer);
    }

    while(true) {
        char* buffer = queries_queue_.pop();
        if (NULL == buffer) {
            break;
        }
        queries_queue_.push(buffer);
    }

    SetThreadState(THREADSTATE_TERMINATE);

    while(running_ || query_thread_) {
        sEnv.Sleep(100);
        if (!running_) {
            break;
        }
    }
}

AsyncQuery::~AsyncQuery() {
    delete func_;

    for (std::vector<AsyncQueryResult>::iterator itr = queries_.begin(); itr != queries_.end(); ++itr) {
        if (itr->result_) {
            delete itr->result_;
        }
        delete[] itr->query_;
    }
}

void AsyncQuery::AddQuery(const char *format, ...) {
    AsyncQueryResult result;
    char buffer[QUERY_STRING_SIZE];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, QUERY_STRING_SIZE, format, ap);
    va_end(ap);

    size_t len = strlen(buffer);
    ASSERT(len);

    result.query_ = new char[len + 1];
    result.query_[len] = 0;
    result.result_ = NULL;
    memcpy(result.query_, buffer, len);
    queries_.push_back(result);
}

void AsyncQuery::Perform() {
    DatabaseConnection* conn = db_->GetFreeConnection();
    for (std::vector<AsyncQueryResult>::iterator itr = queries_.begin(); itr != queries_.end(); ++itr) {
        itr->result_ = db_->FQuery(itr->query_, conn);
    }
    conn->Busy_.UnLock();
    func_->Run(queries_);

    delete this;
}

void QueryBuffer::AddQuery(const char* format, ...) {
    char buffer[QUERY_STRING_SIZE];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, QUERY_STRING_SIZE, format, ap);
    va_end(ap);

    AddQueryNA(buffer);
}

void QueryBuffer::AddQueryNA(const char* str) {
    size_t len = strlen(str);
    ASSERT(len);
    char* query = new char[len + 1];
    memcpy(query, str, len + 1);

    queries_.push_back(query);
}

void QueryBuffer::AddQueryStr(const std::string& str) {
    size_t len = str.size();
    char* query = new char[len + 1];
    memcpy(query, str.c_str(), len + 1);

    queries_.push_back(query);
}

QueryThread::~QueryThread() {
    db_->query_thread_ = NULL;
}

bool QueryThread::Run() {
    db_->ThreadProcessQuery();
    return true;
}
