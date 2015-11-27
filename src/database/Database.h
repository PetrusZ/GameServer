/*
 * =====================================================================================
 *
 *       Filename:  Database.h
 *
 *    Description:  Database基类
 *
 *        Version:  1.0
 *        Created:  11/03/2015 03:50:00 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef DATABASE_H_0SZOVYQA
#define DATABASE_H_0SZOVYQA

#include <sstream>

#include "base/Callback.hpp"
#include "base/FQueue.hpp"
#include "common/Common.h"
#include "thread/CThread.h"
#include "thread/Mutex.h"

#include "Field.h"

#define QUERY_STRING_SIZE   16384

struct DatabaseConnection;
struct AsyncQueryResult;

class AsyncQuery;
class QueryBuffer;
class QueryResult;
class QueryThread;

class Database : public CThread {
    public:
        Database();
        virtual ~Database();

        const std::string& hostname() const { return hostname_; }
        const std::string& database_name() const { return database_name_; }

        /**********************************************************************/
        /* Thread Stuff                                                       */
        /**********************************************************************/
        bool Run();

        /**********************************************************************/
        /* Virtual Functions                                                  */
        /**********************************************************************/
        virtual bool Initialize(const char* hostname, uint16_t port,
                                const char* username, const char* password,
                                const char* database_name, uint32_t connection_count) = 0;
        virtual void Shutdown() = 0;

        /**********************************************************************/
        /* None Async Query                                                   */
        /**********************************************************************/
        virtual QueryResult* Query(const char* query_string, ...);
        virtual QueryResult* QueryNA(const char* query_string);
        virtual QueryResult* FQuery(const char* query_string, DatabaseConnection* conn);
        virtual bool WaitExecute(const char* query_string, ...);
        virtual bool WaitExecuteNA(const char* query_string);
        virtual bool FWaitExecute(const char* query_string, DatabaseConnection* conn);
        virtual bool Execute(const char* query_string, ...);
        virtual bool ExecuteNA(const char* query_string);

        // Async Query
        void QueueAsyncQuery(AsyncQuery* query);

        // Query Buffer
        void AddQueryBuffer(QueryBuffer* buffer);
        void PerformQueryBuffer(QueryBuffer* buffer, DatabaseConnection* conn);

        // Pure Virtual Escape Function
        virtual std::string EscapeString(std::string escape) = 0;
        virtual void EscapeLongString(const char* str, uint32_t len, std::stringstream& out) = 0;
        virtual std::string EscapeString(const char* escape, DatabaseConnection* conn) = 0;

        DatabaseConnection* GetFreeConnection();

        void ThreadProcessQuery();
        void EndThreads();

    protected:
        // spawn threads
        void Initialize_();

        virtual void BeginTransaction(DatabaseConnection* conn) = 0;
        virtual void EndTransaction(DatabaseConnection* conn) = 0;

        // actual query function
        virtual bool SendQuery(DatabaseConnection* conn, const char* sql) = 0;
        virtual QueryResult* StoreQueryResult(DatabaseConnection* conn) = 0;

        bool running_ = false;

        DatabaseConnection** connections_;
        uint32_t connection_count_;

        QueryThread* query_thread_;

        // query queue
        FQueue<QueryBuffer*> query_buffer_;
        FQueue<char*> queries_queue_;

        // database info
        std::string hostname_;
        std::string username_;
        std::string password_;
        std::string database_name_;
        uint16_t port_;

    private:
        friend class QueryThread;
};

struct DatabaseConnection {
    Mutex Busy_;
};

struct AsyncQueryResult {
    QueryResult* result_;
    char* query_;
};

class AsyncQuery {
    public:
        AsyncQuery(QueryCallbackBase* f) : func_(f) {}
        virtual ~AsyncQuery();

        void AddQuery(const char* format, ...);
        void Perform();
        void SetDB(Database* db) { db_ = db; }

    private:
        // friend class Database;
        QueryCallbackBase* func_;
        std::vector<AsyncQueryResult> queries_;
        Database* db_;
};

class QueryBuffer {
    public:
        void AddQuery(const char* format, ...);
        void AddQueryNA(const char* str);
        void AddQueryStr(const std::string& str);

    private:
        // friend class Database;
        friend class Database;
        std::vector<char*> queries_;
};

class QueryResult {
    public:
        QueryResult(uint32_t fields, uint32_t rows) : field_count_(fields), row_count_(rows), current_row_(NULL) {}
        virtual ~QueryResult() {}

        virtual bool NextRow() = 0;
        void Delete() { delete this; }

        Field* Fetch() { return current_row_; }
        uint32_t field_count() const { return field_count_; }
        uint32_t row_count() const { return row_count_; }

    protected:
        uint32_t field_count_;
        uint32_t row_count_;
        Field* current_row_;
};

class QueryThread : public ThreadBase {
    public:
        QueryThread(Database* db) : ThreadBase(), db_(db) {}
        virtual ~QueryThread();
        bool Run();

    private:
        friend class Database;
        Database* db_;
};

#endif /* end of include guard: DATABASE_H_0SZOVYQA */
